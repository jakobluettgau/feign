REPO_HOME=../../../..
PLUGIN=${REPO_HOME}/plugins/posix

# original outputfile prefix as specified in compile.sh
OUTPUT_FILE=posix-ll-wrapper

all:
	./compile.sh


# clear <feign>/plugin/posix folder and create waf compatible plugin
export: all
	rm -rf ../../../../plugins/posix/*
	cp ${OUTPUT_FILE}-feign-provider.c ${PLUGIN}/provider.cpp
	cp ${OUTPUT_FILE}-feign-replayer.c ${PLUGIN}/replayer.cpp
	cp ${OUTPUT_FILE}-feign-precreator.c ${PLUGIN}/precreator.cpp
	cp ${OUTPUT_FILE}-feign-datatypes.h ${PLUGIN}/datatypes.h
	cp feign-*-helper.h ${PLUGIN}/
	cp feign-extras/wscript ${PLUGIN}/


clean:
	rm -f ${OUTPUT_FILE}-*.c
	rm -f ${OUTPUT_FILE}-*.h

