#pragma once

#include "globals.h"
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
    void create_blob_file();
};

string read_blob(string blob_hash);
