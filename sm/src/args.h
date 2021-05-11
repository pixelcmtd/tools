#include <string.h>

#define ARGSTART                                                         \
        int _args_inargs = 1;                                            \
        for(int _args_argc_i = 1; _args_argc_i < argc; _args_argc_i++) { \
                if(!strcmp(argv[_args_argc_i], "--")) _args_inargs = 0
#define ARG(unix, gnu)                                                    \
        else if(_args_inargs && (!strcmp(argv[_args_argc_i], "-" unix) || \
                                 !strcmp(argv[_args_argc_i], "--" gnu)))
#define ARGEND }
