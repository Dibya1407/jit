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

void blob::create_blob_file() {
    object blob_object(blob_contents);
    blob_object.create_object_file();
}

string read_blob(string blob_hash) {
    string decompressed_data=decompress_object(blob_hash);
    string contents=decompressed_data.substr(decompressed_data.find('\0')+1);

    return contents;
}