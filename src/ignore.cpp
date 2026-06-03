#include "ignore.h"

unordered_set<string> ignore_list={".jit"};

void make_ignore_list() {
    if (exists(repo_path/".jitignore")) {
        ifstream ignore_file(repo_path/".jitignore");
        string line;
        while (getline(ignore_file, line)) {
                ignore_list.insert(line);
        }
    }
}
