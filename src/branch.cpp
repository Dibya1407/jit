#include "branch.h"

void create_branch(string branch_name) {
    if (!exists(repo_path/".jit/HEAD")) {
        throw RepoNotFound("Repository not initialized. Please run 'jit init' first.");
    }
    if (is_branch(branch_name)) {
        throw JitError("branch with name "+branch_name+" already exists.");
    }

    string parent_hash=get_head_hash();
    string branch_path="refs/heads/"+branch_name;

    // ofstream head_file(repo_path/".jit/HEAD");
    // head_file << branch_path;
    // head_file.close();

    ofstream branch_file(repo_path/".jit"/branch_path);
    branch_file << parent_hash;
    branch_file.close();
}

bool is_branch(string branch_name) {
    if (exists(repo_path/".jit/refs/heads"/branch_name)) {
        return true;
    }
    return false;
}

void list_branches() {
    if (!exists(repo_path/".jit/refs/heads")) {
        throw RepoNotFound("Repository not initialized. Please run 'jit init' first.");
    }

    ifstream head_file(repo_path/".jit/HEAD");
    string head_contents;
    getline(head_file, head_contents);
    head_file.close();

    for (auto &p:directory_iterator(repo_path/".jit/refs/heads")) {
        if (relative(p.path(),repo_path/".jit").string() == head_contents)  cout << "* ";
        else cout << "  ";
        cout << p.path().filename().string() << endl;
    }
}