#include "filter.h"

using namespace std;
using namespace cv;

Filter::Filter(
    const FilterType filterType,
    const FilterName filterName,
    const float distance,
    const int32_t order, 
    const float width) :
    m_filterType(filterType),
    m_filterName(filterName),
    m_distance(distance),
    m_order(order),
    m_width(width)
{

}

void Filter::setSize(const Size2i size)
{
    if (size.height <= 0 || size.width <= 0)
    {
        return;
    }
    m_size = size;
    calculateFilter();
}

Size2i Filter::getSize() const
{
    return m_size;
}

void Filter::setFilter(const Mat1f& filter)
{
    if (filter.empty() == true)
    {
        return;
    }
    m_filter = filter;
}

Mat1f Filter::getFilter() const
{
    return m_filter.clone();
}

void Filter::showFilter()
{
    if (m_filter.empty() == true)
    {
        return;
    }
    imshow("Filter [FFT]", m_filter);
}

void Filter::setFilterType(const FilterType filterType, const bool isUpdate)
{
    switch (filterType)
    {
    case FilterType::LOW_PASS:
    case FilterType::HIGH_PASS:
    case FilterType::REJECTOR:
        m_filterType = filterType;
        if (isUpdate == true)
        {
            calculateFilter();
        }
        break;
    default:
        break;
    }
}

FilterType Filter::getFilterType() const
{
    return m_filterType;
}

void Filter::setFilterName(const FilterName filterName, const bool isUpdate)
{
    switch (filterName)
    {
    case FilterName::IDEAL:
    case FilterName::BUTTERWORTH:
    case FilterName::GAUSS:
        m_filterName = filterName;
        if (isUpdate == true)
        {
            calculateFilter();
        }
        break;
    default:
        break;
    }
}

FilterName Filter::getFilterName() const
{
    return m_filterName;
}

void Filter::setDistance(const float distance, const bool isUpdate)
{
    if (distance < 0)
    {
        return;
    }
    m_distance = distance;
    if (isUpdate == true)
    {
        calculateFilter();
    }
}

float Filter::getDistance() const
{
    return m_distance;
}

void Filter::setWidth(const float width, const bool isUpdate)
{
    if (width < 0)
    {
        return;
    }
    m_width = width;
    if (isUpdate == true)
    {
        calculateFilter();
    }
}

float Filter::getWidth() const
{
    return m_width;
}

void Filter::setOrder(const int32_t order, const bool isUpdate)
{
    if (order < 0)
    {
        return;
    }
    m_order = order;
    if (isUpdate == true)
    {
        calculateFilter();
    }
}

int32_t Filter::getOrder() const
{
    return m_order;
}

void Filter::configure(const FilterType filterType, const FilterName filterName,
                       const float distance, const int32_t order, const float width)
{
    setFilterType(filterType, false);
    setFilterName(filterName, false);
    setDistance(distance, false);
    setWidth(width, false);
    setOrder(order, false);
    calculateFilter();
}

void Filter::calculateFilter()
{
    if (m_size.height <= 0 || m_size.width <= 0)
    {
        return;
    }

    if (m_distance <= 0)
    {
        return;
    }

    auto filter = Mat1f(m_size, 0);
    auto center = Point2i(m_size.width / 2, m_size.height / 2);
    switch (m_filterType)
    {
    case FilterType::LOW_PASS:
        switch (m_filterName)
        {
        case FilterName::IDEAL:
            for (auto row = 0; row < filter.rows; row++)     
            {                                                
                auto ptr = filter.ptr<float>(row);
                for (auto col = 0; col < filter.cols; col++)
                {                                            
                    ptr[col] = hypotf(col - center.x, row - center.y) > m_distance ? 0 : 1;                       
                }                                            
            }
            break;
        case FilterName::BUTTERWORTH:
            if (m_order <= 0)
            {
                return;
            }
            for (auto row = 0; row < filter.rows; row++)
            {
                auto ptr = filter.ptr<float>(row);
                for (auto col = 0; col < filter.cols; col++)
                {
                    auto distance = hypotf(col - center.x, row - center.y);
                    ptr[col] = 1 / (1 + powf(distance / m_distance, 2 * m_order));
                }
            }
            break;
        case FilterName::GAUSS:
            for (auto row = 0; row < filter.rows; row++)
            {
                auto ptr = filter.ptr<float>(row);
                for (auto col = 0; col < filter.cols; col++)
                {
                    auto distance = hypotf(col - center.x, row - center.y);
                    ptr[col] = expf(-powf(distance, 2) / (2 * powf(m_distance, 2)));
                }
            }
            break;
        default:
            break;
        }
        break;
    case FilterType::HIGH_PASS:
        switch (m_filterName)
        {
        case FilterName::IDEAL:
            for (auto row = 0; row < filter.rows; row++)
            {
                auto ptr = filter.ptr<float>(row);
                for (auto col = 0; col < filter.cols; col++)
                {
                    ptr[col] = hypotf(col - center.x, row - center.y) > m_distance ? 1 : 0;
                }
            }
            break;
        case FilterName::BUTTERWORTH:
            if (m_order <= 0)
            {
                return;
            }
            for (auto row = 0; row < filter.rows; row++)
            {
                auto ptr = filter.ptr<float>(row);
                for (auto col = 0; col < filter.cols; col++)
                {
                    auto distance = hypotf(col - center.x, row - center.y);
                    ptr[col] = 1 / (1 + powf(m_distance / distance, 2 * m_order));
                }
            }
            break;
        case FilterName::GAUSS:
            for (auto row = 0; row < filter.rows; row++)
            {
                auto ptr = filter.ptr<float>(row);
                for (auto col = 0; col < filter.cols; col++)
                {
                    auto distance = hypotf(col - center.x, row - center.y);
                    ptr[col] = 1 - expf(-powf(distance, 2) / (2 * powf(m_distance, 2)));
                }
            }
            break;
        default:
            break;
        }
        break;
    case FilterType::REJECTOR:
        if (m_width <= 0)
        {
            return;
        }
        switch (m_filterName)
        {
        case FilterName::IDEAL:
            for (auto row = 0; row < filter.rows; row++)
            {
                auto ptr = filter.ptr<float>(row);
                for (auto col = 0; col < filter.cols; col++)
                {
                    ptr[col] = (hypotf(col - center.x, row - center.y) < m_distance - m_width / 2 ||
                                hypotf(col - center.x, row - center.y) > m_distance + m_width / 2) ? 1 : 0;
                }
            }
            break;
        case FilterName::BUTTERWORTH:
            if (m_order <= 0)
            {
                return;
            }
            for (auto row = 0; row < filter.rows; row++)
            {
                auto ptr = filter.ptr<float>(row);
                for (auto col = 0; col < filter.cols; col++)
                {
                    auto distance = hypotf(col - center.x, row - center.y);
                    auto value = (distance * m_width) / (powf(distance, 2) - powf(m_distance, 2));
                    ptr[col] = 1 / (1 + powf(value, 2 * m_order));
                }
            }
            break;
        case FilterName::GAUSS:
            for (auto row = 0; row < filter.rows; row++)
            {
                auto ptr = filter.ptr<float>(row);
                for (auto col = 0; col < filter.cols; col++)
                {
                    auto distance = hypotf(col - center.x, row - center.y);
                    auto value = (powf(distance, 2) - powf(m_distance, 2)) / (distance * m_width);
                    ptr[col] = 1 - expf(-powf(value, 2));
                }
            }
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
    m_filter = filter;
}
