#!/bin/sh

feign \
--plugin $FEIGN_DEV_PLUGIN_PATH/sample/libfeign_sample.so \
--plugin $FEIGN_DEV_PLUGIN_PATH/sample-provider/libfeign_sample-provider.so \
--plugin $FEIGN_DEV_PLUGIN_PATH/sample-filter/libfeign_sample-filter.so \
--plugin $FEIGN_DEV_PLUGIN_PATH/sample-replayer/libfeign_sample-replayer.so \
--plugin $FEIGN_DEV_PLUGIN_PATH/sample-mutator/libfeign_sample-mutator.so \
--plugin $FEIGN_DEV_PLUGIN_PATH/sample-filter-context/libfeign_sample-filter-context.so \
--plugin $FEIGN_DEV_PLUGIN_PATH/sample-mutator-context/libfeign_sample-mutator-context.so \
$@

#--plugin $FEIGN_DEV_PLUGIN_PATH/sample-provider/libfeign_sample-provider2.so \
#--plugin $FEIGN_DEV_PLUGIN_PATH/sample-filter-context/libfeign_sample-filter-context2.so \
#--plugin $FEIGN_DEV_PLUGIN_PATH/sample-mutator-context/libfeign_sample-mutator-context2.so \
#--plugin $FEIGN_DEV_PLUGIN_PATH/sample-replayer/libfeign_sample-replayer2.so \
