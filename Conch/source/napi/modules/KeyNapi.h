#ifndef _KEYNAPI_H_
#define _KEYNAPI_H_

#include "conch/JCScriptRuntime.h"
#include <ace/xcomponent/native_xcomponent_key_event.h>

enum class KeyCode {
    ESCAPE = 27,
    GRAVE = 192,
    MINUS = 189,
    EQUALS = 187,
    DEL = 8,
    TAB = 9,
    LEFT_BRACKET = 219,
    RIGHT_BRACKET = 221,
    BACKSLASH = 220,
    CAPS_LOCK = 20,
    SEMICOLON = 186,
    APOSTROPHE = 222,
    ENTER = 13,
    SHIFT_LEFT = 16,
    COMMA = 188,
    PERIOD = 190,
    SLASH = 191,
    SHIFT_RIGHT = 20016,
    CTRL_LEFT = 17,
    ALT_LEFT = 18,
    SPACE = 32,
    ALT_RIGHT = 20018,
    CTRL_RIGHT = 20017,
    DPAD_LEFT = 37,
    DPAD_RIGHT = 39,
    DPAD_DOWN = 40,
    DPAD_UP = 38,
    INSERT = 45,
};

using namespace laya;

extern "C"
{
    void nativeHandleKey(OH_NativeXComponent_KeyAction action, OH_NativeXComponent_KeyCode code);
}

#endif
