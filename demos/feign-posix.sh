#!/bin/sh

feign \
--plugin $FEIGN_DEV_PLUGIN_PATH/posix/libfeign_provider-posix.so \
--plugin $FEIGN_DEV_PLUGIN_PATH/posix/libfeign_replayer-posix.so \
--plugin $FEIGN_DEV_PLUGIN_PATH/posix/libfeign_precreator-posix.so \
