#pragma once

#include <openssl/sha.h>
#include <sstream>
#include <iomanip>

using namespace std;

string sha1(const string& str);