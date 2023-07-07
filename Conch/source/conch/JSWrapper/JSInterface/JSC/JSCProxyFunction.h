
#ifndef _JSCProxyFunction_h
#define _JSCProxyFunction_h

namespace laya
{
    
template<class T>
JSValueRef ToJSValue(T t) {
    return __TransferToJs<T>::ToJs(t);
}
    
inline bool checkJSToCArgs(size_t given, size_t expect) {
    assert(given >= expect);
    if (given >= expect)
        return true;
    static char buff[512];
    sprintf(buff, "console.log('arguments number err: %d:%d');var e = new Error();console.log(e.stack);", expect, given);
    //__JSRun::Run(buff);
    return false;
}
    
#define JS2CCALL_GET_COBJ \
    Cls* pThis = (Cls*)JSObjectGetPrivate( thisObject );


#define JS2CCALL_GET_PARAMS1 \
    if(!checkJSToCArgs(argumentCount,1))return JSValueMakeUndefined(ctx);\
    P1 p1 = __TransferToCpp<P1>::ToCpp(arguments[0]);

#define JS2CCALL_GET_PARAMS2 \
    if(!checkJSToCArgs(argumentCount,2))return JSValueMakeUndefined(ctx);\
    P1 p1 = __TransferToCpp<P1>::ToCpp(arguments[0]);\
    P2 p2 = __TransferToCpp<P2>::ToCpp(arguments[1]);

#define JS2CCALL_GET_PARAMS3 \
    if(!checkJSToCArgs(argumentCount,3))return JSValueMakeUndefined(ctx);\
    P1 p1 = __TransferToCpp<P1>::ToCpp(arguments[0]);\
    P2 p2 = __TransferToCpp<P2>::ToCpp(arguments[1]);\
    P3 p3 = __TransferToCpp<P3>::ToCpp(arguments[2]);

#define JS2CCALL_GET_PARAMS4 \
    if(!checkJSToCArgs(argumentCount,4))return JSValueMakeUndefined(ctx);\
    P1 p1 = __TransferToCpp<P1>::ToCpp(arguments[0]);\
    P2 p2 = __TransferToCpp<P2>::ToCpp(arguments[1]);\
    P3 p3 = __TransferToCpp<P3>::ToCpp(arguments[2]);\
    P4 p4 = __TransferToCpp<P4>::ToCpp(arguments[3]);

#define JS2CCALL_GET_PARAMS5 \
    if(!checkJSToCArgs(argumentCount,5))return JSValueMakeUndefined(ctx);\
    P1 p1 = __TransferToCpp<P1>::ToCpp(arguments[0]);\
    P2 p2 = __TransferToCpp<P2>::ToCpp(arguments[1]);\
    P3 p3 = __TransferToCpp<P3>::ToCpp(arguments[2]);\
    P4 p4 = __TransferToCpp<P4>::ToCpp(arguments[3]);\
    P5 p5 = __TransferToCpp<P5>::ToCpp(arguments[4]);

#define JS2CCALL_GET_PARAMS6 \
    if(!checkJSToCArgs(argumentCount,6))return JSValueMakeUndefined(ctx);\
    P1 p1 = __TransferToCpp<P1>::ToCpp(arguments[0]);\
    P2 p2 = __TransferToCpp<P2>::ToCpp(arguments[1]);\
    P3 p3 = __TransferToCpp<P3>::ToCpp(arguments[2]);\
    P4 p4 = __TransferToCpp<P4>::ToCpp(arguments[3]);\
    P5 p5 = __TransferToCpp<P5>::ToCpp(arguments[4]);\
    P6 p6 = __TransferToCpp<P6>::ToCpp(arguments[5]);

#define JS2CCALL_GET_PARAMS7 \
    if(!checkJSToCArgs(argumentCount,7))return JSValueMakeUndefined(ctx);\
    P1 p1 = __TransferToCpp<P1>::ToCpp(arguments[0]);\
    P2 p2 = __TransferToCpp<P2>::ToCpp(arguments[1]);\
    P3 p3 = __TransferToCpp<P3>::ToCpp(arguments[2]);\
    P4 p4 = __TransferToCpp<P4>::ToCpp(arguments[3]);\
    P5 p5 = __TransferToCpp<P5>::ToCpp(arguments[4]);\
    P6 p6 = __TransferToCpp<P6>::ToCpp(arguments[5]);\
    P7 p7 = __TransferToCpp<P7>::ToCpp(arguments[6]);

#define JS2CCALL_GET_PARAMS8 \
    if(!checkJSToCArgs(argumentCount,8))return JSValueMakeUndefined(ctx);\
    P1 p1 = __TransferToCpp<P1>::ToCpp(arguments[0]);\
    P2 p2 = __TransferToCpp<P2>::ToCpp(arguments[1]);\
    P3 p3 = __TransferToCpp<P3>::ToCpp(arguments[2]);\
    P4 p4 = __TransferToCpp<P4>::ToCpp(arguments[3]);\
    P5 p5 = __TransferToCpp<P5>::ToCpp(arguments[4]);\
    P6 p6 = __TransferToCpp<P6>::ToCpp(arguments[5]);\
    P7 p7 = __TransferToCpp<P7>::ToCpp(arguments[6]);\
    P8 p8 = __TransferToCpp<P8>::ToCpp(arguments[7]);

#define JS2CCALL_GET_PARAMS9 \
    if(!checkJSToCArgs(argumentCount,9))return JSValueMakeUndefined(ctx);\
    P1 p1 = __TransferToCpp<P1>::ToCpp(arguments[0]);\
    P2 p2 = __TransferToCpp<P2>::ToCpp(arguments[1]);\
    P3 p3 = __TransferToCpp<P3>::ToCpp(arguments[2]);\
    P4 p4 = __TransferToCpp<P4>::ToCpp(arguments[3]);\
    P5 p5 = __TransferToCpp<P5>::ToCpp(arguments[4]);\
    P6 p6 = __TransferToCpp<P6>::ToCpp(arguments[5]);\
    P7 p7 = __TransferToCpp<P7>::ToCpp(arguments[6]);\
    P8 p8 = __TransferToCpp<P8>::ToCpp(arguments[7]);\
    P9 p9 = __TransferToCpp<P9>::ToCpp(arguments[8]);

#define JS2CCALL_GET_PARAMS10 \
    if(!checkJSToCArgs(argumentCount,10))return JSValueMakeUndefined(ctx);\
    P1 p1 = __TransferToCpp<P1>::ToCpp(arguments[0]);\
    P2 p2 = __TransferToCpp<P2>::ToCpp(arguments[1]);\
    P3 p3 = __TransferToCpp<P3>::ToCpp(arguments[2]);\
    P4 p4 = __TransferToCpp<P4>::ToCpp(arguments[3]);\
    P5 p5 = __TransferToCpp<P5>::ToCpp(arguments[4]);\
    P6 p6 = __TransferToCpp<P6>::ToCpp(arguments[5]);\
    P7 p7 = __TransferToCpp<P7>::ToCpp(arguments[6]);\
    P8 p8 = __TransferToCpp<P8>::ToCpp(arguments[7]);\
    P9 p9 = __TransferToCpp<P9>::ToCpp(arguments[8]);\
    P10 p10 = __TransferToCpp<P10>::ToCpp(arguments[9]);\

#define JS2CCALL_GET_PARAMS11 \
    if(!checkJSToCArgs(argumentCount,11))return JSValueMakeUndefined(ctx);\
    P1 p1 = __TransferToCpp<P1>::ToCpp(arguments[0]);\
    P2 p2 = __TransferToCpp<P2>::ToCpp(arguments[1]);\
    P3 p3 = __TransferToCpp<P3>::ToCpp(arguments[2]);\
    P4 p4 = __TransferToCpp<P4>::ToCpp(arguments[3]);\
    P5 p5 = __TransferToCpp<P5>::ToCpp(arguments[4]);\
    P6 p6 = __TransferToCpp<P6>::ToCpp(arguments[5]);\
    P7 p7 = __TransferToCpp<P7>::ToCpp(arguments[6]);\
    P8 p8 = __TransferToCpp<P8>::ToCpp(arguments[7]);\
    P9 p9 = __TransferToCpp<P9>::ToCpp(arguments[8]);\
    P10 p10 = __TransferToCpp<P10>::ToCpp(arguments[9]);\
    P11 p11 = __TransferToCpp<P11>::ToCpp(arguments[10]);
    
#define JS2CCALL_GET_PARAMS12 \
    if(!checkJSToCArgs(argumentCount,12))return JSValueMakeUndefined(ctx);\
    P1 p1 = __TransferToCpp<P1>::ToCpp(arguments[0]);\
    P2 p2 = __TransferToCpp<P2>::ToCpp(arguments[1]);\
    P3 p3 = __TransferToCpp<P3>::ToCpp(arguments[2]);\
    P4 p4 = __TransferToCpp<P4>::ToCpp(arguments[3]);\
    P5 p5 = __TransferToCpp<P5>::ToCpp(arguments[4]);\
    P6 p6 = __TransferToCpp<P6>::ToCpp(arguments[5]);\
    P7 p7 = __TransferToCpp<P7>::ToCpp(arguments[6]);\
    P8 p8 = __TransferToCpp<P8>::ToCpp(arguments[7]);\
    P9 p9 = __TransferToCpp<P9>::ToCpp(arguments[8]);\
    P10 p10 = __TransferToCpp<P10>::ToCpp(arguments[9]);\
    P11 p11 = __TransferToCpp<P11>::ToCpp(arguments[10]);\
	P11 p12 = __TransferToCpp<P11>::ToCpp(arguments[11]);
	
struct IJSCCallback
{
    virtual ~IJSCCallback(){}
    virtual JSValueRef call(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){assert(false);return nullptr;}
    virtual JSObjectRef constructor(JSContextRef ctx, JSObjectRef constructor, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){assert(false);return nullptr;}
    virtual uint16_t getNumArgs() = 0;
};

template<typename T>
struct JSCCallback: public IJSCCallback {
    JSCCallback(T func){}
    virtual JSValueRef call(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        assert(false && "JSCCallback::call not implemented");
        return nullptr;
    }
    virtual uint16_t getNumArgs()  { return 0; }
};

template<typename R, typename Cls>
struct JSCCallback<R(Cls::*)(void)> :public IJSCCallback{
    typedef R(Cls::*F)(void);
    JSCCallback(F func):m_func(func){}
    virtual JSValueRef call(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        JS2CCALL_GET_COBJ;
        JSValueRef ret = ToJSValue<R>((pThis->*m_func)());
        resetJsStrBuf();
        return ret;
    }
    virtual uint16_t getNumArgs() { return 0; }
    F m_func;
};

template<typename R, typename P1>
struct JSCCallback<R(*)(P1)>: public IJSCCallback {
    typedef R(*F)(P1);
    JSCCallback(F func):m_func(func){}
    virtual JSValueRef call(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        JS2CCALL_GET_PARAMS1;
        JSValueRef ret = ToJSValue<R>((*m_func)(p1));
        resetJsStrBuf();
        return ret;
    }
    virtual uint16_t getNumArgs() { return 1; }
    F m_func;
};

template<typename R, typename Cls, typename P1>
struct JSCCallback<R(Cls::*)(P1)>: public IJSCCallback {
    typedef R(Cls::*F)(P1);
    JSCCallback(F func):m_func(func){}
    virtual JSValueRef call(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        JS2CCALL_GET_COBJ;
        JS2CCALL_GET_PARAMS1;
        JSValueRef ret = ToJSValue<R>((pThis->*m_func)(p1));
        resetJsStrBuf();
        return ret;
    }
    virtual uint16_t getNumArgs() { return 1; }
    F m_func;
};

template<typename R, typename Cls, typename P1, typename P2>//2
struct JSCCallback<R(Cls::*)(P1, P2)>:public IJSCCallback {
    typedef R(Cls::*F)(P1, P2);
    JSCCallback(F func):m_func(func){}
    virtual JSValueRef call(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        JS2CCALL_GET_COBJ;
        JS2CCALL_GET_PARAMS2;
        JSValueRef ret = ToJSValue<R>((pThis->*m_func)(p1,p2));
        resetJsStrBuf();
        return ret;
    }
    virtual uint16_t getNumArgs() { return 2; }
    F m_func;
};

template<typename R, typename Cls, typename P1, typename P2, typename P3>//3
struct JSCCallback<R(Cls::*)(P1, P2, P3)>:public IJSCCallback {
    typedef R(Cls::*F)(P1, P2, P3);
    JSCCallback(F func):m_func(func){}
    virtual JSValueRef call(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        JS2CCALL_GET_COBJ;
        JS2CCALL_GET_PARAMS3;
        JSValueRef ret = ToJSValue<R>((pThis->*m_func)(p1, p2,p3));
        resetJsStrBuf();
        return ret;
    }
    virtual uint16_t getNumArgs() { return 3; }
    F m_func;
};

template<typename R, typename Cls, typename P1, typename P2, typename P3, typename P4>//4
struct JSCCallback<R(Cls::*)(P1, P2, P3, P4)>:public IJSCCallback {
    typedef R(Cls::*F)(P1, P2, P3, P4);
    JSCCallback(F func):m_func(func){}
    virtual JSValueRef call(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        JS2CCALL_GET_COBJ;
        JS2CCALL_GET_PARAMS4;
        JSValueRef ret = ToJSValue<R>((pThis->*m_func)(p1, p2, p3, p4));
        resetJsStrBuf();
        return ret;
    }
    virtual uint16_t getNumArgs() { return 4; }
    F m_func;
};

template<typename R, typename Cls, typename P1, typename P2, typename P3, typename P4, typename P5>//5
struct JSCCallback<R(Cls::*)(P1, P2, P3, P4, P5)>:public IJSCCallback {
    typedef R(Cls::*F)(P1, P2, P3, P4, P5);
    JSCCallback(F func):m_func(func){}
    virtual JSValueRef call(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        JS2CCALL_GET_COBJ;
        JS2CCALL_GET_PARAMS5;
        JSValueRef ret = ToJSValue<R>((pThis->*m_func)(p1, p2, p3, p4,p5));
        resetJsStrBuf();
        return ret;
    }
    virtual uint16_t getNumArgs() { return 5; }
    F m_func;
};

template<typename R, typename Cls, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>//6
struct JSCCallback<R(Cls::*)(P1, P2, P3, P4, P5, P6)>:public IJSCCallback {
    typedef R(Cls::*F)(P1, P2, P3, P4, P5, P6);
    JSCCallback(F func):m_func(func){}
    virtual JSValueRef call(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        JS2CCALL_GET_COBJ;
        JS2CCALL_GET_PARAMS6;
        JSValueRef ret = ToJSValue<R>((pThis->*m_func)(p1, p2, p3, p4, p5,p6));
        resetJsStrBuf();
        return ret;
    }
    virtual uint16_t getNumArgs() { return 6; }
    F m_func;
};

template<typename R, typename Cls, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>//7
struct JSCCallback<R(Cls::*)(P1, P2, P3, P4, P5, P6, P7)>:public IJSCCallback {
    typedef R(Cls::*F)(P1, P2, P3, P4, P5, P6, P7);
    JSCCallback(F func):m_func(func){}
    virtual JSValueRef call(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        JS2CCALL_GET_COBJ;
        JS2CCALL_GET_PARAMS7;
        JSValueRef ret = ToJSValue<R>((pThis->*m_func)(p1, p2, p3, p4, p5, p6, p7));
        resetJsStrBuf();
        return ret;
    }
    virtual uint16_t getNumArgs() { return 7; }
    F m_func;
};

template<typename R, typename Cls, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10, typename P11, typename P12>//12
struct JSCCallback<R(Cls::*)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12)>:public IJSCCallback {
    typedef R(Cls::*F)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12);
    JSCCallback(F func):m_func(func){}
    virtual JSValueRef call(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        JS2CCALL_GET_COBJ;
        JS2CCALL_GET_PARAMS12;
        JSValueRef ret = ToJSValue<R>((pThis->*m_func)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12));
        resetJsStrBuf();
        return ret;
    }
    virtual uint16_t getNumArgs() { return 12; }
    F m_func;
};
    
 
template<typename Cls>
struct JSCCallback<void(Cls::*)(void)>:public IJSCCallback {
    typedef void(Cls::*F)(void);
    JSCCallback(F func):m_func(func){}
    virtual JSValueRef call(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        JS2CCALL_GET_COBJ;
        (pThis->*m_func)();
        resetJsStrBuf();
        return JSValueMakeUndefined(ctx);
    }
    virtual uint16_t getNumArgs() { return 0; }
    F m_func;
};

