#!/bin/sh

# target dir
TRACE_DIR=$1
PROGRAM=$2

if [ -z "$TRACE_DIR" ];
then
	echo "Please specifiy a directory to create and store the trace in."
	echo "Example:"
	echo "$0 mytrace"
	exit
fi

# prepare directory
mkdir $TRACE_DIR
cd $TRACE_DIR

# symling siox.conf and create trace
ln -s ../siox.conf
# prepare environment for tracing, then trace actual application
../prepare
siox-inst posix ../program

# symlink activities.dat1234 for convienience
ACTIVITIES=`find . -name "activities.dat[0-9]*" | tail -n 1`
ln -s $ACTIVITIES activities.dat
