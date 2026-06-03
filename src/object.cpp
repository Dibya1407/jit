#include "object.h"

using namespace std;
using namespace std::filesystem;

object::object(string content) {
    contents=content;
    hash=sha1(contents,0);
    hash_raw=sha1(contents,1);
}

void object::create_object_file() {
    if (!exists(repo_path/".jit/objects")) {
        throw RepoNotFound("Repository not initialized. Please run 'jit init' first.");
    }
    try{
        string dirname = hash.substr(0,2);
        string filename = hash.substr(2);

        if (exists(repo_path/".jit/objects"/dirname/filename)) return;

        if(!exists(repo_path/".jit/objects"/dirname)) {
            create_directory(repo_path/".jit/objects"/dirname);
        }

        uLongf compressed_size = compressBound(contents.size());
        Bytef *compressed_data = new Bytef[compressed_size];

        if (compress(compressed_data, &compressed_size, (const Bytef *)contents.data(), contents.size()) != Z_OK) {
            cerr << "Error compressing object contents." << endl;
            delete[] compressed_data;
            return;
        }

        ofstream obj_file(repo_path/".jit/objects"/dirname/filename);
        obj_file.write((const char *)compressed_data, compressed_size);
        delete[] compressed_data;
        obj_file.close();

    } catch (const filesystem_error &e) {
        cerr << "Error creating object: " << e.what() << endl; 
    }
}


string get_full_hash(string object_hash) {
    if (object_hash.size()<4) {
        throw InvalidObject("Invalid object hash");
    }

    string dirname=object_hash.substr(0,2);
    string filename=object_hash.substr(2);

    if (!exists(repo_path/".jit/objects"/dirname)) {
        throw ObjectNotFound("Object with hash " + object_hash + " does not exist.");
    }

    for (auto &p:directory_iterator(repo_path/".jit/objects"/dirname)) {
        if (p.path().filename().string().substr(0,filename.size())==filename) {
            filename=p.path().filename();
            break;
        }
    }

    if(!exists(repo_path/".jit/objects"/dirname/filename)) {
        throw ObjectNotFound("Object with hash " + object_hash + " does not exist.");
    }

    return dirname+filename;
}


string decompress_object(string object_hash) {
    object_hash=get_full_hash(object_hash);
    string dirname=object_hash.substr(0,2);
    string filename=object_hash.substr(2);

    //Decompressing code
    ifstream object_file(repo_path/".jit/objects"/dirname/filename);

    vector<char> compressed_data((istreambuf_iterator<char>(object_file)), istreambuf_iterator<char>());
    object_file.close();

    z_stream strm{};

    strm.next_in = reinterpret_cast<Bytef*>(compressed_data.data());
    strm.avail_in = compressed_data.size();

    if (inflateInit(&strm) != Z_OK) {
        cerr << "Error initializing zlib for decompression." << endl;
        return "";
    }

    const int CHUNK_SIZE=1024;
    char out_buffer[CHUNK_SIZE];

    string decompressed_data;
    int bytes_read,ret;

    do {
        strm.next_out = reinterpret_cast<Bytef*>(out_buffer);
        strm.avail_out = CHUNK_SIZE;

        ret=inflate(&strm,Z_NO_FLUSH);

        if (ret<0) {
            cerr << "Error during zlib decompression." << endl;
            inflateEnd(&strm);
            return "";
        }

        bytes_read = CHUNK_SIZE - strm.avail_out;
        decompressed_data.append(out_buffer, bytes_read);
    } while (ret != Z_STREAM_END);

    inflateEnd(&strm);

    return decompressed_data;
}