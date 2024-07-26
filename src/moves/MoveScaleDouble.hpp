#ifndef MOVE_SCALE_DOUBLE_HPP
#define MOVE_SCALE_DOUBLE_HPP
#include "Move.hpp"

class DoubleParameter;

class MoveScaleDouble : public Move {
    public:
        MoveScaleDouble(DoubleParameter* d);
        double update();
        void tune();
    private:
        DoubleParameter* param;
        double delta;
};

#endif