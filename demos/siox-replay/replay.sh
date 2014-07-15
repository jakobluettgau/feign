#!/bin/sh

# pass datadir with first argument
DATA=$1

# populate environment variables used by the plugin (feing_siox-proivder)
export FEIGN_SIOX_ACTIVITIES=${DATA}/activities.dat
export FEIGN_SIOX_SYSTEMINFO=${DATA}/system-info.dat
export FEIGN_SIOX_ONTOLOGY=${DATA}/ontology.dat
export FEIGN_SIOX_ASSOCIATION=${DATA}/association.dat

feign \
--plugin /home/pq/WR/siox/build/src/tools/TraceReader/feign_plugin/libfeign_siox-provider.so \
--plugin $FEIGN_DEV_PLUGIN_PATH/posix/libfeign_posix-replayer.so \
--plugin $FEIGN_DEV_PLUGIN_PATH/posix/libfeign_posix-precreator.so \
