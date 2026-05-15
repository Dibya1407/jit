#include "sha1.h"

using namespace std;

string sha1(const string& str) {
    unsigned char hash[SHA_DIGEST_LENGTH]; // SHA_DIGEST_LENGTH is 20 bytes
    SHA1(reinterpret_cast<const unsigned char*>(str.c_str()), str.length(), hash);
    
    stringstream ss;
    for(int i = 0; i < SHA_DIGEST_LENGTH; i++) {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return ss.str();
}