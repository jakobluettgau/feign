#!/bin/sh

feign \
--plugin ../build/src/plugins/sample/libfeign_sample.so \
--plugin ../build/src/plugins/sample-provider/libfeign_sample-provider.so \
--plugin ../build/src/plugins/sample-filter/libfeign_sample-filter.so \
--plugin ../build/src/plugins/sample-replayer/libfeign_sample-replayer.so \
--plugin ../build/src/plugins/sample-mutator/libfeign_sample-mutator.so \
--plugin ../build/src/plugins/sample-filter-context/libfeign_sample-filter-context.so \
--plugin ../build/src/plugins/sample-mutator-context/libfeign_sample-mutator-context.so \
$@

#--plugin ../build/src/plugins/sample-provider/libfeign_sample-provider2.so \
#--plugin ../build/src/plugins/sample-filter-context/libfeign_sample-filter-context2.so \
#--plugin ../build/src/plugins/sample-mutator-context/libfeign_sample-mutator-context2.so \
#--plugin ../build/src/plugins/sample-replayer/libfeign_sample-replayer2.so \
