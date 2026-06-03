#pragma once

#include "exceptions.h"
#include "object.h"
#include "blob.h"
#include "tree.h"

using namespace std;
using namespace std::filesystem;

class commit{
    private:
    string contents;
    string tree_hash;

    public:
    string hash;
    string hash_raw;

    commit(string tree_hash,string message,string parent_hash="");

    void create_commit_file();
};

bool is_commit(string object_hash);

void make_commit(string message,string parent_hash="");