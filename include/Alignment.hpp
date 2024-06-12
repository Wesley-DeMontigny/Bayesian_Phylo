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
        std::vector<std::vector<int>> getMatrix() {return matrix;}
        int                         getDataType() {return dataType;}
    private:
        void                        readNucleotideData(NxsCharactersBlock* charBlock);
        //ACGT = 1248
        //?N- = 15
        std::vector<std::vector<int>> matrix;
        int                         numTaxa;
        int                         numChar;
        int                         dataType;
        std::vector<std::string>    taxaNames;
};

#endif