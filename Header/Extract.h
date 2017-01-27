#ifndef ExtractDec
#define ExtractDec

#include <string> // For strcmp() and strlen()
#include <fstream>
#include "BinaryTree.h"
#include "BitVector.h"
#include "PriorityQueue.h"
#include "Vector.h"
#include "dirent.h"

namespace Extract
{
    enum File_Folder_States
    {
        NO_STATE,
        DIRECTORY_START,
        DIRECTORY_END,
        FILE_START,
        CORRUPTED_ARCHIVE
    };

    bool ExtractFolder(std::fstream& archive, const char* WhereToExtract, size_t WhereToExtractLen);

    BinaryTree ExtractHuffmanTree(std::fstream& archive);
    void ExtractFile(std::fstream& archive, std::fstream& file, const BinaryTree& HuffmanTree);
}

#endif // ExtractDec
