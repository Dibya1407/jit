#include <filesystem>
#include <iostream>
#include <fstream>

using namespace std;
using namespace std::filesystem;

int main(int argc, char* argv[]) {

    if (argc < 2) {
        cerr << "No command provided.\n";
        return EXIT_FAILURE;
    }
    
    string command = argv[1];
    
    if (command == "init") {
        path curr_path=current_path();

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
}