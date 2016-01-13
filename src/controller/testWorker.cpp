/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <zookeeper.h>
#include <proto.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/time.h>
#include <unistd.h>
#include <sys/select.h>

#include <time.h>
#include <errno.h>
#include <assert.h>

#define _LL_CAST_ (long long)
#define BARRIER_NAME "/barrier"
#define BARRIER_SIZE 3

static zhandle_t *zh;
static clientid_t myid;

static int shutdownThisThing=0;

static int is_barrier_node = 1;

static int count_child = 0;

static __attribute__ ((unused)) void 
printProfileInfo(struct timeval start, struct timeval end, int thres,
                 const char* msg)
{
  int delay=(end.tv_sec*1000+end.tv_usec/1000)-
    (start.tv_sec*1000+start.tv_usec/1000);
  if(delay>thres)
    fprintf(stderr,"%s: execution time=%dms\n",msg,delay);
}

static const char* state2String(int state){
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

static const char* type2String(int state){
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

inline void prepare_request() {
    shutdownThisThing = 0;
}

inline void complete_request() {
    shutdownThisThing = 1;
}

void watcher(zhandle_t *zzh, int type, int state, const char *path,
             void* context)
{
    /* Be careful using zh here rather than zzh - as this may be mt code
     * the client lib may call the watcher before zookeeper_init returns */

    fprintf(stderr, "Watcher %s state = %s", type2String(type), state2String(state));
    if (path && strlen(path) > 0) {
      fprintf(stderr, " for path %s", path);
    }
    fprintf(stderr, "\n");

    if (type == ZOO_SESSION_EVENT) {
        if (state == ZOO_CONNECTED_STATE) {
            const clientid_t *id = zoo_client_id(zzh);
            if (myid.client_id == 0 || myid.client_id != id->client_id) {
                myid = *id;
                fprintf(stderr, "Got a new session id: 0x%llx\n",
                        _LL_CAST_ myid.client_id);
            }
            complete_request();
        } else if (state == ZOO_AUTH_FAILED_STATE) {
            fprintf(stderr, "Authentication failure. Shutting down...\n");
            zookeeper_close(zzh);
            complete_request();
            zh=0;
        } else if (state == ZOO_EXPIRED_SESSION_STATE) {
            fprintf(stderr, "Session expired. Shutting down...\n");
            zookeeper_close(zzh);
            complete_request();
            zh=0;
        }
    }
}

void check_barrier_node_callback(int rc, const struct Stat *stat, const void *data) {
    fprintf(stderr, "\ncheck barrier node: %d\n", rc);
    if (rc == ZNONODE) {
        is_barrier_node = 0;
    }
    free((void*)data);
    complete_request();
}

void count_child_callback(int rc, const struct String_vector *strings,
        const struct Stat *stat, const void *data) {
    //fprintf(stderr, "number of child at [%s]:, %d\n", data, strings->count);
    count_child = strings->count;

    free((void*)data);
    complete_request();
}

void wait_for_complete() {
    int rc;
    fd_set rfds, wfds, efds;
    FD_ZERO(&rfds);
    FD_ZERO(&wfds);
    FD_ZERO(&efds);

    int count = 0;
    while (!shutdownThisThing) {
        int fd;
        int interest;
        struct timeval tv;
        int rc;
        rc = zookeeper_interest(zh, &fd, &interest, &tv);
        if (rc != ZOK) {
            shutdownThisThing = 1;
        }
        if (fd != -1) {
            if (interest&ZOOKEEPER_READ) {
                FD_SET(fd, &rfds);
            } else {
                FD_CLR(fd, &rfds);
            }
            if (interest&ZOOKEEPER_WRITE) {
                FD_SET(fd, &wfds);
            } else {
                FD_CLR(fd, &wfds);
            }
        } else {
            fd = 0;
        }
        rc = zookeeper_process(zh, interest);
        count++;
    }
    fprintf(stderr, "count %d\n", count);
}

void create_barrier_node() {
    int rc;
    int flags = 0;
    char line[10] = BARRIER_NAME;

    prepare_request();
    rc = zoo_aexists(zh, line, 1, check_barrier_node_callback, strdup(line));
    if (rc) {
        fprintf(stderr, "Error %d for %s\n", rc, line);
    }
    wait_for_complete();
    
    if (is_barrier_node == 0) {
        // it seems that zoo_create can work ...
        fprintf(stderr, "\nCreating [%s] node\n", line);
        rc = zoo_create(zh, line, NULL, 0, &ZOO_OPEN_ACL_UNSAFE, flags,
                NULL, 0);
        if (rc) {
            fprintf(stderr, "rc = %d: error to create %s\n", rc, line);
        }
    }
}

void join(char* name) {
    int rc;
    char line[30];
    sprintf(line, BARRIER_NAME "/%s", name);

    int flags = ZOO_EPHEMERAL | ZOO_SEQUENCE;
    fprintf(stderr, "\nCreating [%s] node\n", line);
    rc = zoo_create(zh, line, NULL, 0, &ZOO_OPEN_ACL_UNSAFE, flags,
                NULL, 0);
    if (rc) {
        fprintf(stderr, "rc = %d: error to create %s\n", rc, line);
    }

    while (count_child < BARRIER_SIZE) {
        prepare_request();
        rc= zoo_aget_children2(zh, BARRIER_NAME, 0, count_child_callback, strdup(line));
        if (rc) {
            fprintf(stderr, "Error %d for %s\n", rc, line);
        }
        wait_for_complete();
        sleep(1);
    }
}

int main(int argc, char **argv) {
    int processed=0;
    FILE *fh;

    if (argc != 2) {
        fprintf(stderr,
                "USAGE %s zookeeper_host_list [clientid_file|cmd:(ls|ls2|create|od|...)]\n", 
                argv[0]);
        fprintf(stderr,
                "Version: ZooKeeper cli (c client) version %d.%d.%d\n", 
                ZOO_MAJOR_VERSION,
                ZOO_MINOR_VERSION,
                ZOO_PATCH_VERSION);
        return 2;
    }

    zoo_set_debug_level(ZOO_LOG_LEVEL_WARN);
    zoo_deterministic_conn_order(1); // enable deterministic order
    zh = zookeeper_init(argv[1], watcher, 100000, &myid, 0, 0);
    if (!zh) {
        return errno;
    }
    wait_for_complete();
    create_barrier_node();
    join("work");

    /*
    char c;
    scanf("%c", &c);
    */

    zookeeper_close(zh);
    return 0;
}
