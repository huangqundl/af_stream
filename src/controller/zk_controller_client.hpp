/*
 *
 * CoordinatorClient.hpp
 *
 */

#ifndef __AFS_COORDINATORCLIENT_HPP_INCLUDED__
#define __AFS_COORDINATORCLIENT_HPP_INCLUDED__

#include <inttypes.h>
#include <zookeeper.h>
#include <string.h>
#include <string>
#include <errno.h>
#include "ZkUtil.hpp"
#include "util.hpp"

#include <unordered_set>

std::unordered_set<std::string> alive_workers;

class CoordinatorClient {
protected:

    static zhandle_t *zh;
    static clientid_t myid;
    std::string server_name;
    static int num_of_workers;

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
                zh = 0;
            }
            else if (state == ZOO_EXPIRED_SESSION_STATE) {
                fprintf(stderr, "Session expired. Shutting down...\n");
                zookeeper_close(zzh);
                zh = 0;
            }
        }
        else if (type == ZOO_CHANGED_EVENT ) {
            char tmp[10];
            int tmp_len = 10;
            struct Stat stat;
            int rc = zoo_get(zh, path, 0, tmp, &tmp_len, &stat);
            if (rc != 0) {
                LOG_WARN("get worker content %s: %d", path, rc);
            }
            LOG_MSG("%s change to %s\n", path, tmp);
            rc = zoo_delete(zh, path, -1);
            if (rc) {
                LOG_WARN("rc = %d: error to delete %s\n", rc, path);
            }
        }
        else if (type == ZOO_CHILD_EVENT ) {
            if (!strcmp(path, NODE_NAME_ALIVE)) {
                int rc;
                struct String_vector strings;
                rc = zoo_wget_children(zh, NODE_NAME_ALIVE, global_watcher, NULL, &strings);
                if (rc) {
                    LOG_MSG("Error %d for get child %s\n", rc, NODE_NAME_ALIVE);
                }
                else {
                    if (strings.count > num_of_workers) {
                        LOG_MSG("%d new worker added\n", strings.count - num_of_workers);
                    }
                    else if (strings.count < num_of_workers) {
                        LOG_MSG("%d worker deleted\n", num_of_workers - strings.count);
                    }
                    num_of_workers = strings.count;
                    for (int i=0; i<strings.count; i++) {
                        char tmp[10];
                        int tmp_len = 10;
                        struct Stat stat;
                        std::string worker_name = NODE_NAME_ALIVE;
                        worker_name += "/";
                        worker_name += strings.data[i];
                        rc = zoo_wget(zh, worker_name.c_str(), global_watcher, NULL, tmp, &tmp_len, &stat);
                        //rc = zoo_get(zh, worker_name.c_str(), 0, tmp, &tmp_len, &stat);
                        if (rc != 0) {
                            LOG_WARN("get worker content %s: %d", worker_name.c_str(), rc);
                        }
                        LOG_MSG("Worker name %s: %s\n", strings.data[i], tmp);
                    }
                }
            }
        }
    }

    /*
    static void workload_watcher(zhandle_t *zzh, int type, int state, const char *path,
             void* context) {
        
        if (type == ZOO_CHILD_EVENT) {
            if (!strcmp(path, NODE_NAME_WORKLOAD)) {
                int rc;
                struct String_vector strings;
                //int flags = ZOO_SEQUENCE;
                int flags = 0;
                struct Stat stat;

                while (1) {
                    //rc = zoo_wget_children(zh, NODE_NAME_WORKLOAD, workload_watcher, NULL, &strings);
                    rc = zoo_get_children(zh, NODE_NAME_WORKLOAD, 0, &strings);
                    if (rc) {
                        LOG_MSG("Error %d for get child %s\n", rc, NODE_NAME_ALIVE);
                    }
                    else {
                        if (strings.count == 0) {
                            break;
                        }
                        LOG_MSG("%u overloaded worker\n", strings.count);
                        for (int i=0; i<strings.count; ++i) {
                            char tmp[50];
                            char tmp1[50];
                            tmp1[0] = 0;
                            int queue_index = -1;
                            // qhuang: TODO: need to refine
                            if (strcmp(strings.data[i], "FlowDPI_DPIbolt1") == 0) {
                                sprintf(tmp, NODE_NAME_STATE "/FlowDPI_Flowspout1/hot");
                                sprintf(tmp1, NODE_NAME_STATE "/FlowDPI_Flowspout2/hot");
                                queue_index = 0;
                            }
                            else if (strcmp(strings.data[i], "FlowDPI_DPIbolt2") == 0) {
                                sprintf(tmp, NODE_NAME_STATE "/FlowDPI_Flowspout1/hot");
                                sprintf(tmp1, NODE_NAME_STATE "/FlowDPI_Flowspout2/hot");
                                queue_index = 1;
                            }
                            else if (strcmp(strings.data[i], "FlowDPI_DPIbolt3") == 0) {
                                sprintf(tmp, NODE_NAME_STATE "/FlowDPI_Flowspout1/hot");
                                sprintf(tmp1, NODE_NAME_STATE "/FlowDPI_Flowspout2/hot");
                                queue_index = 2;
                            }
                            else if (strcmp(strings.data[i], "FlowDPI_DPIbolt4") == 0) {
                                sprintf(tmp, NODE_NAME_STATE "/FlowDPI_Flowspout1/hot");
                                sprintf(tmp1, NODE_NAME_STATE "/FlowDPI_Flowspout2/hot");
                                queue_index = 3;
                            }
                            else if (strcmp(strings.data[i], "FakeWorkload_Flowbolt1") == 0) {
                                sprintf(tmp, NODE_NAME_STATE "/FakeWorkload_Spout1/hot");
                                queue_index = 0;
                            }
                            else if (strcmp(strings.data[i], "FakeWorkload_Flowbolt2") == 0) {
                                sprintf(tmp, NODE_NAME_STATE "/FakeWorkload_Spout1/hot");
                                queue_index = 1;
                            }
                            else if (strcmp(strings.data[i], "FakeWorkload_Flowbolt3") == 0) {
                                sprintf(tmp, NODE_NAME_STATE "/FakeWorkload_Spout1/hot");
                                queue_index = 2;
                            }
                            else if (strcmp(strings.data[i], "FakeWorkload_Flowbolt4") == 0) {
                                sprintf(tmp, NODE_NAME_STATE "/FakeWorkload_Spout1/hot");
                                queue_index = 3;
                            }
                            else {}
                            if ((rc=zoo_exists(zh, tmp, 0, &stat)) == ZNONODE) {
                                rc = zoo_create(zh, tmp, (char*)&queue_index, sizeof(int), &ZOO_OPEN_ACL_UNSAFE, flags, NULL, 0);
                                if (rc) {
                                    LOG_MSG("Error at create state node [%s]: %d\n", tmp, rc);
                                }
                            }

                            if (strlen(tmp1) && (rc=zoo_exists(zh, tmp1, 0, &stat)) == ZNONODE) {
                                rc = zoo_create(zh, tmp1, (char*)&queue_index, sizeof(int), &ZOO_OPEN_ACL_UNSAFE, flags, NULL, 0);
                                if (rc) {
                                    LOG_MSG("Error at create state node [%s]: %d\n", tmp1, rc);
                                }
                            }

                            sprintf(tmp, NODE_NAME_WORKLOAD "/%s", strings.data[i]);
                            rc = zoo_delete(zh, tmp, -1);
                            if (rc) {
                                LOG_MSG("Error at delete workload node: %d\n", rc);
                            }
                        }
                    }
                }

                rc = zoo_wget_children(zh, NODE_NAME_WORKLOAD, workload_watcher, NULL, &strings);
                if (rc) {
                    LOG_MSG("Error %d for get child %s\n", rc, NODE_NAME_WORKLOAD);
                }
            }
        }
    }
    */

    void create_barrier_node() {
        int rc;
        int flags = 0;

        LOG_MSG("\nCreating [%s] node\n", NODE_NAME_BARRIER);
        rc = zoo_create(zh, NODE_NAME_BARRIER, NULL, 0, &ZOO_OPEN_ACL_UNSAFE, flags, NULL, 0);
        if (rc) {
            LOG_MSG("rc = %d: error to create %s\n", rc, NODE_NAME_BARRIER);
        }
    }

    void create_state_node() {
        int rc;
        int flags = 0;

        LOG_MSG("\nCreating [%s] node\n", NODE_NAME_STATE);
        rc = zoo_create(zh, NODE_NAME_STATE, NULL, 0, &ZOO_OPEN_ACL_UNSAFE, flags, NULL, 0);
        if (rc) {
            LOG_MSG("rc = %d: error to create %s\n", rc, NODE_NAME_STATE);
        }
    }

    /*
    void create_workload_node() {
        int rc;
        struct String_vector strings;
        int flags = 0;

        LOG_MSG("\nCreating [%s] node\n", NODE_NAME_WORKLOAD);
        rc = zoo_create(zh, NODE_NAME_WORKLOAD, NULL, 0, &ZOO_OPEN_ACL_UNSAFE, flags, NULL, 0);
        if (rc) {
            LOG_MSG("rc = %d: error to create %s\n", rc, NODE_NAME_WORKLOAD);
        }
        
        rc = zoo_wget_children(zh, NODE_NAME_WORKLOAD, workload_watcher, NULL, &strings);
        if (rc) {
            LOG_MSG("Error %d for get child %s\n", rc, NODE_NAME_WORKLOAD);
        }
    }
    
    void delete_workload_node() {
        int rc;

        rc = zoo_delete(zh, NODE_NAME_WORKLOAD, -1);
        if (rc) {
            LOG_MSG("Error at delete workload node: %d\n", rc);
        }
    }
    */

    void delete_state_node() {
        int rc;

        struct String_vector strings;
        rc = zoo_get_children(zh, NODE_NAME_STATE, 0, &strings);
        if (rc) {
            LOG_MSG("Error %d for get child %s\n", rc, NODE_NAME_STATE);
        }
        else {
            char tmp[100];
            for (int i=0; i<strings.count; ++i) {
                sprintf(tmp, NODE_NAME_STATE "/%s", strings.data[i]);
                rc = zoo_delete(zh, tmp, -1);
                if (rc && rc != ZNONODE) {
                    LOG_MSG("Error at delete state node [%s]: %d\n", tmp, rc);
                }
            }
        }

        rc = zoo_delete(zh, NODE_NAME_STATE, -1);
        if (rc) {
            LOG_MSG("Error at delete state node: %d\n", rc);
        }
    }

    void delete_barrier_node() {
        int rc;

        rc = zoo_delete(zh, NODE_NAME_BARRIER, -1);
        if (rc) {
            LOG_MSG("Error at delete barrier node: %d\n", rc);
        }
    }

    void create_alive_node() {
        int rc;
        int flags = 0;

        LOG_MSG("\nCreating [%s] node\n", NODE_NAME_ALIVE);
        char data[10];
        sprintf(data, "%d\n", num_of_workers);
        rc = zoo_create(zh, NODE_NAME_ALIVE, data, strlen(data), &ZOO_OPEN_ACL_UNSAFE, flags, NULL, 0);
        if (rc) {
            LOG_MSG("rc = %d: error to create %s\n", rc, NODE_NAME_ALIVE);
        }

        struct String_vector strings;
        if (num_of_workers > 0) {
            // wait for all workers ready
            int current_worker_num = 0;
            while (current_worker_num < num_of_workers) {
                rc = zoo_get_children(zh, NODE_NAME_ALIVE, 0, &strings);
                if (rc) {
                    LOG_MSG("Error %d for get child %s\n", rc, NODE_NAME_ALIVE);
                }
                else {
                    current_worker_num = strings.count;
                    sleep(1);
                }
            }
        }

        rc = zoo_wget_children(zh, NODE_NAME_ALIVE, global_watcher, NULL, &strings);
        if (rc) {
            LOG_MSG("Error %d for get child %s\n", rc, NODE_NAME_ALIVE);
        }

    }

    void delete_alive_node() {
        int rc;
        struct String_vector strings;

        // wait for all workers complete
        int current_worker_num = num_of_workers;
        LOG_MSG("current worker: %d\n", current_worker_num);
        while (current_worker_num > 0) {
            rc = zoo_get_children(zh, NODE_NAME_ALIVE, 0, &strings);
            if (rc) {
                fprintf(stderr, "Error %d for get child %s\n", rc, NODE_NAME_ALIVE);
            }
            else {
                current_worker_num = strings.count;
                sleep(1);
            }
        }
        
        // delete alive node
        rc = zoo_delete(zh, NODE_NAME_ALIVE, -1);
        if (rc) {
            LOG_MSG("Error at delete alive node: %d\n", rc);
        }
    }

    void create_config() {
        int rc;
        int flags = 0;

        // create node
        std::string config_node_name = NODE_NAME_CONFIG;
        fprintf(stderr, "\nCreating [%s] node\n", config_node_name.c_str());
        rc = zoo_create(zh, config_node_name.c_str(), NULL, 0, &ZOO_OPEN_ACL_UNSAFE, flags, NULL, 0);
        if (rc) {
            fprintf(stderr, "rc = %d: error to create %s\n", rc, config_node_name.c_str());
        }

        // set up parameters of workers
        std::string para_node_name = config_node_name + NODE_NAME_PARAMETER;
        FILE* config_file = fopen(CONFIG_FILE, "r");
        if (config_file == NULL) {
            LOG_MSG("Open error\n");
        }
        char line[100];
        std::string data = "";
        while (fgets(line, 100, config_file) != NULL) {
            data += line;
        }

        fprintf(stderr, "\nCreating [%s] node\n", NODE_NAME_CONFIG NODE_NAME_PARAMETER);
        rc = zoo_create(zh, para_node_name.c_str(), data.c_str(), data.length(), &ZOO_OPEN_ACL_UNSAFE, flags, NULL, 0);
        if (rc) {
            fprintf(stderr, "rc = %d: error to create %s\n", rc, para_node_name.c_str());
        }
    }

    void delete_config_node() {
        int rc;
        struct String_vector strings;

        // delete all children of config node
        rc = zoo_get_children(zh, NODE_NAME_CONFIG, 0, &strings);
        if (rc) {
            fprintf(stderr, "Error %d for get child %s\n", rc, NODE_NAME_CONFIG);
        }
        else {
            for (int i=0; i<strings.count; ++i) {
                char tmp[100];
                sprintf(tmp, NODE_NAME_CONFIG "/%s", strings.data[i]);
                rc = zoo_delete(zh, tmp, -1);
                if (rc) {
                    LOG_MSG("Error at delete config child %s: %d\n", tmp, rc);
                }
            }
        }

        // delete config node
        rc = zoo_delete(zh, NODE_NAME_CONFIG, -1);
        if (rc) {
            LOG_MSG("Error at delete config node: %d\n", rc);
        }
    }

    void create_stat_node() {
        int rc;
        int flags = 0;

        // create node
        fprintf(stderr, "\nCreating [%s] node\n", NODE_NAME_STAT);
        rc = zoo_create(zh, NODE_NAME_STAT, NULL, 0, &ZOO_OPEN_ACL_UNSAFE, flags, NULL, 0);
        if (rc) {
            fprintf(stderr, "rc = %d: error to create %s\n", rc, NODE_NAME_STAT);
        }
    }

    void delete_stat_node() {
        int rc;
        struct String_vector strings;

        // delete all children of stat node
        rc = zoo_get_children(zh, NODE_NAME_STAT, 0, &strings);
        if (rc) {
            fprintf(stderr, "Error %d for get child %s\n", rc, NODE_NAME_STAT);
        }
        else {
            for (int i=0; i<strings.count; ++i) {
                char tmp[100];
                sprintf(tmp, NODE_NAME_STAT "/%s", strings.data[i]);
                rc = zoo_delete(zh, tmp, -1);
                if (rc) {
                    LOG_MSG("Error deleting STAT child %s: %d\n", tmp, rc);
                }
            }
        }

        // delete STAT node
        rc = zoo_delete(zh, NODE_NAME_STAT, -1);
        if (rc) {
            LOG_MSG("Error at delete STAT node: %d\n", rc);
        }
    }

public:
    void set_server_name(std::string addr) {
        server_name = addr;
    }
    
    CoordinatorClient() {
    
    }

    ~CoordinatorClient() {
    }

    int initialize() {
        zoo_set_debug_level(ZOO_LOG_LEVEL_WARN);
        zoo_deterministic_conn_order(1); // enable deterministic order
        zh = zookeeper_init(server_name.c_str(), global_watcher, 300, &myid, NULL, 0);
        if (!zh) {
            return errno;
        }

        create_barrier_node();
        //create_state_node();
        //create_workload_node();
        //create_config();
        create_alive_node();
        //create_stat_node();
        return 0;
    }

    void finish() {
        delete_alive_node();
        //delete_config_node();
        //delete_workload_node();
        //delete_state_node();
        delete_barrier_node();
        //delete_stat_node();
        zookeeper_close(zh);
    }

    virtual void output_stat() {
    }
};

#endif
