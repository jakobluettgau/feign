#!/bin/sh

# pass datadir with first argument
TRACE_DIR=demo-trace
CHUNKSIZE=$2

#if [ -z "$TRACE_DIR" ]; then
#	echo "Please specifiy a directory that contains a trace to replay."
#	echo "Example:"
#	echo "$0 mytrace"
#	exit
#fi


# options for feign
export FEIGN_CHROOT=$PWD/chroot

# populate environment variables used by the plugin (feing_siox-provider)
export FEIGN_SIOX_ACTIVITIES=${TRACE_DIR}/activities.dat
export FEIGN_SIOX_SYSTEMINFO=${TRACE_DIR}/system-info.dat
export FEIGN_SIOX_ONTOLOGY=${TRACE_DIR}/ontology.dat
export FEIGN_SIOX_ASSOCIATION=${TRACE_DIR}/association.dat

# environment variables used by be the coalescing plugin
export FEIGN_COALESCING_CHUNKSIZE=${CHUNKSIZE}


FEIGN_SIOX_PROVIDER=/home/pq/wr/siox/build/src/tools/TraceReader/feign_siox-provider



activities=("demo-trace/activities.dat1738" "demo-trace/activities.dat1741" "demo-trace/activities.dat1743" "demo-trace/activities.dat1745" "demo-trace/activities.dat1746" "demo-trace/activities.dat1747" "demo-trace/activities.dat1748" "demo-trace/activities.dat1749")


# clear and create feign chroot
rm -rf _feign-chroot
mkdir _feign-chroot

# some traces require the files to be present
touch _feign-chroot/flash_io_test_hdf5_chk_0000
touch _feign-chroot/flash_io_test_hdf5_plt_cnt_0000
touch _feign-chroot/flash_io_test_hdf5_plt_crn_0000


for((i=0;i<${#activities};i++)); 
do 
	echo ${activities[$i]};

	if [ "${activities[$i]}" != "" ];then
	export FEIGN_SIOX_ACTIVITIES=${activities[$i]};
	
	touch _feign-chroot/flash_io_test_hdf5_chk_0000
    touch _feign-chroot/flash_io_test_hdf5_plt_cnt_0000
	touch _feign-chroot/flash_io_test_hdf5_plt_crn_0000


	feign \
--plugin $FEIGN_SIOX_PROVIDER/libfeign_siox-provider.so \
--plugin $FEIGN_DEV_PLUGIN_PATH/../demos/siox-posix-coalescing/feign_posix-coalescing/libfeign_posix-coalescing.so \
--plugin $FEIGN_DEV_PLUGIN_PATH/../demos/siox-posix-coalescing/feign_fix-open-path/libfeign_fix-open-path.so \
--plugin $FEIGN_DEV_PLUGIN_PATH/posix/libfeign_posix-replayer.so \
$@ > /dev/null;

	cat _feign-chroot/flash_io_test_hdf5_chk_0000 >> _feign-chroot/AGGREG_flash_io_test_hdf5_chk_0000;
	cat _feign-chroot/flash_io_test_hdf5_plt_cnt_0000 >> _feign-chroot/AGGREG_flash_io_test_hdf5_plt_cnt_0000;
	cat _feign-chroot/flash_io_test_hdf5_plt_crn_0000 >> _feign-chroot/AGGREG_flash_io_test_hdf5_plt_crn_0000;

	rm _feign-chroot/flash_io_test_hdf5_chk_0000
	rm _feign-chroot/flash_io_test_hdf5_plt_cnt_0000
	rm _feign-chroot/flash_io_test_hdf5_plt_crn_0000


	fi


done


ls -lh _feign-chroot

#--plugin $FEIGN_DEV_PLUGIN_PATH/posix/libfeign_posix-precreator.so \
