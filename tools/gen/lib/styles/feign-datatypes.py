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

        # write all needed includes
        for match in templateParameters["includes"]:
            print('#include ', match, end='\n', file=output)
        print('\n', file=output)

        # write global once from template
        print(templateParameters["globalOnce"], file=output)

        # write enum for function names for easy switching
        print("enum functions {", end='\n', sep='', file=output)
        for function in functionList:
            functionVariables = self.functionVariables(function)
            # write function enum name form signature
            print("\t", "POSIX_", function.name, end=',\n', sep='', file=output)
        
        print("};", end='\n\n', sep='', file=output)

        # write all functions-bodies
        for function in functionList:
            functionVariables = self.functionVariables(function)

            # write function signature
            print("struct", end=' ', sep='', file=output)
            print("posix_", function.name, "_struct", end='\n', sep='', file=output)
            print("{", end='\n', sep='', file=output)

            # add parameters to struct
            for parameter in function.parameterList:
                if parameter.type != "..." and parameter.type != "void":
                    print("\t", parameter.type, end=' ', sep='', file=output) 
                    print(parameter.name, end=';\n', sep='', file=output) 

            # a variable to save the return-value
            returnType = function.type

            if returnType != "void":
                print('\t', returnType, ' ret;', end='\n', sep='',
                      file=output)
            
            # close the struct defintion
            print('};', end='\n', file=output)

            #e.g.  typedef struct posix_open_struct posix_open_data;

            print("typedef struct", end=' ', sep='', file=output)
            print("posix_", function.name, "_struct", end=' ', sep='', file=output) 
            print("posix_", function.name, "_data", end=';\n\n\n', sep='', file=output) 


        # close the file
        output.close()
