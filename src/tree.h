#pragma once

#include "exceptions.h"
#include <algorithm>
#include "globals.h"
#include "blob.h"
#include "object.h"

using namespace std;
using namespace std::filesystem;

class tree {
    private:
    string tree_contents="";

    public:
    string hash;
    string hash_raw;

    tree(path dir,bool make_blobs=false);

    void create_tree_file();
};

bool is_tree(string object_hash);

string read_tree(string tree_hash);