template< typename P1, typename Cls>//1
struct JSCCallback<void(Cls::*)(P1)> :public IJSCCallback{
    typedef void(Cls::*F)(P1);
    JSCCallback(F func):m_func(func){}
    virtual JSValueRef call(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        JS2CCALL_GET_COBJ;
        JS2CCALL_GET_PARAMS1;
        (pThis->*m_func)(p1);
        resetJsStrBuf();
        return JSValueMakeUndefined(ctx);
    }
    virtual uint16_t getNumArgs() { return 1; }
    F m_func;
};

template<typename Cls, typename P1, typename P2>//2
struct JSCCallback<void(Cls::*)(P1, P2)>:public IJSCCallback {
    typedef void(Cls::*F)(P1, P2);
    JSCCallback(F func):m_func(func){}
    virtual JSValueRef call(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        JS2CCALL_GET_COBJ;
        JS2CCALL_GET_PARAMS2;
        (pThis->*m_func)(p1,p2);
        resetJsStrBuf();
        return JSValueMakeUndefined(ctx);
    }
    virtual uint16_t getNumArgs() { return 2; }
    F m_func;
};

template<typename Cls, typename P1, typename P2, typename P3>//3
struct JSCCallback<void(Cls::*)(P1, P2, P3)>:public IJSCCallback {
    typedef void(Cls::*F)(P1, P2, P3);
    JSCCallback(F func):m_func(func){}
    virtual JSValueRef call(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        JS2CCALL_GET_COBJ;
        JS2CCALL_GET_PARAMS3;
        (pThis->*m_func)(p1,p2,p3);
        resetJsStrBuf();
        return JSValueMakeUndefined(ctx);
    }
    virtual uint16_t getNumArgs() { return 3; }
    F m_func;
};

