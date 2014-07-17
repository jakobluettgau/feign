
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


        templateParameters["includes"].append("<map>")
        templateParameters["includes"].append("\"wrapper.h\"")
        templateParameters["includes"].append("\"datatypes.h\"")

        # write all needed includes
        for match in templateParameters["includes"]:
            print('#include ', match, end='\n', file=output)
        print('\n', file=output)   


        print("""/**
 * Map the trace filehandles to actual file handles
 */
int issued = 3;	// 3 will be first issued by this replayer

std::map<int,int> fds;
std::map<int,FILE> streams;""", file=output)

        # declare function prototypes
        for function in functionList:
            functionVariables = self.functionVariables(function)
            # write function signature

            print("int wrapped_", function.name, "(void * data)", sep='', file=output)
            print("{\n", "\t", "posix_", function.name ,"_data * d = (posix_", function.name  ,"_data*) data;", sep='', file=output)

            print("// GENERATED FROM TEMPLATE\n", file=output)
            for templ in function.usedTemplateList:
                outputString = templ.output('init', functionVariables)
                if outputString != '':
                    print('\t', outputString, end='\n', sep='', file=output)
            print("// GENERATED FROM TEMPLATE END\n", file=output)

            print("\t","DEBUG(\"'-", function.name, "()\");", sep='', file=output)
            print("\t","return 0;\n}", sep='', file=output)



        # close the file
        output.close()
