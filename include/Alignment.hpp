#ifndef alignmentClass
#define alignmentClass
#include <string>
#include "ncl/nxsmultiformat.h"

class Alignment{
    public:
        Alignment(void) = delete;
        Alignment(std::string fn);
        ~Alignment();
        int getNumTaxa(){return numTaxa;}
        int getNumChar(){return numChar;}
        int** getMatrix(){return matrix;}
    private:
        void readNucleotideData(NxsCharactersBlock* charBlock);
        //ATCG = 1248
        //?N = 15
        int** matrix;
        int numTaxa;
        int numChar;
};

#endif