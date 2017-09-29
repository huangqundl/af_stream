#ifndef __AFS_ADAPTER_DISK_HPP_INCLUDED__
#define __AFS_ADAPTER_DISK_HPP_INCLUDED__

#include "adapter_base.hpp"

#include <stdio.h>
#include <string>

#include "../config.hpp" 
#include "../util.hpp"


namespace afs {

class AdapterDisk : public AdapterBase {

public:

    // derived from AdapterBase
    void Init();
    void Clean();
    void AddSource(const char* source);

    AdapterDisk();

private:
    
    FILE* file_;

    char buf[MAX_RECORD_LENGTH];
    unsigned long long int max;
    unsigned long long int cnt;

    // derived from AdapterBase
    void ReadRecord(void** data, uint32_t *data_len);
};

AdapterDisk::AdapterDisk() :
    file_(NULL) { }

void AdapterDisk::Init() {

    Config* config = Config::getInstance();

    max = config->get_ull("adapter.max_data", 0);

    char* filename = config->getstring("trace_file", NULL);
    afs_assert(filename, "[trace_file] must be specified in the config file\n");

    file_ = fopen(filename, "r");
    afs_assert(file_, "Input file %s not found, exit\n", filename);
}

void AdapterDisk::Clean() {
    fclose(file_);
}

/*
void AdapterDisk::AddSource(const char* source) {
    LOG_ERR("We only consider single file for adapter_disk\n");
}
*/

// assume each record is a one-line, readable string
void AdapterDisk::ReadRecord(void** data, uint32_t *data_len) {
    if (fgets(buf, MAX_RECORD_LENGTH, file_) == NULL) {
        *data = NULL;
        //*len = 0;
    }
    if (max && cnt==max) {
        *data = NULL;
    }
    else {
        *data = buf;
        cnt++;
        //*len = strlen(buf);
    }
}

}
#endif

