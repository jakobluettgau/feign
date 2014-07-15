#!/bin/sh

# target dir
TRACE_DIR=$1
PROGRAM=$2

# prepare directory
mkdir $TRACE_DIR
cd $TRACE_DIR

# symling siox.conf and create trace
ln -s ../siox.conf
siox-inst posix ../program

# symlink activities.dat1234 for convienience
ACTIVITIES=`find . -name "activities.dat[0-9]*" | tail -n 1`
ln -s $ACTIVITIES activities.dat
