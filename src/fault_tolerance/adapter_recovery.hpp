#ifndef __ROLL_ADAPTER_Recovery_HPP_INCLUDED__
#define __ROLL_ADAPTER_Recovery_HPP_INCLUDED__

#include "adapter_base.hpp"

#include <stdio.h>
#include <string>

#include "config.hpp" 
#include "util.hpp"

#define BLK_SIZE 4096

namespace afs {

uint32_t align(uint32_t orig_size, uint32_t blk_size) {
    if (orig_size % blk_size == 0) {
        return orig_size;
    }
    else {
        return blk_size * (orig_size/blk_size + 1);
    }
}

class AdapterRecovery : public AdapterBase {

public:

    // derived from AdapterBase
    void Init();
    void Clean();
    //void AddSource(const char* source);

    AdapterRecovery(FILE* data, FILE* seq);

    // derived from AdapterBase
    void ReadRecord(void** data, uint32_t *len);

private:

    FILE* data_file_, *seq_file_;

    unsigned char* data_buf_;
    uint64_t data_size_;

    uint64_t ptr_;
};

AdapterRecovery::AdapterRecovery(FILE* data, FILE* seq) :
    data_file_(data),
    seq_file_(seq),
    data_buf_(NULL),
    data_size_(0),
    ptr_(0) {
    }

void AdapterRecovery::Init() {
    struct stat buf;

    int fd = fileno(data_file_);
    fstat(fd, &buf);
    data_size_ = buf.st_size;
    if (data_size_ == 0) {
        return;
    }
    LOG_MSG("    backup data file size %lu\n", data_size_);
    data_buf_ = (unsigned char*)calloc(sizeof(unsigned char), align(data_size_, BLK_SIZE));
    afs_assert(data_buf_, "Data buffer allocate failure\n");

    fd = fileno(seq_file_);
    fstat(fd, &buf);
    int seq_size = buf.st_size;
    LOG_MSG("    backup seq file size %d\n", seq_size);
    afs_assert(seq_size%sizeof(uint64_t) == 0, "Seq size error\n");
    uint64_t* seq_buf = (uint64_t*)calloc(sizeof(unsigned char), align(seq_size, BLK_SIZE));
    afs_assert(seq_buf, "Seq buffer allocate failure\n");

    int ret = fread(data_buf_, sizeof(char), data_size_, data_file_);
    afs_assert(ret = data_size_, "Data recovery file read failure\n");
    ret = fread(seq_buf, sizeof(char), seq_size, seq_file_);
    afs_assert(ret = seq_size, "Seq recovery file read failure\n");

    int num = seq_size / sizeof(uint64_t);
    uint32_t start = 0;
    uint64_t last_seq = seq_buf[0];
    for (auto i=1; i<num; i++) {
        uint64_t cur_seq = seq_buf[i];
        if (cur_seq-last_seq == 1) {
            last_seq = cur_seq;
        }
        else {
            start = i;
            last_seq = cur_seq;
        }
    }
    //printf("start %u, seq %lu\n", start, seq_buf[start]);

    for (uint32_t i=0; i<start; i++) {
        uint32_t len = *((uint32_t*)(data_buf_+ptr_));
        ptr_ += sizeof(uint32_t) + len;
    }
}

void AdapterRecovery::Clean() {
    free(data_buf_);
}

/*
void AdapterRecovery::AddSource(const char* source) {
    LOG_ERR("We only consider single file for adapter_disk\n");
}
*/

// assume each record is a one-line, readable string
void AdapterRecovery::ReadRecord(void** data, uint32_t *data_len) {
    if (ptr_ == data_size_) {
        *data = NULL;
        return;
    }

    uint32_t len = *((uint32_t*)(data_buf_+ptr_));
    ptr_ += sizeof(uint32_t);
    *data = (void*)(data_buf_+ptr_);
    *data_len = len;
    ptr_ += len;

    //printf("len %u\n", len);
}

}
#endif

