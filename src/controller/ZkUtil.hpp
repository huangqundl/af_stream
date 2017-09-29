/*
 *
 * ZkUtil.hpp
 *
 */

#ifndef __ZkUtil_hpp__
#define __ZkUtil_hpp__

#include<string>

#define CONFIG_FILE "../conf/Demo-6.ini"

#define NODE_NAME_BARRIER "/barrier"
#define NODE_NAME_STATE "/state"
#define NODE_NAME_CONFIG "/Config"
#define NODE_NAME_PARAMETER "/Parameter"
#define NODE_NAME_WORKLOAD "/Workload"
//#define NODE_NAME_NUM_OF_WORKER "/NumOfWorker"
#define NODE_NAME_ALIVE "/Alive"
#define NODE_NAME_STAT "/stat"

std::string state2String(int state);
std::string type2String(int state);
    
#endif
