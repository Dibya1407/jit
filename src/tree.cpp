#include "tree.h"

tree::tree(path dir) {
    if (!is_directory(dir)) {
        cerr<< "not a directory path can't make a tree\n";
        return;
    }

    

}