REPO_HOME=../../../..
PLUGIN=${REPO_HOME}/plugins/posix

# original outputfile prefix as specified in compile.sh
OUTPUT_FILE=generated

all:
	./compile.sh


# clear <feign>/plugin/posix folder and create waf compatible plugin
export: all
	rm -rf ../../../../plugins/posix/*
	cp ${OUTPUT_FILE}-feign-datatypes.h ${PLUGIN}/datatypes.h
	cp ${OUTPUT_FILE}-feign-provider.cpp ${PLUGIN}/provider.cpp
	cp ${OUTPUT_FILE}-feign-wrapper.cpp ${PLUGIN}/wrapper.cpp
	cp ${OUTPUT_FILE}-feign-replayer.cpp ${PLUGIN}/replayer.cpp
	cp ${OUTPUT_FILE}-feign-precreator.cpp ${PLUGIN}/precreator.cpp
	cp feign-*-helper.h ${PLUGIN}/
	cp feign-extras/wscript ${PLUGIN}/


clean:
	rm -f ${OUTPUT_FILE}-*.cpp
	rm -f ${OUTPUT_FILE}-*.h

