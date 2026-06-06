#include "checkout.h"

void checkout(string commit_hash,bool direct) {
    if (!is_commit(commit_hash)) {
        throw InvalidCommit("Object with hash " + commit_hash + " is not a commit.");
    }

    string decompressed_data=decompress_object(commit_hash);
    string tree_hash=decompressed_data.substr(decompressed_data.find('\0')+6,40); //the format is "commit <size>\0tree <tree_hash>....."

    make_ignore_list();
    checkout_tree(tree_hash, repo_path);

    if (direct) {
        ofstream head_file(repo_path/".jit/HEAD");
        head_file << get_full_hash(commit_hash);
        head_file.close();
    }
}


void checkout_tree(string tree_hash, path dirpath) {
    if (!is_tree(tree_hash)) {
        throw InvalidTree("checkout.cpp,checkout_tree: Object with hash " + tree_hash + " doesn't exist.");
    }

    if (!exists(dirpath)) {
        create_directory(dirpath);
    }

    tree t(dirpath);
    if (t.hash == tree_hash) {
        return;
    }

    string decompressed_data=decompress_object(tree_hash);
    string contents=decompressed_data.substr(decompressed_data.find('\0')+1);
    
    unordered_map<string,string> file_hash_map,dir_hash_map;
    unordered_map<string,bool> visited;

    int ind=0;

    while (ind<contents.size()) {
        string mode=contents.substr(ind,6);
        ind+=7;

        string name=contents.substr(ind,contents.find('\0',ind)-ind);
        ind=contents.find('\0',ind)+1;

        string hash_raw=contents.substr(ind,20);
        ind+=20;

        stringstream ss;
        for(unsigned char c:hash_raw) {
            ss << hex << setw(2) << setfill('0') << (int)c;
        }
        string hash=ss.str();

        if (mode.substr(0,3)=="100") {
            file_hash_map[name]=hash;
            visited[name]=false;
        }
        else if (mode.substr(0,3)=="040") {
            dir_hash_map[name]=hash;
            visited[name]=false;
        }

    }

    directory_iterator dir_iter(dirpath);

    for (const auto& entry : dir_iter) {
        string rel_path=relative(entry.path(), repo_path).string();
        if (ignore_list.count(rel_path)) {
            visited[entry.path().filename().string()]=true;
            continue;
        }

        if (entry.is_regular_file()) {
            string filename=entry.path().filename().string();

            if (file_hash_map.count(filename)) {
                visited[filename]=true;

                blob b(entry.path());
                if (b.hash==file_hash_map[filename]) {
                    continue;
                }

                string new_contents=read_blob(file_hash_map[filename]);

                ofstream file(entry.path());
                file << new_contents;
                file.close();
            }
            else {
                remove(entry.path());
            }
        }
        else if (entry.is_directory()) {
            string dirname=entry.path().filename().string();

            if (dir_hash_map.count(dirname)) {
                visited[dirname]=true;

                checkout_tree(dir_hash_map[dirname], dirpath/dirname);
            }
            else {
                remove_all(entry.path());
            }
        }
    }

    for (auto& [filename,hash]:file_hash_map) {
        if (!visited[filename]) {
            string contents=read_blob(hash);

            ofstream file(dirpath/filename);
            file << contents;
            file.close();
        }
    }

    for (auto& [dirname,hash]:dir_hash_map) {
        if (!visited[dirname]) {
            checkout_tree(hash, dirpath/dirname);
        }
    }
}

void checkout_branch(string branch_name) {
    if (!is_branch(branch_name)) {
        throw JitError("branch with name "+branch_name+" doesn't exist.");
    }

    ifstream branch_file(repo_path/".jit/refs/heads"/branch_name);
    string commit_hash;
    branch_file >> commit_hash;
    branch_file.close();

    checkout(commit_hash);

    ofstream head_file(repo_path/".jit/HEAD");
    head_file << "ref: refs/heads/"+branch_name;
    head_file.close();
}