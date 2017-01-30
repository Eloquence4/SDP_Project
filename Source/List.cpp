#include "../Header/List.h"
#include "../Header/Extract.h"

bool ListFiles::ListFolder(std::fstream& archive, const char* curFolderName, size_t curFolderNameLen)
{
    while(archive.good())
    {
        File_Folder_States state = NO_STATE;

        while(state != DIRECTORY_END && state != DIRECTORY_START && state != FILE_START)
        {
            archive.read((char*)&state, sizeof(state));
            if(!archive.good())
                return true;
        }

        if(!archive.good())
            return true;

        if(state == DIRECTORY_START)
        {
            size_t new_folder_name_size = 0;
            archive.read((char*)&new_folder_name_size, sizeof(new_folder_name_size));

            char* buffer = nullptr;
            size_t bufferLen = 0;

            if(curFolderName == nullptr)
            {
                bufferLen = new_folder_name_size;
                buffer = new char[bufferLen + 1];
                archive.read(buffer, bufferLen+1);
            }
            else
            {
                bufferLen = curFolderNameLen + 1 + new_folder_name_size;
                buffer = new char[bufferLen + 1];
                char* newNameBeg = buffer + curFolderNameLen + 1;

                sprintf(buffer, "%s\\", curFolderName);
                archive.read(newNameBeg, new_folder_name_size+1);
            }

            printf("%s\\\n", buffer);
            ListFolder(archive, buffer, bufferLen);
            delete[] buffer;
        }
        else if(state == FILE_START)
        {
            size_t fileNameLen = 0;
            archive.read((char*)&fileNameLen, sizeof(fileNameLen));

            char* filePath = new char[fileNameLen + 1];
            archive.read(filePath, fileNameLen + 1);

            printf("%s\\%s\n", curFolderName, filePath);

            delete[] filePath;
            
            BinaryTree HuffmanTree = Extract::ExtractHuffmanTree(archive);

            //skipFile(archive, HuffmanTree);
        }
        else if(state == DIRECTORY_END)
            return true;
        else
            throw Extract::CORRUPTED_ARCHIVE;
    }
    return true;
}

//void ListFiles::skipFile(std::fstream& archive, const BinaryTree& HuffmanTree)
//{
//    int nullCharHeight = HuffmanTree.height('\0');
//    int maxBitSetOverhead = (nullCharHeight / BitVector::dataSize) + 2;
//    char* pattern = nullptr;
//
//    HuffmanTree.CreatePattern(pattern, '\0');
//
//    BitVector vect;
//    vect.resize(maxBitSetOverhead);
//
//    for(int i = 0; i < maxBitSetOverhead; i++)
//    {
//        vect.readSet(archive, i, i);
//
//        if(vect.findPattern(pattern, nullCharHeight))
//        {
//            delete[] pattern;
//            return;
//        }
//    }
//
//    do
//    {
//        if(!archive.good())
//        {
//            delete[] pattern;
//            throw Extract::CORRUPTED_ARCHIVE;
//        }
//
//        vect.remove(0);
//        vect.readSet(archive, maxBitSetOverhead - 1, maxBitSetOverhead - 1);
//    } while(!vect.findPattern(pattern, nullCharHeight));
//
//    delete[] pattern;
//}






















