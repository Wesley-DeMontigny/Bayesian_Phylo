#ifndef ABSTRACT_EVENT_HPP
#define ABSTRACT_EVENT_HPP

class AbstractEvent{
    public:
        virtual void initialize()=0;
        virtual void call(int iteration)=0;
    protected:
        int interval;
};

#endif