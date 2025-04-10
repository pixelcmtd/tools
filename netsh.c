#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NAME "netsh"
#define VERSION "1.6.7"
#define COPYRIGHT "(c) 2019-2025 Pixel Häußler, chrissx Media"

#define HELP NAME" "VERSION"\n"\
             COPYRIGHT"\n"\
             "\n"\
             "Usage: netsh [OPTIONS] [URL]  ... \n"\
             "Usage: netsh [OPTIONS] [REPO] ... \n"\
             "\n"\
             "  --\n"\
             "    Treat all remaining arguments as repos/URLs.\n"\
             "  -c, --clone\n"\
             "    Set or unset the clone flag.\n"\
             "  -f, --file\n"\
             "    Set the file used when clone is disabled.\n"\
             "  -C, --command\n"\
             "    Set the command executed when clone is enabled.\n"\
             "  -s, --scm\n"\
             "    Set the SCM used for cloning. (git and hg are tested)\n"\
             "  -v, --version\n"\
             "    Print the version number.\n"\
             "  -h, --help\n"\
             "    Print this screen.\n"\

#define fail(...) { fprintf(stderr, __VA_ARGS__); return 1; }

#define url(s) (!strncmp((s), "http://", 7) || \
        !strncmp((s), "https://", 8) || !strncmp((s), "git://", 6) ||\
        !strncmp((s), "svn://", 6) || !strncmp((s), "cvs://", 6) || \
        !strncmp((s), "hg://", 5))

void rurl(char *u, char *bfr, int c, char *f) {
        if(url(u)) strcpy(bfr, u);
        else if(!strncmp(u, "bb://", 5) && c)
                sprintf(bfr, "https://bitbucket.org/%s", u + 5);
        else sprintf(bfr, c ? "https://github.com/%s" :
                        // TODO: am i dumb
                "https://github.com/%s/raw/master/%s",
                strncmp(u, "gh://", 5) ? u : u + 5, f);
}

#define ARG(s, unix, gnu) else if(!strcmp(s, "-"unix) || !strcmp(s, "--"gnu))

int main(int argc, char **argv) {
        int c = 0, argsended = 0;
        char *cmd = "./netsh", *f = "netsh", *scm = "git";
        argv++;
        while(argc-- > 1) {
                char *s = *argv++;
                if(!strcmp(s, "--") || argsended) argsended = 1;
                ARG(s, "c", "clone") c = !c;
                ARG(s, "f", "file") f = *argv++;
                ARG(s, "C", "command") cmd = *argv++;
                ARG(s, "s", "scm") scm = *argv++;
                ARG(s, "v", "version") puts(VERSION);
                ARG(s, "h", "help") puts(HELP);
                else {
                        char *fd = strdup("/tmp/netsh.XXXXXXXX");
                        if(!fd) fail("strdup: %s\n", strerror(errno));
                        char cmdbuf[strlen(s) + 512];
                        char urlbuf[strlen(s) + 512];
                        // ik mktemp's deprecated and stuff, but it makes sense here
                        mktemp(fd);
                        rurl(s, urlbuf, c, f);
                        if(c) sprintf(cmdbuf,
                                      "%s clone '%s' '%s' && cd '%s' && { %s; }",
                                      scm, urlbuf, fd, fd, cmd);
                        else  sprintf(cmdbuf,
                                      "curl -Lo '%s' '%s'; chmod +x '%s' && %s",
                                      fd, urlbuf, fd, fd);
                        if(system(cmdbuf)) fail("system(\"%s\"): %s\n", cmdbuf, strerror(errno));
                }
        }
        return 0;
}
