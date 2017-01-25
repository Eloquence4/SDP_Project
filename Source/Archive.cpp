#include <stdio.h>
#include "../Header/Archive.h"

bool Archive::CompressFolder(std::fstream& archive, const char* folder_name, size_t folder_name_len)
{
    File_Folder_States state = NO_STATE;
    WIN32_FIND_DATA fdFile;
    HANDLE hFind;
    char* buffer = nullptr;

    if(folder_name[folder_name_len - 1] == '\\')
    {
        buffer = new char[folder_name_len + 4];
        sprintf(buffer, "%s*.*", buffer);
    }
    else
    {
        buffer = new char[folder_name_len + 5];
        sprintf(buffer, "%s\\*.*", buffer);
    }

    // folder_name is not a valid path to a folder
    if((hFind = FindFirstFile(buffer, &fdFile)) == INVALID_HANDLE_VALUE)
    {
        delete[] buffer;
        return false;
    }

    // Metadata for the folder
    state = DIRECTORY_START;
    unsigned file_name_size = strlen(folder_name) + 1;
    archive.write((char*) &state, sizeof(state));
    archive.write((char*) &file_name_size, sizeof(file_name_size));
    archive.write(folder_name, file_name_size + 1);
    ///

    while(true)
    {
        if(!FindNextFile(hFind, &fdFile))
        {
            state = DIRECTORY_END;
            archive.write((char*)&state, sizeof(state));
            FindClose(hFind);
            return true; // Successfully traversed the folder and archived all the files
        }

        if(strcmp(fdFile.cFileName, ".") == 0)
            continue;
        if(strcmp(fdFile.cFileName, "..") == 0)
            continue;

        // If pent is a folder, recursively compress that
        if(fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            CompressFolder(archive, fdFile.cFileName, strlen(fdFile.cFileName));
        else // Otherwise compress the file
        {
            std::fstream file(fdFile.cFileName, std::ios::in);
            if(!file)
            {
                // Some message
            }
            BinaryTree HuffmanTree = ConstructHuffmanTree(file);
            FillMetaData(archive, HuffmanTree, fdFile.cFileName, strlen(fdFile.cFileName));
            CompressFile(file, archive, HuffmanTree);
        }
    }
}

BinaryTree Archive::ConstructHuffmanTree(std::fstream& file)
{
    file.clear();
    file.seekg(0, std::ios::beg);

    Vector data;

    FillWeightVector(file, data);

    PriorityQueue queue;
    for(size_t i = 0; i < data.size(); i++)
        queue.push(BinaryTree(data[i]));

    while(queue.size() > 1)
        queue.push(queue.pop() + queue.pop());

    return queue.pop();
}

void Archive::FillWeightVector(std::fstream& file, Vector& data)
{
    while(file.good())
    {
        char letter = file.get();
        size_t i = 0;

        for(; i < data.size(); i++)
        {
            if(data[i].letter == letter)
            {
                data[i].weight++;
                break;
            }
        }

        if(i == data.size())
            data.push_back(Tree_Node(1, letter));
    }
}

void Archive::FillMetaData(std::fstream& file, const BinaryTree& HuffmanTree, const char* fileName, size_t fileNameLen)
{
    File_Folder_States state = FILE_START;
    file.write((char*) &state, sizeof(state));
    file.write((char*) &fileNameLen, sizeof(fileNameLen));
    file.write(fileName, fileNameLen+1);
    HuffmanTree.BinaryExport(file);
}

void Archive::CompressFile(std::fstream& from, std::fstream& target, const BinaryTree& HuffmanTree)
{
    from.clear();
    from.seekg(0, std::ios::beg);

    BitVector bits;
    size_t pos = 0;

    while(from.good())
    {
        char letter = from.get();

        HuffmanTree.search(bits, pos, letter);

        if(pos >= BitVector::dataSize)
        {
            target.write((char*) &bits.getBitSet(0), BitVector::dataSize);
            bits.remove(0);
            pos -= BitVector::dataSize;
        }
    }

    if(pos != 0)
        target.write((char*) &bits.getBitSet(0), BitVector::dataSize);
}








