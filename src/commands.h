#pragma once

#include <filesystem>
#include <iostream>
#include <fstream>
#include "ignore.h"
#include "exceptions.h"
#include "globals.h"
#include "object.h"
#include "blob.h"
#include "tree.h"
#include "commit.h"
#include "checkout.h"
#include "log.h"

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
int do_log();
int do_checkout();
int branch();
