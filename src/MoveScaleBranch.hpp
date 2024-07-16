#ifndef MOVE_SCALE_BRANCH_HPP
#define MOVE_SCALE_BRANCH_HPP
#include "TreeParameter.hpp"
#include "AbstractMove.hpp"

class MoveScaleBranch : public AbstractMove{
    public:
        MoveScaleBranch(TreeParameter* t);
        double update();
        void accept();
        void reject();
        void tune();
    private:
        TreeParameter* param;
        double delta;
};

#endif