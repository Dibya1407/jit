#pragma once
#include <filesystem>
#include <fstream>
#include <string.h>
#include <iostream>

using namespace std;
using namespace std::filesystem;

class blob {
    private:
    path filepath;
    string blob_contents;

    public:
    string hash;

    blob(path filepath);

    void create_blob_file(path repo_path);
};
