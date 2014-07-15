#!/bin/sh

feign \
--plugin $FEIGN_DEV_PLUGIN_PATH/posix/libfeign_posix-provider.so \
--plugin $FEIGN_DEV_PLUGIN_PATH/posix/libfeign_posix-replayer.so \
--plugin $FEIGN_DEV_PLUGIN_PATH/posix/libfeign_posix-precreator.so \
