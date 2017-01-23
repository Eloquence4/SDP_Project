#include "../Header/Archive.h"

bool Archive::CompressFolder(std::fstream& archive, const char* folder_name)
{
    File_Folder_States state = NO_STATE;
    DIR* pDir = opendir(folder_name);

    if(!pDir) // folder_name is not a valid path to a folder, is probably a file
        return false;

    // Metadata for the folder
    state = DIRECTORY_START;
    unsigned file_name_size = strlen(folder_name);
    archive.write((char*) &state, sizeof(state));
    archive.write((char*) &file_name_size, sizeof(file_name_size));
    archive.write(folder_name, file_name_size);
    ///

    struct dirent* pent = nullptr;

    while(pent = readdir(pDir))
    {
        if(!pent)
        {
            state = DIRECTORY_END;
            archive.write((char*)&state, sizeof(state));
            closedir(pDir);
            return true; // Successfully traversed the folder and archived all the files
        }

        if(strcmp(pent->d_name, ".") == 0)
            continue;
        if(strcmp(pent->d_name, "..") == 0)
            continue;

        // Will return false only if this is a file
        // If pend->d_name is a folder, it will recursively archive all the files in it
        if(CompressFolder(archive, pent->d_name))
            continue;

        // The loop would execture this code only if pent->d_name is a file
        std::fstream file(pent->d_name, std::ios::in);
        BinaryTree HuffmanTree = ConstructHuffmanTree(file);
        FillMetaData(archive, HuffmanTree, pent->d_name, pent->d_namlen);
        CompressFile(file, archive, HuffmanTree);
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
    file.write((char*) &fileNameLen, sizeof(size_t));
    file.write(fileName, fileNameLen);
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








