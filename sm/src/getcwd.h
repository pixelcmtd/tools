/*
 * this header - despite the name - doesn't only define getcwd, but
 * also R_OK and PATH_MAX
 */

#ifdef __APPLE__
#include <sys/param.h>
#include <unistd.h>
#elif defined(__linux__)
#include <linux/limits.h>
#include <unistd.h>
#else
#error "Your OS is currently unsupported, you will have to specify "\
       "a header for getcwd and PATH_MAX yourself. If this piece of "\
       "software is still somewhat supported (it's the 2020s and I "\
       "am still alive), please make a PR on GitHub. (chrissxYT/tools)"
#endif
