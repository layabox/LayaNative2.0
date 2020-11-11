#include "Log.h"
int g_nDebugLevel = 3;
void(*gLayaLog)(int level, const char* file, int line, const char* fmt, ...) = 0;
void(*gLayaLogNoParam)(int level, const char* file, int line, const char* msg) = 0;
void(*gLayaLogBin)(int level, const char* file, int line, void* pData, int len) = 0;
