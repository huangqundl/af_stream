#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.hpp"

Config* Config::instance_ = NULL;

const char* strconcat(const char* a, const char* b) {
    static char l[MAX_LINE_SIZE+1];

    if (a == NULL)
        return b;
    if (b == NULL)
        return NULL;

    strcpy(l, a);
    strcat(l, b);

    return l;
}

/*
 * Constructor
 */
Config::Config(const char* ininame, const char* section_name) {
    if (ininame == 0) {
        fprintf(stderr, "didn't specify configuration file\n");
        return;
    }

    dict_ = iniparser_load(ininame);
    if (dict_ == 0) {
        fprintf(stderr, "error: iniparser_load()\n");
        return;
    }

    if (strlen(section_name) + 2 > MAX_LINE_SIZE) {
        fprintf(stderr, "error: section name is too long\n");
        return;
    }

    strcpy(section_name_, section_name);
    section_name_[strlen(section_name_)] = ':';
    section_name_[strlen(section_name_) + 1] = 0;
}

Config::~Config() {
    iniparser_freedict(dict_);
}
