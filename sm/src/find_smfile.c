#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "getcwd.h"

inline static char *append_smfile(char *c) {
        c += strlen(c);
        *c++ = '/';
        char *d = c;
        *c++ = 'S';
        *c++ = 'm';
        *c++ = 'f';
        *c++ = 'i';
        *c++ = 'l';
        *c++ = 'e';
        return d;
}

inline static void append_dots(char *nd) {
        nd += strlen(nd);
        *nd++ = '/';
        *nd++ = '.';
        *nd++ = '.';
}

#define return_err                                            \
        {                                                     \
                fprintf(stderr, "sm: %s\n", strerror(errno)); \
                free(fn);                                     \
                return NULL;                                  \
        }

/*
 * This might not be the best implementation,
 * but it's the first one I got to work.
 * TODO: fix
 */
char *find_smfile() {
        for(;;) {
                char *fn = (char *)malloc(PATH_MAX + 8);
                memset(fn, 0, PATH_MAX + 8);
                if(!getcwd(fn, PATH_MAX)) return_err;
                if(!fn[1]) {
                        free(fn);
                        return NULL;
                }
                char *c = append_smfile(fn);
                if(!access(fn, R_OK)) return fn;
                *c = 's';
                if(!access(fn, R_OK)) return fn;
                char nd[PATH_MAX + 4] = {0};
                if(!getcwd(nd, PATH_MAX)) return_err;
                append_dots(nd);
                if(chdir(nd)) return_err;
                free(fn);
        }
}
