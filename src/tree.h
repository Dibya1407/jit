#pragma once

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

    tree(path dir);

    void create_tree();
};

string read_tree(string tree_hash);
