#pragma once

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

enum class FilterType
{
    LOW_PASS = 0,
    HIGH_PASS = 1,
    REJECTOR = 2
};

enum class FilterName
{
    IDEAL = 0,
    BUTTERWORTH = 1,
    GAUSS = 2
};

class Filter
{
public:
    Filter(const FilterType filterType = FilterType::LOW_PASS,
           const FilterName filterName = FilterName::BUTTERWORTH,
           const float distance = 50, const int32_t order = 5, const float width = 10);
    ~Filter() = default;

    void setSize(const cv::Size2i size);
    cv::Size2i getSize() const;

    void setFilter(const cv::Mat1f& filter);
    cv::Mat1f getFilter() const;
    void showFilter();

    void setFilterType(const FilterType filterType, const bool isUpdate = true);
    FilterType getFilterType() const;

    void setFilterName(const FilterName filterName, const bool isUpdate = true);
    FilterName getFilterName() const;

    void setDistance(const float distance, const bool isUpdate = true);
    float getDistance() const;

    void setWidth(const float width, const bool isUpdate = true);
    float getWidth() const;

    void setOrder(const int32_t order, const bool isUpdate = true);
    int32_t getOrder() const;

    void configure(const FilterType filterType, const FilterName filterName,
                   const float distance = -1, const int32_t order = -1, const float width = -1);

private:
    void calculateFilter();

    cv::Size2i m_size;
    cv::Mat1f m_filter;
    FilterType m_filterType;
    FilterName m_filterName;
    float m_distance;
    float m_width;
    int32_t m_order;
};
