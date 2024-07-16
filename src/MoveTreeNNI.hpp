#ifndef MOVE_TREE_NNI_HPP
#define MOVE_TREE_NNI_HPP
#include "AbstractParameter.hpp"
#include "TreeParameter.hpp"
#include "AbstractMove.hpp"

class MoveTreeNNI : public AbstractMove{
    public:
        MoveTreeNNI(TreeParameter* t);
        double update();
        AbstractParameter* getParameter(){return param;}
        void accept();
        void reject();
        void tune();
    private:
        TreeParameter* param;
};

#endif