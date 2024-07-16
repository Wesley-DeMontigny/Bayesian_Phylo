#ifndef MOVE_HPP
#define MOVE_HPP

class AbstractParameter;

class AbstractMove{
    public:
        AbstractMove(){};
        virtual double update()=0;
        virtual void accept()=0;
        virtual void reject()=0;
        virtual void tune()=0;
    protected:
        int countSinceTune = 0;
        int acceptedSinceTune = 0;
        int rejectedCount = 0;
        int acceptedCount = 0;
};

#endif