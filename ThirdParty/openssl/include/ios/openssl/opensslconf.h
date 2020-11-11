#ifdef __arm64__
#include "opensslconf_ios_arm64.h"
#elif __arm__
#include  "opensslconf_ios_armv7.h"
#elif __i386__
#include  "opensslconf_ios_i386.h"
#elif __x86_64__
#include  "opensslconf_ios_x86_64.h"
#else
#error "Unsupported architecture!"
#endif
