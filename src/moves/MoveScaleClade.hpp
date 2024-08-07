#ifndef MOVE_SCALE_CLADE_HPP
#define MOVE_TREE_SCALE_CLADE_HPP
#include "modeling/parameters/trees/TreeParameter.hpp"
#include "Move.hpp"
#include <cmath>

class MoveScaleClade : public Move{
    public:
        MoveScaleClade(TreeParameter* t);
        double update();
        void tune();
    private:
        TreeParameter* param;
        double delta = std::log(4.0);
};

#endif