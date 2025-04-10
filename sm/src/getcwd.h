/*
 * this header - despite the name - doesn't only define getcwd, but
 * also R_OK and PATH_MAX
 */

#include <unistd.h>

#ifdef __APPLE__
#include <sys/param.h>
#elif defined(__linux__)
#include <linux/limits.h>
#else
#error "Your OS is unsupported, please add "\
       "a header for getcwd, R_OK and PATH_MAX yourself."
#endif
