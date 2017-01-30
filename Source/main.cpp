#include <fstream>
#include <stdio.h>
#include "../Header/BinaryTree.h"
#include "../Header/BitVector.h"
#include "../Header/PriorityQueue.h"
#include "../Header/Vector.h"
#include "../Header/Archive.h"
#include "../Header/Extract.h"
#include "../Header/List.h"

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
            {
                printf("Could not archive the folder. Check if the location is right. Exitting...\n");
                return 1;
            }
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
                {
                    archive.close();
                    printf("Could not extract at the specified location. Check if it's a valid path. Exitting...\n");
                    return 1;
                }
                archive.close();
            }
            catch(Extract::EXTRACTION_ERRORS& err)
            {
                if(err == Extract::CORRUPTED_ARCHIVE)
                {
                    printf("The file is not a valid archive or it is corrupted. Exitting...\n");
                    return 1;
                }
                else
                    throw err;
            }
        }
        //else if(strcmp(argv[1], "-Search") == 0
        //     || strcmp(argv[1], "-search") == 0)
        //{
        //    // Search
        //}
        else
        {
            printf("Invalid command line arguments! Use -help or -?\n");
            return 1;
        }
    }
    else if(argc == 3)
    {
        if(    strcmp(argv[1], "-List") == 0
            || strcmp(argv[1], "-list") == 0)
        {
            std::fstream archive(argv[2], std::ios::in | std::ios::binary);

            if(!archive)
            {
                printf("Could not upen the file in the given location. Exitting...\n");
                return 1;
            }

            try
            {
                ListFiles::ListFolder(archive);
            }
            catch(Extract::EXTRACTION_ERRORS& err)
            {
                printf("The file is not a valid archive or it is corrupted. Exitting...\n");
                return 1;
            }
        }
        else
        {
            printf("Invalid command line arguments! Use -help or -?\n");
            return 1;
        }
    }
    else if(argc == 2)
    {
        if(    strcmp(argv[1], "-?") == 0
            || strcmp(argv[1], "-help") == 0
            || strcmp(argv[1], "-Help") == 0)
        {
            printf("Commands:\n");
            printf("-Unpack \"<path to archive>\" \"<where to extract>\"\n");
            printf("Unpacks the archive to the specified location, overwrites current files.\n");
            printf("-Pack \"<path to what to archive>\" \"<path and filename of the archive>\"\n");
            printf("Compresses the files in the directory and creates an archive in the specified location.\n");
            printf("-List \"<path to archive>\"\n");
            printf("Lists the contents in the archive.\n");
        }
        else
        {
            printf("Invalid command line arguments! Use -help or -?\n");
            return 1;
        }
    }
    else if(argc < 2)
    {
        printf("This program requires command line arguments. Use -help or -?\n");
        return 1;
    }
    else // argc > 4
    {
        printf("This program requires less command line arguments. Use -help or -?\n");
        return 1;
    }

    return 0;
}


















