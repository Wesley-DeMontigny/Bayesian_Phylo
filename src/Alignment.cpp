#include "Alignment.hpp"
#include "Msg.hpp"

Alignment::Alignment(std::string fn){
    
    MultiFormatReader nexusReader;
    const char* fileName = fn.c_str();
    nexusReader.ReadFilepath(fileName, MultiFormatReader::NEXUS_FORMAT);

    size_t numTaxaBlocks = nexusReader.GetNumTaxaBlocks();
    if(numTaxaBlocks > 1)
        Msg::error("Too many taxa blocks (> 1)");

    for(size_t tBlock = 0; tBlock < numTaxaBlocks; tBlock++){
        NxsTaxaBlock* taxaBlock = nexusReader.GetTaxaBlock(tBlock);
        std::string taxaBlockTitle = taxaBlock->GetTitle();
        const unsigned numCharBlocks = nexusReader.GetNumCharactersBlocks(taxaBlock);
        const unsigned numUnalignedCharBlocks = nexusReader.GetNumUnalignedBlocks(taxaBlock);
        
        if(numUnalignedCharBlocks > 0)
            Msg::error("No unaligned data allowed!");
        if(numCharBlocks > 1)
            Msg::error("Too many char blocks (> 1)");

        for(size_t cBlock = 0; cBlock < numCharBlocks; cBlock++){
            NxsCharactersBlock* charBlock = nexusReader.GetCharactersBlock(taxaBlock, cBlock);
            std::string charBlockTitle = charBlock->GetTitle();
            int dataType = charBlock->GetDataType();
            if(dataType == NxsCharactersBlock::dna || dataType == NxsCharactersBlock::nucleotide || dataType == NxsCharactersBlock::rna){
                readNucleotideData(charBlock);
            }
            else{
                Msg::error("Cannot read this data type!");
            }
        }
        
    }
}

Alignment::~Alignment(){
    for(int i = 0; i < numTaxa; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

void Alignment::readNucleotideData(NxsCharactersBlock* charBlock){
    numTaxa = charBlock->GetNumActiveTaxa();
    numChar = charBlock->GetNumActiveChar();
    matrix = new int*[numTaxa];
    for(int i = 0; i < numTaxa; i++){
        matrix[i] = new int[numChar];
        for(int j = 0; j < numChar; j++){
            char state = charBlock->GetState(i,j);
            if(state == 'A')
                matrix[i][j] = 1;
            else if (state == 'T')
                matrix[i][j] = 2;
            else if (state == 'C')
                matrix[i][j] = 4;
            else if (state == 'G')
                matrix[i][j] = 8;
            else if (state == 'N' || state == '-' || state == '?')
                matrix[i][j] = 15;
        }
    }
}