/*
 * Packet.cpp
 * - packet header parsed from raw packets
 */

#include <string.h>
#include <stdlib.h>

#include "item.hpp"
    
void Item::parse(char* buf) {
    char *token;
    char search[] = " ";
    token = strtok(buf, search);    // tag
    tag = strtoul(token, NULL, 10);
    token = strtok(NULL, search);   // # of features
    count = strtoul(token, NULL, 10);
    token = strtok(NULL, search);   // features
    feature[0] = strtoll(token, NULL, 10);
    token = strtok(NULL, search);
    feature[1] = strtoll(token, NULL, 10);
}
