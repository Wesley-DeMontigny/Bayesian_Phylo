#ifndef ITERATION_TRACKER_EVENT
#define ITERATION_TRACKER_EVENT
#include "AbstractEvent.hpp"

class AbstractLikelihood;
class AbstractPrior;
class AbstractParameter;

class IterationTrackerEvent : public AbstractEvent{
    public:
        IterationTrackerEvent(void);
        void initialize();
        void call(int iteration);
    private:
};

#endif