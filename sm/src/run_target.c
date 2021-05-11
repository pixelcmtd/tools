#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "sm.h"

#define noroot getuid

#define NEWLINE(c) ((c) == '\r' || (c) == '\n')
#define SON(c) ((c) == ' ' || (c) == '\t' || NEWLINE(c))
#define READ_WHILE(b) for(c = fgetc(f); (b) && c != EOF; c = fgetc(f))
#define COPY_WHILE(b) READ_WHILE(b) *d++ = c
#define SKIP_WHILE(b) \
        while(b) c = fgetc(f)

#define OPTIONSTART if(0)
#define OPTION(s) else if(!strcmp(s, bfr))
#define OPTIONEND \
        else fprintf(stderr, "sm: Omitting unknown option \"%s\".\n", bfr);

#define TRETURN    \
        fclose(f); \
        free(bfr); \
        return

int run_command(int mod, char *cmd) {
        switch(mod) {
                /*
                 * TODO: implement a modifier that fails the build if
                 *       the return code is non-zero
                 */
                case '%': return run_builtin(cmd);
                case '!': return system(cmd);
                case '#': return 0;
                default:
                        fprintf(stderr,
                                "sm: Omitting command \"%s\" with unknown "
                                "modifier \"%c\".\n",
                                cmd,
                                mod);
                        return 0;
        }
}

int run_target(char *smfile, char *target, char *argv0) {
        off_t size;
        struct stat s;
        int c;
        char *d;
        if(stat(smfile, &s)) {
                /* TODO: Check if this could be a security vulnerability */
                printf("Can't get file size (assuming 256K): %s\n",
                       strerror(errno));
                size = 256 * 1024;
        } else
                size = s.st_size;
        char *bfr = (char *)malloc(size + strlen(target) + strlen(argv0));
        FILE *f = fopen(smfile, "r");
        READ_WHILE(1) {
                if(SON(c)) continue; /* skip whitespace */

                /* read the target name */
                d = bfr;
                *d++ = c;
                COPY_WHILE(!SON(c) && c != '{' && c != '(');
                *d = '\0';
                if(strcmp(bfr, target))
                { /* target != this one */
                        /* skip the whole code block */
                        READ_WHILE(c != '}');
                        continue;
                }
                if(c == '{') goto execute;
                if(c == '(') goto deps;
                READ_WHILE(c != '{' && c != '(') {
                        if(SON(c)) continue;
                        d = bfr;
                        *d++ = c;
                        COPY_WHILE(!SON(c) && c != '{');
                        *d = '\0';
                        OPTIONSTART;
                        OPTION("root") {
                                if(noroot()) {
                                        /* TODO: get rid of this somehow
                                         * TODO: also make sure that the deps
                                         * arent run as root
                                         */
                                        sprintf(bfr,
                                                "sudo '%s' '%s'",
                                                argv0,
                                                target);
                                        fclose(f);
                                        c = system(bfr);
                                        free(bfr);
                                        return c;
                                }
                        }
                        OPTIONEND;
                }
                if(c == '{') goto execute;
deps: /* read all the dependencies and run them */
                while(c != ')') {
                        SKIP_WHILE(SON(c) || c == ',' || c == '(');
                        if(c == ')') break;
                        d = bfr;
                        *d++ = c;
                        COPY_WHILE(!SON(c) && c != ',' && c != ')');
                        *d = '\0';
                        int m = run_target(smfile, bfr, argv0);
                        if(m) { TRETURN m; }
                }
                SKIP_WHILE(c != '{');
execute: /* execute the code */
                READ_WHILE(c != '}') {
                        SKIP_WHILE(SON(c));
                        if(c == '}') break;
                        int m = c;
                        d = bfr;
                        COPY_WHILE(!NEWLINE(c));
                        *d = '\0';
                        run_command(m, bfr);
                }
                TRETURN 0;
        }
        fprintf(stderr, "Target \"%s\" not found.\n", target);
        TRETURN 1;
}
