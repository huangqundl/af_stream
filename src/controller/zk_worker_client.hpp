/*
 *
 * ZkWorkerClient.hpp
 *
 */

#ifndef __ZookeeperClient_hpp__
#define __ZookeeperClient_hpp__

#include <inttypes.h>
#include <zookeeper.h>
#include <string.h>
#include <string>
#include <set>
//#include <deque>
#include "ZkUtil.hpp"
#include "util.hpp"
//#include "queues/ControlMessage.hpp"
using namespace std;

class ZkWorkerClient {
public:
    //static deque<ControlMsg> remoteCmd;
    string server_name;

private:

    zhandle_t *zh;
    static clientid_t myid;

    string my_name;
    int num_of_workers;
    static set<string> upstream_workers;
    
    static void global_watcher(zhandle_t *zzh, int type, int state, const char *path,
             void* context) {
        
        fprintf(stderr, "Watcher %s state = %s", type2String(type).c_str(), state2String(state).c_str());
        if (path && strlen(path) > 0) {
            fprintf(stderr, " for path %s", path);
        }
        fprintf(stderr, "\n");

        if (type == ZOO_SESSION_EVENT) {
            if (state == ZOO_CONNECTED_STATE) {
                const clientid_t *id = zoo_client_id(zzh);
                if (myid.client_id == 0 || myid.client_id != id->client_id) {
                    myid = *id;
                    fprintf(stderr, "Got a new session id: 0x%" PRIu64 "\n", (uint64_t) myid.client_id);
                }
            }
            else if (state == ZOO_AUTH_FAILED_STATE) {
                fprintf(stderr, "Authentication failure. Shutting down...\n");
                zookeeper_close(zzh);
                //zh = 0;
                zzh = 0;
            }
            else if (state == ZOO_EXPIRED_SESSION_STATE) {
                fprintf(stderr, "Session expired. Shutting down...\n");
                zookeeper_close(zzh);
                //zh = 0;
                zzh = 0;
            }
        }
    }

    /*
    static void worker_state_watcher(zhandle_t *zzh, int type, int state, const char *path,
             void* context) {
        
        if (type == ZOO_CHILD_EVENT) {
            int rc;
            struct String_vector strings;

            while (1) {
                rc = zoo_get_children(zh, path, 0, &strings);
                if (rc) {
                    // LOG_WARN("Error %d for get child %s\n", rc, path);
                    break;
                }
                else {
                    if (strings.count == 0) {
                        break;
                    }
                    for (int i=0; i<strings.count; ++i) {
                        char tmp[50];
                        sprintf(tmp, "%s/%s", path, strings.data[i]);

                        if (strcmp(strings.data[i], "hot") == 0) {
                            struct Stat stat;
                            ControlMsg msg;
                            msg.length = CONTROL_MSG_MAX_SIZE;
                            rc = zoo_get(zh, tmp, 0, msg.data, (int*)(&msg.length), &stat);
                            if (rc) {
                                continue;
                            }

                            msg.type = NOTIFY_OVERLOAD;
                            msg.length = stat.dataLength;
                            remoteCmd.push_back(msg);      
                        }

                        rc = zoo_delete(zh, tmp, -1);
                        if (rc) {
                            // LOG_WARN("Error at delete state chile node: %d\n", rc);
                        }
                    }
                }
            }
            rc = zoo_wget_children(zh, path, worker_state_watcher, NULL, &strings);
            if (rc) {
                // LOG_WARN("rc = %d: error to wget child %s\n", rc, path);
            }
        }
    }
    */

    /*
    void init_upstream_worker(char* data, int data_len) {
    }

    void read_config() {
        int rc;
        struct Stat stat;

        char data[1000];
        int data_len = 1000;

        int retry = 0;
        while (1) {
            rc = zoo_wget(zh, NODE_NAME_CONFIG NODE_NAME_PARAMETER, global_watcher, NULL, data, &data_len, &stat);
            if (rc) {
                LOG_WARN("rc = %d: error to read %s\n", rc, NODE_NAME_CONFIG NODE_NAME_PARAMETER);
            }
            if (data_len == stat.dataLength) {
                data[data_len] = 0;
                LOG_MSG("data length: %d\n%s\n", data_len, data);
                break;
            }
            sleep(0.1);
            retry++;
        }
        LOG_MSG("retry: %d\n", retry);

        init_upstream_worker(data, data_len);
    }
    */

