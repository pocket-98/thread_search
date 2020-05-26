#ifndef SYNC_READER_HPP
#define SYNC_READER_HPP

#include <mutex>

namespace sync {
    // synchonized reader
    class reader {
        private:
        FILE* file;
        std::mutex read_mutt;

        public:
        reader(FILE* f);
        bool readline(char* out, size_t n);
    };
}

#endif // SYNC_READER_HPP
