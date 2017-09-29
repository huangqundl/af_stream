#!/bin/bash
# usage: ./install install_path
if [ $# -eq 0 ]; then
    prefix=`dirname "$0"`
    prefix=`cd "$prefix"; pwd`
else
    prefix=$1
fi

echo $prefix

cp third_party/iniparser/src/*.h $prefix/include

cp third_party/zookeeper-3.4.7/include/*.h $prefix/include
cp third_party/zookeeper-3.4.7/generated/*.h $prefix/include