    /*
    void join_barrier() {
        int rc;

        // create barrier child
        int flags = ZOO_EPHEMERAL;
        string barrier_node_name = NODE_NAME_BARRIER;
        string new_str = barrier_node_name + "/" + my_name;
        LOG_MSG("\nCreating [%s] node\n", new_str.c_str());
        rc = zoo_create(zh, new_str.c_str(), NULL, 0, &ZOO_OPEN_ACL_UNSAFE, flags, NULL, 0);
        if (rc) {
            LOG_WARN("rc = %d: error to create %s\n", rc, new_str.c_str());
        }

        // wait for other barrier children
        struct String_vector strings;
        int current_barrier_size = 0;
        while (current_barrier_size < num_of_workers) {
            rc = zoo_get_children(zh, barrier_node_name.c_str(), 0, &strings);
            if (rc) {
                LOG_WARN("Error %d for %s\n", rc, new_str.c_str());
            }
            else {
                current_barrier_size = strings.count;
                sleep(1);
            }
        }
    }

    void leave_barrier() {
        int rc;
        string barrier_node_name = NODE_NAME_BARRIER;
        string new_str = barrier_node_name + "/" + my_name;

        // delete barrier child
        rc = zoo_delete(zh, new_str.c_str(), -1);
        if (rc) {
            LOG_WARN("rc = %d: error to delete %s\n", rc, new_str.c_str());
        }

        // wait for other barrier children
        struct String_vector strings;
        int current_barrier_size = num_of_workers;
        while (current_barrier_size > 0) {
            rc = zoo_get_children(zh, barrier_node_name.c_str(), 0, &strings);
            if (rc) {
                LOG_WARN("Error %d for %s\n", rc, new_str.c_str());
            }
            else {
                current_barrier_size = strings.count;
                sleep(1);
            }
        }
    }
    */

    void create_my_alive_node() {
        int rc;

        struct Stat stat;
        char tmp[1000];
        int tmp_len = 1000;
        // make sure the server is OK
        while ((rc=zoo_exists(zh, NODE_NAME_ALIVE, 0, &stat) == ZNONODE)) {
            sleep(0.1);
        }
        if (rc) {
            LOG_WARN("check alive node error: %d", rc);
            return;
        }
        else {
            //rc = zoo_wget(zh, NODE_NAME_ALIVE, global_watcher, &upstream_workers, tmp, &tmp_len, &stat);
            rc = zoo_get(zh, NODE_NAME_ALIVE, 0, tmp, &tmp_len, &stat);
            if (rc) {
                LOG_WARN("get num of worker: %d", rc);
            }
            num_of_workers = atoi(tmp);
            LOG_MSG("Num of workers: %d\n", num_of_workers);
        }

        // create alive child
        int flags = ZOO_EPHEMERAL;
        string alive_node_name = NODE_NAME_ALIVE;
        alive_node_name += "/" + my_name;
        LOG_MSG("\nCreating [%s] node\n", alive_node_name.c_str());
        char data[] = "Alive\n";
        rc = zoo_create(zh, alive_node_name.c_str(), data, strlen(data), &ZOO_OPEN_ACL_UNSAFE, flags, NULL, 0);
        if (rc) {
            LOG_WARN("rc = %d: error to create %s\n", rc, alive_node_name.c_str());
        }
        
        // create state child
        /*
        struct String_vector strings;
        string state_node_name = NODE_NAME_STATE;
        state_node_name += "/" + my_name;
        rc = zoo_create(zh, state_node_name.c_str(), NULL, 0, &ZOO_OPEN_ACL_UNSAFE, 0, NULL, 0);
        if (rc) {
            LOG_WARN("rc = %d: error to create %s\n", rc, state_node_name.c_str());
        }
        rc = zoo_wget_children(zh, state_node_name.c_str(), worker_state_watcher, NULL, &strings);
        if (rc) {
            LOG_WARN("rc = %d: error to wget %s\n", rc, state_node_name.c_str());
        }
        */

        // wait for other children
        struct String_vector strings;
        int current_worker_num = 0;
        while (current_worker_num < num_of_workers) {
            rc = zoo_get_children(zh, NODE_NAME_ALIVE, 0, &strings);
            if (rc) {
                LOG_WARN("Error %d for get children of [%s]\n", rc, NODE_NAME_ALIVE);
            }
            else {
                current_worker_num = strings.count;
                sleep(1);
            }
        }
    }

    void delete_my_alive_node() {
        int rc;
        //struct String_vector strings;

        /*
        string state_node_name = NODE_NAME_STATE;
        state_node_name += "/" + my_name;

        // delete all unprocessed events
        while (1) {
            rc = zoo_get_children(zh, state_node_name.c_str(), 0, &strings);
            if (rc) {
                LOG_WARN("Error at get state chile node: %d\n", rc);
                break;
            }
            else {
                if (strings.count == 0) {
                    break;
                }
                for (int i=0; i<strings.count; ++i) {
                    char tmp[50];
                    sprintf(tmp, "%s/%s", state_node_name.c_str(), strings.data[i]);
                    rc = zoo_delete(zh, tmp, -1);
                    if (rc) {
                        LOG_WARN("Error at delete state chile node: %d\n", rc);
                    }
                }
            }
        }
            
        // delete state node
        rc = zoo_delete(zh, state_node_name.c_str(), -1);
        if (rc) {
            LOG_WARN("rc = %d: error to delete %s\n", rc, state_node_name.c_str());
        }
        */

        // delete my alive node
        string alive_node_name = NODE_NAME_ALIVE;
        alive_node_name += "/" + my_name;

        char data[] = "Dead\n";
        rc = zoo_set(zh, alive_node_name.c_str(), data, strlen(data), -1);
        if (rc) {
            LOG_WARN("rc = %d: error to modify %s\n", rc, alive_node_name.c_str());
        }

        // wait for controller to delete all dead workers
        struct Stat stat;
        while ((rc=zoo_exists(zh, alive_node_name.c_str(), 0, &stat)) == ZOK) {
            sleep(0.1);
        }
        if (rc != ZNONODE) {
            LOG_WARN("wait to delete alive node error: %d", rc);
        }

        /*
        rc = zoo_delete(zh, alive_node_name.c_str(), -1);
        if (rc) {
            LOG_WARN("rc = %d: error to delete %s\n", rc, alive_node_name.c_str());
        }
        */
    }

public:
    
