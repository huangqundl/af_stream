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

std::string state2String(int state){
  if (state == 0)
    return "CLOSED_STATE";
  if (state == ZOO_CONNECTING_STATE)
    return "CONNECTING_STATE";
  if (state == ZOO_ASSOCIATING_STATE)
    return "ASSOCIATING_STATE";
  if (state == ZOO_CONNECTED_STATE)
    return "CONNECTED_STATE";
  if (state == ZOO_EXPIRED_SESSION_STATE)
    return "EXPIRED_SESSION_STATE";
  if (state == ZOO_AUTH_FAILED_STATE)
    return "AUTH_FAILED_STATE";

  return "INVALID_STATE";
}

std::string type2String(int state){
  if (state == ZOO_CREATED_EVENT)
    return "CREATED_EVENT";
  if (state == ZOO_DELETED_EVENT)
    return "DELETED_EVENT";
  if (state == ZOO_CHANGED_EVENT)
    return "CHANGED_EVENT";
  if (state == ZOO_CHILD_EVENT)
    return "CHILD_EVENT";
  if (state == ZOO_SESSION_EVENT)
    return "SESSION_EVENT";
  if (state == ZOO_NOTWATCHING_EVENT)
    return "NOTWATCHING_EVENT";

  return "UNKNOWN_EVENT_TYPE";
}
    
#endif
