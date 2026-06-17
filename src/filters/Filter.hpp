#pragma once
#include "../sinks/SignalSink.hpp"

// generic filter class
class Filter : public SignalSink
{
public:
    Filter(SignalSink& destination)
        : destination(destination)
    {}

    virtual ~Filter() = default;

protected:
    SignalSink& destination;
};
