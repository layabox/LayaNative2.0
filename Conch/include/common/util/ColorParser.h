#ifndef __ColorParser_H__
#define __ColorParser_H__
#include <string>
namespace laya
{
bool parseRGBAFromString(const char *str, uint32_t &r, uint32_t &g, uint32_t &b, uint32_t &a);
} // namespace laya
#endif
