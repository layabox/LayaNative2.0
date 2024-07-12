#ifndef __JSOHOSEditBox_H__
#define __JSOHOSEditBox_H__

#include <stdio.h>
#include <string>
#include "../JSInterface/JSInterface.h"

namespace laya{

//the same value with the openharmony TextInputType
enum EditBoxType{
    NORMAL=0,
    PASSWORD,
    NUMBER,
    NUMBER_PASSWORD,
};
class JSOHOSEditBox:public JsObjBase, public JSObjNode{
    public:
        static JsObjClassInfo JSCLSINFO;
        void JSConstructor(JsFuncArgs& args){};

        static void exportJS();
        JSOHOSEditBox();
        ~JSOHOSEditBox();
    public:
        int set_Left(int p_nLeft);
        int get_Left();
        int set_Top(int p_nTop);
        int get_Top();
        int set_Width(int p_nWidth);
        int get_Width();
        int set_Height(int p_nHeight);
        int get_Height();
        float set_Opacity(float p_Opacity);
        float get_Opacity();
        const char* set_Value(const char* p_sValue);
        const char* get_Value();
        void set_Style(const char* p_sStyle);
        const char* get_Style();
        bool set_Visible(bool p_bVisible);
        bool get_Visible();
    public:
        void setColor(const char* p_sColor);
        void setFontSize(int p_nFontSize);
        void setPos(int x,int y);
        void setSize(int w,int h);
        void setCursorPosition(int pos);
        void setLeft(int p_nLeft);
        void setTop(int p_nTop);
        void setWidth(int p_nWidth);
        void setHeight(int p_nHeight);
        void setOpacity(float p_Opacity);
        void setValue(const char* p_sValue);
        const char* getValue();
        void setStyle(const char* p_sStyle);
        void setVisible(bool p_bVisible);
        void setFont(const char* p_sFont);
        void focus();
        void blur();
        void setForbidEdit(bool bForbidEdit);
        bool getForbidEdit();
    public:
        void setScale(float p_nSx, float p_nSy);
        void setMaxLength(int p_nMaxLength);
        void setType(const char* p_sType);
        void setRegular(const char* p_sRegular);
        void setNumberOnly(bool p_bNumberOnly);
        void addEventListener(const char* p_sName, JSValueAsParam p_pFunction);
        void setMultiAble(bool p_bMultiAble);
        void onInputCallJSFunction(std::weak_ptr<int> callbackref);
        void onInput();
    public:
        int m_nLeft;
        int m_nTop;
        int m_nWidth;
        int m_nHeight;
        bool m_bVisible;
        float m_fOpacity;
        int m_nMaxLength;
        int m_nFontSize;
        float m_nScaleX;
        float m_nScaleY;
        std::string m_sType;
        std::string m_sStyle;
        std::string m_sValue;
        std::string m_sRegular;
        bool m_bForbidEdit;
    private:
        int m_tag;
        std::shared_ptr<int> m_CallbackRef;
        JsObjHandle m_pJSFunctionOnInput;//JS的回调
};
}
#endif //__JSOHOSEditBox_H__