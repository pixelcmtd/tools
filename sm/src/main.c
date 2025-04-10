#include <stdio.h>
#include <stdlib.h>

#include "args.h"
#include "sm.h"

#define PROGNAME "sm"
#define VERSION "0.2.0"
#define YEARS "2019-2025"
#define AUTHORS "Pixel Häußler, chrissx Media"
#define VERSIONINFO PROGNAME " " VERSION "\n(c) " YEARS " " AUTHORS

#define HELP                                                          \
        VERSIONINFO "\n"                                              \
                    "\n"                                              \
                    "Usage: %s [OPTIONS] [TARGETS]\n"                 \
                    "\n"                                              \
                    "If the TARGETS are omitted, \"all\" is run.\n"   \
                    "\n"                                              \
                    "Options:\n"                                      \
                    "\n"                                              \
                    "  --\n"                                          \
                    "    Treat all remaining arguments as targets.\n" \
                    "  -v, --version\n"                               \
                    "    Print version information.\n"                \
                    "  -h, --help\n"                                  \
                    "    Print this screen.\n"                        \
                    "\n"

int main(int argc, char **argv) {
        char *smfile = find_smfile();
        if(!smfile) {
                puts("*** No Smfile found. ***");
                return 1;
        }
        int res = 0;
        ARGSTART;
        ARG("v", "version") return puts(VERSIONINFO) < 0;
        ARG("h", "help") return !printf(HELP, *argv);
        else {
                res = run_target(smfile, argv[_args_argc_i], *argv);
                if(res) goto ret;
        }
        ARGEND;
        if(argc < 2) res = run_target(smfile, (char *)"all", *argv);
ret:
        free(smfile);
        return res;
}
