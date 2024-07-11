#ifndef MOVE_SCALE_BRANCH_HPP
#define MOVE_SCALE_BRANCH_HPP
#include "AbstractParameter.hpp"
#include "TreeParameter.hpp"
#include "AbstractMove.hpp"

class MoveScaleBranch : public AbstractMove{
    public:
        MoveScaleBranch(TreeParameter* t);
        double update();
        AbstractParameter* getParameter(){return param;}
    private:
        TreeParameter* param;
};

#endif