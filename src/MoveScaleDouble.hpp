#ifndef MOVE_SCALE_DOUBLE_HPP
#define MOVE_SCALE_DOUBLE_HPP
#include "AbstractMove.hpp"

class DoubleParameter;

class MoveScaleDouble : public AbstractMove {
    public:
        MoveScaleDouble(DoubleParameter* d);
        double update();
        void accept();
        void reject();
        void tune();
    private:
        DoubleParameter* param;
        double delta;
};

#endif