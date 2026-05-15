#include <filesystem>
#include <fstream>
#include <iostream>
#include "sha1.h"

using namespace std;
using namespace std::filesystem;

class tree {
    public:
    string hash;

    tree(path dir);
};
