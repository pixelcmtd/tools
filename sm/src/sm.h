#pragma once
#include <stdbool.h>

/**
 * Searches for the file [Ss]mfile in the cwd and its parents
 *
 * Returns a malloced string if it found one, NULL otherwise.
 */
char *find_smfile();

/**
 * runs specified target
 * from smfile
 * using argv0 to call sudo when necessary
 *
 * when target is marked as root in smfile,
 * the return value of system is returned.
 * else 0 is returned after successful execution,
 * non-zero return values indicate errors like:
 *   target wasn't found in smfile
 */
int run_target(char *smfile, char *target, char *argv0);

/**
 * runs the command cmd with the modifier mod
 */
int run_command(int mod, char *cmd);

/**
 * runs the specified builtin command
 */
int run_builtin(char *);
