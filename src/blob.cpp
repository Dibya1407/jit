#include "blob.h"
#include "sha1.h"

blob::blob(path filepath) {
    this->filepath=filepath;
    string data;
    ifstream file(filepath);
    stringstream buffer;
    
    buffer<<file.rdbuf();
    data=buffer.str();

    blob_contents="blob "+to_string(data.size())+'\0'+data;
    hash=sha1(blob_contents);
}

void blob::create_blob_file(path repo_path) {
    if (!exists(repo_path/".jit/objects")) {
        cerr << "fatal: Repository not initialized. Please run 'jit init' first." << endl;
        return;
    }
    try{
        string dirname=hash.substr(0,2);
        string filename=hash.substr(2);

        create_directory(repo_path/".jit/objects"/dirname);

        ofstream blob_file(repo_path/".jit/objects"/dirname/filename);
        blob_file << blob_contents;
        blob_file.close();

    } catch (const filesystem_error &e) {
        cerr << "Error creating blob: " << e.what() << endl; 
    }
}