    ZkWorkerClient(string name, string addr): my_name(name), num_of_workers(0) {
        server_name = addr;
    }

    ~ZkWorkerClient() {
    }

    int initialize() {
        zoo_set_debug_level(ZOO_LOG_LEVEL_WARN);
        zoo_deterministic_conn_order(1); // enable deterministic order
        zh = zookeeper_init(server_name.c_str(), global_watcher, 300, &myid, NULL, 0);
        if (!zh) {
            return errno;
        }

        create_my_alive_node();
        //join_barrier();
        //read_config();
        // init other things
        //leave_barrier();
        // start work
        return 0;
    }

    void finish() {
        delete_my_alive_node();
        zookeeper_close(zh);
    }

    bool is_done() {
        return upstream_workers.size() == 0;
    }

    /*
#ifdef LOAD_BALANCE
    int export_state(char* key, uint32_t key_length, const char* data, uint32_t len, bool is_shared_state, bool is_rewrite) {
        char node_name[30];
        sprintf(node_name, NODE_NAME_STATE "/");
        memcpy(node_name+7, key, key_length);
        node_name[7+key_length] = 0;

        int rc = zoo_create(zh, node_name, data, len, &ZOO_OPEN_ACL_UNSAFE, 0, NULL, 0);
        if (rc == ZNODEEXISTS) {
            if (is_rewrite) {
                zoo_set(zh, node_name, data, len, -1);
                return 0;
            }
            else if (is_shared_state) {
                return 0;
            }
            else {
                LOG_WARN("rc = %d: error to create %s\n", rc, node_name);
                return rc;
            }
        }
        else if (rc) {
            LOG_WARN("rc = %d: error to create %s\n", rc, node_name);
            return rc;
        }
        return 0;
    
    }

    int get_state(char* key, uint32_t key_length, char* data, uint32_t* data_len) {
        char node_name[30];
        sprintf(node_name, NODE_NAME_STATE "/");
        memcpy(node_name+7, key, key_length);
        node_name[7+key_length] = 0;

        struct Stat stat;
        int rc;
        uint32_t retry = 0;
        while (1) {
            // avoid blocking the Daemon
            if (retry == 500) {
                break;
            }
            rc = zoo_get(zh, node_name, 0, data, (int*)data_len, &stat);
            if (rc == 0) {
                break;
            }
            retry++;
        }
        if (rc) {
            return rc;
        }
        *data_len = stat.dataLength;
        return 0;
    }
    */

    /*
    int complete_read_state(char* key, uint32_t key_length) {
        char node_name[30];
        sprintf(node_name, NODE_NAME_STATE "/");
        memcpy(node_name+7, key, key_length);
        node_name[7+key_length] = 0;

        // delete barrier child
        int rc = zoo_delete(zh, node_name, -1);
        if (rc) {
            LOG_WARN("rc = %d: error to delete %s\n", rc, node_name);
            return rc;
        }
        return 0;
    }
    */

/*
#ifdef LOAD_BALANCE_FEEDBACK
    int report_overload() {
        int flags = ZOO_EPHEMERAL;
        char tmp[50];
        sprintf(tmp, NODE_NAME_WORKLOAD "/%s", my_name.c_str());
        int rc = zoo_create(zh, tmp, NULL, 0, &ZOO_OPEN_ACL_UNSAFE, flags, NULL, 0);
        if (rc) {
            if (rc == ZNODEEXISTS) {
                return 0;
            }
            LOG_WARN("rc = %d: error to create %s\n", rc, tmp);
            return rc;
        }
        return 0;
    }
#endif // LOAD_BALANCE_FEEDBACK
#endif // LOAD_BALANCE

    int report_stat(char* stat_data, int len) {
        static int count = 0;
        char node_name[50];
        sprintf(node_name, NODE_NAME_STAT "/%s%d", my_name.c_str(), count++);
        int rc = zoo_create(zh, node_name, stat_data, len, &ZOO_OPEN_ACL_UNSAFE, 0, NULL, 0);
        if (rc == ZNODEEXISTS)
            zoo_set(zh, node_name, stat_data, len, -1);
        else if (rc) {
            LOG_WARN("rc = %d: error to create %s\n", rc, node_name);
            return rc;
        }
        return 0;
    }
*/
};

#endif
