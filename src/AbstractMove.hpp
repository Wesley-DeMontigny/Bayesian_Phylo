#ifndef MOVE_HPP
#define MOVE_HPP

class AbstractParameter;

class AbstractMove{
    public:
        AbstractMove(){};
        virtual double update()=0;
        virtual AbstractParameter* getParameter()=0;
};

#endif