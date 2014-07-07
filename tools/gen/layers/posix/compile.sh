#!/bin/bash -e

HEADER=posix-low-level-io-feign.h
#TEMPLATE=../../template.py
TEMPLATE=template-feign.py

# SIOX default
#OUTPUT_FILE=generated-ll-wrapper
#../../siox-skeleton-builder.py -b -t ../../template.py -o $OUTPUT_FILE-blank.h posix-low-level-io.h
#../../siox-skeleton-builder.py -s dlsym -t $TEMPLATE -o $OUTPUT_FILE-dlsym.c $HEADER
#../../siox-skeleton-builder.py -s wrap  -t $TEMPLATE -o $OUTPUT_FILE-wrap.c  $HEADER
#../../siox-skeleton-builder.py -s plain -t $TEMPLATE -o $OUTPUT_FILE-plain.c $HEADER

# FEIGN specific
OUTPUT_PATH=generated-feign-plugin/
mkdir -p $OUTPUT_PATH
../../siox-skeleton-builder.py -s feign-datatypes-header -t $TEMPLATE -o ${OUTPUT_PATH}datatypes.h       $HEADER
../../siox-skeleton-builder.py -s feign-wrapper          -t $TEMPLATE -o ${OUTPUT_PATH}wrapper.cpp       $HEADER
../../siox-skeleton-builder.py -s feign-wrapper-header   -t $TEMPLATE -o ${OUTPUT_PATH}wrapper.h         $HEADER
../../siox-skeleton-builder.py -s feign-replayer         -t $TEMPLATE -o ${OUTPUT_PATH}replayer.cpp      $HEADER
../../siox-skeleton-builder.py -s feign-siox-provider    -t $TEMPLATE -o ${OUTPUT_PATH}siox-provider.cpp $HEADER
../../siox-skeleton-builder.py -s feign-precreator       -t $TEMPLATE -o ${OUTPUT_PATH}precreator.cpp    $HEADER


