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

        # truncate template include list and repopulate
        templateParameters["includes"] = []
        templateParameters["includes"].append("<iostream>")
        templateParameters["includes"].append("<string>")
        templateParameters["includes"].append("<stdlib.h>")
        templateParameters["includes"].append("<stdio.h>")
        templateParameters["includes"].append("<C/siox.h>")
        templateParameters["includes"].append("\"../TraceReader.hpp\"")

        # write all needed includes
        for match in templateParameters["includes"]:
            print('#include ', match, end='\n', file=output)
        print('\n', file=output)  

        for function in functionList:
            functionVariables = self.functionVariables(function)

            for templ in function.usedTemplateList:
                outputString = templ.output('init', functionVariables)
                if outputString != '':
                    print(outputString, end='\n', sep='', file=output)

        print('\n', file=output)  

        # seperate feign logic from siox logic, as symbol names may collide
        print("""namespace feign {
    #define FEIGN_NO_CPP_INCLUDES
    #include "feign.h"
    #include "feign_posix/datatypes.h"
}
""", end='\n\n', file=output)


#        # declare function prototypes
#        for function in functionList:
#            functionVariables = self.functionVariables(function)
#            # write function signature
#
#            print("int wrapped_", function.name, "(void * data)", sep='', file=output)
#            print("{\n", "\t", "posix_", function.name ,"_data * d = (posix_", function.name  ,"_data*) data;", sep='', file=output)
#
#            print("\t","DEBUG(\"'-", function.name, "()\");", sep='', file=output)
#            print("\t","return 0;\n}", sep='', file=output)    


        # seperate feign logic from siox logic, as symbol names may collide
        # Note: namespace not closed
        print("""namespace feign {""", end='\n', file=output)
        # generate init does a little bit more than creating one function
        print(self.generate_init(functionList), file=output)
        print(self.generate_create_activity(functionList), file=output)
        print(self.generate_convert_siox_to_feign(functionList), file=output)
        print(self.generate_provide(functionList), file=output)
        # destroy also generates a helper
        print(self.generate_destroy(functionList), file=output)
        print(self.generate_reset(functionList), file=output)

        print ("} // close feign namespace", file=output)

        # close the file
        output.close()


    ###########################################################################
    # init
    ###########################################################################
    def generate_init(self, functionList):
        str = "// init\n"
        
        str += """
Plugin plugin = {
    .name = "siox-provider",
    .version = NULL,
    .intents = FEIGN_PROVIDER,
};


TraceReader * tr2;

char const * get_dat_env(char const * name, char const * fallback) {
    char const * value;
    value = getenv (name);
    if (! value) {
        value = fallback;
    }
    else {
        printf ("%s = %s\\n", name, value);
    }

    return value;
}

void create_trace_reader() {
    char const * activities = get_dat_env("FEIGN_SIOX_ACTIVITIES", "activities.dat");
    char const * systeminfo = get_dat_env("FEIGN_SIOX_SYSTEMINFO", "systeminfo.dat");
    char const * ontology = get_dat_env("FEIGN_SIOX_ONTOLOGY", "ontology.dat");
    char const * association = get_dat_env("FEIGN_SIOX_ASSOCIATION", "association.dat");

    if ( tr2 != NULL )
        delete tr2;

    tr2 = new TraceReader( (string)activities, (string)systeminfo, (string)ontology, (string)association, "");
}


Plugin * init() {
    // maybe provide an ENUM Bitmap Type to easilly announce capabilities
    // e.g. FEIGN_PLUGIN_SUCCESS && FEIGN_PLUGIN_PROVIDER && FEIGN_PLUGIN_REPLAYER
    // to say: init was completed succesfully, i can provide and replay tracedata
    // please call the appropiate handlers

    printf("Hello from %s\\n", __FILE__);
    // gonna be provider and replayer
    // but provider should negotiate identifier

    //main(0, NULL);



    create_trace_reader();

    // announce features and request further action
    return &plugin;
}


int provided_count = 0;
long int last_start = -1;
long int last_stop = -1;
long int siox_get_offset(monitoring::Activity * a) {
	
	long int start = a->time_start();
	long int stop = a->time_stop();

	if ( 0 == provided_count ) {
		last_stop = start;
		provided_count++;
	}

	//printf("* last_start: %ld\\n", last_start);
	//printf("* last_stop: %ld\\n", last_stop);
	//printf("* start: %ld\\n", start);
	//printf("* stop: %ld\\n", stop);

	long int offset = start - last_stop;
	printf("* offset: %ld\\n", offset);

	last_start = start;
	last_stop = stop;

	return offset;
}

"""

        return str



    ###########################################################################
    # create_subactivity
    ###########################################################################
    def generate_create_subactivity(self, functionList):
        str = "// create_activity\n"
        
        str += """Activity * create_activity(long offset, int type) {
    int layer_id = 13;
    Activity * activity = (Activity *) malloc(sizeof(Activity));

    posix_activity * sub_activity = (posix_activity *) malloc(sizeof(posix_activity));


    void * data;

    switch ( type ) { \n"""

        # declare function prototypes
        for function in functionList:
            functionVariables = self.functionVariables(function)
            # write function signature

            str += "\t"*2 + """case POSIX_%s:\n""" % (function.name)
            str += "\t"*3 + """DEBUG("create %s()");\n""" % (function.name)
            str += "\t"*3 + """break;\n\n"""

        str += """
    }

    sub_activity->type = type;
    //sub_activity->data = data;

    activity->layer = layer_id;
    activity->offset = offset;
    activity->data = sub_activity;
    activity->provider = plugin.instance_id;

    return activity;
}"""
        return str


    ###########################################################################
    # create_activity
    ###########################################################################
    def generate_create_activity(self, functionList):
        str = "// create_activity\n"
        
        str += """Activity * create_activity(long offset, int type, monitoring::Activity * a) {
    int layer_id = 13;
    Activity * activity = (Activity *) malloc(sizeof(Activity));

    posix_activity * sub_activity = (posix_activity *) malloc(sizeof(posix_activity));

    void * data;

    switch ( type ) { \n"""

        # declare function prototypes
        for function in functionList:
            functionVariables = self.functionVariables(function)
           
            str += "\t"*2 + """case POSIX_%s:\n""" % (function.name)
            str += "\t"*2 + "{\n"
            ###
            str += "\t"*3 + "posix_%s_data * d = (posix_%s_data *) malloc(sizeof(posix_%s_data));\n" % (function.name, function.name, function.name)
            #str += "\t"*3 + "posix_%s_data * d = (posix_%s_data*) data;\n" % (function.name, function.name)

            str += "\t"*3 + "data = (void *) d; \n"
        
            # insert template code for creating feign activities
            str += "\t"*3 + "// GENERATED FROM TEMPLATE\n"
            for templ in function.usedTemplateList:
                outputString = templ.output('feign_provider_before', functionVariables)
                if outputString != '':
                    str += "\t"*3 + outputString + '\n'

            for templ in function.usedTemplateList:
                outputString = templ.output('siox2feign', functionVariables)
                if outputString != '':
                    str += "\t"*3 + outputString + '\n'

            for templ in function.usedTemplateList:
                outputString = templ.output('feign_provider_after', functionVariables)
                if outputString != '':
                    str += "\t"*3 + outputString + '\n'

            str += "\t"*3 + "// GENERATED FROM TEMPLATE END\n"

            str += "\t"*3 + """FEIGN_LOG(3, "create %s()");\n""" % (function.name)
            ###
            str += "\t"*2 + "}\n"
            str += "\t"*3 + """break;\n\n"""

        str += """
    }

    sub_activity->type = type;
    sub_activity->data = data;

    activity->layer = layer_id;
    activity->offset = offset;
    activity->data = sub_activity;
    activity->provider = plugin.instance_id;

    return activity;
}"""
        return str

    ###########################################################################
    # convert siox to feign
    ###########################################################################
    def generate_convert_siox_to_feign(self, functionList):
        str = "// create_activity\n"
  
        # generate string lookup strings
        for function in functionList:
            functionVariables = self.functionVariables(function)
            # create match string
            str += "\t"*0 + """std::string str_%s ("%s");\n""" % (function.name, function.name)

        str += """Activity * convert_siox_to_feign(monitoring::Activity * a) {
	Activity * activity = NULL;\n\n"""

        # declare function prototypes


        str += """\n\tstring activity_name = tr2->s->lookup_activity_name( a->ucaid() );\n\n"""

        # declare function prototypes
        for function in functionList:
            functionVariables = self.functionVariables(function)
            # create match string
            str += """\n\t// %s
	if ( str_%s.compare(activity_name) == 0) {
		activity = create_activity(0, POSIX_%s, a);
	} else\n""" % (function.name, function.name, function.name)
  
        str += """\t{\n\t\t// no match \n\t}\n """

        #TODO: offset calculation is wrong, needs to consider previos activity!
        str += """
	// siox timestamps come in nanosecond resolution
	long offset = a->time_stop() - a->time_start();

	if ( activity != NULL ) {
		activity->offset = offset;

		//tr2->printActivity( a );
	}

	return activity;
}"""

        return str

    ###########################################################################
    # provide 
    ###########################################################################
    def generate_provide(self, functionList):
        str = "// provide"
        str += """
Activity * provide(Activity * activity) {
    FEIGN_LOG(3, "provide");

    activity = NULL;

jump:

    monitoring::Activity * a = tr2->nextActivity();
    if( a == nullptr )
        return NULL;

    tr2->printActivity( a );

    activity = convert_siox_to_feign(a);
    delete(a);

    if ( activity == NULL )
        goto jump;


    return activity;
}"""
        return str

    ###########################################################################
    # destroy 
    ###########################################################################
    def generate_destroy(self, functionList):
        str = "// destroy"
        str += """
void free_sub_activity(posix_activity * pa) {
    switch ( pa->type ) {\n"""

        # declare function prototypes
        for function in functionList:
            functionVariables = self.functionVariables(function)
            # write function signature

            str += "\t"*2 + """case POSIX_%s:\n""" % (function.name)
            str += "\t"*3 + """FEIGN_LOG(4, "free %s()");\n""" % (function.name)
            str += "\t"*3 + """break;\n\n"""


        str +="""
    }

}"""
        # actual destroy
        str += """\n\n
Activity * destroy(Activity * activity) {
    FEIGN_LOG(3, "destroy");

    if ( activity->provider == plugin.instance_id ) {
        if ( activity->data != NULL ) {
            posix_activity * sub_activity = (posix_activity *)(activity->data);

            if ( sub_activity->data != NULL ) {
                //free(sub_activity->data);
            } else {
            }
            free(sub_activity);
        } else {
        }
        free(activity);
        return NULL;
    } else {
        return activity;
    }

    return NULL;
}"""
        return str


    ###########################################################################
    # destroy 
    ###########################################################################
    def generate_reset(self, functionList):
        str = "// reset"
        str = """
Activity * reset(Activity * activity) {
    FEIGN_LOG(3,"reset");
    provided_count = 0;
    create_trace_reader();
    return NULL;
}"""
        
        return str
