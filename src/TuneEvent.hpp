#ifndef TUNE_EVENT_HPP
#define TUNE_EVENT_HPP
#include "AbstractEvent.hpp"

class MoveScheduler;

class TuneEvent : public AbstractEvent{
    public:
        TuneEvent(void);
        TuneEvent(MoveScheduler* m);
        void initialize();
        void call(int iteration);
        void setMoveScheduler(MoveScheduler* m) {moveScheduler = m;}
    private:
        MoveScheduler* moveScheduler;
};

#endif;