#pragma once

#include <filesystem>
#include <iostream>
#include <fstream>
#include "exceptions.h"
#include "globals.h"
#include "object.h"
#include "blob.h"
#include "tree.h"
#include "commit.h"

using namespace std;
using namespace std::filesystem;

int init();
int hash_object();
int cat_file();
int make_tree();
int ls_tree();
int config();
int commit_tree();
int do_commit();
