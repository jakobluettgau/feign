#!/bin/bash -e

OUTPUT_FILE=posix-ll-wrapper
ORIGINAL_OUTPUT_FILE=original

python ../../siox-skeleton-builder.py -b -t ../../template_trace.py -o $ORIGINAL_OUTPUT_FILE-blank.h posix-low-level-io.h
python ../../siox-skeleton-builder.py -s dlsym -t ../../template_trace.py -o $ORIGINAL_OUTPUT_FILE-dlsym.c posix-low-level-io.h
python ../../siox-skeleton-builder.py -s wrap -t ../../template_trace.py -o $ORIGINAL_OUTPUT_FILE-wrap.c posix-low-level-io.h
python ../../siox-skeleton-builder.py -s plain -t ../../template_trace.py -o $ORIGINAL_OUTPUT_FILE-plain.c posix-low-level-io.h

python ../../siox-skeleton-builder.py -b -t ../../template.py -o $OUTPUT_FILE-blank.h posix-low-level-io.h
python ../../siox-skeleton-builder.py -s dlsym -t ../../template.py -o $OUTPUT_FILE-dlsym.c posix-low-level-io.h
python ../../siox-skeleton-builder.py -s wrap -t ../../template.py -o $OUTPUT_FILE-wrap.c posix-low-level-io.h
python ../../siox-skeleton-builder.py -s plain -t ../../template.py -o $OUTPUT_FILE-plain.c posix-low-level-io.h

#gcc $OUTPUT_FILE -c -fPIC -I ../../../low-level-C-interface `pkg-config --cflags glib-2.0`
#gcc ${OUTPUT_FILE%%.c}.o -o libsiox-posix-ll-wrapper-dlsym.so -shared -ldl `pkg-config --libs glib-2.0`

