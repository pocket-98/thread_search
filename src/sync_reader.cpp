#include "sync_reader.hpp"
#include <iostream>

using namespace sync;

reader::reader(FILE* f) {
    this->file = f;
}

bool reader::readline(char* out, size_t n) {
    this->read_mutt.lock();
    bool got_line = (-1 != getline(&out, &n, this->file));
    this->read_mutt.unlock();

    return got_line;
}

