# export related
REPO_HOME=../../../..
PLUGIN=${REPO_HOME}/plugins/posix

# build related
OUTPUT_PATH=generated-feign-plugin
HEADER=posix-low-level-io-feign.h
#HEADER=posix-low-level-io.h
#TEMPLATE=../../template.py
TEMPLATE=template-feign.py
TEMPLATE_SIOX_PROVIDER=template-feign_siox-provider.py

# path to siox-trace-reader
SIOX_TR=~/wr/siox/src/tools/TraceReader/feign_siox-provider

all:
	# build sources
	mkdir -p ${OUTPUT_PATH}
	../../siox-skeleton-builder.py -s feign-datatypes-header -t ${TEMPLATE} -o ${OUTPUT_PATH}/datatypes.h       ${HEADER}
	../../siox-skeleton-builder.py -s feign-wrapper          -t ${TEMPLATE} -o ${OUTPUT_PATH}/wrapper.cpp       ${HEADER}
	../../siox-skeleton-builder.py -s feign-wrapper-header   -t ${TEMPLATE} -o ${OUTPUT_PATH}/wrapper.h         ${HEADER}
	../../siox-skeleton-builder.py -s feign-replayer         -t ${TEMPLATE} -o ${OUTPUT_PATH}/replayer.cpp      ${HEADER}
	../../siox-skeleton-builder.py -s feign-siox-provider    -t ${TEMPLATE_SIOX_PROVIDER} -o ${OUTPUT_PATH}/siox-provider.cpp ${HEADER}
	../../siox-skeleton-builder.py -s feign-precreator       -t ${TEMPLATE} -o ${OUTPUT_PATH}/precreator.cpp    ${HEADER}
	# dlsym instrumenter for reference
	../../siox-skeleton-builder.py -s dlsym -t ../../template.py -o ${OUTPUT_PATH}/reference_instrumented.c		${HEADER}
	# add additional files
	cp feign-extras/feign-*-helper.h ${OUTPUT_PATH}/
	cp feign-extras/wscript ${OUTPUT_PATH}/
	

siox: export
	# remove old generated plugin, recreate folder
	rm -rf ${SIOX_TR}/*
	#TODO: revert /* and recreate folder
	#mkdir -p ${SIOX_TR}
	# symlink generated 
	ln -s `cd ${PLUGIN} && pwd` ${SIOX_TR}/feign_posix
	cp ${REPO_HOME}/include/feign.h ${SIOX_TR}  # TODO: maybe replace with symlink?
	cp feign-extras/CMakeLists.txt ${SIOX_TR}
	cp ${OUTPUT_PATH}/siox-provider.cpp ${SIOX_TR}/feign_siox-provider.cpp
	# You may want to apply the patch to TraceReader CMakeLists.txt manually:
	# patch ${SIOX_TR}/../CMakeLists.txt < feign-extras/patchfiles/CMakeLists.txt-add-feign_siox-provider.patch
	# patch ${SIOX_TR}/../TraceReader.hpp < feign-extras/patchfiles/TraceReader.hpp-getAttributeValueByName.patch
	# patch ${SIOX_TR}/../TraceReader.cpp < feign-extras/patchfiles/TraceReader.cpp-getAttributeValueByName.patch

# clear <feign>/plugin/posix folder and create waf compatible plugin
export: all
	# export
	rm -rf ${PLUGIN}
	cp -r ${OUTPUT_PATH} ${PLUGIN}

clean:
	rm -rf ${OUTPUT_PATH}

clean-siox:
	echo "blub"
	#TODO: retract patch
