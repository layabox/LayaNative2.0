#include "KeyNapi.h"
#include <ace/xcomponent/native_xcomponent_key_event.h>
using namespace laya;
extern "C"
{
    int ohKeyCodeToLayaKeyCode(OH_NativeXComponent_KeyCode ohKeyCode) {
        static const int keyZeroInLaya = 48;
        static const int keyF1InLaya = 112;
        static const int keyAInLaya = 65;
        static std::unordered_map<OH_NativeXComponent_KeyCode, KeyCode> keyCodeMap = {
            {KEY_ESCAPE, KeyCode::ESCAPE},
            {KEY_GRAVE, KeyCode::GRAVE},
            {KEY_MINUS, KeyCode::MINUS},
            {KEY_EQUALS, KeyCode::EQUALS},
            {KEY_DEL, KeyCode::DEL},
            {KEY_TAB, KeyCode::TAB},
            {KEY_LEFT_BRACKET, KeyCode::LEFT_BRACKET},
            {KEY_RIGHT_BRACKET, KeyCode::RIGHT_BRACKET},
            {KEY_BACKSLASH, KeyCode::BACKSLASH},
            {KEY_CAPS_LOCK, KeyCode::CAPS_LOCK},
            {KEY_SEMICOLON, KeyCode::SEMICOLON},
            {KEY_APOSTROPHE, KeyCode::APOSTROPHE},
            {KEY_ENTER, KeyCode::ENTER},
            {KEY_SHIFT_LEFT, KeyCode::SHIFT_LEFT},
            {KEY_COMMA, KeyCode::COMMA},
            {KEY_PERIOD, KeyCode::PERIOD},
            {KEY_SLASH, KeyCode::SLASH},
            {KEY_SHIFT_RIGHT, KeyCode::SHIFT_RIGHT},
            {KEY_CTRL_LEFT, KeyCode::CTRL_LEFT},
            {KEY_ALT_LEFT, KeyCode::ALT_LEFT},
            {KEY_SPACE, KeyCode::SPACE},
            {KEY_ALT_RIGHT, KeyCode::ALT_RIGHT},
            {KEY_CTRL_RIGHT, KeyCode::CTRL_RIGHT},
            {KEY_DPAD_LEFT, KeyCode::DPAD_LEFT},
            {KEY_DPAD_RIGHT, KeyCode::DPAD_RIGHT},
            {KEY_DPAD_DOWN, KeyCode::DPAD_DOWN},
            {KEY_DPAD_UP, KeyCode::DPAD_UP},
            {KEY_INSERT, KeyCode::INSERT},
        };
        if (keyCodeMap.find(ohKeyCode) != keyCodeMap.end()) {
            return int(keyCodeMap[ohKeyCode]);
        }
        if (ohKeyCode >= KEY_0 && ohKeyCode <= KEY_9) {
            return keyZeroInLaya + ohKeyCode - KEY_0;
        }
        if (ohKeyCode >= KEY_A && ohKeyCode <= KEY_Z) {
            return keyAInLaya + ohKeyCode - KEY_A;
        }
        if (ohKeyCode >= KEY_F1 && ohKeyCode <= KEY_F12) {
            return keyF1InLaya + ohKeyCode - KEY_F1;
        }
        return ohKeyCode;
    }

    void nativeHandleKey(OH_NativeXComponent_KeyAction action, OH_NativeXComponent_KeyCode code)
    {
        inputEvent e;
        if (action == OH_NATIVEXCOMPONENT_KEY_ACTION_DOWN) {
            e.nType = E_ONKEYDOWN;
            e.keyCode = ohKeyCodeToLayaKeyCode(code);
            if (code == KEY_ALT_LEFT || code == KEY_ALT_RIGHT) {
                e.bAlt = true;
            }
            else if (code == KEY_SHIFT_LEFT || code == KEY_SHIFT_RIGHT) {
                e.bShift = true;
            }
            else if (code == KEY_CTRL_LEFT || code == KEY_CTRL_RIGHT) {
                e.bCtrl = true;        
            }
            strncpy(e.type, "keydown", 256);
            JCScriptRuntime::s_JSRT->dispatchInputEvent(e);
        }
        else if (action == OH_NATIVEXCOMPONENT_KEY_ACTION_UP) {
            e.nType = E_ONKEYUP;
            e.keyCode = ohKeyCodeToLayaKeyCode(code);
            if (code == KEY_ALT_LEFT || code == KEY_ALT_RIGHT) {
                e.bAlt = true;
            }
            else if (code == KEY_SHIFT_LEFT || code == KEY_SHIFT_RIGHT) {
                e.bShift = true;
            }
            else if (code == KEY_CTRL_LEFT || code == KEY_CTRL_RIGHT) {
                e.bCtrl = true;        
            }
            strncpy(e.type, "keyup", 256);
            JCScriptRuntime::s_JSRT->dispatchInputEvent(e);
        }
        else {
            // KEY_ACTION_UNKNOWN
        }
    }
}