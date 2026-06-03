#pragma once

#include <filesystem>
#include <vector>
#include <unordered_set>

using namespace std;
using namespace std::filesystem;

extern path repo_path;
extern vector<string> args;
extern unordered_set<string> ignore_list;