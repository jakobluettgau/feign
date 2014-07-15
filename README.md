#Flexible Event Imitation Engine

Feign is a modular tool to recreate system activity for benchmarking, stress testing
and debugging targeting parallel workloads.


## Building and Installation

Requirements:

* C/C++ compiler with C++11 support
* Glib 2.0
* MPI

Navigate into feigns repository and run the following commands:

	FEIGN=<feign-repository-root>
    cd $FEIGN
    ./waf configure build

Install using:

    ./waf install


## Plugins

### Using Feign to replay SIOX traces (POSIX)

	cd $FEIGN/tools/gen/layers/posix
	make export                       # also patches $FEIGN/plugins/wscript
	cd $FEIGN
	./waf install




