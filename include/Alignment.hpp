#ifndef alignmentClass
#define alignmentClass
#include <string>
#include "ncl/nxsmultiformat.h"

class Alignment{
    public:
                                    Alignment(void) = delete;
                                    Alignment(std::string fn);
                                    ~Alignment();
        int                         getNumTaxa() {return numTaxa;}
        int                         getNumChar() {return numChar;}
        std::vector<std::string>    getTaxaNames() {return taxaNames;}
        int**                       getMatrix() {return matrix;}
        int                         getCharCode(int i, int j) {return matrix[i][j];}
        int                         getDataType() {return dataType;}
    private:
        void                        readNucleotideData(NxsCharactersBlock* charBlock);
        //ACGT = 1248
        //?N- = 15
        int**                       matrix;
        int                         numTaxa;
        int                         numChar;
        int                         dataType;
        std::vector<std::string>    taxaNames;
};

#endif