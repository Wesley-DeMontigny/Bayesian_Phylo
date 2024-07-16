#ifndef MOVE_TREE_NNI_HPP
#define MOVE_TREE_NNI_HPP
#include "TreeParameter.hpp"
#include "AbstractMove.hpp"

class MoveTreeNNI : public AbstractMove{
    public:
        MoveTreeNNI(TreeParameter* t);
        double update();
        void accept();
        void reject();
        void tune();
    private:
        TreeParameter* param;
};

#endif