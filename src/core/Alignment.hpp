#ifndef ALIGNMENT_HPP
#define ALIGNMENT_HPP
#include <string>
#include "ncl/nxsmultiformat.h"
#include "DoubleMatrix.hpp"

class Alignment{
    public:
                                    Alignment(void) = delete;
                                    Alignment(std::string fn);
                                    ~Alignment();
        int                         getCharCode(int i, int j) {return matrix[i][j];}
        int                         getDataType() {return dataType;}
        int**                       getMatrix() {return matrix;}
        int                         getNumChar() {return numChar;}
        int                         getNumTaxa() {return numTaxa;}
        DoubleMatrix                getPairwiseIdentities();
        int                         getStateSpace() {return stateSpace;}
        std::vector<std::string>    getTaxaNames() {return taxaNames;}
    private:
        int                         dataType;
        int**                       matrix;
        int                         numTaxa;
        int                         numChar;
        int                         stateSpace;
        void                        readNucleotideData(NxsCharactersBlock* charBlock);
        void                        readAminoAcidData(NxsCharactersBlock* charBlock);
        std::vector<std::string>    taxaNames;
};

#endif