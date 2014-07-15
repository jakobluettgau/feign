# export related
REPO_HOME=../../../..
PLUGIN=${REPO_HOME}/plugins/posix

# build related
OUTPUT_PATH=generated-feign-plugin/
HEADER=posix-low-level-io-feign.h
#TEMPLATE=../../template.py
TEMPLATE=template-feign.py

# path to siox-trace-reader
SIOX=~/wr/siox/src/tools/TraceReader/feing-siox-provider

all:
	# build sources
	mkdir -p ${OUTPUT_PATH}
	../../siox-skeleton-builder.py -s feign-datatypes-header -t ${TEMPLATE} -o ${OUTPUT_PATH}datatypes.h       ${HEADER}
	../../siox-skeleton-builder.py -s feign-wrapper          -t ${TEMPLATE} -o ${OUTPUT_PATH}wrapper.cpp       ${HEADER}
	../../siox-skeleton-builder.py -s feign-wrapper-header   -t ${TEMPLATE} -o ${OUTPUT_PATH}wrapper.h         ${HEADER}
	../../siox-skeleton-builder.py -s feign-replayer         -t ${TEMPLATE} -o ${OUTPUT_PATH}replayer.cpp      ${HEADER}
	../../siox-skeleton-builder.py -s feign-siox-provider    -t ${TEMPLATE} -o ${OUTPUT_PATH}siox-provider.cpp ${HEADER}
	../../siox-skeleton-builder.py -s feign-precreator       -t ${TEMPLATE} -o ${OUTPUT_PATH}precreator.cpp    ${HEADER}
	# add additional files
	cp feign-*-helper.h ${OUTPUT_PATH}/
	cp feign-extras/wscript ${OUTPUT_PATH}/
	

siox:
	mkdir -p 

# clear <feign>/plugin/posix folder and create waf compatible plugin
export: all
	# export
	rm -rf ${PLUGIN}
	cp -r ${OUTPUT_PATH} ${PLUGIN}

clean:
	rm -rf ${OUTPUT_PATH}

clean-siox:
	echo "blub"
