#ifndef MOVE_TREE_LOCAL_HPP
#define MOVE_TREE_LOCAL_HPP
#include "TreeParameter.hpp"
#include "AbstractMove.hpp"
#include <cmath>

class MoveTreeLocal : public AbstractMove{
    public:
        MoveTreeLocal(TreeParameter* t);
        double update();
        void accept();
        void reject();
        void tune();
    private:
        TreeParameter* param;
        double delta = std::log(4.0);
};

#endif