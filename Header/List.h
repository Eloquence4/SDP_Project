#ifndef ListFilesDec
#define ListFilesDec

#include <string> // For strcmp() and strlen()
#include <fstream>
#include "BinaryTree.h"
#include "BitVector.h"
#include "PriorityQueue.h"
#include "Vector.h"
#include "FileStates.h"

namespace ListFiles
{
    bool ListFolder(std::fstream& archive, const char* curFolderName = nullptr, size_t curFolderNameLen = 0);
    //void skipFile(std::fstream& archive, const BinaryTree& HuffmanTree);
}

#endif // ListDec
