#Flexible Event Imitation Engine

Feign is a modular tool to recreate system activity for benchmarking, stress testing
and debugging targeting parallel workloads.


## Building and Installation

Requirements:

* C/C++ compiler with C++11 support
* Glib 2.0
* MPI

Navigate into feigns repository and run the following commands:

	export FEIGN=$PWD
    cd $FEIGN
    ./waf configure build

Install using:

    ./waf install


## Plugins

Feign can be extended by using plugins to support arbitrary layers (e.g. POSIX,
MPI, ..). Via plugins it is also possible to manipulate and change how the trace
is replayed.

### Replaying SIOX traces (POSIX)

To replay SIOX trace files a number of feign-layer plugins as well as a
feign-siox-provider plugin need to be generated.

	cd $FEIGN/tools/gen/layers/posix
	# modify the Makefile and change $SIOX_TR if needed
	make siox                       # also patches $FEIGN/plugins/wscript
	cd $FEIGN
	./waf install
	# you will also need to rebuild SIOX or at least the siox-tracereader

A simple example to build on can be found in demos directory. Follow the README
for the specific demo to get started.

    cd $FEIGN/demos/siox-replay
    cat README



