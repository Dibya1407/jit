#include "commands.h"

int init() {
    try {
        if (!exists(repo_path/".jit")) create_directory(repo_path/".jit");
        if (!exists(repo_path/".jit/objects")) create_directory(repo_path/".jit/objects");
        if (!exists(repo_path/".jit/refs")) create_directory(repo_path/".jit/refs");

        ofstream head(repo_path/".jit/HEAD");
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
        throw InvalidArgument("No file path provided.");
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
            throw InvalidArgument("Invalid option");
        }
    }
    else {
        throw InvalidArgument("Too many files or options");
    }

    
    if (!exists(file_path)) {
        throw InvalidArgument("File does not exist.");
    }

    blob b(file_path);
    cout << b.hash << endl;

    if (to_write) {
        b.create_blob_file();
    }

    return 0;

}

int cat_file() {
    if (!args.size()) {
        throw InvalidArgument("Invalid number of arguments.");
    }

    string blob_hash = args[0];
    cout << read_blob(blob_hash) << endl;

    return 0;
}

int make_tree() {
    int arg_size=args.size();
    path file_path;
    bool to_write=false;

    if (!arg_size) {
        throw InvalidArgument("No file path provided.");
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
            throw InvalidArgument("Invalid option");
        }
    }
    else {
        throw InvalidArgument("Too many files or options");
    }

    
    if (!exists(file_path)) {
        throw InvalidArgument("File does not exist.");
    }

    if (!is_directory(file_path)) {
        throw InvalidArgument("Not a directory.");
    }

    tree t(file_path);
    cout << t.hash << endl;

    if (to_write) {
        t.create_tree_file();
    }

    return 0;

}

int ls_tree() {
    if (!args.size()) {
        throw InvalidArgument("Invalid number of arguments.");
    }

    string tree_hash = args[0];
    cout << read_tree(tree_hash);

    return 0;
}

int config() {
    if (args.size()!=2) {
        throw InvalidArgument("Invalid number of arguments.");
    }

    if (args[0]=="user") {
        ofstream config_file(repo_path/".jit/config");
        config_file << "username: " << args[1] << endl;
        config_file.close();
    }
    else if (args[0]=="email") {
        if (!exists(repo_path/".jit/config")) {
            throw ConfigError("run jit config user <username> first.");
        }
        ifstream file(repo_path/".jit/config");
        string line;
        while (getline(file, line)) {
            if (line.find("email: ") == 0) {
                file.close();
                throw ConfigError("username and email already set.\nrun jit config user <username> and jit config email <email> again.");
            }
        }
        file.close();

        ofstream config_file(repo_path/".jit/config",ios::app);
        config_file << "email: " << args[1] << endl;
        config_file.close();
    }
    else {
        throw InvalidArgument("Invalid option.");
    }

    return 0;
}

int commit_tree() {
    if (args.size()<2) {
        throw InvalidArgument("Invalid number of arguments.");
    }

    string tree_hash = args[0];
    string message = args[1];
    string parent_hash="";
    if (args.size() > 2) parent_hash = args[2];

    commit c(tree_hash, message, parent_hash);

    return 0;
}

int do_commit() {
    if (args.size() < 2) {
        throw InvalidArgument("Invalid number of arguments.");
    }
    if (args[0] != "-m") {
        throw InvalidArgument("Invalid option.");
    }

    string message = args[1];

    make_commit(message);

    return 0;
}