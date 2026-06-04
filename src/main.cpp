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
        cerr << "No command provided.\n";
        return EXIT_FAILURE;
    }
    
    string command = argv[1];

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
        {"",[]() { cerr << "fatal: No command provided.\n"; return EXIT_FAILURE; }}
    };

    try {
        if (commands.find(command) != commands.end()) return commands[command]();
        else {
            cout<<"Unknown command: "<<command<<endl;
            return EXIT_FAILURE;
        }
    } catch (const JitError& e) {
        cerr << "fatal: " << e.what() << endl;
        return EXIT_FAILURE;
    }
    catch (const std::exception& e) {
        cerr << "Unexpected Error: " << e.what() << endl;
        return EXIT_FAILURE;
    }

}