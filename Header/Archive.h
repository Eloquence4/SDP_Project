#ifndef ArchiveDec
#define ArchiveDec

#include <fstream>
#include "BinaryTree.h"
#include "BitVector.h"
#include "PriorityQueue.h"
#include "Vector.h"

namespace Archive
{
    BinaryTree ConstructHuffmanTree(std::fstream& file);

    void FillWeightVector(std::fstream& file, Vector& data);

    void FillMetaData(std::fstream& file, const BinaryTree& HuffmanTree, const char* fileName, size_t fileNameLen);

    void Compress(std::fstream& from, std::fstream& target, const BinaryTree& HuffmanTree);
}

#endif // ArchiveDec
