#define DEFAULT_CC       "cc"
#define DEFAULT_CPPC     "c++"
#ifdef __APPLE__
#define DEFAULT_CFLAGS   "-Wall -Wextra -pedantic -O3"
#define DEFAULT_CPPFLAGS "-Wall -Wextra -pedantic -std=c++2a -O3"
#else
#define DEFAULT_CFLAGS   "-Wall -Wextra -pedantic -O3 -s"
#define DEFAULT_CPPFLAGS "-Wall -Wextra -pedantic -std=c++2a -O3 -s"
#endif
