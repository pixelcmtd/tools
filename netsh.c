#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NAME "netsh"
#define VERSION "1.6.8"
#define COPYRIGHT "(c) 2019-2025 Pixel Häußler, chrissx Media"

#define HELP NAME" "VERSION"\n"\
             COPYRIGHT"\n"\
             "\n"\
             "Usage: netsh [OPTIONS] [URLs/REPOs]  ... \n"\
             "\n"\
             "  --\n"\
             "    Treat all remaining arguments as repos/URLs.\n"\
             "  -c, --clone\n"\
             "    Toggle the clone flag. Clones a repo instead of downloading and running a single file.\n"\
             "  -f [FILE], --file [FILE]\n"\
             "    Set the file used when clone is disabled. (defaults to netsh)\n"\
             "  -C [CMD], --command [CMD]\n"\
             "    Set the command executed when clone is enabled. (defaults to ./netsh)\n"\
             "  -s [SCM], --scm [SCM]\n"\
             "    Set the SCM used for cloning. (defaults to git; hg is supported)\n"\
             "  -v, --version\n"\
             "    Print the version number.\n"\
             "  -h, --help\n"\
             "    Print this screen.\n"\

#define fail(...) { fprintf(stderr, __VA_ARGS__); exit(1); }

#define url(s) (!strncmp((s), "http://", 7) || !strncmp((s), "https://", 8) ||\
                !strncmp((s), "git://", 6)  || !strncmp((s), "hg://", 5))

static inline char *smprintf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int len = vsnprintf(NULL, 0, fmt, args);
    va_end(args);

    if(len < 0) return NULL;
    char *buf = malloc(len + 1);
    if(!buf) return NULL;

    va_start(args, fmt);
    vsnprintf(buf, len + 1, fmt, args);
    va_end(args);

    return buf;
}

char *rurl(char *u, int clone, char *f) {
        if(url(u)) return strdup(u);
        if(clone && !strncmp(u, "bb://", 5))
                return smprintf("https://bitbucket.org/%s", u + 5);
        if(clone) return smprintf("https://github.com/%s", strncmp(u, "gh://", 5) ? u : u + 5);
        return smprintf("https://github.com/%s/raw/master/%s", strncmp(u, "gh://", 5) ? u : u + 5, f);
}

#define ARG(s, unix, gnu) else if(!strcmp(s, "-"unix) || !strcmp(s, "--"gnu))
// horrible gcc extension
#define poparg() ({ if(argc <= 0) fail("Missing argument."); argc--; *argv++; })

int main(int argc, char **argv) {
        int clone = 0, argend = 0;
        char *cmd = "./netsh", *f = "netsh", *scm = "git";
        poparg();
        while(argc > 0) {
                char *s = poparg();
                if(!strcmp(s, "--") || argend) argend = 1;
                ARG(s, "c", "clone") clone = !clone;
                ARG(s, "f", "file") f = poparg();
                ARG(s, "C", "command") cmd = poparg();
                ARG(s, "s", "scm") scm = poparg();
                ARG(s, "v", "version") puts(VERSION);
                ARG(s, "h", "help") puts(HELP);
                else {
                        char *fd = tmpnam(NULL);
                        if(!fd) fail("tmpnam: %s\n", strerror(errno));
                        char *url = rurl(s, clone, f);
                        if(!url) fail("rurl (strdup/malloc): %s\n", strerror(errno));
                        char *cmdbuf = clone ?
                                smprintf("%s clone '%s' '%s' && cd '%s' && { %s; }",
                                         scm, url, fd, fd, cmd) :
                                smprintf("curl -Lfo '%s' '%s' && chmod +x '%s' && '%s'",
                                         fd, url, fd, fd);
                        if(!cmdbuf) fail("smprintf (malloc): %s\n", strerror(errno));
                        if(system(cmdbuf)) fail("system(\"%s\") failed.\n", cmdbuf);
                        free(fd);
                        free(url);
                        free(cmdbuf);
                }
        }
        return 0;
}
