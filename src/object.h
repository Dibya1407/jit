#pragma once

#include <filesystem>
#include <string>
#include <fstream>
#include <iostream>
#include <zlib.h>
#include <vector>
#include "globals.h"
#include "sha1.h"

using namespace std;
using namespace std::filesystem;

class object {
    private:
    string contents;

    public:
    string hash;
    string hash_raw;

    object(string content);

    void create_object_file();
};

string decompress_object(string object_hash);
