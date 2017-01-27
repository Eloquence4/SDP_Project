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
        sprintf(buffer, "%s*.*", folder_name);
    }
    else
    {
        buffer = new char[folder_name_len + 5];
        sprintf(buffer, "%s\\*.*", folder_name);
    }

    // folder_name is not a valid path to a folder
    if((hFind = FindFirstFile(buffer, &fdFile)) == INVALID_HANDLE_VALUE)
    {
        delete[] buffer;
        return false;
    }

    delete[] buffer;

    // Metadata for the folder
    DirectoryMetaData(archive, folder_name, folder_name_len);
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
        {
            size_t newFolderLen = strlen(fdFile.cFileName);

            if(folder_name[folder_name_len - 1] == '\\')
            {
                newFolderLen = folder_name_len + newFolderLen + 1;
                buffer = new char[newFolderLen];
                sprintf(buffer, "%s%s", folder_name, fdFile.cFileName);
            }
            else
            {
                newFolderLen = folder_name_len + 1 + newFolderLen + 1;
                buffer = new char[newFolderLen];
                sprintf(buffer, "%s\\%s", folder_name, fdFile.cFileName);
            }

            CompressFolder(archive, buffer, strlen(fdFile.cFileName));
            delete[] buffer;
        }
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

void Archive::DirectoryMetaData(std::fstream& file, const char* folder_name, size_t folder_name_len)
{
    File_Folder_States state = DIRECTORY_START;

    // If folder_name is a filepath to the folder, this will point to where the folder_name actually is
    const char* folder_name_beg = nullptr;

    size_t folder_name_beg_len = folder_name_len - 2;

    while(true)
    {
        if(folder_name[folder_name_beg_len] == '\\')
        {
            folder_name_beg = folder_name + folder_name_beg_len + 1;
            break;
        }
        if(folder_name_beg_len == 0)
        {
            folder_name_beg = folder_name;
            break;
        }
        folder_name_beg_len--;
    }

    folder_name_beg_len = folder_name_len - folder_name_beg_len - 1;
    if(folder_name[folder_name_len-1] == '\\')
    {
        folder_name_beg_len--;
        char tmp = '\0';
        file.write((char*)&state, sizeof(state));
        file.write((char*)&folder_name_beg_len, sizeof(folder_name_beg_len));
        file.write(folder_name_beg, folder_name_beg_len);
        file.write((char*)&tmp, 1);
    }
    else
    {
        file.write((char*)&state, sizeof(state));
        file.write((char*)&folder_name_beg_len, sizeof(folder_name_beg_len));
        file.write(folder_name_beg, folder_name_beg_len + 1);
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

        HuffmanTree.writeBits(bits, pos, letter);

        while(pos >= BitVector::dataSize)
        {
            target.write((char*) &bits.getBitSet(0), BitVector::dataSize);
            bits.remove(0);
            pos -= BitVector::dataSize;
        }
    }

    if(pos != 0)
        target.write((char*) &bits.getBitSet(0), BitVector::dataSize);
}








