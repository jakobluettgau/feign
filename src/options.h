#ifndef OPTIONS_H_ZTNECL9V
#define OPTIONS_H_ZTNECL9V

// to handle environment variables
int register_env();
void parse_env();

// to handle arguments
int register_arg();
void parse_args(int * argc, char *argv[]);
void print_args(int argc, char *argv[]);

#endif /* end of include guard: OPTIONS_H_ZTNECL9V */

