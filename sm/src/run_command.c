#include <stdio.h>
#include <stdlib.h>
#include "sm.h"

void run_command(int modifier, char *cmd)
{
        switch(modifier)
        {
                /*
                 * TODO: implement a modifier that fails the build if
                 *       the return code is non-zero
                 */
                case '%': run_builtin(cmd); break;
                case '!': system(cmd); break;
                case '#': break;
                default : printf("Omitting command \"%s\" with unknown modifier %c.\n", cmd, modifier); break;
        }
}
