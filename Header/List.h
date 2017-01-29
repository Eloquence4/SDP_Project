#ifndef ListFilesDec
#define ListFilesDec



namespace ListFiles
{
    bool ListFolder(std::fstream& archive);
    bool ListFolder(std::fstream& archive, const char* curFolderName);

    size_t CreatePath(char*& buffer, const char* path, const char* name, size_t path_len, size_t name_len);
}

#endif // ListDec
