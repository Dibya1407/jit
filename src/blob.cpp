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

    object blob_object(blob_contents);
    this->hash=blob_object.hash;
    this->hash_raw=blob_object.hash_raw;
}

void blob::create_blob_file(path repo_path) {
    object blob_object(blob_contents);
    blob_object.create_object_file(repo_path);
}

string read_blob(string blob_hash,path repo_path) {
    string decompressed_data=decompress_object(blob_hash,repo_path);
    string contents=decompressed_data.substr(decompressed_data.find('\0')+1);

    return contents;
}