template< typename Cls, typename P1, typename P2, typename P3, typename P4>//4
struct JSCCallback<void(Cls::*)(P1, P2, P3, P4)>:public IJSCCallback {
    typedef void(Cls::*F)(P1, P2, P3, P4);
    JSCCallback(F func):m_func(func){}
    virtual JSValueRef call(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        JS2CCALL_GET_COBJ;
        JS2CCALL_GET_PARAMS4;
        (pThis->*m_func)(p1, p2, p3, p4);
        resetJsStrBuf();
        return JSValueMakeUndefined(ctx);
    }
    virtual uint16_t getNumArgs() { return 4; }
    F m_func;
};

template< typename Cls, typename P1, typename P2, typename P3, typename P4, typename P5>//5
struct JSCCallback<void(Cls::*)(P1, P2, P3, P4, P5)>:public IJSCCallback {
    typedef void(Cls::*F)(P1, P2, P3, P4, P5);
    JSCCallback(F func):m_func(func){}
    virtual JSValueRef call(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        JS2CCALL_GET_COBJ;
        JS2CCALL_GET_PARAMS5;
        (pThis->*m_func)(p1, p2, p3, p4, p5);
        resetJsStrBuf();
        return JSValueMakeUndefined(ctx);
    }
    virtual uint16_t getNumArgs() { return 5; }
    F m_func;
};

