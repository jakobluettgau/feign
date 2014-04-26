#!/bin/sh

./feign \
--plugin ../build/src/plugins/mpi/libfeign_provider-mpi.so \
--plugin-global ../build/src/plugins/mpi/libfeign_replayer-mpi.so \
$@

#--plugin ../build/src/plugins/mpi/libfeign_precreator-mpi.so \
