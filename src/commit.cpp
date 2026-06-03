#include "commit.h"

commit::commit(string tree_hash,string message,string parent_hash) {
    if (!is_tree(tree_hash)) {
        throw InvalidTree("Object with hash " + tree_hash + " is not a tree.");
    }
    tree_hash=get_full_hash(tree_hash);
    this->tree_hash=tree_hash;

    if (parent_hash!="" && !is_commit(parent_hash)) {
        throw InvalidCommit("Object with hash " + parent_hash + " is not a commit.");
    }
    if (parent_hash!="") parent_hash=get_full_hash(parent_hash);

    string username,email;

    bool email_exists=false, username_exists=false;
    ifstream config_file(repo_path/".jit/config");
    string line;
    while (getline(config_file, line)) {
        if (line.find("username: ") == 0) {
            username=line.substr(10);
            username_exists=true;
        }
        if (line.find("email: ") == 0) {
            email=line.substr(7);
            email_exists=true;
            break;
        }
    }
    config_file.close();

    if (!email_exists || !username_exists) {
            throw ConfigError("run jit config user <username> and jit config email <email> first.");
    }

    string time_epoch=to_string(time(nullptr));
    string time_zone="+0530";

    contents+="tree "+tree_hash+'\n';
    if (parent_hash!="") contents+="parent "+parent_hash+'\n';
    contents+="author "+username+" <"+email+"> "+time_epoch+' '+time_zone+'\n';
    contents+="committer "+username+" <"+email+"> "+time_epoch+' '+time_zone+'\n';
    contents+='\n';
    contents+=message+'\n';

    contents="commit "+to_string(contents.size())+'\0'+contents;
    hash=sha1(contents,0);
    hash_raw=sha1(contents,1);

    cout<<hash<<endl;

    create_commit_file();
}


void commit::create_commit_file() {
    object commit(contents);
    commit.create_object_file();
}


bool is_commit(string object_hash) {
    string decompressed_data=decompress_object(object_hash);

    string should_be_commit=decompressed_data.substr(0,decompressed_data.find(' '));
    if (should_be_commit != "commit") {
        return false;
    }
    return true;
}

void make_commit(string message,string parent_hash) {
    tree t(repo_path,true);
    t.create_tree_file();
    commit c(t.hash,message,parent_hash);
    c.create_commit_file();
}