template< typename Cls, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>//6
struct JSCCallback<void(Cls::*)(P1, P2, P3, P4, P5, P6)>:public IJSCCallback {
    typedef void(Cls::*F)(P1, P2, P3, P4, P5, P6);
    JSCCallback(F func):m_func(func){}
    virtual JSValueRef call(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        JS2CCALL_GET_COBJ;
        JS2CCALL_GET_PARAMS6;
        (pThis->*m_func)(p1, p2, p3, p4, p5, p6);
        resetJsStrBuf();
        return JSValueMakeUndefined(ctx);
    }
    virtual uint16_t getNumArgs() { return 6; }
    F m_func;
};

template< typename Cls, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>//7
struct JSCCallback<void(Cls::*)(P1, P2, P3, P4, P5, P6, P7)>:public IJSCCallback {
    typedef void(Cls::*F)(P1, P2, P3, P4, P5, P6, P7);
    JSCCallback(F func):m_func(func){}
    virtual JSValueRef call(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        JS2CCALL_GET_COBJ;
        JS2CCALL_GET_PARAMS7;
        (pThis->*m_func)(p1, p2, p3, p4, p5, p6, p7);
        resetJsStrBuf();
        return JSValueMakeUndefined(ctx);
    }
    virtual uint16_t getNumArgs() { return 7; }
    F m_func;
};

