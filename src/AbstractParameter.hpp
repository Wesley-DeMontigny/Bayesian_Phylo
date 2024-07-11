#ifndef ABSTRACT_PARAMETER_HPP
#define ABSTRACT_PARAMETER_HPP

class AbstractParameter{
    public:
        virtual void accept()=0;
        virtual void reject()=0;
    private:
};

#endif