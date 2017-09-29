#!/bin/bash


BUILD_PATH="build.tmp"
BUILDTYPE="Release"

TARGET_PATH="bin/"

CMAKECOMMAND=cmake
CMAKECONFPATH=../
MAKECOMMAND=make

# ----- Remove previously generated files. ----- #
rm -Rf $BUILD_PATH

# ----- Generating build directory. ----- #
mkdir $BUILD_PATH
cd $BUILD_PATH

$CMAKECOMMAND -DCMAKE_BUILD_TYPE=$BUILDTYPE $CMAKECONFPATH
$MAKECOMMAND

cd ..

# ----- Remove previously binary files. ----- #
rm -Rf $TARGET_PATH

# ----- Generating binary directory. ----- #
mkdir $TARGET_PATH

# ----- Copy target files. ----- #
cp $BUILD_PATH/libafstream $TARGET_PATH
cp $BUILD_PATH/controller $TARGET_PATH
cp $BUILD_PATH/backup_server $TARGET_PATH
