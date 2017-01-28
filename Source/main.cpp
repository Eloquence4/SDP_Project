#include <fstream>
#include <stdio.h>
#include "../Header/BinaryTree.h"
#include "../Header/BitVector.h"
#include "../Header/PriorityQueue.h"
#include "../Header/Vector.h"
#include "../Header/Archive.h"
#include "../Header/Extract.h"

const int max_buffer_size = 1048576; // 1 MB

int main(int argc, char* argv[])
{
    if(argc == 4)
    {
        if(    strcmp(argv[1], "-Pack") == 0
            || strcmp(argv[1], "-pack") == 0)
        {
            char cont = false;
            printf("Will overwrite any existing file in the specified location:\n%s\nType 'Y' to continue:\n", argv[3]);
            scanf("%c", &cont);

            if(cont != 'Y' && cont != 'y')
            {
                printf("Shutting down.\n");
                return 0;
            }

            std::fstream archive(argv[3], std::ios::out | std::ios::binary | std::ios::trunc);
            if(!archive)
            {
                printf("Could not create an archive file in the given location. Shutting down.\n");
                return 1;
            }

            if(Archive::CompressFolder(archive, argv[2], strlen(argv[2])))
                printf("Success! Exitting...\n");
            else
                printf("Could not archive the folder. Check if the location is right. Exitting...\n");
            archive.close();
        }
        else if(strcmp(argv[1], "-Unpack") == 0
             || strcmp(argv[1], "-unpack") == 0)
        {
            char cont = false;
            printf("Will overwrite any existing file in the specified location:\n%s\nType 'Y' to continue:\n", argv[3]);
            scanf("%c", &cont);

            if(cont != 'Y' && cont != 'y')
            {
                printf("Shutting down.\n");
                return 0;
            }

            std::fstream archive(argv[2], std::ios::in | std::ios::binary);
            if(!archive)
            {
                printf("Could not upen the file in the given location. Exitting...\n");
                return 1;
            }

            try
            {
                if(Extract::ExtractFolder(archive, argv[3], strlen(argv[3])))
                    printf("Success! Exitting...\n");
                else
                    printf("Could not extract at the specified location. Check if it's a valid path. Exitting...\n");
                archive.close();
            }
            catch(Extract::EXTRACTION_ERRORS& err)
            {
                if(err == Extract::CORRUPTED_ARCHIVE)
                    printf("The file is not a valid archive or it is corrupted. Exitting...\n");
                else
                    throw err;
            }
        }
        else if(strcmp(argv[1], "-Search") == 0
             || strcmp(argv[1], "-search") == 0)
        {

        }
        else
            printf("Invalid command line arguments! Use -help or -?\n");
    }
    else if(argc == 3)
    {
        if(    strcmp(argv[1], "-List") == 0
            || strcmp(argv[1], "-list") == 0)
        {

        }
    }
    else if(argc == 2)
    {
        // -help -?
    }
    else
    {
        printf("This program requires command line arguments. Use -help or -?\n");
    }

    return 0;
}


















