#include <filesystem>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <functional>
#include "object.h"
#include "blob.h"
#include "tree.h"

using namespace std;
using namespace std::filesystem;

int init();
int hash_object();
int cat_file();
int make_tree();
int ls_tree();

vector<string> args;
path curr_path;

int main(int argc, char* argv[]) {

    if (argc < 2) {
        cerr << "No command provided.\n";
        return EXIT_FAILURE;
    }
    
    string command = argv[1];

    args.assign(argv + 2, argv + argc);
    curr_path = current_path();

    unordered_map<string,function<int()>> commands = {
        {"init",init},
        {"hash-object",hash_object},
        {"cat-file",cat_file},
        {"make-tree",make_tree},
        {"ls-tree",ls_tree},
        {"",[]() { cerr << "fatal: No command provided.\n"; return EXIT_FAILURE; }}
    };

    if (commands.find(command) != commands.end()) return commands[command]();
    else {
        cout<<"Unknown command: "<<command<<endl;
        return EXIT_FAILURE;
    }

}

int init() {
    try {
        if (!exists(curr_path/".jit")) create_directory(curr_path/".jit");
        if (!exists(curr_path/".jit/objects")) create_directory(curr_path/".jit/objects");
        if (!exists(curr_path/".jit/refs")) create_directory(curr_path/".jit/refs");

        ofstream head(curr_path/".jit/HEAD");
        head << "ref: refs/heads/main" << endl;
        head.close();

        return 0;
    } catch (const filesystem_error& e) {
        cerr << "Error initializing JIT: " << e.what() << endl;
        return EXIT_FAILURE; 
    }
}

int hash_object() {
    int arg_size=args.size();
    path file_path;
    bool to_write=false;

    if (!arg_size) {
        cerr << "fatal: No file path provided.\n";
        return EXIT_FAILURE;
    }
    if (arg_size==1) file_path=args[0];
    else if (arg_size==2) {
        string arg2 = args[0];
        string arg3 = args[1];

        if (arg2=="-w" || arg2=="--write") {
            file_path=arg3;
            to_write=true;
        }
        else if (arg3=="-w" || arg3=="--write") {
            file_path=arg2;
            to_write=true;
        }
        else {
            cerr<<"fatal: Invalid option"<<endl;
            return EXIT_FAILURE;
        }
    }
    else {
        cerr<<"fatal: Too many files or options"<<endl;
        return EXIT_FAILURE;
    }

    
    if (!exists(file_path)) {
        cerr << "fatal: File does not exist.\n";
        return EXIT_FAILURE;
    }

    blob b(file_path);
    cout << b.hash << endl;

    if (to_write) {
        b.create_blob_file(curr_path);
    }

    return 0;

}

int cat_file() {
    if (!args.size()) {
        cerr << "fatal: Invalid number of arguments.\n";
        return EXIT_FAILURE;
    }

    string blob_hash = args[0];
    cout << read_blob(blob_hash, curr_path) << endl;

    return 0;
}

int make_tree() {
    int arg_size=args.size();
    path file_path;
    bool to_write=false;

    if (!arg_size) {
        cerr << "fatal: No file path provided.\n";
        return EXIT_FAILURE;
    }
    if (arg_size==1) file_path=args[0];
    else if (arg_size==2) {
        string arg2 = args[0];
        string arg3 = args[1];

        if (arg2=="-w" || arg2=="--write") {
            file_path=arg3;
            to_write=true;
        }
        else if (arg3=="-w" || arg3=="--write") {
            file_path=arg2;
            to_write=true;
        }
        else {
            cerr<<"fatal: Invalid option"<<endl;
            return EXIT_FAILURE;
        }
    }
    else {
        cerr<<"fatal: Too many files or options"<<endl;
        return EXIT_FAILURE;
    }

    
    if (!exists(file_path)) {
        cerr << "fatal: File does not exist.\n";
        return EXIT_FAILURE;
    }

    if (!is_directory(file_path)) {
        cerr << "fatal: Not a directory.\n";
        return EXIT_FAILURE;
    }

    tree t(file_path);
    cout << t.hash << endl;

    if (to_write) {
        t.create_tree(curr_path);
    }

    return 0;

}

int ls_tree() {
    if (!args.size()) {
        cerr << "fatal: Invalid number of arguments.\n";
        return EXIT_FAILURE;
    }

    string tree_hash = args[0];
    cout << read_tree(tree_hash, curr_path);

    return 0;
}