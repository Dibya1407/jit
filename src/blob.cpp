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

        uLongf compressed_size = compressBound(blob_contents.size());
        Bytef *compressed_data = new Bytef[compressed_size];

        if (compress(compressed_data, &compressed_size, (const Bytef *)blob_contents.data(), blob_contents.size()) != Z_OK) {
            cerr << "Error compressing blob contents." << endl;
            delete[] compressed_data;
            return;
        }

        ofstream blob_file(repo_path/".jit/objects"/dirname/filename);
        blob_file.write((const char *)compressed_data, compressed_size);
        delete[] compressed_data;
        blob_file.close();

    } catch (const filesystem_error &e) {
        cerr << "Error creating blob: " << e.what() << endl; 
    }
}


string read_blob(string blob_hash,path repo_path) {
    string dirname=blob_hash.substr(0,2);
    string filename=blob_hash.substr(2);

    if (!exists(repo_path/".jit/objects"/dirname) || blob_hash.size()<4) {
        cerr << "fatal: Invalid blob hash" << endl;
        return "67";
    }

    for (auto &p:directory_iterator(repo_path/".jit/objects"/dirname)) {
        if (p.path().filename().string().substr(0,filename.size())==filename) {
            filename=p.path().filename();
            break;
        }
    }

    if(!exists(repo_path/".jit/objects"/dirname/filename)) {
        cerr << "fatal: Blob with hash " << blob_hash << " does not exist." << endl;
        return "67";
    }

    ifstream blob_file(repo_path/".jit/objects"/dirname/filename);

    vector<char> compressed_data((istreambuf_iterator<char>(blob_file)), istreambuf_iterator<char>());
    blob_file.close();

    z_stream strm{};

    strm.next_in = reinterpret_cast<Bytef*>(compressed_data.data());
    strm.avail_in = compressed_data.size();

    if (inflateInit(&strm) != Z_OK) {
        cerr << "Error initializing zlib for decompression." << endl;
        return "67";
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
            return "67";
        }

        bytes_read = CHUNK_SIZE - strm.avail_out;
        decompressed_data.append(out_buffer, bytes_read);
    } while (ret != Z_STREAM_END);

    inflateEnd(&strm);

    string contents=decompressed_data.substr(decompressed_data.find('\0')+1);

    return contents;

}