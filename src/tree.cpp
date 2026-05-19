#include "tree.h"

tree::tree(path dir) {
    if (!is_directory(dir)) {
        cerr<< "not a directory path can't make a tree\n";
        return;
    }

    directory_iterator dir_iter(dir);

    vector<directory_entry> entries;
    
    for (auto& entry:dir_iter) {
        entries.push_back(entry);
    }

    //need sorted entries by filename for deterministic hash.
    sort(entries.begin(), entries.end(), [](const directory_entry& a, const directory_entry& b) {
        return a.path().filename().string() < b.path().filename().string();
    });

    for (auto& entry:entries) {
        if (entry.is_regular_file()) {
            path file_path=entry.path();

            perms perm=entry.status().permissions();
            int mode=static_cast<int>(perm);
            string mode_str="100" + (stringstream{} << oct << (mode & 0777)).str();

            string blob_hash=blob(file_path).hash_raw;

            string filename=file_path.filename().string();

            tree_contents+=mode_str+' '+filename+'\0'+blob_hash;
        }
        if (entry.is_directory()) {
            string dirname=entry.path().filename().string();
            string dir_hash=tree(entry.path()).hash_raw;    //recursive call

            tree_contents+="040000 "+dirname+'\0'+dir_hash;
        }
    }

    tree_contents="tree "+to_string(tree_contents.size())+'\0'+tree_contents;
    hash=sha1(tree_contents,0);
    hash_raw=sha1(tree_contents,1);
}


void tree::create_tree() {
    object tree_object(tree_contents);
    tree_object.create_object_file();
}


string read_tree(string tree_hash) {
    string decompressed_data=decompress_object(tree_hash);

    string should_be_tree=decompressed_data.substr(0,decompressed_data.find(' '));
    if (should_be_tree != "tree") {
        cerr << "fatal: Invalid tree object" << endl;
        return "";
    }

    string contents=decompressed_data.substr(decompressed_data.find('\0')+1);

    string formatted_contents="";
    int ind=0;

    while (ind<contents.size()) {
        string mode=contents.substr(ind,6);
        ind+=7;

        string type;
        if (mode.substr(0,3)=="100") type="blob";
        else if (mode.substr(0,3)=="040") type="tree";

        string name=contents.substr(ind,contents.find('\0',ind)-ind);
        ind=contents.find('\0',ind)+1;

        string hash_raw=contents.substr(ind,20);
        ind+=20;

        stringstream ss;
        for(unsigned char c:hash_raw) {
            ss << hex << setw(2) << setfill('0') << (int)c;
        }
        string hash=ss.str();

        formatted_contents+=mode+' '+type+' '+hash+'\t'+name+'\n';
    }

    return formatted_contents;
}