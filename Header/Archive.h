#ifndef ArchiveDec
#define ArchiveDec

#include <string> // For strcmp() and strlen()
#include <fstream>
#include <Windows.h>
#include "BinaryTree.h"
#include "BitVector.h"
#include "PriorityQueue.h"
#include "Vector.h"

namespace Archive
{
    enum File_Folder_States
    {
        NO_STATE,
        DIRECTORY_START,
        DIRECTORY_END,
        FILE_START
    };

    bool CompressFolder(std::fstream& archive, const char* folder_name, size_t folder_name_len);

    BinaryTree ConstructHuffmanTree(std::fstream& file);

    void FillWeightVector(std::fstream& file, Vector& data);

    void FillMetaData(std::fstream& file, const BinaryTree& HuffmanTree, const char* fileName, size_t fileNameLen);

    void CompressFile(std::fstream& from, std::fstream& target, const BinaryTree& HuffmanTree);

    void DirectoryMetaData(std::fstream& file, const char* folder_name, size_t folder_name_len);
    
    size_t CreatePath(char*& buffer, const char* path, const char* name, size_t path_len, size_t name_len);
}

#endif // ArchiveDec
