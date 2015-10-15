#include <string.h>
#include "util.hpp"

#include "recovery_state.hpp"

#define INIT_MAX_SIZE 8192

namespace afs {

RecoveryState::RecoveryState() : size(0), max_size(INIT_MAX_SIZE), num_record(0) {
    data = (unsigned char*)calloc(INIT_MAX_SIZE, sizeof(unsigned char));
}

RecoveryState::~RecoveryState() {
    free(data);
}

int RecoveryState::ExpandDataArray() {
    max_size = max_size << 1;
    data = (unsigned char*)realloc(data, max_size);
    if (data == NULL) {
        LOG_MSG("failed to re-allocate memory for recovery state - desired size: %lu\n", max_size);
        return 1;
    }
    return 0;
}

void RecoveryState::LoadFromFile(char* filename) {
    char line[100];
    FILE* state_file = fopen(filename, "r");
    if (state_file == NULL) {
        LOG_MSG("no file\n");
    }

    while (1) {
        if (fgets(line, 100, state_file) == NULL) {
            break;
        }
        uint32_t len = strlen(line);
        line[len-1] = 0;
        len--;
        if (size+len+sizeof(uint32_t) >= max_size) {
            ExpandDataArray();
        }
        memcpy(data+size, &len, sizeof(uint32_t)); 
        memcpy(data+size+sizeof(uint32_t), line, len);
        size += sizeof(uint32_t) + len;
        num_record++;
    }

    fclose(state_file);
}


} // namespace afs
