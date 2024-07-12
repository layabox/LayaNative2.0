#include "ColorParser.h"
#include <regex>
#include <unordered_map>
#include "Log.h"
#include <algorithm>

namespace laya
{
static const std::unordered_map<std::string, uint32_t> namedColors = {
    {"transparent", 0xFFFFFF00},
    {"aliceblue", 0xF0F8FFFF},
    {"antiquewhite", 0xFAEBD7FF},
    {"aqua", 0x00FFFFFF},
    {"aquamarine", 0x7FFFD4FF},
    {"azure", 0xF0FFFFFF},
    {"beige", 0xF5F5DCFF},
    {"bisque", 0xFFE4C4FF},
    {"black", 0x000000FF},
    {"blanchedalmond", 0xFFEBCDFF},
    {"blue", 0x0000FFFF},
    {"blueviolet", 0x8A2BE2FF},
    {"brown", 0xA52A2AFF},
    {"burlywood", 0xDEB887FF},
    {"cadetblue", 0x5F9EA0FF},
    {"chartreuse", 0x7FFF00FF},
    {"chocolate", 0xD2691EFF},
    {"coral", 0xFF7F50FF},
    {"cornflowerblue", 0x6495EDFF},
    {"cornsilk", 0xFFF8DCFF},
    {"crimson", 0xDC143CFF},
    {"cyan", 0x00FFFFFF},
    {"darkblue", 0x00008BFF},
    {"darkcyan", 0x008B8BFF},
    {"darkgoldenrod", 0xB8860BFF},
    {"darkgray", 0xA9A9A9FF},
    {"darkgreen", 0x006400FF},
    {"darkgrey", 0xA9A9A9FF},
    {"darkkhaki", 0xBDB76BFF},
    {"darkmagenta", 0x8B008BFF},
    {"darkolivegreen", 0x556B2FFF},
    {"darkorange", 0xFF8C00FF},
    {"darkorchid", 0x9932CCFF},
    {"darkred", 0x8B0000FF},
    {"darksalmon", 0xE9967AFF},
    {"darkseagreen", 0x8FBC8FFF},
    {"darkslateblue", 0x483D8BFF},
    {"darkslategray", 0x2F4F4FFF},
    {"darkslategrey", 0x2F4F4FFF},
    {"darkturquoise", 0x00CED1FF},
    {"darkviolet", 0x9400D3FF},
    {"deeppink", 0xFF1493FF},
    {"deepskyblue", 0x00BFFFFF},
    {"dimgray", 0x696969FF},
    {"dimgrey", 0x696969FF},
    {"dodgerblue", 0x1E90FFFF},
    {"firebrick", 0xB22222FF},
    {"floralwhite", 0xFFFAF0FF},
    {"forestgreen", 0x228B22FF},
    {"fuchsia", 0xFF00FFFF},
    {"gainsboro", 0xDCDCDCFF},
    {"ghostwhite", 0xF8F8FFFF},
    {"gold", 0xFFD700FF},
    {"goldenrod", 0xDAA520FF},
    {"gray", 0x808080FF},
    {"green", 0x008000FF},
    {"greenyellow", 0xADFF2FFF},
    {"grey", 0x808080FF},
    {"honeydew", 0xF0FFF0FF},
    {"hotpink", 0xFF69B4FF},
    {"indianred", 0xCD5C5CFF},
    {"indigo", 0x4B0082FF},
    {"ivory", 0xFFFFF0FF},
    {"khaki", 0xF0E68CFF},
    {"lavender", 0xE6E6FAFF},
    {"lavenderblush", 0xFFF0F5FF},
    {"lawngreen", 0x7CFC00FF},
    {"lemonchiffon", 0xFFFACDFF},
    {"lightblue", 0xADD8E6FF},
    {"lightcoral", 0xF08080FF},
    {"lightcyan", 0xE0FFFFFF},
    {"lightgoldenrodyellow", 0xFAFAD2FF},
    {"lightgray", 0xD3D3D3FF},
    {"lightgreen", 0x90EE90FF},
    {"lightgrey", 0xD3D3D3FF},
    {"lightpink", 0xFFB6C1FF},
    {"lightsalmon", 0xFFA07AFF},
    {"lightseagreen", 0x20B2AAFF},
    {"lightskyblue", 0x87CEFAFF},
    {"lightslategray", 0x778899FF},
    {"lightslategrey", 0x778899FF},
    {"lightsteelblue", 0xB0C4DEFF},
    {"lightyellow", 0xFFFFE0FF},
    {"lime", 0x00FF00FF},
    {"limegreen", 0x32CD32FF},
    {"linen", 0xFAF0E6FF},
    {"magenta", 0xFF00FFFF},
    {"maroon", 0x800000FF},
    {"mediumaquamarine", 0x66CDAAFF},
    {"mediumblue", 0x0000CDFF},
    {"mediumorchid", 0xBA55D3FF},
    {"mediumpurple", 0x9370DBFF},
    {"mediumseagreen", 0x3CB371FF},
    {"mediumslateblue", 0x7B68EEFF},
    {"mediumspringgreen", 0x00FA9AFF},
    {"mediumturquoise", 0x48D1CCFF},
    {"mediumvioletred", 0xC71585FF},
    {"midnightblue", 0x191970FF},
    {"mintcream", 0xF5FFFAFF},
    {"mistyrose", 0xFFE4E1FF},
    {"moccasin", 0xFFE4B5FF},
    {"navajowhite", 0xFFDEADFF},
    {"navy", 0x000080FF},
    {"oldlace", 0xFDF5E6FF},
    {"olive", 0x808000FF},
    {"olivedrab", 0x6B8E23FF},
    {"orange", 0xFFA500FF},
    {"orangered", 0xFF4500FF},
    {"orchid", 0xDA70D6FF},
    {"palegoldenrod", 0xEEE8AAFF},
    {"palegreen", 0x98FB98FF},
    {"paleturquoise", 0xAFEEEEFF},
    {"palevioletred", 0xDB7093FF},
    {"papayawhip", 0xFFEFD5FF},
    {"peachpuff", 0xFFDAB9FF},
    {"peru", 0xCD853FFF},
    {"pink", 0xFFC0CBFF},
    {"plum", 0xDDA0DDFF},
    {"powderblue", 0xB0E0E6FF},
    {"purple", 0x800080FF},
    {"rebeccapurple", 0x663399FF} // Source: CSS Color Level 4 draft
    ,
    {"red", 0xFF0000FF},
    {"rosybrown", 0xBC8F8FFF},
    {"royalblue", 0x4169E1FF},
    {"saddlebrown", 0x8B4513FF},
    {"salmon", 0xFA8072FF},
    {"sandybrown", 0xF4A460FF},
    {"seagreen", 0x2E8B57FF},
    {"seashell", 0xFFF5EEFF},
    {"sienna", 0xA0522DFF},
    {"silver", 0xC0C0C0FF},
    {"skyblue", 0x87CEEBFF},
    {"slateblue", 0x6A5ACDFF},
    {"slategray", 0x708090FF},
    {"slategrey", 0x708090FF},
    {"snow", 0xFFFAFAFF},
    {"springgreen", 0x00FF7FFF},
    {"steelblue", 0x4682B4FF},
    {"tan", 0xD2B48CFF},
    {"teal", 0x008080FF},
    {"thistle", 0xD8BFD8FF},
    {"tomato", 0xFF6347FF},
    {"turquoise", 0x40E0D0FF},
    {"violet", 0xEE82EEFF},
    {"wheat", 0xF5DEB3FF},
    {"white", 0xFFFFFFFF},
    {"whitesmoke", 0xF5F5F5FF},
    {"yellow", 0xFFFF00FF},
    {"yellowgreen", 0x9ACD32FF}};

static int hex2int(char c)
{
    switch (c)
    {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        return c - '0';
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
        return (c - 'a') + 10;
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
        return (c - 'A') + 10;
    }
    return 0;
}

static bool RGBAFromHex8String(const char *str, uint32_t &r, uint32_t &g, uint32_t &b, uint32_t &a)
{
    r = (hex2int(str[0]) << 4) + hex2int(str[1]);
    g = (hex2int(str[2]) << 4) + hex2int(str[3]);
    b = (hex2int(str[4]) << 4) + hex2int(str[5]);
    a = (hex2int(str[6]) << 4) + hex2int(str[7]);
    return true;
}
static bool RGBAFromHex6String(const char *str, uint32_t &r, uint32_t &g, uint32_t &b, uint32_t &a)
{
    r = (hex2int(str[0]) << 4) + hex2int(str[1]);
    g = (hex2int(str[2]) << 4) + hex2int(str[3]);
    b = (hex2int(str[4]) << 4) + hex2int(str[5]);
    a = 255;
    return true;
}

static bool RGBAFromHex4String(const char *str, uint32_t &r, uint32_t &g, uint32_t &b, uint32_t &a)
{
    r = (hex2int(str[0]) << 4) + hex2int(str[0]);
    g = (hex2int(str[1]) << 4) + hex2int(str[1]);
    b = (hex2int(str[2]) << 4) + hex2int(str[2]);
    a = (hex2int(str[3]) << 4) + hex2int(str[3]);
    return true;
}

static bool RGBAFromHex3String(const char *str, uint32_t &r, uint32_t &g, uint32_t &b, uint32_t &a)
{
    r = (hex2int(str[0]) << 4) + hex2int(str[0]);
    g = (hex2int(str[1]) << 4) + hex2int(str[1]);
    b = (hex2int(str[2]) << 4) + hex2int(str[2]);
    a = 255;
    return true;
}
static bool RGBAFromHexString(const char *str, uint32_t &r, uint32_t &g, uint32_t &b, uint32_t &a)
{
    size_t len = strlen(str);
    switch (len)
    {
    case 8:
        return RGBAFromHex8String(str, r, g, b, a);
    case 6:
        return RGBAFromHex6String(str, r, g, b, a);
    case 4:
        return RGBAFromHex4String(str, r, g, b, a);
    case 3:
        return RGBAFromHex3String(str, r, g, b, a);
    }
    return false;
}
template <typename parsed_t> static bool parseInteger(const char **pStr, parsed_t *pParsed)
{
    parsed_t &c = *pParsed;
    const char *&str = *pStr;
    int8_t sign = 1;

    c = 0;
    if (*str == '-')
    {
        sign = -1;
        ++str;
    }
    else if (*str == '+')
        ++str;

    if (*str >= '0' && *str <= '9')
    {
        do
        {
            c *= 10;
            c += *str++ - '0';
        } while (*str >= '0' && *str <= '9');
    }
    else
    {
        return false;
    }
    if (sign < 0)
        c = -c;
    return true;
}
static bool parseRGBChannel(const char **pStr, uint8_t *pChannel)
{
    int channel;
    if (parseInteger(pStr, &channel))
    {
        *pChannel = (uint8_t)(std::clamp(channel, 0, 255));
        return true;
    }
    return false;
}

#define CHANNEL(NAME)                                                                                                  \
    if (!parseRGBChannel(&str, &NAME))                                                                                 \
        return false;

#define SKIP_WHITESPACE                                                                                                \
    while (' ' == *str)                                                                                                \
        ++str;

#define SKIP_WHITESPACE_OR_COMMA                                                                                       \
    while (' ' == *str || ',' == *str)                                                                                 \
        ++str;

#define ALPHA(NAME)                                                                                                    \
    if (*str >= '1' && *str <= '9')                                                                                    \
    {                                                                                                                  \
        NAME = 1;                                                                                                      \
    }                                                                                                                  \
    else                                                                                                               \
    {                                                                                                                  \
        if ('0' == *str)                                                                                               \
        {                                                                                                              \
            NAME = 0;                                                                                                  \
            ++str;                                                                                                     \
        }                                                                                                              \
        if ('.' == *str)                                                                                               \
        {                                                                                                              \
            ++str;                                                                                                     \
            NAME = 0;                                                                                                  \
            float n = .1f;                                                                                             \
            while (*str >= '0' && *str <= '9')                                                                         \
            {                                                                                                          \
                NAME += (*str++ - '0') * n;                                                                            \
                n *= .1f;                                                                                              \
            }                                                                                                          \
        }                                                                                                              \
    }                                                                                                                  \
    do                                                                                                                 \
    {                                                                                                                  \
    } while (0)

static bool RGBAFromRGBString(const char *str, uint32_t &r, uint32_t &g, uint32_t &b, uint32_t &a)
{
    if (str == strstr(str, "rgb("))
    {
        str += 4;
        SKIP_WHITESPACE;
        uint8_t r1 = 0, g1 = 0, b1 = 0;
        CHANNEL(r1);
        SKIP_WHITESPACE_OR_COMMA;
        CHANNEL(g1);
        SKIP_WHITESPACE_OR_COMMA;
        CHANNEL(b1);
        SKIP_WHITESPACE;
        r = r1;
        g = g1;
        b = b1;
        a = 255u;
        return true;
    }
    return false;
}

static bool RGBAFromRGBAString(const char *str, uint32_t &r, uint32_t &g, uint32_t &b, uint32_t &a)
{
    if (str == strstr(str, "rgba("))
    {
        str += 5;
        SKIP_WHITESPACE;
        uint8_t r1 = 0, g1 = 0, b1 = 0;
        float a1 = 1.f;
        CHANNEL(r1);
        SKIP_WHITESPACE_OR_COMMA;
        CHANNEL(g1);
        SKIP_WHITESPACE_OR_COMMA;
        CHANNEL(b1);
        SKIP_WHITESPACE_OR_COMMA;
        ALPHA(a1);
        SKIP_WHITESPACE;

        r = r1;
        g = g1;
        b = b1;
        a = (uint32_t)(a1 * 255);
        return true;
    }
    return false;
}

static bool RGBAFromNameString(const char *str, uint32_t &r, uint32_t &g, uint32_t &b, uint32_t &a)
{
    std::string lowered(str);
    std::transform(lowered.begin(), lowered.end(), lowered.begin(), tolower);
    auto color = namedColors.find(lowered);
    if (color != namedColors.end())
    {
        uint32_t rgba = color->second;

        r = rgba >> 24 & 0xff;
        g = rgba >> 16 & 0xff;
        b = rgba >> 8 & 0xff;
        a = rgba & 0xff;
        return true;
    }
    return false;
}

static bool RGBAFromHSLAString(const char *str, uint32_t &r, uint32_t &g, uint32_t &b, uint32_t &a)
{
    LOGE("color HSLA not implemented %s", str);
    return false;
}

static bool RGBAFromHSLString(const char *str, uint32_t &r, uint32_t &g, uint32_t &b, uint32_t &a)
{
    LOGE("color HSL not implemented %s", str);
    return false;
}

bool parseRGBAFromString(const char *str, uint32_t &r, uint32_t &g, uint32_t &b, uint32_t &a)
{
    if ('#' == str[0])
        return RGBAFromHexString(++str, r, g, b, a);
    if (str == strstr(str, "rgba"))
        return RGBAFromRGBAString(str, r, g, b, a);
    if (str == strstr(str, "rgb"))
        return RGBAFromRGBString(str, r, g, b, a);
    if (str == strstr(str, "hsla"))
        return RGBAFromHSLAString(str, r, g, b, a);
    if (str == strstr(str, "hsl"))
        return RGBAFromHSLString(str, r, g, b, a);
    return RGBAFromNameString(str, r, g, b, a);
}
} // namespace laya