template< typename Cls, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>//9
struct JSCCallback<void(Cls::*)(P1, P2, P3, P4, P5, P6, P7, P8, P9)>:public IJSCCallback {
    typedef void(Cls::*F)(P1, P2, P3, P4, P5, P6, P7, P8, P9);
    JSCCallback(F func):m_func(func){}
    virtual JSValueRef call(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        JS2CCALL_GET_COBJ;
        JS2CCALL_GET_PARAMS9;
        (pThis->*m_func)(p1, p2, p3, p4, p5, p6, p7, p8, p9);
        resetJsStrBuf();
        return JSValueMakeUndefined(ctx);
    }
    virtual uint16_t getNumArgs() { return 9; }
    F m_func;
};


template<typename Cls, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10>//10
struct JSCCallback<void(Cls::*)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10)>:public IJSCCallback {
    typedef void(Cls::*F)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10);
    JSCCallback(F func):m_func(func){}
    virtual JSValueRef call(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        JS2CCALL_GET_COBJ;
        JS2CCALL_GET_PARAMS10;
        (pThis->*m_func)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
        resetJsStrBuf();
        return JSValueMakeUndefined(ctx);
    }
    virtual uint16_t getNumArgs() { return 10; }
    F m_func;
};

template< typename Cls, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10, typename P11>//11
struct JSCCallback<void(Cls::*)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11)>:public IJSCCallback {
    typedef void(Cls::*F)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11);
    JSCCallback(F func):m_func(func){}
    virtual JSValueRef call(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        JS2CCALL_GET_COBJ;
        JS2CCALL_GET_PARAMS11;
        (pThis->*m_func)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);
        resetJsStrBuf();
        return JSValueMakeUndefined(ctx);
    }
    virtual uint16_t getNumArgs() { return 11; }
    F m_func;
};

template< typename Cls, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10, typename P11, typename P12>//12
struct JSCCallback<void(Cls::*)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12)>:public IJSCCallback {
    typedef void(Cls::*F)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12);
    JSCCallback(F func):m_func(func){}
    virtual JSValueRef call(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        JS2CCALL_GET_COBJ;
        JS2CCALL_GET_PARAMS12;
        (pThis->*m_func)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);
        resetJsStrBuf();
        return JSValueMakeUndefined(ctx);
    }
    virtual uint16_t getNumArgs() { return 12; }
    F m_func;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define JS2CCONSTRUCTOR_GET_PARAMS1 \
    P1 p1 = __TransferToCpp<P1>::ToCpp(arguments[0]);
    
#define JS2CCONSTRUCTOR_GET_PARAMS2 \
    P1 p1 = __TransferToCpp<P1>::ToCpp(arguments[0]);\
    P2 p2 = __TransferToCpp<P2>::ToCpp(arguments[1]);
    
#define JS2CCONSTRUCTOR_GET_PARAMS3 \
    P1 p1 = __TransferToCpp<P1>::ToCpp(arguments[0]);\
    P2 p2 = __TransferToCpp<P2>::ToCpp(arguments[1]);\
    P3 p3 = __TransferToCpp<P3>::ToCpp(arguments[2]);
    
#define JS2CCONSTRUCTOR_GET_PARAMS4 \
    P1 p1 = __TransferToCpp<P1>::ToCpp(arguments[0]);\
    P2 p2 = __TransferToCpp<P2>::ToCpp(arguments[1]);\
    P3 p3 = __TransferToCpp<P3>::ToCpp(arguments[2]);\
    P4 p4 = __TransferToCpp<P4>::ToCpp(arguments[3]);

#define JS2CCONSTRUCTOR_GET_PARAMS5 \
    P1 p1 = __TransferToCpp<P1>::ToCpp(arguments[0]);\
    P2 p2 = __TransferToCpp<P2>::ToCpp(arguments[1]);\
    P3 p3 = __TransferToCpp<P3>::ToCpp(arguments[2]);\
    P4 p4 = __TransferToCpp<P4>::ToCpp(arguments[3]);\
    P5 p5 = __TransferToCpp<P5>::ToCpp(arguments[4]);

template<typename Cls>
struct JSCConstructor0: public IJSCCallback {
    virtual JSObjectRef constructor(JSContextRef ctx, JSObjectRef constructor, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        return JSCClass<Cls>::getInstance()->transferObjPtrToJS(new Cls());
    }
    virtual uint16_t getNumArgs()  { return 0; }
};
    
template<typename Cls>
IJSCCallback* regConstructor(){
    return new JSCConstructor0<Cls>();
}
    
