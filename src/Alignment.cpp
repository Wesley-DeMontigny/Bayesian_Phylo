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
    delete [] matrix[0];
    delete [] matrix;
}

DoubleMatrix Alignment::getPairwiseIdentities(){
    DoubleMatrix pairwiseID(numTaxa);

    for(int i = 0; i < numTaxa; i++){
        for(int j = i; j < numTaxa; j++){
            if(i == j)
                pairwiseID(i, j) = 1.0;
            else{
                double pID = 0.0;
                for(int k = 0; k < numChar; k++)
                    pID += matrix[i][k] == matrix[j][k]; // This implementation isn't ambiguous-character-friendly
                
                pID = pID/numChar;
                pairwiseID(i, j) = pID;
                pairwiseID(j, i) = pID;
            }
        }
    }

    return pairwiseID;
}

void Alignment::readNucleotideData(NxsCharactersBlock* charBlock){
    numTaxa = charBlock->GetNumActiveTaxa();
    numChar = charBlock->GetNumActiveChar();
    matrix = new int*[numTaxa];
    matrix[0] = new int[numTaxa*numChar];
    for(int i = 1; i < numTaxa; i++)
        matrix[i] = matrix[i-1] + numChar;
    for(int i = 0; i < numTaxa; i++)
        for(int j = 0; j < numChar; j++)
            matrix[i][j] = 0;

    for(int i = 0; i < numTaxa; i++){
        taxaNames.push_back(charBlock->GetTaxonLabel(i));
        for(int j = 0; j < numChar; j++){
            char state = charBlock->GetState(i,j);
            if(state == 'A' || state == 'R' || state == 'W' || state == 'M' || state == 'D' || state == 'H' || state == 'V')
                matrix[i][j] += 1;
            if(state == 'C' || state == 'Y' || state == 'S' || state == 'M' || state == 'B' || state == 'H' || state == 'V')
                matrix[i][j] += 2;
            if(state == 'G' || state == 'R' || state == 'S' || state == 'K' || state == 'B' || state == 'D' || state == 'V')
                matrix[i][j] += 4;
            if(state == 'T' || state == 'Y' || state == 'W' || state == 'K' || state == 'B' || state == 'D' || state == 'H')
                matrix[i][j] += 8;
            if(state == 'N' || state == '-' || state == '?')
                matrix[i][j] = 15;
        }
    }
}