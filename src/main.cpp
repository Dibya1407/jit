#include <unordered_map>
#include <functional>
#include "exceptions.h"
#include "globals.h"
#include "commands.h"

using namespace std;
using namespace std::filesystem;

path repo_path;
vector<string> args;

int main(int argc, char* argv[]) {

    if (argc < 2) {
        cerr << "No command provided. Use 'jit -h' or 'jit --help' for usage info.\n";
        return EXIT_FAILURE;
    }
    
    string command = argv[1];

    if (command == "-h" || command == "--help") {
        cout << "jit - Git inspired minimal version control system.\n\n"
             << "Usage: jit <command> [<args>]\n\n"
             << "Commands:\n"
             << "  init           Initialize a new local repository\n"
             << "  hash-object    Hashes an object (file) and optionally writes it (-w)\n"
             << "  cat-file       Prints the contents of an object given its hash\n"
             << "  make-tree      Creates a tree object for a directory and optionally writes it (-w)\n"
             << "  ls-tree        Lists the contents of a tree object\n"
             << "  config         Configure user details (e.g. user and email)\n"
             << "  commit-tree    Create a commit object mapping to a tree object\n"
             << "  commit         Create a full commit mapping to the current repository\n"
             << "  log            View the commit history\n"
             << "  checkout       Switch the repository state of a given commit hash\n\n"
             << "Run 'jit <command> -h' to see specific usage for a command.\n";
        return EXIT_SUCCESS;
    }

    args.assign(argv + 2, argv + argc);
    repo_path = current_path();

    unordered_map<string,function<int()>> commands = {
        {"init",init},
        {"hash-object",hash_object},
        {"cat-file",cat_file},
        {"make-tree",make_tree},
        {"ls-tree",ls_tree},
        {"config",config},
        {"commit-tree",commit_tree},
        {"commit",do_commit},
        {"log",log},
        {"checkout",do_checkout},
        {"",[]() { cerr << "fatal: No command provided. Use 'jit -h' for help.\n"; return EXIT_FAILURE; }}
    };

    try {
        if (commands.find(command) != commands.end()) return commands[command]();
        else {
            cout<<"Unknown command: "<<command<<". Use 'jit -h' for help."<<endl;
            return EXIT_FAILURE;
        }
    } catch (const JitError& e) {
        cerr << "fatal: " << e.what() << "\nUse 'jit " << command << " -h' for usage information." << endl;
        return EXIT_FAILURE;
    }
    catch (const std::exception& e) {
        cerr << "Unexpected Error: " << e.what() << endl;
        return EXIT_FAILURE;
    }

}