#ifdef __aarch64__
#include "lws_config-arm64.h"
#elif __arm__
#include "lws_config-arm32.h"
#elif __i386__
#include "lws_config-x86.h"
#else
#error "Unsupported architecture!"
#endif