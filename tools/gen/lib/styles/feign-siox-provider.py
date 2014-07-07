from __future__ import print_function

import skeletonBuilder

class Style(skeletonBuilder.Writer):
    #
    # @brief Write a source file
    #
    # @param functions A list of function-objects to write
    def writeOutput(self, options, functionList, templateParameters, precompiler):
        # open the output file for writing
        output = open(options.outputFile, 'w')
        print("/**\n * GENERATED PLUGIN! - Modify carefully. May loose changes on regeneration. \n */", end='\n', sep='', file=output)

        templateParameters["includes"].append("<iostream>")
        templateParameters["includes"].append("<string>")
        templateParameters["includes"].append("<stdlib.h>")
        templateParameters["includes"].append("<stdio.h>")
        
        templateParameters["includes"].append("\"../TraceReader.hpp\"")

        # write all needed includes
        for match in templateParameters["includes"]:
            print('#include ', match, end='\n', file=output)
        print('\n', file=output)  

        
        print("""namespace feign {
	#define FEIGN_NO_CPP_INCLUDES
	#include "feign.h"
	#include "feign-plugin_posix/datatypes.h"
}""", end='\n', file=output)


        # declare function prototypes
        for function in functionList:
            functionVariables = self.functionVariables(function)
            # write function signature

            print("int wrapped_", function.name, "(void * data)", sep='', file=output)
            print("{\n", "\t", "posix_", function.name ,"_data * d = (posix_", function.name  ,"_data*) data;", sep='', file=output)

            print("\t","DEBUG(\"'-", function.name, "()\");", sep='', file=output)
            print("\t","return 0;\n}", sep='', file=output)    


        # close the file
        output.close()



    def generate_create_activity():
        str = ""
        return str

    def generate_provide():
        str = ""
        return str

    def generate_destroy():
        str = ""
        return str
