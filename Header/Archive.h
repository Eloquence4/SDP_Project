#ifndef ArchiveDec
#define ArchiveDec

#include <string> // For strcmp() and strlen()
#include <fstream>
#include "BinaryTree.h"
#include "BitVector.h"
#include "PriorityQueue.h"
#include "Vector.h"
#include "dirent.h"

enum File_Folder_States
{
    NO_STATE,
    DIRECTORY_START,
    DIRECTORY_END,
    FILE_START
};

namespace Archive
{
    bool CompressFolder(std::fstream& archive, const char* folder_name);

    BinaryTree ConstructHuffmanTree(std::fstream& file);

    void FillWeightVector(std::fstream& file, Vector& data);

    void FillMetaData(std::fstream& file, const BinaryTree& HuffmanTree, const char* fileName, size_t fileNameLen);

    void CompressFile(std::fstream& from, std::fstream& target, const BinaryTree& HuffmanTree);
}

#endif // ArchiveDec
