#pragma once

#include "exceptions.h"
#include "object.h"
#include "blob.h"
#include "tree.h"
#include <ctime>

using namespace std;
using namespace std::filesystem;

class commit{
    private:
    string contents;
    string tree_hash;

    public:
    string hash;
    string hash_raw;

    commit(string tree_hash,string message,string parent_hash="",bool log=false);

    void create_commit_file();
};

bool is_commit(string object_hash);

void make_commit(string message);

string get_head_hash();