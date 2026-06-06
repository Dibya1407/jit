#pragma once

#include <unordered_map>
#include <unordered_set>
#include "globals.h"
#include "ignore.h"
#include "exceptions.h"
#include "commit.h"
#include "branch.h"

void checkout(string commit_hash,bool direct=false);

void checkout_tree(string tree_hash, path dirpath);

void checkout_branch(string branch_name);