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

            char* fileName = new char[fileNameLen + 1];
            archive.read(fileName, fileNameLen+1);

            BinaryTree HuffmanTree = ExtractHuffmanTree(archive);

            size_t bufferLen = 0;
            char* buffer = nullptr;

            if(WhereToExtract[WhereToExtractLen - 1] != '\\')
            {
                // Length of initial path + \\ + len of new fodler + '\0'
                bufferLen = WhereToExtractLen + 1 + fileNameLen + 1;
                buffer = new char[bufferLen];
                sprintf(buffer, "%s\\%s", WhereToExtract, fileName);
            }
            else
            {
                // Length of initial path + len of new fodler + '\0'
                bufferLen = WhereToExtractLen + fileNameLen + 1;
                buffer = new char[bufferLen];
                sprintf(buffer, "%s%s", WhereToExtract, fileName);
            }

            std::fstream file(buffer, std::ios::out | std::ios::trunc);

            ExtractFile(archive, file, HuffmanTree);
            delete[] fileName;
            delete[] buffer;
        }
        else // state == DIRECTORY_END
            return true;
    }

}

BinaryTree Extract::ExtractHuffmanTree(std::fstream& archive)
{
    BinaryTree HuffmanTree;

    HuffmanTree.BinaryImport(archive);

    return HuffmanTree;
}

void Extract::ExtractFile(std::fstream& archive, std::fstream& file, const BinaryTree& HuffmanTree)
{

}





























