#pragma once

#include <unordered_map>
#include <unordered_set>
#include "globals.h"
#include "ignore.h"
#include "exceptions.h"
#include "commit.h"

void checkout(string commit_hash);

void checkout_tree(string tree_hash, path dirpath);