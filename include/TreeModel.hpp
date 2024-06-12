#ifndef tree_model
#define tree_model
#include "Tree.hpp"
#include "Alignment.hpp"
#include "Node.hpp"
#include "ncl/nxsmultiformat.h"

class TreeModel{
    public:
        TreeModel();
        double FelPrune(Tree t, Alignment aln, bool debug);
        void setStationaryDist(std::vector<double> dist) {stationaryDist = dist;}
        std::vector<double> getStationaryDist() {return stationaryDist;}
    protected:
        virtual std::vector<std::vector<double>> transitionProbability(double time) = 0;
        std::vector<double> stationaryDist;
};

#endif