template<typename Cls, typename P1>
struct JSCConstructor1:public IJSCCallback{
    virtual JSObjectRef constructor(JSContextRef ctx, JSObjectRef constructor, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        JS2CCONSTRUCTOR_GET_PARAMS1;
        return JSCClass<Cls>::getInstance()->transferObjPtrToJS(new Cls(p1));
    }
    virtual uint16_t getNumArgs() { return 1; }
};

template<typename Cls,typename P1>
IJSCCallback* regConstructor(){
    return new JSCConstructor1<Cls,P1>();
}
    
template<typename Cls, typename P1,typename P2>
struct JSCConstructor2:public IJSCCallback{
    virtual JSObjectRef constructor(JSContextRef ctx, JSObjectRef constructor, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        JS2CCONSTRUCTOR_GET_PARAMS2;
        return JSCClass<Cls>::getInstance()->transferObjPtrToJS(new Cls(p1,p2));
    }
    virtual uint16_t getNumArgs() { return 2; }
};
    
template<typename Cls,typename P1,typename P2>
IJSCCallback* regConstructor(){
    return new JSCConstructor2<Cls,P1,P2>();
}

template<typename Cls, typename P1,typename P2, typename P3>
struct JSCConstructor3:public IJSCCallback{
    virtual JSObjectRef constructor(JSContextRef ctx, JSObjectRef constructor, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        JS2CCONSTRUCTOR_GET_PARAMS3;
        return JSCClass<Cls>::getInstance()->transferObjPtrToJS(new Cls(p1,p2,p3));
    }
    virtual uint16_t getNumArgs() { return 3; }
};
    
template<typename Cls,typename P1,typename P2, typename P3>
IJSCCallback* regConstructor(){
    return new JSCConstructor3<Cls,P1,P2,P3>();
}

template<typename Cls, typename P1,typename P2, typename P3, typename P4>
struct JSCConstructor4:public IJSCCallback{
    virtual JSObjectRef constructor(JSContextRef ctx, JSObjectRef constructor, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        JS2CCONSTRUCTOR_GET_PARAMS4;
        return JSCClass<Cls>::getInstance()->transferObjPtrToJS(new Cls(p1,p2,p3,p4));
    }
    virtual uint16_t getNumArgs() { return 4; }
};
    
template<typename Cls,typename P1,typename P2, typename P3, typename P4>
IJSCCallback* regConstructor(){
    return new JSCConstructor4<Cls,P1,P2,P3,P4>();
}

template<typename Cls, typename P1,typename P2, typename P3, typename P4, typename P5>
struct JSCConstructor5:public IJSCCallback{
    virtual JSObjectRef constructor(JSContextRef ctx, JSObjectRef constructor, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        JS2CCONSTRUCTOR_GET_PARAMS5;
        return JSCClass<Cls>::getInstance()->transferObjPtrToJS(new Cls(p1,p2,p3,p4, p5));
    }
    virtual uint16_t getNumArgs() { return 5; }
};
    
