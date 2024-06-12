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
            dataType = charBlock->GetDataType();
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
}

void Alignment::readNucleotideData(NxsCharactersBlock* charBlock){
    numTaxa = charBlock->GetNumActiveTaxa();
    numChar = charBlock->GetNumActiveChar();
    matrix.clear();

    for(int i = 0; i < numTaxa; i++){
        std::vector<int> taxaChars;
        taxaChars.reserve(numChar);
        taxaNames.push_back(charBlock->GetTaxonLabel(i));
        for(int j = 0; j < numChar; j++){
            char state = charBlock->GetState(i,j);
            if(state == 'A')
                taxaChars.push_back(1);
            else if (state == 'C')
                taxaChars.push_back(2);
            else if (state == 'G')
                taxaChars.push_back(4);
            else if (state == 'T')
                taxaChars.push_back(8);
            else if (state == 'N' || state == '-' || state == '?')
                taxaChars.push_back(15);
        }
        
        matrix.push_back(taxaChars);
    }
}