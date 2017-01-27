#include <Windows.h>
#include <stdio.h>
#include "../Header/Extract.h"

bool Extract::ExtractFolder(std::fstream& archive, const char* WhereToExtract, size_t WhereToExtractLen)
{
    while(archive.good())
    {
        File_Folder_States state = NO_STATE;

        archive.read((char*)&state, sizeof(state));

        if(state == DIRECTORY_START)
        {
            size_t new_folder_name_size = 0;
            archive.read((char*) &new_folder_name_size, sizeof(new_folder_name_size));

            size_t bufferLen = 0;
            char* buffer = nullptr;

            if(WhereToExtract[WhereToExtractLen-1] != '\\')
            {
                // Length of initial path + \\ + len of new fodler + '\0'
                bufferLen = WhereToExtractLen + 1 + new_folder_name_size + 1;
                buffer = new char[bufferLen];
                sprintf(buffer, "%s\\", WhereToExtract);

                // bufferEnd will be right after the '\\' in buffer
                char* bufferEnd = buffer + WhereToExtractLen + 1;
                archive.read(bufferEnd, new_folder_name_size + 1);
            }
            else
            {
                // Length of initial path + len of new fodler + '\0'
                bufferLen = WhereToExtractLen + new_folder_name_size + 1;
                buffer = new char[bufferLen];
                sprintf(buffer, "%s", WhereToExtract);

                // bufferEnd will be at '\0'
                char* bufferEnd = buffer + WhereToExtractLen;
                archive.read(bufferEnd, new_folder_name_size + 1);
            }

            if(CreateDirectory(buffer, nullptr) == ERROR_PATH_NOT_FOUND)
            {
                delete[] buffer;
                return false;
            }

            ExtractFolder(archive, buffer, bufferLen);
            delete[] buffer;
        }
        else if(state == FILE_START)
        {
            size_t fileNameLen = 0;
            archive.read((char*) &fileNameLen, sizeof(fileNameLen));

            char* filePath;
            char* fileName;

            size_t filePathLen = 0;

            if(WhereToExtract[WhereToExtractLen - 1] != '\\')
            {
                // Length of initial path + \\ + len of new fodler + '\0'
                filePathLen = WhereToExtractLen + 1 + fileNameLen + 1;
                filePath = new char[filePathLen];

                // FileName will be right after the \\ in FilePath
                fileName = filePath + WhereToExtractLen + 1;

                sprintf(filePath, "%s\\", WhereToExtract);
                archive.read(fileName, fileNameLen + 1);
            }
            else
            {
                // Length of initial path + len of new fodler + '\0'
                filePathLen = WhereToExtractLen + fileNameLen + 1;
                filePath = new char[filePathLen];

                // FileName will be right after the \\ in FilePath
                fileName = filePath + WhereToExtractLen;

                sprintf(filePath, "%s", WhereToExtract);
                archive.read(fileName, fileNameLen + 1);
            }

            archive.read(fileName, fileNameLen+1);

            BinaryTree HuffmanTree = ExtractHuffmanTree(archive);

            std::fstream file(filePath, std::ios::out | std::ios::trunc);
            if(!file) // Can only happen if the directory doesn't exist
            {
                // Error
            }

            ExtractFile(archive, file, HuffmanTree);
            delete[] fileName;
        }
        else // state == DIRECTORY_END
            return true;
    }
    return true;
}

BinaryTree Extract::ExtractHuffmanTree(std::fstream& archive)
{
    BinaryTree HuffmanTree;

    HuffmanTree.BinaryImport(archive);

    return HuffmanTree;
}

void Extract::ExtractFile(std::fstream& archive, std::fstream& file, const BinaryTree& HuffmanTree)
{
    size_t HuffmanTreeHeight = HuffmanTree.height();
    size_t pos = 0;

    size_t bitSetMaxOverhead = (HuffmanTreeHeight / 64) + 1;

    BitVector bitSet;
    bitSet.resize(bitSetMaxOverhead);
    bitSet.readSet(archive, 0, bitSetMaxOverhead-1);

    char curLetter;

    while(curLetter = HuffmanTree.extractBits(bitSet, pos))
    {
        if(curLetter == -1)
            throw CORRUPTED_ARCHIVE;

        file << curLetter;

        while(pos >= BitVector::dataSize)
        {
            bitSet.remove(0);
            bitSet.readSet(archive, bitSetMaxOverhead - 1, bitSetMaxOverhead - 1);
            pos -= BitVector::dataSize;
        }
    }
}





