template<typename Cls,typename P1,typename P2, typename P3, typename P4, typename P5>
IJSCCallback* regConstructor(){
    return new JSCConstructor5<Cls,P1,P2,P3,P4,P5>();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////


struct IJSCFunction
{
    virtual ~IJSCFunction(){}
    virtual JSValueRef call(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception) = 0;
    virtual uint16_t getNumArgs() = 0;
};

template<typename T>
struct JSCFunction: public IJSCFunction {
    JSCFunction(T func){}
    virtual JSValueRef call(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        assert(false && "JSCFunction::call not implemented");
        return nullptr;
    }
    virtual uint16_t getNumArgs()  { return 0; }
};

template<typename R>//R(void)
struct JSCFunction<R(*)(void)>: public IJSCFunction {
    typedef R(*F)(void);
    JSCFunction(F func):m_func(func){}
    virtual JSValueRef call(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        JSValueRef ret = ToJSValue<R>((*m_func)());
        resetJsStrBuf();
        return ret;
    }
    virtual uint16_t getNumArgs()  { return 0; }
    F m_func;
};

template<typename R, typename P1>//R(p1)
struct JSCFunction<R(*)(P1)>: public IJSCFunction {
    typedef R(*F)(P1);
    JSCFunction(F func):m_func(func){}
    virtual JSValueRef call(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        JS2CCALL_GET_PARAMS1;
        JSValueRef ret = ToJSValue<R>((*m_func)(p1));
        resetJsStrBuf();
        return ret;
    }
    virtual uint16_t getNumArgs()  { return 1; }
    F m_func;
};

template<typename R, typename P1>//const R(p1)
struct JSCFunction<const R(*)(P1)>:public IJSCFunction {
    typedef const R(*F)(P1);
    JSCFunction(F func):m_func(func){}
    virtual JSValueRef call(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        JS2CCALL_GET_PARAMS1;
        JSValueRef ret = ToJSValue<R>((*m_func)(p1));
        resetJsStrBuf();
        return ret;
    }
    virtual uint16_t getNumArgs()  { return 1; }
    F m_func;
};

template<typename R, typename P1, typename P2>//R(p1,p2)
struct JSCFunction<R(*)(P1, P2)>: public IJSCFunction {
    typedef R(*F)(P1,P2);
    JSCFunction(F func):m_func(func){}
    virtual JSValueRef call(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        JS2CCALL_GET_PARAMS2;
        JSValueRef ret = ToJSValue<R>((*m_func)(p1, p2));
        resetJsStrBuf();
        return ret;
    }
    virtual uint16_t getNumArgs()  { return 2; }
    F m_func;
};

template<typename R, typename P1, typename P2, typename P3>//3
struct JSCFunction<R(*)(P1, P2, P3)>: public IJSCFunction {
    typedef R(*F)(P1, P2, P3);
    JSCFunction(F func):m_func(func){}
    virtual JSValueRef call(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        JS2CCALL_GET_PARAMS3;
        JSValueRef ret = ToJSValue<R>((*m_func)(p1, p2, p3));
        resetJsStrBuf();
        return ret;
    }
    virtual uint16_t getNumArgs()  { return 3; }
    F m_func;
};

template<typename R, typename P1, typename P2, typename P3, typename P4>//4
struct JSCFunction<R(*)(P1, P2, P3, P4)>: public IJSCFunction {
    typedef R(*F)(P1, P2, P3, P4);
    JSCFunction(F func):m_func(func){}
    virtual JSValueRef call(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        JS2CCALL_GET_PARAMS4;
        JSValueRef ret = ToJSValue<R>((*m_func)(p1, p2, p3, p4));
        resetJsStrBuf();
        return ret;
    }
    virtual uint16_t getNumArgs()  { return 4; }
    F m_func;
};

template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5>//5
struct JSCFunction<R(*)(P1, P2, P3, P4, P5)>: public IJSCFunction {
    typedef R(*F)(P1, P2, P3, P4, P5);
    JSCFunction(F func):m_func(func){}
    virtual JSValueRef call(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        JS2CCALL_GET_PARAMS5;
        JSValueRef ret = ToJSValue<R>((*m_func)(p1, p2, p3, p4, p5));
        resetJsStrBuf();
        return ret;
    }
    virtual uint16_t getNumArgs()  { return 5; }
    F m_func;
};

template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>//6
struct JSCFunction<R(*)(P1, P2, P3, P4, P5, P6)>: public IJSCFunction {
    typedef R(*F)(P1, P2, P3, P4, P5, P6);
    JSCFunction(F func):m_func(func){}
    virtual JSValueRef call(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        JS2CCALL_GET_PARAMS6;
        JSValueRef ret = ToJSValue<R>((*m_func)(p1, p2, p3, p4, p5, p6));
        resetJsStrBuf();
        return ret;
    }
    virtual uint16_t getNumArgs()  { return 6; }
    F m_func;
};

template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>//7
struct JSCFunction<R(*)(P1, P2, P3, P4, P5, P6, P7)>: public IJSCFunction {
    typedef R(*F)(P1, P2, P3, P4, P5, P6, P7);
    JSCFunction(F func):m_func(func){}
    virtual JSValueRef call(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        JS2CCALL_GET_PARAMS7;
        JSValueRef ret = ToJSValue<R>((*m_func)(p1, p2, p3, p4, p5, p6, p7));
        resetJsStrBuf();
        return ret;
    }
    virtual uint16_t getNumArgs()  { return 7; }
    F m_func;
};

template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>//8
struct JSCFunction<R(*)(P1, P2, P3, P4, P5, P6, P7, P8)>: public IJSCFunction {
    typedef R(*F)(P1, P2, P3, P4, P5, P6, P7, P8);
    JSCFunction(F func):m_func(func){}
    virtual JSValueRef call(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        JS2CCALL_GET_PARAMS8;
        JSValueRef ret = ToJSValue<R>((*m_func)(p1, p2, p3, p4, p5, p6, p7, p8));
        resetJsStrBuf();
        return ret;
    }
    virtual uint16_t getNumArgs()  { return 8; }
    F m_func;
};

template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>//9
struct JSCFunction<R(*)(P1, P2, P3, P4, P5, P6, P7, P8, P9)>: public IJSCFunction {
    typedef R(*F)(P1, P2, P3, P4, P5, P6, P7, P8, P9);
    JSCFunction(F func):m_func(func){}
    virtual JSValueRef call(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        JS2CCALL_GET_PARAMS9;
        JSValueRef ret =  ToJSValue<R>((*m_func)(p1, p2, p3, p4, p5, p6, p7, p8, p9));
        resetJsStrBuf();
        return ret;
    }
    virtual uint16_t getNumArgs()  { return 9; }
    F m_func;
};

//void
template<> //void(void)
struct JSCFunction<void(*)(void)>: public IJSCFunction {
    typedef void(*F)(void);
    JSCFunction(F func):m_func(func){}
    virtual JSValueRef call(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        (*m_func)();
        resetJsStrBuf();
        return JSValueMakeUndefined(ctx);
    }
    virtual uint16_t getNumArgs()  { return 0; }
    F m_func;
};

template<typename P1>
struct JSCFunction<void(*)(P1)>: public IJSCFunction {//void(p1)
    typedef void(*F)(P1);
    JSCFunction(F func):m_func(func){}
    virtual JSValueRef call(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        JS2CCALL_GET_PARAMS1;
        (*m_func)(p1);
        resetJsStrBuf();
        return JSValueMakeUndefined(ctx);
    }
    virtual uint16_t getNumArgs()  { return 1; }
    F m_func;
};

template<typename P1, typename P2>//void(p1,p2)
struct JSCFunction<void(*)(P1, P2)>: public IJSCFunction {
    typedef void(*F)(P1, P2);
    JSCFunction(F func):m_func(func){}
    virtual JSValueRef call(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        JS2CCALL_GET_PARAMS2;
        (*m_func)(p1, p2);
        resetJsStrBuf();
        return JSValueMakeUndefined(ctx);
    }
    virtual uint16_t getNumArgs()  { return 2; }
    F m_func;
};

template< typename P1, typename P2, typename P3>//3
struct JSCFunction<void(*)(P1, P2, P3)>: public IJSCFunction {
    typedef void(*F)(P1, P2, P3);
    JSCFunction(F func):m_func(func){}
    virtual JSValueRef call(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        JS2CCALL_GET_PARAMS3;
        (*m_func)(p1, p2, p3);
        resetJsStrBuf();
        return JSValueMakeUndefined(ctx);
    }
    virtual uint16_t getNumArgs()  { return 3; }
    F m_func;
};

template<typename P1, typename P2, typename P3, typename P4>//4
struct JSCFunction<void(*)(P1, P2, P3, P4)>: public IJSCFunction {
    typedef void(*F)(P1, P2, P3, P4);
    JSCFunction(F func):m_func(func){}
    virtual JSValueRef call(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        JS2CCALL_GET_PARAMS4;
        (*m_func)(p1, p2, p3, p4);
        resetJsStrBuf();
        return JSValueMakeUndefined(ctx);
    }
    virtual uint16_t getNumArgs()  { return 4; }
    F m_func;
};

template<typename P1, typename P2, typename P3, typename P4, typename P5>//5
struct JSCFunction<void(*)(P1, P2, P3, P4, P5)>: public IJSCFunction {
    typedef void(*F)(P1, P2, P3, P4, P5);
    JSCFunction(F func):m_func(func){}
    virtual JSValueRef call(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        JS2CCALL_GET_PARAMS5;
        (*m_func)(p1, p2, p3, p4, p5);
        resetJsStrBuf();
        return JSValueMakeUndefined(ctx);
    }
    virtual uint16_t getNumArgs()  { return 5; }
    F m_func;
};

template<typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>//6
struct JSCFunction<void(*)(P1, P2, P3, P4, P5, P6)>: public IJSCFunction {
    typedef void(*F)(P1, P2, P3, P4, P5, P6);
    JSCFunction(F func):m_func(func){}
    virtual JSValueRef call(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        JS2CCALL_GET_PARAMS6;
        (*m_func)(p1, p2, p3, p4, p5, p6);
        resetJsStrBuf();
        return JSValueMakeUndefined(ctx);
    }
    virtual uint16_t getNumArgs()  { return 6; }
    F m_func;
};

template< typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>//7
struct JSCFunction<void(*)(P1, P2, P3, P4, P5, P6, P7)>: public IJSCFunction {
    typedef void(*F)(P1, P2, P3, P4, P5, P6, P7);
    JSCFunction(F func):m_func(func){}
    virtual JSValueRef call(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        JS2CCALL_GET_PARAMS7;
        (*m_func)(p1, p2, p3, p4, p5, p6, p7);
        resetJsStrBuf();
        return JSValueMakeUndefined(ctx);
    }
    virtual uint16_t getNumArgs()  { return 7; }
    F m_func;
};

template<typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>//8
struct JSCFunction<void(*)(P1, P2, P3, P4, P5, P6, P7, P8)>: public IJSCFunction {
    typedef void(*F)(P1, P2, P3, P4, P5, P6, P7, P8);
    JSCFunction(F func):m_func(func){}
    virtual JSValueRef call(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        JS2CCALL_GET_PARAMS8;
        (*m_func)(p1, p2, p3, p4, p5, p6, p7, p8);
        resetJsStrBuf();
        return JSValueMakeUndefined(ctx);
    }
    virtual uint16_t getNumArgs()  { return 8; }
    F m_func;
};

template<typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>//9
struct JSCFunction<void(*)(P1, P2, P3, P4, P5, P6, P7, P8, P9)>: public IJSCFunction {
    typedef void(*F)(P1, P2, P3, P4, P5, P6, P7, P8, P9);
    JSCFunction(F func):m_func(func){}
    virtual JSValueRef call(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        JS2CCALL_GET_PARAMS9;
        (*m_func)(p1, p2, p3, p4, p5, p6, p7, p8, p9);
        resetJsStrBuf();
        return JSValueMakeUndefined(ctx);
    }
    virtual uint16_t getNumArgs()  { return 9; }
    F m_func;
};

template<typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10, typename P11, typename P12>//12
struct JSCFunction<void(*)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12)>: public IJSCFunction {
    typedef void(*F)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12);
    JSCFunction(F func):m_func(func){}
    virtual JSValueRef call(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        JS2CCALL_GET_PARAMS12;
        (*m_func)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);
        resetJsStrBuf();
        return JSValueMakeUndefined(ctx); 
    }
    virtual uint16_t getNumArgs()  { return 12; }
    F m_func;
};
}   // namespace __JSCProxy

#endif
