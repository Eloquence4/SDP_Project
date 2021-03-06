#include <stdio.h>
#include "../Header/Archive.h"

bool Archive::CompressFolder(std::fstream& archive, const char* folder_name, size_t folder_name_len)
{
    File_Folder_States state = NO_STATE;
    WIN32_FIND_DATA fdFile;
    HANDLE hFind;
    char* buffer = nullptr;

    CreatePath(buffer, folder_name, "*.*", folder_name_len, 3);

    // if folder_name is not a valid path to a folder
    if((hFind = FindFirstFile(buffer, &fdFile)) == INVALID_HANDLE_VALUE)
    {
        delete[] buffer;
        return false;
    }

    delete[] buffer;

    // Metadata for the folder
    DirectoryMetaData(archive, folder_name, folder_name_len);

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

        // If we find a folder, recursively compress it
        if(fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            size_t bufferLen = CreatePath(buffer, folder_name, fdFile.cFileName, folder_name_len, strlen(fdFile.cFileName));
            CompressFolder(archive, buffer, bufferLen);
            delete[] buffer;
        }
        else // Otherwise compress the file
        {
            CreatePath(buffer, folder_name, fdFile.cFileName, folder_name_len, strlen(fdFile.cFileName));
            
            std::fstream file(buffer, std::ios::in);

            if(!file) 
            {
                printf("The following file could not be opened to be archived:\n%s\n", buffer);
                delete[] buffer;
                continue;
            }

            BinaryTree HuffmanTree = ConstructHuffmanTree(file);
            FillMetaData(archive, HuffmanTree, fdFile.cFileName, strlen(fdFile.cFileName));
            CompressFile(file, archive, HuffmanTree, buffer);
            delete[] buffer;
            file.close();
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

    data.resize(256); // The maximum amount of letters we can have
    FillWeightVector(file, data);

    data.shrink_to_fit();

    size_t dataSize = data.size();

    PriorityQueue queue;
    queue.resize(dataSize);

    for(size_t i = 0; i < dataSize; i++)
        queue.push(BinaryTree(data[i]));

    while(queue.size() > 1)
    {
        BinaryTree lhs = queue.pop();
        BinaryTree rhs = queue.pop();
        lhs += std::move(rhs);
        queue.push(std::move(lhs));
    }

    return queue.pop();
}

void Archive::FillWeightVector(std::fstream& file, Vector& data)
{
    while(file.good())
    {
        char letter = file.get();
        if(letter == -1)
        {
            data.push_back(Tree_Node(1, '\0'));
            break;
        }
        size_t i = 0;
        size_t dataSize = data.size();

        for(; i < dataSize; i++)
        {
            if(data[i].letter == letter)
            {
                data[i].weight++;
                break;
            }
        }

        if(i == dataSize)
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

void Archive::CompressFile(std::fstream& from, std::fstream& target, const BinaryTree& HuffmanTree, const char* fileName)
{
    from.clear();
    from.seekg(0, std::ios::beg);

    size_t step = HuffmanTree.weight();
    size_t letters = 0;

    int percent = 5;

    BitVector bits;
    size_t pos = 0;

    while(from.good())
    {
        char letter = from.get();

        if(letter == -1)
            letter = '\0';

        HuffmanTree.writeBits(bits, pos, letter);
        letters += 20;

        while(pos >= BitVector::dataSize)
        {
            target.write((char*) &bits.getBitSet(0), sizeof(unsigned long long));
            bits.remove(0);
            pos -= BitVector::dataSize;
        }

        if(percent != 100 && letters >= step)
        {
            letters = 0;
            printf("%s: %d%% done!\n", fileName, percent);
            percent += 5;
        }
    }

    printf("%s: 100%% done!\n", fileName);

    if(pos != 0)
        target.write((char*) &bits.getBitSet(0), sizeof(unsigned long long));
}

size_t Archive::CreatePath(char*& buffer, const char* path, const char* name, size_t path_len, size_t name_len)
{
    size_t bufferLen = 0;
    if(path[path_len - 1] == '\\')
    {
        bufferLen = path_len + name_len + 1;
        buffer = new char[bufferLen];
        sprintf(buffer, "%s%s", path, name);
    }
    else
    {
        bufferLen = path_len + 1 + name_len + 1;
        buffer = new char[bufferLen];
        sprintf(buffer, "%s\\%s", path, name);
    }
    // BufferLen-1 because BufferLen includes the null character '\0'
    return bufferLen-1;
}

