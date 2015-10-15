// License here.

#ifndef RECOVERY_STATE_HPP_
#define RECOVERY_STATE_HPP_

#include <stddef.h>
#include <unordered_set>

namespace afs {

// The structure for restored state
// After worker recovery, daemon will retrieve states from backup servers 
// Then maintain them as the class RecoveryState
// Later on, components will transform it into their internal states

class RecoveryState {

private:
    size_t size, max_size;
    size_t num_record;

    // layout in array:
    // len - 4 bytes
    // (key,value) pair - $len bytes 
    // then follows
    unsigned char* data;

    int ExpandDataArray();

public:
    RecoveryState();
    ~RecoveryState();

    size_t get_num_record() {
        return num_record;
    }

    unsigned char* get_data() {
        return data;
    }

    void LoadFromFile(char* filename);

};

} // namespace afs

#endif // RECOVERY_STATE_
