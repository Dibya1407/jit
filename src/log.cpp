#include "log.h"

unordered_map<string, string> log_map;

void make_log_map() {
    if (!exists(repo_path/".jit/refs/heads")) {
        return;
    }

    string head_commit=get_head_hash();
    log_map[head_commit]="HEAD";

    for (const auto& entry : directory_iterator(repo_path/".jit/refs/heads")) {
        if (entry.is_regular_file()) {
            string commit_hash, branch_name = entry.path().filename().string();
            ifstream branch_file(entry.path());
            getline(branch_file, commit_hash);

            if (log_map.count(commit_hash)) {
                if (log_map[commit_hash] == "HEAD") {
                    log_map[commit_hash] = "HEAD -> " + branch_name;
                }
                else {
                    log_map[commit_hash] += ", " + branch_name;
                }
            }
            else log_map[commit_hash] = entry.path().filename();
        }
    }

}

void log() {
    if (!exists(repo_path/".jit/log")) {
        throw InvalidArgument("No commits yet.");
    }

    make_log_map();

    ifstream log_file(repo_path/".jit/log");
    string line;
    while (getline(log_file, line)) {
        cout << line;
        if (line.find('c') == 0) {
            string commit_hash = line.substr(7, 40);
            if (log_map.count(commit_hash)) {
                cout << " (" << log_map[commit_hash] << ")";
            }
        }
        cout << endl;
    }
    log_file.close();
}