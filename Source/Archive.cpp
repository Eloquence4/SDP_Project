#include "../Header/Archive.h"

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
    file.write((char*) &fileNameLen, sizeof(size_t));
    file.write(fileName, fileNameLen);
    HuffmanTree.BinaryExport(file);
}

void Archive::Compress(std::fstream& from, std::fstream& target, const BinaryTree& HuffmanTree)
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








