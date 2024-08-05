#ifndef MOVE_SLIDE_DOUBLE_HPP
#define MOVE_SLIDE_DOUBLE_HPP
#include "Move.hpp"

class DoubleParameter;

class MoveSlideDouble : public Move {
    public:
        MoveSlideDouble(DoubleParameter* p, double w);
        double update();
        void tune();
    private:
        DoubleParameter* param;
        double window;
        double delta;
};

#endif