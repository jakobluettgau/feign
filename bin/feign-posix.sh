#!/bin/sh

feign \
--plugin ../build/src/plugins/posix/libfeign_provider-posix.so \
--plugin ../build/src/plugins/posix/libfeign_replayer-posix.so \
--plugin ../build/src/plugins/posix/libfeign_precreator-posix.so \
