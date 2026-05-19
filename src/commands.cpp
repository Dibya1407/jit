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
        b.create_blob_file();
    }

    return 0;

}

int cat_file() {
    if (!args.size()) {
        cerr << "fatal: Invalid number of arguments.\n";
        return EXIT_FAILURE;
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
        t.create_tree();
    }

    return 0;

}

int ls_tree() {
    if (!args.size()) {
        cerr << "fatal: Invalid number of arguments.\n";
        return EXIT_FAILURE;
    }

    string tree_hash = args[0];
    cout << read_tree(tree_hash);

    return 0;
}

int config() {
    if (args.size()!=2) {
        cerr << "fatal: Invalid number of arguments.\n";
        return EXIT_FAILURE;
    }

    if (args[0]=="user") {
        ofstream config_file(repo_path/".jit/config");
        config_file << "username: " << args[1] << endl;
        config_file.close();
    }
    else if (args[0]=="email") {
        if (!exists(repo_path/".jit/config")) {
            cerr << "fatal: run jit config user <username> first.\n";
            return EXIT_FAILURE;
        }
        ifstream file(repo_path/".jit/config");
        string line;
        while (getline(file, line)) {
            if (line.find("email: ") == 0) {
                cerr<< "fatal: username and email already set.\nrun jit config user <username> and jit config email <email> again."<<endl;
                file.close();
                return EXIT_FAILURE;
            }
        }
        file.close();

        ofstream config_file(repo_path/".jit/config",ios::app);
        config_file << "email: " << args[1] << endl;
        config_file.close();
    }
    else {
        cerr << "fatal: Invalid config option.\n";
        return EXIT_FAILURE;
    }

    return 0;
}