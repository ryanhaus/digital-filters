#pragma once
#include "Filter.hpp"
#include <vector>
#include <memory>

// pipelines samples through multiple filters
class FilterPipeline : public Filter
{
public:
    FilterPipeline(std::vector<std::shared_ptr<Filter>> filters);
    void processSample(complex<float> sample) override;
    void attachTo(SignalSink& dest) override;

private:
    std::vector<std::shared_ptr<Filter>> filters;
};
