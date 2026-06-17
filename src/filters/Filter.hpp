#pragma once
#include "../sinks/SignalSink.hpp"
#include <vector>

// generic filter class
class Filter : public SignalSink
{
public:
    Filter() = default;

    Filter(SignalSink& destination)
    {
        destinations.push_back(&destination);
    }

    virtual ~Filter() = default;

    virtual void attachTo(SignalSink& dest)
    {
        destinations.push_back(&dest);
    }

protected:
    std::vector<SignalSink*> destinations;

    void pushSample(complex<float> sample)
    {
        for (auto* dest : destinations)
        {
            dest->processSample(sample);
        }
    }
};
