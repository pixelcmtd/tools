#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../config.h"
#include "sm.h"

#define BUILTINSTART if(0)
#define BUILTIN(s) else if(!strcmp(builtin, s))
#define BUILTINEND                                                  \
        else fprintf(stderr,                                        \
                     "sm: Omitting unrecognized builtin \"%s\".\n", \
                     builtin)

#define _set(var, val)                                 \
        {                                              \
                if(var) free(var);                     \
                var = (char *)malloc(strlen(val) + 2); \
                strcpy(var, val);                      \
        }
#define _init(var, env, def)                                     \
        {                                                        \
                char *envval = getenv(env);                      \
                if(envval) _set(var, envval) else _set(var, def) \
        }
#define SETNAMESTART if(0)
#define SETNAME(s, var) else if(!strcmp(name, s)) _set(var, args)
#define SETNAMEEND                                                      \
        else fprintf(stderr,                                            \
                     "sm: Not setting unrecognized variable \"%s\".\n", \
                     name)

#define _APPEND(var, val)                                                      \
        {                                                                      \
                if(!(var)) {                                                   \
                        var = (char *)malloc(strlen(val) + 2);                 \
                        strcpy(var, val);                                      \
                } else {                                                       \
                        char *tmpvar =                                         \
                                (char *)malloc(strlen(val) + strlen(var) + 2); \
                        sprintf(tmpvar, "%s %s", var, val);                    \
                        free(var);                                             \
                        var = tmpvar;                                          \
                }                                                              \
        }
#define APPENDNAMESTART if(0)
#define APPENDNAME(s, var) else if(!strcmp(name, s)) _APPEND(var, args)
#define APPENDNAMEEND                                                        \
        else fprintf(stderr,                                                 \
                     "sm: Not appending to unrecognized variable \"%s\".\n", \
                     name)

int first_index_of(char *s, char c) {
        for(int i = 0; s[i]; i++)
                if(s[i] == c) return i;
        return -1;
}

char *cc = NULL, *cflags = NULL, *cppc = NULL, *cppflags = NULL;

void check_vars() {
        if(!cc) _init(cc, "CC", DEFAULT_CC);
        if(!cppc) _init(cppc, "CXX", DEFAULT_CPPC);
        if(!cflags) _init(cflags, "CFLAGS", DEFAULT_CFLAGS);
        if(!cppflags) _init(cppflags, "CXXFLAGS", DEFAULT_CPPFLAGS);
}

int CC(char *args) {
        int i = first_index_of(args, ' ');
        args[i] = '\0';
        char *output = args;
        args += i + 1;
        char *cmd = (char *)malloc(strlen(cc) + strlen(args) + strlen(output) +
                                   strlen(cflags) + 32);
        sprintf(cmd, "%s %s -o %s %s", cc, cflags, output, args);
        int r = system(cmd);
        free(cmd);
        return r;
}

int CPPC(char *args) {
        int i = first_index_of(args, ' ');
        args[i] = '\0';
        char *output = args;
        args += i + 1;
        char *cmd = (char *)malloc(strlen(cppc) + strlen(args) + strlen(output) +
                                   strlen(cppflags) + 16);
        sprintf(cmd, "%s %s -o %s %s", cppc, args, output, cppflags);
        int r = system(cmd);
        free(cmd);
        return r;
}

void SET(char *args) {
        int i = first_index_of(args, ' ');
        args[i] = '\0';
        char *name = args;
        args += i + 1;
        SETNAMESTART;
        SETNAME("CFLAGS", cflags)
        SETNAME("CC", cc)
        SETNAME("CPPFLAGS", cppflags)
        SETNAME("CPPC", cppc)
        SETNAMEEND;
}

void APPEND(char *args) {
        int i = first_index_of(args, ' ');
        args[i] = '\0';
        char *name = args;
        args += i + 1;
        APPENDNAMESTART;
        APPENDNAME("CFLAGS", cflags)
        APPENDNAME("CC", cc)
        APPENDNAME("CPPFLAGS", cppflags)
        APPENDNAME("CPPC", cppc)
        APPENDNAMEEND;
}

int run_builtin(char *cmd) {
        check_vars();
        int i = first_index_of(cmd, ' ');
        cmd[i] = '\0';
        char *builtin = cmd;
        cmd += i + 1;
        BUILTINSTART;
        BUILTIN("CC") return CC(cmd);
        BUILTIN("CPPC") return CPPC(cmd);
        BUILTIN("SET") SET(cmd);
        BUILTIN("APPEND") APPEND(cmd);
        BUILTINEND;
        /* TODO: give more builtins errors */
        return 0;
}
