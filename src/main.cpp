#include <filesystem>
#include <iostream>
#include <fstream>
#include "blob.h"

using namespace std;
using namespace std::filesystem;

int main(int argc, char* argv[]) {

    if (argc < 2) {
        cerr << "No command provided.\n";
        return EXIT_FAILURE;
    }
    
    string command = argv[1];
    
    path curr_path=current_path();

    if (command == "init") {
        try {
            create_directory(curr_path/".jit");
            create_directory(curr_path/".jit/objects");
            create_directory(curr_path/".jit/refs");
    
            ofstream head(curr_path/".jit/HEAD");
            head << "ref: refs/heads/main" << endl;
            head.close();
        } catch (const filesystem_error& e) {
            cerr << "Error initializing JIT: " << e.what() << endl;
            return EXIT_FAILURE;
        }
    }

    if (command=="hash-object") {
        path file_path;
        bool to_write=false;

        if (argc<3) {
            cerr << "fatal: No file path provided.\n";
            return EXIT_FAILURE;
        }
        if (argc==3) file_path=argv[2];
        else if (argc==4) {
            string arg2 = argv[2];
            string arg3 = argv[3];

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
    }

    if(command=="cat-file") {
        if (argc<3) {
            cerr << "fatal: No blob hash provided.\n";
            return EXIT_FAILURE;
        }
        string blob_hash = argv[2];
        cout << read_blob(blob_hash, curr_path) << endl;
    }
}