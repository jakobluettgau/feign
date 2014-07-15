#!/bin/sh

DATA=$1

export FEIGN_SIOX_ACTIVITIES=${DATA}/activities.dat
export FEIGN_SIOX_SYSTEMINFO=${DATA}/system-info.dat
export FEIGN_SIOX_ONTOLOGY=${DATA}/ontology.dat
export FEIGN_SIOX_ASSOCIATION=${DATA}/association.dat

feign \
--plugin /home/pq/WR/siox/build/src/tools/TraceReader/feign_plugin/libfeign_siox-provider.so \
--plugin $FEIGN_DEV_PLUGIN_PATH/posix/libfeign_replayer-posix.so \
--plugin $FEIGN_DEV_PLUGIN_PATH/posix/libfeign_precreator-posix.so \
