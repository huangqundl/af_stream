# Compiler
CC = g++

# Compiling options
CFLAGS = -Wall -std=c++11

#ifeq ($(BUILD), debug)   
# "Debug" build - no optimization, and debugging symbols
#CFLAGS += -O0 -g
#else
# "Release" build - optimization, and no debug symbols
CFLAGS += -O2 -s -DNDEBUG
#endif

# Compiling header and libs
HEADER = -I./../../include
LIBB = -L./../../bin -lpthread -lafstream
#LIBB = libafstream.a -L./../../build.tmp
#LIBRA = -lpthread -ldl -larmadillo

# Compiling rules
all: cm_worker merge_worker

cm_worker: cm_worker.cpp
	$(CC) cm_worker.cpp $(CFLAGS) $(HEADER) $(LIBB) -o cm_worker

merge_worker: merge_worker.cpp
	$(CC) merge_worker.cpp $(CFLAGS) $(HEADER) $(LIBB) -o merge_worker

clean:
	rm -rf cm_worker merge_worker
