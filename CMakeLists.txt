cmake_minimum_required (VERSION 2.6.4)

project (roll)

#--------------------------- ROLL ---------------------------

#option (FAULT_TOLERENCE
#    "Enable ROLL fault tolerent feasure" ON
#    )
#
#if (FAULT_TOLERENCE)
#    ADD_DEFINITIONS("-DFAULT_TOLERENCE")
    include_directories(src)
    add_library(afstream_op SHARED
        src/operator/stateful_operator.cpp
        src/operator/stateless_operator.cpp
        src/operator/compound_operator.cpp
        src/operator/hash.cpp
        src/operator/counter_array.cpp
        src/operator/hashmap_maxnorm.cpp
        src/operator/hashmap_maxnorm_str.cpp
        src/operator/hashmap_l1norm.cpp
        src/operator/count_min.cpp
        src/fault_tolerance/state_manager.cpp
        src/fault_tolerance/data_manager.cpp
        src/recovery_state.cpp
        )
    target_link_libraries(afstream_op)
#endif (FAULT_TOLERENCE)

#---------------------- shared library ----------------------
# add nDPI library
include_directories(third_party/nDPI/src/include third_party/nDPI/src/lib)
file(GLOB_RECURSE NDPI_SRC third_party/nDPI/src/lib/*.c)
add_library(ndpi SHARED
    ${NDPI_SRC}
    )

# add ZeroMQ library
include_directories(third_party/zeromq/include)
file(GLOB_RECURSE ZMQ_SRC third_party/zeromq/src/*.cpp)
add_library(zmq SHARED
    ${ZMQ_SRC}
    )
target_link_libraries(zmq rt)

# add ZooKeeper C Bindings
include_directories(third_party/zookeeper-3.4.5 third_party/zookeeper-3.4.5/include third_party/zookeeper-3.4.5/generated)
file(GLOB_RECURSE ZOOKEEPER_MT_SRC
    third_party/zookeeper-3.4.5/generated/*.c
    third_party/zookeeper-3.4.5/src/mt_adaptor.c
    third_party/zookeeper-3.4.5/src/zk_hashtable.c
    third_party/zookeeper-3.4.5/src/zk_log.c
    third_party/zookeeper-3.4.5/src/recordio.c
    third_party/zookeeper-3.4.5/src/hashtable/*.c
    third_party/zookeeper-3.4.5/src/zookeeper.c)
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

# add libnids
include_directories(third_party/libnids-1.24/src)
file(GLOB_RECURSE NIDS_SRC third_party/libnids-1.24/src/*.c)
add_library(nids SHARED
    ${NIDS_SRC}
    )
target_link_libraries(nids glib-2.0 nsl gthread-2.0 net)

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

include_directories(
    src/runtime
    src/queues)

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

include_directories(src/control_channel)
add_library(afstream_net_bench SHARED
    src/net/epoll.cpp
    src/net/poller_base.cpp
    src/net/clock.cpp
    src/net/options.cpp
    src/net/random.cpp
    src/net/address.cpp
    src/net/tcp_address.cpp
    src/net/tcp.cpp
    src/net/io_object.cpp
    src/net/msg.cpp
    src/net/v2_encoder.cpp
    src/net/v2_decoder.cpp
    src/net/null_mechanism.cpp
    src/net/mechanism.cpp
    )
target_link_libraries(afstream_net_bench zmq_mailbox)

add_library(afstream_core SHARED
    src/runtime/thread.cpp
    src/config.cpp
    )
target_link_libraries(afstream_core pthread afstream_net afstream_op iniparser zmq)

add_library(afstream_core_bench SHARED
    src/runtime/thread.cpp
    src/config.cpp
    )
target_link_libraries(afstream_core_bench pthread afstream_net_bench iniparser)

add_executable(sample_worker1
    src/example/sample_worker1.cpp
    )

add_executable(sample_worker2
    src/example/sample_worker2.cpp
    )

add_executable(bi_worker1
    src/example/bi_worker1.cpp
    )

add_executable(bi_worker2
    src/example/bi_worker2.cpp
    )

#--------------------------- Link ---------------------------

#target_link_libraries(Coordinator zkmt)
target_link_libraries(sample_worker1 afstream_core)
target_link_libraries(sample_worker2 afstream_core)
target_link_libraries(bi_worker1 afstream_core)
target_link_libraries(bi_worker2 afstream_core)

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

ADD_DEFINITIONS("-D_BSD_SOURCE -DLIBNET_VER=1 -DHAVE_ICMPHDR=1 -DHAVE_TCP_STATES=1 -DHAVE_BSD_UDPHDR=1")

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -g -O3 -Wl,-no-as-needed")
