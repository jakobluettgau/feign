#!/bin/sh

# pass datadir with first argument
TRACE_DIR=$1

if [ -z "$TRACE_DIR" ]; then
	echo "Please specifiy a directory that contains a trace to replay."
	echo "Example:"
	echo "$0 mytrace"
	exit
fi

# populate environment variables used by the plugin (feing_siox-provider)
export FEIGN_SIOX_ACTIVITIES=${TRACE_DIR}/activities.dat
export FEIGN_SIOX_SYSTEMINFO=${TRACE_DIR}/system-info.dat
export FEIGN_SIOX_ONTOLOGY=${TRACE_DIR}/ontology.dat
export FEIGN_SIOX_ASSOCIATION=${TRACE_DIR}/association.dat


FEIGN_SIOX_PROVIDER=/home/pq/wr/siox/build/src/tools/TraceReader/feign_siox-provider

feign \
--plugin $FEIGN_SIOX_PROVIDER/libfeign_siox-provider.so \
--plugin $FEIGN_DEV_PLUGIN_PATH/posix/libfeign_posix-replayer.so \
$@
#--plugin $FEIGN_DEV_PLUGIN_PATH/posix/libfeign_posix-precreator.so \
