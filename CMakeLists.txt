cmake_minimum_required (VERSION 2.6.4)

project (afstream)

#---------------------- shared library ----------------------

### add ZeroMQ library
    include_directories(third_party/zeromq/include)
    file(GLOB_RECURSE ZMQ_SRC third_party/zeromq/src/*.cpp)
    add_library(zmq OBJECT ${ZMQ_SRC})
    #target_link_libraries(zmq pthread rt)

### add ZooKeeper C Bindings
    include_directories(third_party/zookeeper-3.4.7 third_party/zookeeper-3.4.7/include third_party/zookeeper-3.4.7/generated)
    file(GLOB_RECURSE ZOOKEEPER_MT_SRC
        third_party/zookeeper-3.4.7/generated/*.c
        third_party/zookeeper-3.4.7/src/mt_adaptor.c
        third_party/zookeeper-3.4.7/src/zk_hashtable.c
        third_party/zookeeper-3.4.7/src/zk_log.c
        third_party/zookeeper-3.4.7/src/recordio.c
        third_party/zookeeper-3.4.7/src/hashtable/*.c
        third_party/zookeeper-3.4.7/src/zookeeper.c)
    add_library(zkmt OBJECT ${ZOOKEEPER_MT_SRC})
    #target_link_libraries(zkmt m rt pthread)

### add iniParser library
    include_directories(third_party/iniparser/src)
    file(GLOB_RECURSE INIPARSER_SRC third_party/iniparser/src/*.c)
    add_library(iniparser OBJECT ${INIPARSER_SRC})

# add ZeroMQ mailbox library
    include_directories(third_party/zeromq/include)
    file(GLOB_RECURSE ZMQ_MAILBOX_SRC src/control_channel/*.cpp)
    add_library(zmq_mailbox OBJECT ${ZMQ_MAILBOX_SRC})
    #target_link_libraries(zmq_mailbox rt)

#--------------------------- AF-Stream ---------------------------

include_directories(src)
include_directories(src/control_channel)

add_library(afstream SHARED
    src/config.cpp
    src/zmq_err.cpp
    src/thread/thread.cpp
    src/thread/thread_id.cpp
    src/operator/hash.cpp
    src/fault_tolerance/operator_tracker.cpp
    src/fault_tolerance/backup_client.cpp
    src/controller/ZkUtil.cpp
    src/controller/zk_worker_client.cpp
    src/net/epoll.cpp
    src/net/poller_base.cpp
    src/net/clock.cpp
    src/net/options.cpp
    src/net/tcp_listener.cpp
    src/net/tcp_connecter.cpp
    src/net/random.cpp
    src/net/address.cpp
    src/net/tcp_address.cpp
    src/net/tcp.cpp
    src/net/io_object.cpp
    src/net/stream_engine.cpp
    src/net/msg.cpp
    src/net/v2_encoder.cpp
    src/net/v2_decoder.cpp
    src/net/null_mechanism.cpp
    src/net/mechanism.cpp
    ${ZOOKEEPER_MT_SRC}
    ${INIPARSER_SRC}
    $<TARGET_OBJECTS:zmq>
    $<TARGET_OBJECTS:zmq_mailbox>
    )
target_link_libraries(afstream pthread rt)

#--------------------------- Controller ---------------------------

add_executable(controller
    src/controller/ZkUtil.cpp
    src/controller/main.cpp
    #${ZOOKEEPER_MT_SRC}
    $<TARGET_OBJECTS:zkmt>
    )
target_link_libraries(controller m rt pthread)

#--------------------------- Backup Server ------------------------

add_executable(backup_server
    src/fault_tolerance/backup_server.cpp
    $<TARGET_OBJECTS:zmq>
    )
target_link_libraries(backup_server pthread rt)

#----------------------- executable -------------------------

#add_executable(sample_worker1
#    apps/sample/sample_worker1.cpp
#    )
#
#add_executable(sample_worker2
#    apps/sample/sample_worker2.cpp
#    )

#add_executable(bi_worker1
#    apps/sample/bi_worker1.cpp
#    )
#
#add_executable(bi_worker2
#    apps/sample/bi_worker2.cpp
#    )
#
#add_executable(cm_worker
#    apps/count_min/cm_sketch.cpp
#    apps/count_min/cm_worker.cpp
#    )
#
#add_executable(cm_merger
#    apps/count_min/cm_sketch.cpp
#    apps/count_min/merge_worker.cpp
#    )

#add_executable(adn_worker
#    apps/adn_dc/adn_worker.cpp
#    )

#--------------------------- Benchmarks ---------------------------

#add_executable(bench_mulwriter
#    benchmark/ringbuffer/multi_writer.cpp
#    src/thread/thread.cpp
#    src/thread/thread_id.cpp
#    )
#target_link_libraries(bench_mulwriter pthread rt zmq_mailbox)

#--------------------------- Link ---------------------------

#target_link_libraries(sample_worker1 afstream)
#target_link_libraries(sample_worker2 afstream)
#target_link_libraries(bi_worker1 afstream)
#target_link_libraries(bi_worker2 afstream)
#target_link_libraries(cm_worker afstream)
#target_link_libraries(cm_merger afstream)
#target_link_libraries(adn_worker afstream)

#--------------------------- Flags --------------------------

## to compile multi-threaded ZooKeeper C Bindings
ADD_DEFINITIONS("-DTHREADED")

## to print PRIu64
ADD_DEFINITIONS("-D__STDC_FORMAT_MACROS")

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -g -O3 -Wl,-no-as-needed")
