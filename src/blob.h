#pragma once

#include <vector>
#include "object.h"

using namespace std;
using namespace std::filesystem;

class blob {
    private:
    path filepath;
    string blob_contents;

    public:
    string hash;
    string hash_raw;

    blob(path filepath);
    void create_blob_file(path repo_path);

};

string read_blob(string blob_hash,path repo_path);
