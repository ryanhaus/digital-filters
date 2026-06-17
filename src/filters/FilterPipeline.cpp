#include "FilterPipeline.hpp"

FilterPipeline::FilterPipeline(std::vector<std::shared_ptr<Filter>> filters)
    : Filter(),
      filters(filters)
{
    if (!filters.empty())
    {
        for (size_t i = 0; i < filters.size() - 1; i++)
        {
            filters[i]->attachTo(*filters[i + 1]);
        }
    }
}

void FilterPipeline::processSample(complex<float> sample)
{
    if (!filters.empty())
    {
        filters.front()->processSample(sample);
    }
    else
    {
        this->pushSample(sample);
    }
}

void FilterPipeline::attachTo(SignalSink& dest)
{
    if (!filters.empty())
    {
        filters.back()->attachTo(dest);
    }
    else
    {
        Filter::attachTo(dest);
    }
}
