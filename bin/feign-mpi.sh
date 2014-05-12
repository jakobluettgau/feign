#!/bin/sh

feign \
--plugin $FEIGN_DEV_PLUGIN_PATH/mpi/libfeign_provider-mpi.so \
--plugin-global $FEIGN_DEV_PLUGIN_PATH/mpi/libfeign_replayer-mpi.so \
$@

#--plugin ../build/src/plugins/mpi/libfeign_precreator-mpi.so \
