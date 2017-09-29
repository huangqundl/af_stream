#ifndef __AFS_CONFIG_INCLUDED_HPP__
#define __AFS_CONFIG_INCLUDED_HPP__

#include <stdint.h>

#include <iniparser.h>

#include "util.hpp"
#include "zmq_config.hpp"

#define MAX_LINE_SIZE 1024
#define MAX_RECORD_LENGTH 8192

const char* strconcat(const char* a, const char* b);

class Config {

private:
    static Config *instance_;

    dictionary *dict_;
    char section_name_[MAX_LINE_SIZE];

    Config(const char* ininame = NULL, const char* section_name = "");

public:
	~Config();

    static Config *getInstance(
            const char* ininame = NULL,
            const char* section_name = "") {
        if (!instance_) {
            instance_ = new Config(ininame, section_name);
        }
        return instance_;
    }

    inline char* getstring(const char* key, char* def) {
        return iniparser_getstring(dict_, strconcat(section_name_, key), def);
    }
    inline int getint(const char* key, int notfound) {
        return iniparser_getint(dict_, strconcat(section_name_, key), notfound);
    }
    inline unsigned long long int get_ull(const char* key, unsigned long long int notfound) {
        char* str = iniparser_getstring(dict_, strconcat(section_name_, key), NULL);
        if (str == NULL) return notfound;
        return strtoull(str, NULL, 10);
    }
    inline double getdouble(const char* key, double notfound) {
        return iniparser_getdouble(dict_, strconcat(section_name_, key), notfound);
    }
    inline int getboolean(const char* key, int notfound) {
        return iniparser_getboolean(dict_, strconcat(section_name_, key), notfound);
    }

    inline const char *get_section_name() {
        return section_name_;
    }
};

#endif
