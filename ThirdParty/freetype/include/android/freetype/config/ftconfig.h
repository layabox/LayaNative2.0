#ifdef __aarch64__
#include "ftconfig-arm64.h"
#elif __arm__
#include "ftconfig-arm32.h"
#elif __i386__
#include "ftconfig-x86.h"
#else
#error "Unsupported architecture!"
#endif