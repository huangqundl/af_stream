#ifndef __ROLL_ADAPTER_DUMMY_HPP_INCLUDED__
#define __ROLL_ADAPTER_DUMMY_HPP_INCLUDED__

#include "adapter_base.hpp"
#include "dummy_item.hpp"
#include "config.hpp"

class AdapterDummy : public afs::AdapterBase {
private:
    DummyType item;
    int cur, max;

public:
    //void AddSource(const char* source) {}
    
    AdapterDummy() : afs::AdapterBase() {
        Config* config = Config::getInstance();
        max = config->getint("max_dummy_event", 0);
    }

    void ReadRecord(void** data, uint32_t *data_len) {
        if (cur == max) {
            *data = NULL;
            return;
        }

        item.number++;
        *data = &item;
        cur++;
    }

    void Init() {
        item.number = 0;
        cur = 0;
    };

    void Clean() {};
};

#endif
