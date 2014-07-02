#!/bin/bash -e

HEADER=posix-low-level-io-feign.h
#TEMPLATE=../../template.py
TEMPLATE=../../template-feign.py
OUTPUT_FILE=posix-ll-wrapper
ORIGINAL_OUTPUT_FILE=original

#../../siox-skeleton-builder.py -b -t ../../template_trace.py -o $ORIGINAL_OUTPUT_FILE-blank.h posix-low-level-io.h
#../../siox-skeleton-builder.py -s dlsym -t ../../template_trace.py -o $ORIGINAL_OUTPUT_FILE-dlsym.c posix-low-level-io.h
#../../siox-skeleton-builder.py -s wrap -t ../../template_trace.py -o $ORIGINAL_OUTPUT_FILE-wrap.c posix-low-level-io.h
#../../siox-skeleton-builder.py -s plain -t ../../template_trace.py -o $ORIGINAL_OUTPUT_FILE-plain.c posix-low-level-io.h

#../../siox-skeleton-builder.py -b -t ../../template.py -o $OUTPUT_FILE-blank.h posix-low-level-io.h
../../siox-skeleton-builder.py -s dlsym -t $TEMPLATE -o $OUTPUT_FILE-dlsym.c $HEADER
../../siox-skeleton-builder.py -s wrap  -t $TEMPLATE -o $OUTPUT_FILE-wrap.c  $HEADER
../../siox-skeleton-builder.py -s plain -t $TEMPLATE -o $OUTPUT_FILE-plain.c $HEADER
../../siox-skeleton-builder.py -s feign-datatypes -t $TEMPLATE -o $OUTPUT_FILE-feign-datatypes.h $HEADER
../../siox-skeleton-builder.py -s feign-replayer -t $TEMPLATE -o $OUTPUT_FILE-feign.c $HEADER
../../siox-skeleton-builder.py -s feign-provider -t $TEMPLATE -o $OUTPUT_FILE-feign-provider.c $HEADER

#gcc $OUTPUT_FILE -c -fPIC -I ../../../low-level-C-interface `pkg-config --cflags glib-2.0`
#gcc ${OUTPUT_FILE%%.c}.o -o libsiox-posix-ll-wrapper-dlsym.so -shared -ldl `pkg-config --libs glib-2.0`

