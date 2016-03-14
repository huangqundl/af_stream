#ifndef __AFS_I_ADAPTER_HPP_INCLUDED__
#define __AFS_I_ADAPTER_HPP_INCLUDED__

#include "util.hpp"

namespace afs {

char adapter_types[] = "\n    =============\n"
                        "    ram\n"
                        "    disk\n"
                        //"dummy\n"
                        "\n    =============\n";

void PrintUseBuiltinAdapter() {
    LOG_MSG("    No user-specified adapters, search configure file for a built-in adapter\n");
}

void PrintAdapterNotFound(const char* adapter_type) {
    LOG_ERR("Adapter type %s is not supported!\n"
            "Existing adaters include:%s",
            adapter_type, adapter_types);
}

class AdapterBase {

public:
    //virtual void AddSource(const char* source) = 0;

    virtual void ReadRecord(void** data, uint32_t *data_len) = 0;

    virtual void Init() = 0;
    virtual void Clean() = 0;

    virtual ~AdapterBase() {}

    /*
    virtual uint64_t CountMsg() = 0;
    virtual uint64_t CountByte() = 0;
    */
};

}

#endif
