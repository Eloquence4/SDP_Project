#include <Windows.h>
#include <stdio.h>
#include "../Header/Extract.h"

bool Extract::ExtractFolder(std::fstream& archive, const char* WhereToExtract, size_t WhereToExtractLen)
{
    while(archive.good())
    {
        File_Folder_States state = NO_STATE;

        archive.read((char*)&state, sizeof(state));

        if(!archive.good())
            return true;

        if(state == DIRECTORY_START)
        {
            char* buffer = nullptr;
            size_t bufferLen = ReadFileName(buffer, archive, WhereToExtract, WhereToExtractLen);

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
            char* filePath = nullptr;

            ReadFileName(filePath, archive, WhereToExtract, WhereToExtractLen);

            BinaryTree HuffmanTree = ExtractHuffmanTree(archive);

            std::fstream file(filePath, std::ios::out | std::ios::trunc);
            if(!file)
            {
                printf("Could not create a file at this location:\n%s\n", filePath);
                delete[] filePath;

                // Find the next file/directory
                while(state != DIRECTORY_END && state != DIRECTORY_START && state != FILE_START)
                {
                    archive.read((char*)&state, sizeof(state));
                    if(!archive.good())
                        return true;
                }
                archive.seekg(-sizeof(state), std::ios::cur);
                // Then continue
                continue;
            }

            ExtractFile(archive, file, HuffmanTree, filePath);
            delete[] filePath;
            file.close();
        }
        else if(state == DIRECTORY_END)
            return true;
        else
            throw CORRUPTED_ARCHIVE;
    }
    return true;
}

BinaryTree Extract::ExtractHuffmanTree(std::fstream& archive)
{
    BinaryTree HuffmanTree;

    HuffmanTree.BinaryImport(archive);

    return HuffmanTree;
}

void Extract::ExtractFile(std::fstream& archive, std::fstream& file, const BinaryTree& HuffmanTree, const char* fileName)
{
    size_t pos = 0;

    BitVector bitSet;
    bitSet.resize(1);
    bitSet.readSet(archive, 0, 0);

    size_t step = HuffmanTree.weight();
    size_t letters = 0;
    int percent = 0;

    char curLetter;

    size_t oldPos = 0;

    while(curLetter = HuffmanTree.extractBits(bitSet, pos))
    {
        if(curLetter == -1)
        {
            pos = oldPos;

            if(pos < BitVector::dataSize)
            {
                bitSet.resize(bitSet.bitSetSize()+1);
                bitSet.readSet(archive, bitSet.bitSetSize()-1, bitSet.bitSetSize()-1);
            }
            else
            {
                bitSet.remove(0);
                bitSet.readSet(archive, bitSet.bitSetSize()-1, bitSet.bitSetSize()-1);
                pos -= BitVector::dataSize;
            }
            continue;
        }
        file << curLetter;
        oldPos = pos;
        letters += 20;

        if(percent != 100 && letters >= step)
        {
            letters = 0;
            printf("%s: %d%% done!\n", fileName, percent);
            percent += 5;
        }
    }
    printf("%s: 100%% done!\n", fileName);
}

size_t Extract::ReadFileName(char*& file_path, std::fstream& archive, const char* dir_path, size_t dir_path_len)
{
    size_t file_name_len = 0;
    archive.read((char*)&file_name_len, sizeof(file_name_len));

    size_t file_path_len = 0;
    char* file_name = nullptr;

    if(dir_path[dir_path_len - 1] != '\\')
    {
        // Length of initial path + \\ + len of new fodler + '\0'
        file_path_len = dir_path_len + 1 + file_name_len + 1;
        file_path = new char[file_path_len];

        // FileName will be right after the \\ in FilePath
        file_name = file_path + dir_path_len + 1;

        sprintf(file_path, "%s\\", dir_path);
        archive.read(file_name, file_name_len + 1);
    }
    else
    {
        // Length of initial path + len of new fodler + '\0'
        file_path_len = dir_path_len + file_name_len + 1;
        file_path = new char[file_path_len];

        // FileName will be right after the \\ in FilePath
        file_name = file_path + dir_path_len;

        sprintf(file_path, "%s", dir_path);
        archive.read(file_name, file_name_len + 1);
    }

    // -1 because the null character is included in file_path_len
    return file_path_len-1;
}



























