cmake_minimum_required (VERSION 2.6.4)

project (afstream)

#---------------------- shared library ----------------------

# add ZeroMQ library
include_directories(third_party/zeromq/include)
file(GLOB_RECURSE ZMQ_SRC third_party/zeromq/src/*.cpp)
add_library(zmq SHARED
    ${ZMQ_SRC}
    )
target_link_libraries(zmq rt)

# add ZooKeeper C Bindings
include_directories(third_party/zookeeper-3.4.7 third_party/zookeeper-3.4.7/include third_party/zookeeper-3.4.7/generated)
file(GLOB_RECURSE ZOOKEEPER_MT_SRC
    third_party/zookeeper-3.4.7/generated/*.c
    third_party/zookeeper-3.4.7/src/mt_adaptor.c
    third_party/zookeeper-3.4.7/src/zk_hashtable.c
    third_party/zookeeper-3.4.7/src/zk_log.c
    third_party/zookeeper-3.4.7/src/recordio.c
    third_party/zookeeper-3.4.7/src/hashtable/*.c
    third_party/zookeeper-3.4.7/src/zookeeper.c)
add_library(zkmt SHARED
    ${ZOOKEEPER_MT_SRC}
    )
target_link_libraries(zkmt m rt pthread)

# add iniParser library
include_directories(third_party/iniparser/src)
file(GLOB_RECURSE INIPARSER_SRC third_party/iniparser/src/*.c)
add_library(iniparser SHARED
    ${INIPARSER_SRC}
    )

# add ZeroMQ mailbox library
include_directories(third_party/zeromq/include)
file(GLOB_RECURSE ZMQ_MAILBOX src/control_channel/*.cpp)
add_library(zmq_mailbox SHARED
    ${ZMQ_MAILBOX}
    )
target_link_libraries(zmq_mailbox rt)

# GPRS Parser
if (GPRS_PARSER)
    ADD_DEFINITIONS("-DGPRS_PARSER")
    include_directories(third_party/GPRSParser/)
    file(GLOB_RECURSE GPRS_PARSER_SRC third_party/GPRSParser/*.c)
endif (GPRS_PARSER)

#--------------------------- AF-Stream ---------------------------

include_directories(src)
add_library(afstream_ft SHARED
    src/operator/hash.cpp
    src/fault_tolerance/operator_tracker.cpp
    src/fault_tolerance/backup_client.cpp
    )
target_link_libraries(afstream_ft)

include_directories(src/control_channel)
add_library(afstream_net SHARED
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
    )
target_link_libraries(afstream_net zmq_mailbox)

add_library(afstream_core SHARED
    src/thread/thread.cpp
    src/thread/thread_id.cpp
    src/config.cpp
    src/controller/zk_worker_client.cpp
    )
target_link_libraries(afstream_core pthread afstream_net afstream_ft iniparser zmq zkmt)

#add libegn
include_directories(third_party/libegn/)

#------------------------------------------------------------

#----------------------- executable -------------------------

add_executable(sample_worker1
    apps/sample/sample_worker1.cpp
    )

add_executable(sample_worker2
    apps/sample/sample_worker2.cpp
    )

add_executable(bi_worker1
    apps/sample/bi_worker1.cpp
    )

add_executable(bi_worker2
    apps/sample/bi_worker2.cpp
    )

add_executable(cm_worker
    apps/count_min/cm_sketch.cpp
    apps/count_min/cm_worker.cpp
    )

add_executable(cm_merger
    apps/count_min/cm_sketch.cpp
    apps/count_min/merge_worker.cpp
    )

#--------------------------- Controller ---------------------------

add_executable(controller
    src/controller/main.cpp
    )

add_executable(backup_server
    src/fault_tolerance/backup_server.cpp
    )

#--------------------------- Link ---------------------------

target_link_libraries(controller zkmt)
target_link_libraries(sample_worker1 afstream_core)
target_link_libraries(sample_worker2 afstream_core)
target_link_libraries(bi_worker1 afstream_core)
target_link_libraries(bi_worker2 afstream_core)
target_link_libraries(cm_worker afstream_core)
target_link_libraries(cm_merger afstream_core)


#-----------------------------------

target_link_libraries(backup_server zmq)

#--------------------------- Flags --------------------------

if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    ADD_DEFINITIONS("-DEGN_64")
    set(EGN_LIB ${CMAKE_SOURCE_DIR}/third_party/libegn/linux64/libegn.a)
else( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    set(EGN_LIB ${CMAKE_SOURCE_DIR}/third_party/libegn/linux32/libegn.a)
endif( CMAKE_SIZEOF_VOID_P EQUAL 8 )
#target_link_libraries(pcap_decoder sandcore ${EGN_LIB})

## to compile multi-threaded ZooKeeper C Bindings
ADD_DEFINITIONS("-DTHREADED")

ADD_DEFINITIONS("-D__STDC_FORMAT_MACROS")

ADD_DEFINITIONS("-DLIBNET_VER=1 -DHAVE_ICMPHDR=1 -DHAVE_TCP_STATES=1 -DHAVE_BSD_UDPHDR=1")

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -g -O3 -Wl,-no-as-needed")
