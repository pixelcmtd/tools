#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NAME "netsh"
#define YEARS "2019-2020"
#define AUTHORS "Chris E. Häußler, chrissx Media"
#define VERSION "1.6.5"

#define HELP NAME" "VERSION"\n"\
             "(c) "YEARS" "AUTHORS"\n"\
             "\n"\
             "Usage: netsh [OPTIONS] [URL/repo] [OPTIONS] [URL/repo] ... \n"\
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

#define ARGSTART if(!strcmp(s, "--")) argsended = 1
#define ARG(s, unix, gnu) else if(!argsended && (!strcmp(s, "-"unix) || !strcmp(s, "--"gnu)))

#define url(s) (!strncmp((s), "http://", 7) || \
        !strncmp((s), "https://", 8) || !strncmp((s), "git://", 6) ||\
        !strncmp((s), "svn://", 6) || !strncmp((s), "cvs://", 6) || \
        !strncmp((s), "hg://", 5))

#define clonecommand() sprintf(cmdbuf, \
        "%s clone '%s' %s && cd %s && { %s; }; cd ..; rm -rf %s", \
        scm, urlbuf, fd, fd, cmd, fd)
#define elsecommand()  sprintf(cmdbuf, \
        "curl -Lo '%s' '%s'; chmod +x '%s'&& ./%s; rm -f %s", \
        fd, urlbuf, fd, fd, fd)

void rurl(char *u, char *bfr, int c, char *f) {
        if(url(u)) strcpy(bfr, u);
        else if(!strncmp(u, "bb://", 5) && c)
                sprintf(bfr, "https://bitbucket.org/%s", u + 5);
        else sprintf(bfr, c ? "https://github.com/%s" :
                "https://github.com/%s/raw/master/%s",
                strncmp(u, "gh://", 5) ? u : u + 5, f);
}

int main(int argc, char **argv) {
        int c = 0, argsended = 0;
        char *cmd = "./netsh", *f = "netsh", *scm = "git";
        argv++;
        while(argc-- > 1) {
                char *s = *argv++;
                ARGSTART;
                ARG(s, "c", "clone") c = !c;
                ARG(s, "f", "file") f = *argv++;
                ARG(s, "C", "command") cmd = *argv++;
                ARG(s, "s", "scm") scm = *argv++;
                ARG(s, "v", "version") puts(VERSION);
                ARG(s, "h", "help") puts(HELP);
                else {
                        char fd[16];
                        char cmdbuf[strlen(s) + 512];
                        char urlbuf[strlen(s) + 512];
                        sprintf(fd, ".netsh%4lx", time(0));
                        rurl(s, urlbuf, c, f);
                        if(c) clonecommand();
                        else   elsecommand();
                        if(system(cmdbuf))
                                printf("system(\"%s\") failed.\n", cmdbuf);
                }
        }
        return 0;
}
