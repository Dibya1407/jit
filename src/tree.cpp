#include "tree.h"

tree::tree(path dir,bool make_blobs) {
    if (!is_directory(dir)) {
        throw JitError("not a directory path can't make a tree");
    }

    directory_iterator dir_iter(dir);

    vector<directory_entry> entries;
    
    for (auto& entry:dir_iter) {
        entries.push_back(entry);
    }

    string dirname=relative(dir,repo_path).string();
    if (ignore_list.count(dirname)) {
        if (make_blobs) return; // for normal call from make-tree, we want the tree hash once.
    }

    //need sorted entries by filename for deterministic hash.
    sort(entries.begin(), entries.end(), [](const directory_entry& a, const directory_entry& b) {
        return a.path().filename().string() < b.path().filename().string();
    });

    for (auto& entry:entries) {
        if (entry.is_regular_file()) {
            path file_path=entry.path();

            string relative_path=relative(file_path,repo_path).string();
            if (ignore_list.count(relative_path)) continue;
            if (make_blobs) {
                blob(file_path).create_blob_file();
            }

            perms perm=entry.status().permissions();
            int mode=static_cast<int>(perm);
            string mode_str="100" + (stringstream{} << oct << (mode & 0777)).str();

            string blob_hash=blob(file_path).hash_raw;

            string filename=file_path.filename().string();

            tree_contents+=mode_str+' '+filename+'\0'+blob_hash;
        }
        if (entry.is_directory()) {
            string dirname=entry.path().filename().string();

            string relative_path=relative(entry.path(),repo_path).string();
            if (ignore_list.count(relative_path)) continue;

            string dir_hash=tree(entry.path(),make_blobs).hash_raw;    //recursive call

            tree_contents+="040000 "+dirname+'\0'+dir_hash;
        }
    }

    tree_contents="tree "+to_string(tree_contents.size())+'\0'+tree_contents;
    hash=sha1(tree_contents,0);
    hash_raw=sha1(tree_contents,1);

    if (make_blobs) {
        create_tree_file();
    }
}


void tree::create_tree_file() {
    object tree_object(tree_contents);
    tree_object.create_object_file();
}


bool is_tree(string object_hash) {
    string decompressed_data=decompress_object(object_hash);

    string should_be_tree=decompressed_data.substr(0,decompressed_data.find(' '));
    if (should_be_tree != "tree") {
        return false;
    }
    return true;
}


string read_tree(string tree_hash) {
    if (!is_tree(tree_hash)) {
        throw InvalidTree("Object with hash " + tree_hash + " is not a tree.");
    }

    string decompressed_data=decompress_object(tree_hash);

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