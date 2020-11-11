
#ifndef JS_BINDER_V8__H__
#define JS_BINDER_V8__H__

#include <v8.h>
#include <type_traits> 
#include "JSCProxyType.h"
#include <util/Log.h>
#include <JSObjBase.h>
#include "JSCProxyTrnasfer.h"
#include <cassert>

using namespace v8;

namespace laya {
    //js的析构，这个是非全局对象的。直接删除创建的c对象
    template <typename T>
    static void JsDestructor(const WeakCallbackInfo<Persistent<Object> >& data) {
        Isolate* pIso  = data.GetIsolate();
        //Isolate* pIso1 = Isolate::GetCurrent();
        HandleScope __lHandleScope(pIso);

        Persistent<Object> *pPersistentVal = data.GetParameter();
        pPersistentVal->Reset();
        T *ptr = (T *)data.GetInternalField(0);
        delete ptr;
        delete pPersistentVal;  //TODO 
    }
    //这个是绑定全局对象的。不能直接删除c对象
    template <typename T>
    static void JsDestructor1(const WeakCallbackInfo<Persistent<Object> >& data) {
        Isolate* pIso = data.GetIsolate();// Isolate::GetCurrent();
        HandleScope __lHandleScope(pIso);

        Persistent<Object> *pPersistentVal = data.GetParameter();
        T *ptr = (T *)data.GetInternalField(0);
        //delete ptr;
        pPersistentVal->Reset();
        delete pPersistentVal;
    }

    //直接根据c的类创建一个对应的js对象。 并绑定一个已经存在的c对象
    //如果weak=true则表示由js负责这个对象的删除（通过weakCallback）。
    template<class T>
    Local<Object> createJsObjAttachCObj(T* cobj, bool weak) {
        Isolate* pIso = Isolate::GetCurrent();
        EscapableHandleScope handle_scope(pIso);
        Persistent<ObjectTemplate>* pTemp = (Persistent<ObjectTemplate>*)T::JSCLSINFO.objTemplate;
        if (!pTemp) {
            return Local<Object>::Cast(Null(pIso));
        }
        Handle<ObjectTemplate> pTempl = Local<ObjectTemplate>::New(pIso, pTemp->Get(pIso));
        Handle<Object> pNewIns = pTempl->NewInstance();//
        pNewIns->SetAlignedPointerInInternalField(0, cobj);
        pNewIns->SetAlignedPointerInInternalField(1, (void*)&T::JSCLSINFO);
        Persistent<Object>* pPersist = new Persistent<Object>(pIso, pNewIns);
        cobj->mpJsIso = pIso;
        cobj->mpJsThis = pPersist;
        if (weak) {
            pPersist->SetWeak(pPersist, JsDestructor<T>, v8::WeakCallbackType::kInternalFields);
        }
        cobj->createRefArray();

        return handle_scope.Escape(pNewIns);
    }

    typedef Local<Value> JsValue;
    typedef Local<String> JsString;
    typedef FunctionCallbackInfo<Value> JsFuncArgs;
    typedef Local<Function>		JsFunction;

    inline JsString Js_Str(Isolate* pIso, const char* str) {
        return String::NewFromUtf8(pIso, str);
    }

    //下面这个主要是处理void的，让void能像普通返回值 一样，避免下面还要区分是否是void
    template <typename T>
    struct imp_ToJSValue {
        static JsValue trans(T t) {
            return __TransferToJs<T>::ToJs(t);
        }
    };

    template<class T>
    JsValue ToJSValue(T t) {
        return imp_ToJSValue<T>::trans(t);
    }

    template<>
    struct imp_ToJSValue<void> {
        static JsValue trans(void) {
            return Null(Isolate::GetCurrent());
        }
    };

    //template<>
    //struct imp_ToJSValue<void*> {
    //	static JsValue trans(void* t) {
    //		Value* pv = (Value*)t;
    //		if (pv->IsString()) {
    //			return pv->ToString();
    //		}
    //		else if (pv->IsInt32()) {
    //			return pv->ToInt32();
    //		}
    //		else
    //			return pv->ToObject();
    //		return Null(Isolate::GetCurrent());
    //	}
    //};

    template<>
    struct imp_ToJSValue<JsValue> {
        static JsValue trans(JsValue t) {
            return t;
        }
    };

    template<>
    struct imp_ToJSValue<int> {
        static JsValue trans(int t) {
            return Integer::New(Isolate::GetCurrent(), t);
        }
    };

    ////这种情况下直接返回的就是js对象，是Value,Object之类的，不是Local<>
    //template<>
    //struct imp_ToJSValue<const void*> {
    //	static JsValue trans(const void* v) {
    //		Value* pv = (Value*)v;
    //		if (pv->IsString()) {
    //			return pv->ToString();
    //		}
    //		else if (pv->IsInt32()) {
    //			return pv->ToInt32();
    //		}
    //		else if (pv->IsObject())
    //			return pv->ToObject();
    //		else if (pv->IsBoolean())
    //			return pv->ToBoolean();
    //		else if (pv->IsUint32())
    //			return pv->ToUint32();
    //		else if (pv->IsNumber())
    //			return pv->ToNumber();
    //		//不能各种类型都遍历,由于基本类型一般不会返回void*
    //		//其他类型都是从Object继承的，所以这里简化处理一下。
    //		return Null(Isolate::GetCurrent());
    //	}
    //};

    /*
    template<>
    struct ToJSValue<int64_t> {
        static JsValue trans(int64_t t) {
            return Local<Number>::New(Isolate::GetCurrent(),t);
        }
    };
    */

    void imp_js2cfunc_common(const FunctionCallbackInfo<Value>& args);
    //函数无法偏特化，所以用类封装一下
    template <typename T>
    struct imp_JS2CFunc {
        static void call(const FunctionCallbackInfo<Value>& args) {	//偏特化去实现。
            imp_js2cfunc_common(args);
        }
    };

    //直接根据c的类创建一个对应的js对象。 retObj是返回的c对象
    //需要能调到上面的构造函数
    template<class T>
    Local<Object> createAJsObj(T*& retobj) {
        Isolate* pIso = Isolate::GetCurrent();
        EscapableHandleScope handle_scope(pIso);
        Persistent<ObjectTemplate>* pTemp = (Persistent<ObjectTemplate>*)T::JSCLSINFO.objTemplate;
        if (!pTemp) {
            retobj = NULL;
            return Local<Object>::Cast(Null(pIso));
        }
        Handle<ObjectTemplate> pTempl = Local<ObjectTemplate>::New(pIso, pTemp->Get(pIso));
        Handle<Object> pNewIns = pTempl->NewInstance();
        //不知道为什么他不会调用上面的构造，所以自己来做
        //retobj = (T*)Local<External>::Cast(pNewIns->GetInternalField(0))->Value();
        T* obj = new T;
        obj->mpJsIso = pIso;
        pNewIns->SetAlignedPointerInInternalField(0, obj);
        pNewIns->SetAlignedPointerInInternalField(1, (void*)&T::JSCLSINFO);
        Persistent<Object>* pPersist = new Persistent<Object>(pIso, pNewIns);
        obj->mpJsThis = pPersist;
        pPersist->SetWeak(obj->mpJsThis, JsDestructor<T>, v8::WeakCallbackType::kInternalFields);
        obj->createRefArray();
        //obj->JSConstructor((JsFuncArgs&)args);
        retobj = obj;

        return handle_scope.Escape(pNewIns);
    }
    //保存函数指针，是为了给js回调用。
    template <typename T>
    struct FuncInfo {
        T func;
        FuncInfo(T f) {
            func = f;
        }
    };

    //封装两个函数的对象。例如可以保存get，set函数
    template <typename F1, typename F2>
    struct FuncInfo2 {
        F1 f1;
        F2 f2;
        FuncInfo2(F1 a, F2 b) {
            f1 = a; f2 = b;
        }
    };

    //保存属性的get和set函数，只能是类函数，提供类名和数据类型
    template <class G, class S>
    struct PropFuncInfo {
        G fGet;
        S fSet;
        PropFuncInfo(G g, S s) {
            fGet = g;
            fSet = s;
        }
        PropFuncInfo(G g) {//只读的构造
            fGet = g;
            fSet = 0; //这个不知道怎么赋值，没法阻止错误使用
        }
    };

    //属性相关/////////////////////////////////
    //只能是对象的函数
    template <typename G, typename S>
    struct imp_JsGetProp {
        static void call(Local<String> property, const PropertyCallbackInfo<Value>& info) { assert(false); }
    };

    template <class Cls, class R, class S>
    struct imp_JsGetProp<R(Cls::*)(void), S> {
        static void call(Local<String> property, const PropertyCallbackInfo<Value>& info) {
            PropFuncInfo< R(Cls::*)(void), S>* funcInfo = (PropFuncInfo< R(Cls::*)(void), S>*)External::Cast(*info.Data())->Value();
            Local<Object> pthis = info.This();
            JsObjClassInfo* id = (JsObjClassInfo*)pthis->GetAlignedPointerFromInternalField(1);
            if (!IsSubClass<Cls>(id)) { LOGE("throw !IsSubClass %d", id->ClsID); throw - 1; }
            //if (id != Cls::JSCLSINFO.ClsID)	throw - 1;
            Cls *pObj = (Cls*)pthis->GetAlignedPointerFromInternalField(0);
            info.GetReturnValue().Set(ToJSValue<R>((pObj->*funcInfo->fGet)()));
        }
    };

    template <class G, class S>
    struct imp_JsSetProp {
        static void call(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info) { assert(false); }
    };

    template <class G, class Cls, class T>
    struct imp_JsSetProp<G, void(Cls::*)(T)> {
        static void call(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info) {
            PropFuncInfo<G, void(Cls::*)(T)>* funcInfo = (PropFuncInfo<G, void(Cls::*)(T) >*)External::Cast(*info.Data())->Value();
            Local<Object> pthis = info.This();
            JsObjClassInfo* id = (JsObjClassInfo*)pthis->GetAlignedPointerFromInternalField(1);
            if (!IsSubClass<Cls>(id)) { LOGE("throw !IsSubClass2 %d", id->ClsID); throw - 1; }
            //if (id != Cls::JSCLSINFO.ClsID)	throw - 1;
            Cls *pObj = (Cls*)pthis->GetAlignedPointerFromInternalField(0);
            (pObj->*funcInfo->fSet)(__TransferToCpp<T>::ToCpp(value));
        }
    };
    //Set还带返回值的情况
    template <class G, class Cls, class T>
    struct imp_JsSetProp<G, T(Cls::*)(T)> {
        static void call(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info) {
            PropFuncInfo<G, T(Cls::*)(T)>* funcInfo = (PropFuncInfo<G, T(Cls::*)(T)>*)External::Cast(*info.Data())->Value();
            Local<Object> pthis = info.This();
            JsObjClassInfo* id = (JsObjClassInfo*)pthis->GetAlignedPointerFromInternalField(1);
            if (!IsSubClass<Cls>(id)) { LOGE("throw !IsSubClass3 %d", id->ClsID); throw - 1; }
            //if (id != Cls::JSCLSINFO.ClsID)	throw - 1;
            Cls *pObj = (Cls*)pthis->GetAlignedPointerFromInternalField(0);
            T ret = (pObj->*funcInfo->fSet)(__TransferToCpp<T>::ToCpp(value));
            info.GetReturnValue().Set(ToJSValue<T>(ret));
        }
    };
    ///////////////////////////////////////////

    //要求cfunc必须是类成员函数，因为调用的时候是那么假设的
    //让一个js的FunctionTemplate调用到指定的c函数
    //template <class T>
    //Local<Function> bindV8(Local<FunctionTemplate> func, T cfunc) {
    //	Isolate* pIso = Isolate::GetCurrent();
    //	//函數不能轉成數字，只好取地址再轉，但是取的地址是堆栈地址，所以再取*转成int,表示实际地址了，再强转void*
    //	Handle<Value> data = External::New(pIso, (void*)*(int*)&cfunc);
    //	func->SetCallHandler(imp_bindv8cCB<T>::call, data);
    //	//return imp_bindv8c<T>::call(func, cfunc, ptr);
    //	return func->GetFunction();
    //}

    template <class T>
    Local<Value> WrapCFuncAsInt(T cfunc) {
        Isolate* pIso = Isolate::GetCurrent();
        //函數不能轉成數字，只好取地址再轉，但是取的地址是堆栈地址，所以再取*转成int,表示实际地址了，再强转void*
        return External::New(pIso, (void*)*(int*)&cfunc);
    }

    //由於v8在js對象的0的位置存了c對象了，所以只要傳函數地址就行了
    template <class T>
    Local<Function> createJSMethod(T cfunc) {
        Isolate* pIso = Isolate::GetCurrent();
        Local<FunctionTemplate> method = FunctionTemplate::New(pIso);
        FuncInfo<T>* pData = new FuncInfo<T>(cfunc);	//怎么释放呢
        //函數不能轉成數字，只好取地址再轉，但是取的地址是堆栈地址，所以再取*转成int,表示实际地址了，再强转void*
        Handle<Value> data = External::New(pIso, pData);// (void*)*(int*)&cfunc);
        method->SetCallHandler(imp_JS2CFunc<T>::call, data);
        return method->GetFunction();
    }
    template <class T>
    void addJSMethod(const char* name, v8::Local<v8::FunctionTemplate> that, T cfunc) {
        Isolate* pIso = Isolate::GetCurrent();
        FuncInfo<T>* pData = new FuncInfo<T>(cfunc);	//怎么释放呢
         //函數不能轉成數字，只好取地址再轉，但是取的地址是堆栈地址，所以再取*转成int,表示实际地址了，再强转void*
        Handle<Value> data = External::New(pIso, pData);// (void*)*(int*)&cfunc);
        //instancTemp->SetNativeDataProperty(Js_Str(pIso, name), imp_JS2CFunc<T>::call, 0, data);

        v8::Local<v8::Signature> signature = v8::Signature::New(pIso, that);
        v8::Local<v8::FunctionTemplate> t = v8::FunctionTemplate::New(pIso, imp_JS2CFunc<T>::call, data, signature);
        // kInternalized strings are created in the old space.
        const v8::NewStringType type = v8::NewStringType::kInternalized;
        v8::Local<v8::String> name_string = v8::String::NewFromUtf8(pIso, name, type).ToLocalChecked();
        that->PrototypeTemplate()->Set(name_string, t);
        t->SetClassName(name_string);  // NODE_SET_PROTOTYPE_METHOD() compatibility.
    }
    //直接封装符合v8标准的指定函数
    Local<Function> createJSMethodRaw(FunctionCallback func, JsValue data);

    /////////js对象的基类////////////////
    class JSObjBaseV8 {
        friend class JsObjHandle2;
    public:
        JSObjBaseV8();
        virtual ~JSObjBaseV8();

        void retainThis();			//防止自己被刪除，例如做異步任務的時候。
        void releaseThis();			//可以被刪除了。
        static void destroyWeakCB(const WeakCallbackInfo<Persistent<Object> >& data);
        static void destroyWeakRefArray(const WeakCallbackInfo<Persistent<Array> >& data);

        //自己创建一个js对象，主要用于不是与某个固定js对象关联的，又需要
        //持久引用的临时对象，这时候的this就不是弱引用了，需要主动删掉
        void createJSObj();

        //void addRefHandle(JsObjHandle2* handle);
        //創建一個數組，用於保存引用的js對象。
        void createRefArray();

        void setRefObj(int idx, JsValue obj);

        Local<Object> getRefObj(int idx);
        //检查js环境是否变化了，一般用来异步回调的保护
        bool IsMyJsEnv() {
            return Isolate::GetCurrent() == mpJsIso;
        }

        void setJSArgs(const JsFuncArgs* args) { mpCurJsArgs = args; };
        Persistent<Object>* weakHoldJsObj(Local<Object>& pobj);

        //通过jsthis来调用函数
        inline JsValue _callJsFunc(JsFunction& func, int argc, JsValue argv[]) {
            EscapableHandleScope sc(mpJsIso);
            //TODO 能不能不用再new local了
            //Local<Function> localfunc = Local<Function>::New(mpJsIso, Persistent<Function>::Cast(func));
            Local<Object> localthis = Local<Object>::New(mpJsIso, Persistent<Object>::Cast(*mpJsThis));
            //int argc = 0;
            //v8::Local<v8::Value> argv[1];
            return sc.Escape(func->Call(localthis, argc, argv));
        }

        JsValue callJsFunc(JsFunction& func);

        template<class P1>
        JsValue callJsFunc(JsFunction& func, P1 p1) {
            int argc = 1;
            Local<Value> argv[1];
            argv[0] = __TransferToJs<P1>::ToJs(p1);
            return _callJsFunc(func, argc, argv);
        }

        template<class P1, class P2>
        JsValue callJsFunc(JsFunction& func, P1 p1, P2 p2) {
            int argc = 2;
            Local<Value> argv[2];
            argv[0] = __TransferToJs<P1>::ToJs(p1);
            argv[1] = __TransferToJs<P2>::ToJs(p2);
            return _callJsFunc(func, argc, argv);
        }
        template<class P1, class P2, class P3>
        JsValue callJsFunc(JsFunction& func, P1 p1, P2 p2, P3 p3) {
            int argc = 3;
            Local<Value> argv[3];
            argv[0] = __TransferToJs<P1>::ToJs(p1);
            argv[1] = __TransferToJs<P2>::ToJs(p2);
            argv[2] = __TransferToJs<P3>::ToJs(p3);
            return _callJsFunc(func, argc, argv);
        }
        template<class P1, class P2, class P3, class P4>
        JsValue callJsFunc(JsFunction& func, P1 p1, P2 p2, P3 p3, P4 p4) {
            int argc = 4;
            Local<Value> argv[4];
            argv[0] = __TransferToJs<P1>::ToJs(p1);
            argv[1] = __TransferToJs<P2>::ToJs(p2);
            argv[2] = __TransferToJs<P3>::ToJs(p3);
            argv[3] = __TransferToJs<P4>::ToJs(p4);
            return _callJsFunc(func, argc, argv);
        }
        template<class P1, class P2, class P3, class P4, class P5>
        JsValue callJsFunc(JsFunction& func, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5) {
            int argc = 5;
            Local<Value> argv[5];
            argv[0] = __TransferToJs<P1>::ToJs(p1);
            argv[1] = __TransferToJs<P2>::ToJs(p2);
            argv[2] = __TransferToJs<P3>::ToJs(p3);
            argv[3] = __TransferToJs<P4>::ToJs(p4);
            argv[4] = __TransferToJs<P5>::ToJs(p5);
            return _callJsFunc(func, argc, argv);
        }
        template<class P1, class P2, class P3, class P4, class P5, class P6>
        JsValue callJsFunc(JsFunction& func, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6) {
            int argc = 6;
            Local<Value> argv[6];
            argv[0] = __TransferToJs<P1>::ToJs(p1);
            argv[1] = __TransferToJs<P2>::ToJs(p2);
            argv[2] = __TransferToJs<P3>::ToJs(p3);
            argv[3] = __TransferToJs<P4>::ToJs(p4);
            argv[4] = __TransferToJs<P5>::ToJs(p5);
            argv[5] = __TransferToJs<P6>::ToJs(p6);
            return _callJsFunc(func, argc, argv);
        }
        template<class P1, class P2, class P3, class P4, class P5, class P6, class P7>
        JsValue callJsFunc(JsFunction& func, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7) {
            int argc = 7;
            Local<Value> argv[7];
            argv[0] = __TransferToJs<P1>::ToJs(p1);
            argv[1] = __TransferToJs<P2>::ToJs(p2);
            argv[2] = __TransferToJs<P3>::ToJs(p3);
            argv[3] = __TransferToJs<P4>::ToJs(p4);
            argv[4] = __TransferToJs<P5>::ToJs(p5);
            argv[5] = __TransferToJs<P6>::ToJs(p6);
            argv[6] = __TransferToJs<P7>::ToJs(p7);
            return _callJsFunc(func, argc, argv);
        }
        template<class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
        JsValue callJsFunc(JsFunction& func, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8) {
            int argc = 8;
            Local<Value> argv[8];
            argv[0] = __TransferToJs<P1>::ToJs(p1);
            argv[1] = __TransferToJs<P2>::ToJs(p2);
            argv[2] = __TransferToJs<P3>::ToJs(p3);
            argv[3] = __TransferToJs<P4>::ToJs(p4);
            argv[4] = __TransferToJs<P5>::ToJs(p5);
            argv[5] = __TransferToJs<P6>::ToJs(p6);
            argv[6] = __TransferToJs<P7>::ToJs(p7);
            argv[7] = __TransferToJs<P8>::ToJs(p8);
            return _callJsFunc(func, argc, argv);
        }
        template<class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10, class P11, class P12>
        JsValue callJsFunc(JsFunction& func, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10, P11 p11, P12 p12) {
            int argc = 12;
            Local<Value> argv[12];
            argv[0] = __TransferToJs<P1>::ToJs(p1);
            argv[1] = __TransferToJs<P2>::ToJs(p2);
            argv[2] = __TransferToJs<P3>::ToJs(p3);
            argv[3] = __TransferToJs<P4>::ToJs(p4);
            argv[4] = __TransferToJs<P5>::ToJs(p5);
            argv[5] = __TransferToJs<P6>::ToJs(p6);
            argv[6] = __TransferToJs<P7>::ToJs(p7);
            argv[7] = __TransferToJs<P8>::ToJs(p8);
            argv[8] = __TransferToJs<P8>::ToJs(p9);
            argv[9] = __TransferToJs<P8>::ToJs(p10);
            argv[10] = __TransferToJs<P8>::ToJs(p11);
            argv[11] = __TransferToJs<P8>::ToJs(p12);
            return _callJsFunc(func, argc, argv);
        }
    public:
        //js调用当前函数的时候的参数。主要是有时候想要获得一些具体的变量，
        //所有的公用这一个，所以是非线程安全的
        const JsFuncArgs*		mpCurJsArgs;
        Persistent<Object>*		mpJsThis;		//value太基础了，用object，反正function也是object
        Persistent<Array>*		mpRefArray;
        Isolate*				mpJsIso;
    protected:
        Persistent<Object>		mRetainedThis;
        bool					mbWeakThis;
        unsigned short			mnRefArrSize;
    };

    class MyJsObjHandle {
    public:
    };

    struct JsValue2Handle {
        int id;
        JSObjBaseV8* pobj;
        JsValue v;
    };
    class JsObjHandle2 :public MyJsObjHandle {
    public:
        JsObjHandle2() {
            m_pObj = NULL;
            m_nID = 0;
        }
        JSObjBaseV8*	m_pObj;
        int				m_nID;		//數組下標
        Local<Value>	m_pReturn;
        bool Empty();

        JsValue getJsObj() {
            if (!m_pObj)
                return Undefined(Isolate::GetCurrent());
            return m_pObj->getRefObj(m_nID);
        }
        //引用另外一個js對象， 只拷貝內容，idx不要拷。
        JsObjHandle2 &operator=(JsObjHandle2 *p_Val) {
            if (m_pObj)
                m_pObj->setRefObj(m_nID, p_Val->getJsObj());
            return *this;
        }

        //不允许直接从JsValue赋值
        //JsObjHandle2 &operator=(JsValue v)

        JsObjHandle2 &operator=(JsValue2Handle& v) {
            if (m_pObj) {
                m_pObj = v.pobj;
                m_nID = v.id;
                m_pObj->setRefObj(m_nID, v.v);
            }
            return *this;
        }

        void set(int id, JSObjBaseV8* pobj, JsValue& v) {
            m_pObj = pobj;
            m_nID = id;
            if (m_pObj)
                m_pObj->setRefObj(m_nID, v);
        }
        void set(JsValue& v);

        template <typename _Tp>
        static bool IsTypeof(JsValue& val) {
            if (val.IsEmpty())
                return false;
            __InferType<_Tp> _it;
            switch (_it.iType) {
            case __VT_void:
                return false;
            case __VT_string:
                return val->IsString();
            case __VT_bool:
                return val->IsBoolean();
            case __VT_int:
            case __VT_long:
                return (val->IsInt32() || val->IsUint32());
            case __VT_float:
            case __VT_double:
            case __VT_longlong:
                return val->IsNumber();
            case __VT_ArrayBuffer:
                return val->IsArrayBuffer();
            default: {
                if (!val->IsObject())
                    return false;
                int clsid = *(int*)val.As<Object>()->GetAlignedPointerFromInternalField(1);
                return clsid == (int)&_Tp::JSCLSINFO;
            }
            }
        }

        template <typename _Tp>
        bool IsTypeof() {
            JsValue val = m_pObj->getRefObj(m_nID);
            return IsTypeof<_Tp>(val);
        }

        template<typename _Tp>
        bool Get(_Tp* r) {
            JsValue v;
        }

        //如果类型一致，则能转换，进行转换，否则不转换，返回false。这个主要是针对常用方法进行优化，防止调用两次。
        //这个只能处理对象。基本类型用后面的函数
        template <typename _R>
        static bool tryGet(JsValue& val, _R **pRet) {
            Isolate* pIso = Isolate::GetCurrent();
            HandleScope handle_scope(pIso);
            if (val.IsEmpty()) {
                *pRet = 0;
                return false;
            }
            if (!__TransferToCpp<_R>::is(val))
                return false;
            *pRet = __TransferToCpp<_R*>::ToCpp(val);
            return true;
        }

        static bool tryGetStr(JsValue& val, char** ppRet) {
            if (val->IsString()) {
                *ppRet = JsCharToC(val);// __TransferToCpp<char*>::ToCpp(val);
                return true;
            }
            return false;
        }

        static bool tryGetStrVector(JsValue& val, std::vector<std::string>& pRet) {
            if (val->IsArray()) {
                pRet.clear();
                v8::Local<v8::Array> pJSArray = v8::Local<v8::Array>::Cast(val);
                int len = pJSArray->Length();
                for (int i = 0; i < len; ++i) {
                    v8::Local<v8::Value> v = pJSArray->Get(i);
                    const char* _string = JsCharToC(v);
                    pRet.push_back(_string);
                }
                return true;
            }
            return false;
        }

        void Reset() {
            //这个好多是在js对象析构的时候调用的，这时候，新的v8会非法（js对象已经删了，下面却还要用），所以干脆把这个去掉了，对v8来说
            //在js对象析构的时候再做这个本身也没意义。
            //缺点是：如果js对象还在的时候要用这个。。。
            /*
            if (m_pObj) {
                m_pObj->setRefObj(m_nID, Null(m_pObj->mpJsIso));
                m_pObj = NULL;
            }
            */
        }

        void __BindThis(JsObjHandle2 &p_This) {}
        bool IsFunction() {
            return m_pObj->getRefObj(m_nID)->IsFunction();
        }

        bool isValid() {
            return m_pObj && m_pObj->mpJsIso == Isolate::GetCurrent();
        }

        bool Call() {
            if (!m_pObj)return false;
            EscapableHandleScope sc(m_pObj->mpJsIso);
            JsValue func = m_pObj->getRefObj(m_nID);
            if (!func->IsFunction())
                return false;
            Local<Function> jsfun = Local<Function>::Cast(func);
            m_pReturn = sc.Escape(m_pObj->callJsFunc(jsfun));
            return true;
        }
        template <typename P1>
        bool Call(P1 p1) {
            if (!m_pObj)return false;
            EscapableHandleScope sc(m_pObj->mpJsIso);
            JsValue func = m_pObj->getRefObj(m_nID);
            if (!func->IsFunction())
                return false;
            Local<Function> jsfun = Local<Function>::Cast(func);
            m_pReturn = sc.Escape(m_pObj->callJsFunc(jsfun, p1));
            return true;
        }

#define CALLJSPRE \
	if (!m_pObj)return false;\
	EscapableHandleScope sc(m_pObj->mpJsIso);\
	JsValue func = m_pObj->getRefObj(m_nID);\
	if (!func->IsFunction())\
		return false;\
	Local<Function> jsfun = Local<Function>::Cast(func);

        template <typename P1, typename P2>
        bool Call(P1 p1, P2 p2) {
            CALLJSPRE
                m_pReturn = sc.Escape(m_pObj->callJsFunc(jsfun, p1, p2));
            return true;
        }
        template <typename P1, typename P2, typename P3>
        bool Call(P1 p1, P2 p2, P3 p3) {
            CALLJSPRE
                m_pReturn = sc.Escape(m_pObj->callJsFunc(jsfun, p1, p2, p3));
            return true;
        }
        template <typename P1, typename P2, typename P3, typename P4>
        bool Call(P1 p1, P2 p2, P3 p3, P4 p4) {
            CALLJSPRE
                m_pReturn = sc.Escape(m_pObj->callJsFunc(jsfun, p1, p2, p3, p4));
            return true;
        }
        template <typename P1, typename P2, typename P3, typename P4, typename P5>
        bool Call(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5) {
            CALLJSPRE
                m_pReturn = sc.Escape(m_pObj->callJsFunc(jsfun, p1, p2, p3, p4, p5));
            return true;
        }
        template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
        bool Call(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6) {
            CALLJSPRE
                m_pReturn = sc.Escape(m_pObj->callJsFunc(jsfun, p1, p2, p3, p4, p5, p6));
            return true;
        }
        template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
        bool Call(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7) {
            CALLJSPRE
                m_pReturn = sc.Escape(m_pObj->callJsFunc(jsfun, p1, p2, p3, p4, p5, p6, p7));
            return true;
        }
        template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
        bool Call(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8) {
            CALLJSPRE
                m_pReturn = sc.Escape(m_pObj->callJsFunc(jsfun, p1, p2, p3, p4, p5, p6, p7, p8));
            return true;
        }
        template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
        bool Call(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9) {
            CALLJSPRE
                m_pReturn = sc.Escape(m_pObj->callJsFunc(jsfun, p1, p2, p3, p4, p5, p6, p7, p8, p9));
            return true;
        }
    };
    //typedef JSObjBaseV8 JsObjHandle;

    //为了与原来的差不多的宏。 只能在对象的实例中使用。必须按照下面的顺序调用。
    //第一个name是类名，统计用的
#define JSP_CLASS(name,cls) \
	v8::Isolate* pJsIso = v8::Isolate::GetCurrent(); \
	v8::HandleScope sc(pJsIso);	\
	v8::Local<v8::FunctionTemplate> xhrtemp = v8::FunctionTemplate::New(pJsIso, JSCClass<cls>::JsConstructor);	\
	xhrtemp->SetClassName(Js_Str(pJsIso, name));	\
	v8::Local<v8::ObjectTemplate> instancTemp = xhrtemp->InstanceTemplate();	\
	instancTemp->SetInternalFieldCount(2); \
	cls::JSCLSINFO.objTemplate = new Persistent<ObjectTemplate>(pJsIso, instancTemp);	\
	v8::Local<v8::Context> pCtx = pJsIso->GetCurrentContext(); 

//这个的区别是不能直接创建c对象，而是绑定一个已经存在的对象。
#define JSP_GLOBAL_CLASS(name,cls) \
	v8::Isolate* pJsIso = Isolate::GetCurrent(); /*全局的每次都重新创建*/\
	v8::HandleScope sc(pJsIso);	\
	v8::Local<v8::FunctionTemplate> xhrtemp = v8::FunctionTemplate::New(pJsIso);	\
	xhrtemp->SetClassName(Js_Str(pJsIso, name));	\
	v8::Local<v8::ObjectTemplate> instancTemp = xhrtemp->InstanceTemplate();	\
	instancTemp->SetInternalFieldCount(2); \
	cls::JSCLSINFO.objTemplate = new Persistent<ObjectTemplate>(pJsIso, instancTemp);	\
	v8::Local<v8::Context> pCtx = pJsIso->GetCurrentContext();		

#define JSP_REG_CONSTRUCTOR(cls,...) \
    JSCClass<cls>::getInstance()->addConstructor(regConstructor<cls,##__VA_ARGS__>());

#define JSP_ADD_FIXED_PROPERTY(name,cls,val) \
    instancTemp->Set(Js_Str(pJsIso, #name), __TransferToJs<decltype(val)>::ToJs(val));

#define JSP_ADD_METHOD(name,fn)    \
	addJSMethod(name,xhrtemp, &fn);

#define JSP_ADD_METHODRAW(name, func, data) \
	instancTemp->Set(Js_Str(pJsIso, name), createJSMethodRaw(func,data));


#define JSP_INSTALL_CLASS(name,cls)    \
	pCtx->Global()->Set(Js_Str(pJsIso, name), xhrtemp->GetFunction()); \
    JSClassMgr::GetThreadInstance()->allCls.push_back(JSCClass<cls>::reset);

#define JSP_INSTALL_G_CLASS(name,cls,inst)	\
	Local<Object> jsinst = instancTemp->NewInstance();\
	jsinst->SetAlignedPointerInInternalField(0, inst);	\
	jsinst->SetAlignedPointerInInternalField(1, &cls::JSCLSINFO);	\
	Persistent<Object>* pPersist = new Persistent<Object>(pJsIso, jsinst);\
	inst->mpJsThis = pPersist;\
	pPersist->SetWeak(pPersist, JsDestructor1<cls>, v8::WeakCallbackType::kInternalFields);\
	createRefArray();\
	pCtx->Global()->Set(Js_Str(pJsIso, name), jsinst);	

//注意，下面的inst是返回的。
#define JSP_INSTALL_GLOBAL_CLASS(name,cls,inst)	\
	pCtx->Global()->Set(Js_Str(pJsIso, name), createJsObjAttachCObj<cls>(inst,false));

//添加一个只读属性。get是函数地址，funcplace是一个存放get和set函数地址的结构的指针
#define JSP_ADD_PROPERTY_RO(name,cls,get)	\
	static PropFuncInfo<decltype(&cls::get),decltype(&cls::get)> propFunc_R##cls##name(&cls::get);	\
	instancTemp->SetAccessor(Js_Str(pJsIso,#name), \
		imp_JsGetProp<decltype(&cls::get),decltype(&cls::get)>::call,0,	\
		External::New(pJsIso, (void*)&propFunc_R##cls##name),v8::DEFAULT, v8::ReadOnly);

#define JSP_ADD_PROPERTY(name,cls,get,set)	\
	static PropFuncInfo<decltype(&cls::get),decltype(&cls::set)> propFunc_##cls##name(&cls::get, &cls::set);	\
	instancTemp->SetAccessor(Js_Str(pJsIso,#name),		\
			imp_JsGetProp<decltype(&cls::get),decltype(&cls::set)>::call,imp_JsSetProp<decltype(&cls::get),decltype(&cls::set)>::call,	\
			External::New(pJsIso, (void*)&propFunc_##cls##name));

#define JSP_GLOBAL_START1()	\
	v8::Isolate* pIso = v8::Isolate::GetCurrent();	\
	v8::HandleScope sc(pIso);	\
	v8::Local<v8::Context> pCtx = pIso->GetCurrentContext(); \
	v8::Local<v8::Object> gctx = pCtx->Global();

//func必须是全局函数，不允许成员函数。为了简单，还是与原来的兼容把
#define JSP_ADD_GLOBAL_FUNCTION(name, func, ...) \
	gctx->Set(Js_Str(pIso, #name), createJSMethod(func));

#define JSP_ADD_GLOBAL_PROPERTY(name, v)	\
	gctx->Set(Js_Str(pIso, #name), ToJSValue(v));

//检查参数是否>=num个
    bool checkJSToCArgs(const FunctionCallbackInfo<Value>& args, int num);

    /////////////////////////////////////////////////////////

    //回调的偏特化
    //没有参数
    template<typename Cls, typename R>
    struct imp_JS2CFunc<R(Cls::*)(void)> {
        typedef R(Cls::*funcType)(void);
        static void call(const FunctionCallbackInfo<Value>& args) {
            void* pExData = External::Cast(*args.Data())->Value();
            //FuncInfo<funcType>* funcInfo = (FuncInfo<funcType>*)(&pExData);
            FuncInfo<funcType>* funcInfo = (FuncInfo<funcType>*)pExData;
            Local<Object> pthis = args.This();
            JsObjClassInfo* id = (JsObjClassInfo*)pthis->GetAlignedPointerFromInternalField(1);
            if (!IsSubClass<Cls>(id)) { LOGE("throw !IsSubClass 878 %d", id->ClsID); throw - 1; }
            //if (id != Cls::JSCLSINFO.ClsID)	throw - 1;
            Cls *pObj = (Cls*)pthis->GetAlignedPointerFromInternalField(0);
            pObj->setJSArgs(&args);
            args.GetReturnValue().Set(ToJSValue<R>((pObj->*funcInfo->func)()));
        }
    };

#define JS2CCALL_INIT \
	void* pExData = External::Cast(*args.Data())->Value();\
	FuncInfo<funcType>* funcInfo = (FuncInfo<funcType>*)pExData;\
	Local<Object> pthis = args.This();

#define JS2CCALL_GET_COBJ \
	JsObjClassInfo* id = (JsObjClassInfo*)pthis->GetAlignedPointerFromInternalField(1);\
	if (!IsSubClass<Cls>(id)){LOGE("throw isSubClass %d",id->ClsID); throw - 1;}\
	Cls *pObj = (Cls*)pthis->GetAlignedPointerFromInternalField(0);\
	pObj->setJSArgs(&args);

#define JS2CCALL_GET_PARAMS1 \
	if(!checkJSToCArgs(args,1))return;\
	P1 p1 = __TransferToCpp<P1>::ToCpp(args[0]);

#define JS2CCALL_GET_PARAMS2 \
	if(!checkJSToCArgs(args,2))return;\
	P1 p1 = __TransferToCpp<P1>::ToCpp(args[0]);\
	P2 p2 = __TransferToCpp<P2>::ToCpp(args[1]);

#define JS2CCALL_GET_PARAMS3 \
	if(!checkJSToCArgs(args,3))return;\
	P1 p1 = __TransferToCpp<P1>::ToCpp(args[0]);\
	P2 p2 = __TransferToCpp<P2>::ToCpp(args[1]);\
	P3 p3 = __TransferToCpp<P3>::ToCpp(args[2]);

#define JS2CCALL_GET_PARAMS4 \
	if(!checkJSToCArgs(args,4))return;\
	P1 p1 = __TransferToCpp<P1>::ToCpp(args[0]);\
	P2 p2 = __TransferToCpp<P2>::ToCpp(args[1]);\
	P3 p3 = __TransferToCpp<P3>::ToCpp(args[2]);\
	P4 p4 = __TransferToCpp<P4>::ToCpp(args[3]);

#define JS2CCALL_GET_PARAMS5 \
	if(!checkJSToCArgs(args,5))return;\
	P1 p1 = __TransferToCpp<P1>::ToCpp(args[0]);\
	P2 p2 = __TransferToCpp<P2>::ToCpp(args[1]);\
	P3 p3 = __TransferToCpp<P3>::ToCpp(args[2]);\
	P4 p4 = __TransferToCpp<P4>::ToCpp(args[3]);\
	P5 p5 = __TransferToCpp<P5>::ToCpp(args[4]);

#define JS2CCALL_GET_PARAMS6 \
	if(!checkJSToCArgs(args,6))return;\
	P1 p1 = __TransferToCpp<P1>::ToCpp(args[0]);\
	P2 p2 = __TransferToCpp<P2>::ToCpp(args[1]);\
	P3 p3 = __TransferToCpp<P3>::ToCpp(args[2]);\
	P4 p4 = __TransferToCpp<P4>::ToCpp(args[3]);\
	P5 p5 = __TransferToCpp<P5>::ToCpp(args[4]);\
	P6 p6 = __TransferToCpp<P6>::ToCpp(args[5]);

#define JS2CCALL_GET_PARAMS7 \
	if(!checkJSToCArgs(args,7))return;\
	P1 p1 = __TransferToCpp<P1>::ToCpp(args[0]);\
	P2 p2 = __TransferToCpp<P2>::ToCpp(args[1]);\
	P3 p3 = __TransferToCpp<P3>::ToCpp(args[2]);\
	P4 p4 = __TransferToCpp<P4>::ToCpp(args[3]);\
	P5 p5 = __TransferToCpp<P5>::ToCpp(args[4]);\
	P6 p6 = __TransferToCpp<P6>::ToCpp(args[5]);\
	P7 p7 = __TransferToCpp<P7>::ToCpp(args[6]);

#define JS2CCALL_GET_PARAMS8 \
	if(!checkJSToCArgs(args,8))return;\
	P1 p1 = __TransferToCpp<P1>::ToCpp(args[0]);\
	P2 p2 = __TransferToCpp<P2>::ToCpp(args[1]);\
	P3 p3 = __TransferToCpp<P3>::ToCpp(args[2]);\
	P4 p4 = __TransferToCpp<P4>::ToCpp(args[3]);\
	P5 p5 = __TransferToCpp<P5>::ToCpp(args[4]);\
	P6 p6 = __TransferToCpp<P6>::ToCpp(args[5]);\
	P7 p7 = __TransferToCpp<P7>::ToCpp(args[6]);\
	P8 p8 = __TransferToCpp<P8>::ToCpp(args[7]);

#define JS2CCALL_GET_PARAMS9 \
	if(!checkJSToCArgs(args,9))return;\
	P1 p1 = __TransferToCpp<P1>::ToCpp(args[0]);\
	P2 p2 = __TransferToCpp<P2>::ToCpp(args[1]);\
	P3 p3 = __TransferToCpp<P3>::ToCpp(args[2]);\
	P4 p4 = __TransferToCpp<P4>::ToCpp(args[3]);\
	P5 p5 = __TransferToCpp<P5>::ToCpp(args[4]);\
	P6 p6 = __TransferToCpp<P6>::ToCpp(args[5]);\
	P7 p7 = __TransferToCpp<P7>::ToCpp(args[6]);\
	P8 p8 = __TransferToCpp<P8>::ToCpp(args[7]);\
	P9 p9 = __TransferToCpp<P9>::ToCpp(args[8]);

#define JS2CCALL_GET_PARAMS10 \
	if(!checkJSToCArgs(args,10))return;\
	P1 p1 = __TransferToCpp<P1>::ToCpp(args[0]);\
	P2 p2 = __TransferToCpp<P2>::ToCpp(args[1]);\
	P3 p3 = __TransferToCpp<P3>::ToCpp(args[2]);\
	P4 p4 = __TransferToCpp<P4>::ToCpp(args[3]);\
	P5 p5 = __TransferToCpp<P5>::ToCpp(args[4]);\
	P6 p6 = __TransferToCpp<P6>::ToCpp(args[5]);\
	P7 p7 = __TransferToCpp<P7>::ToCpp(args[6]);\
	P8 p8 = __TransferToCpp<P8>::ToCpp(args[7]);\
	P9 p9 = __TransferToCpp<P9>::ToCpp(args[8]);\
	P10 p10 = __TransferToCpp<P9>::ToCpp(args[9]);

#define JS2CCALL_GET_PARAMS11 \
	if(!checkJSToCArgs(args,11))return;\
	P1 p1 = __TransferToCpp<P1>::ToCpp(args[0]);\
	P2 p2 = __TransferToCpp<P2>::ToCpp(args[1]);\
	P3 p3 = __TransferToCpp<P3>::ToCpp(args[2]);\
	P4 p4 = __TransferToCpp<P4>::ToCpp(args[3]);\
	P5 p5 = __TransferToCpp<P5>::ToCpp(args[4]);\
	P6 p6 = __TransferToCpp<P6>::ToCpp(args[5]);\
	P7 p7 = __TransferToCpp<P7>::ToCpp(args[6]);\
	P8 p8 = __TransferToCpp<P8>::ToCpp(args[7]);\
	P9 p9 = __TransferToCpp<P9>::ToCpp(args[8]);\
	P10 p10 = __TransferToCpp<P10>::ToCpp(args[9]);\
	P11 p11 = __TransferToCpp<P11>::ToCpp(args[10]);

#define JS2CCALL_GET_PARAMS12 \
	if(!checkJSToCArgs(args,12))return;\
	P1 p1 = __TransferToCpp<P1>::ToCpp(args[0]);\
	P2 p2 = __TransferToCpp<P2>::ToCpp(args[1]);\
	P3 p3 = __TransferToCpp<P3>::ToCpp(args[2]);\
	P4 p4 = __TransferToCpp<P4>::ToCpp(args[3]);\
	P5 p5 = __TransferToCpp<P5>::ToCpp(args[4]);\
	P6 p6 = __TransferToCpp<P6>::ToCpp(args[5]);\
	P7 p7 = __TransferToCpp<P7>::ToCpp(args[6]);\
	P8 p8 = __TransferToCpp<P8>::ToCpp(args[7]);\
	P9 p9 = __TransferToCpp<P9>::ToCpp(args[8]);\
	P10 p10 = __TransferToCpp<P10>::ToCpp(args[9]);\
	P11 p11 = __TransferToCpp<P11>::ToCpp(args[10]);\
    P12 p12 = __TransferToCpp<P12>::ToCpp(args[11]);

    //一个参数
    template<typename Cls, typename R, typename P1 >
    struct imp_JS2CFunc<R(Cls::*)(P1)> {
        typedef R(Cls::*funcType)(P1);
        static void call(const FunctionCallbackInfo<Value>& args) {
            JS2CCALL_INIT;
            JS2CCALL_GET_COBJ;
            JS2CCALL_GET_PARAMS1;
            args.GetReturnValue().Set(ToJSValue<R>((pObj->*funcInfo->func)(p1)));
            resetJsStrBuf();
        }
    };

    //2
    template<typename Cls, typename R,  typename P1, typename P2 >
    struct imp_JS2CFunc<R(Cls::*)(P1, P2)> {
        typedef R(Cls::*funcType)(P1, P2);
        static void call(const FunctionCallbackInfo<Value>& args) {
            JS2CCALL_INIT;
            JS2CCALL_GET_COBJ;
            JS2CCALL_GET_PARAMS2;
            args.GetReturnValue().Set(ToJSValue<R>((pObj->*funcInfo->func)(p1, p2)));
            resetJsStrBuf();
        }
    };

    //3
    template<typename Cls, typename R,  typename P1, typename P2, typename P3 >
    struct imp_JS2CFunc<R(Cls::*)(P1, P2, P3)> {
        typedef R(Cls::*funcType)(P1, P2, P3);
        static void call(const FunctionCallbackInfo<Value>& args) {
            JS2CCALL_INIT;
            JS2CCALL_GET_COBJ;
            JS2CCALL_GET_PARAMS3;
            args.GetReturnValue().Set(ToJSValue<R>((pObj->*funcInfo->func)(p1, p2, p3)));
            resetJsStrBuf();
        }
    };

    //4
    template<typename Cls, typename R,  typename P1, typename P2, typename P3, typename P4 >
    struct imp_JS2CFunc<R(Cls::*)(P1, P2, P3, P4)> {
        typedef R(Cls::*funcType)(P1, P2, P3, P4);
        static void call(const FunctionCallbackInfo<Value>& args) {
            JS2CCALL_INIT;
            JS2CCALL_GET_COBJ;
            JS2CCALL_GET_PARAMS4;
            args.GetReturnValue().Set(ToJSValue<R>((pObj->*funcInfo->func)(p1, p2, p3, p4)));
            resetJsStrBuf();
        }
    };
    //5
    template<typename Cls, typename R,  typename P1, typename P2, typename P3, typename P4, typename P5>
    struct imp_JS2CFunc<R(Cls::*)(P1, P2, P3, P4, P5)> {
        typedef R(Cls::*funcType)(P1, P2, P3, P4, P5);
        static void call(const FunctionCallbackInfo<Value>& args) {
            JS2CCALL_INIT;
            JS2CCALL_GET_COBJ;
            JS2CCALL_GET_PARAMS5;
            args.GetReturnValue().Set(ToJSValue<R>((pObj->*funcInfo->func)(p1, p2, p3, p4, p5)));
            resetJsStrBuf();
        }
    };
    //6
    template<typename Cls, typename R,  typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
    struct imp_JS2CFunc<R(Cls::*)(P1, P2, P3, P4, P5, P6)> {
        typedef R(Cls::*funcType)(P1, P2, P3, P4, P5, P6);
        static void call(const FunctionCallbackInfo<Value>& args) {
            JS2CCALL_INIT;
            JS2CCALL_GET_COBJ;
            JS2CCALL_GET_PARAMS6;
            args.GetReturnValue().Set(ToJSValue<R>((pObj->*funcInfo->func)(p1, p2, p3, p4, p5, p6)));
            resetJsStrBuf();
        }
    };
    //7
    template<typename Cls, typename R,  typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
    struct imp_JS2CFunc<R(Cls::*)(P1, P2, P3, P4, P5, P6, P7)> {
        typedef R(Cls::*funcType)(P1, P2, P3, P4, P5, P6, P7);
        static void call(const FunctionCallbackInfo<Value>& args) {
            JS2CCALL_INIT;
            JS2CCALL_GET_COBJ;
            JS2CCALL_GET_PARAMS7;
            args.GetReturnValue().Set(ToJSValue<R>((pObj->*funcInfo->func)(p1, p2, p3, p4, p5, p6, p7)));
            resetJsStrBuf();
        }
    };
    template<typename Cls, typename R,  typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
    struct imp_JS2CFunc<R(Cls::*)(P1, P2, P3, P4, P5, P6, P7, P8)> {
        typedef R(Cls::*funcType)(P1, P2, P3, P4, P5, P6, P7, P8);
        static void call(const FunctionCallbackInfo<Value>& args) {
            JS2CCALL_INIT;
            JS2CCALL_GET_COBJ;
            JS2CCALL_GET_PARAMS8;
            args.GetReturnValue().Set(ToJSValue<R>((pObj->*funcInfo->func)(p1, p2, p3, p4, p5, p6, p7, p8)));
            resetJsStrBuf();
        }
    };

    template<typename Cls, typename R,  typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
    struct imp_JS2CFunc<R(Cls::*)(P1, P2, P3, P4, P5, P6, P7, P8, P9)> {
        typedef R(Cls::*funcType)(P1, P2, P3, P4, P5, P6, P7, P8, P9);
        static void call(const FunctionCallbackInfo<Value>& args) {
            JS2CCALL_INIT;
            JS2CCALL_GET_COBJ;
            JS2CCALL_GET_PARAMS9;
            args.GetReturnValue().Set(ToJSValue<R>((pObj->*funcInfo->func)(p1, p2, p3, p4, p5, p6, p7, p8, p9)));
            resetJsStrBuf();
        }
    };
    template<typename Cls, typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10, typename P11, typename P12>
    struct imp_JS2CFunc<R(Cls::*)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12)> {
        typedef R(Cls::*funcType)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12);
        static void call(const FunctionCallbackInfo<Value>& args) {
            JS2CCALL_INIT;
            JS2CCALL_GET_COBJ;
            JS2CCALL_GET_PARAMS12;
            args.GetReturnValue().Set(ToJSValue<R>((pObj->*funcInfo->func)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12)));
            resetJsStrBuf();
        }
    };

    //下面是void的，没想到好办法
    //没有参数
    template<typename Cls>
    struct imp_JS2CFunc<void(Cls::*)(void)> {
        typedef void(Cls::*funcType)(void);
        static void call(const FunctionCallbackInfo<Value>& args) {
            JS2CCALL_INIT;
            JS2CCALL_GET_COBJ;
            (pObj->*funcInfo->func)();
        }
    };

    //一个参数
    template< typename Cls, typename P1 >
    struct imp_JS2CFunc<void(Cls::*)(P1)> {
        typedef void(Cls::*funcType)(P1);
        static void call(const FunctionCallbackInfo<Value>& args) {
            JS2CCALL_INIT;
            JS2CCALL_GET_COBJ;
            JS2CCALL_GET_PARAMS1;
            (pObj->*(funcInfo->func))(p1);
            resetJsStrBuf();
        }
    };

    //2
    template<typename Cls, typename P1, typename P2 >
    struct imp_JS2CFunc<void(Cls::*)(P1, P2)> {
        typedef void(Cls::*funcType)(P1, P2);
        static void call(const FunctionCallbackInfo<Value>& args) {
            JS2CCALL_INIT;
            JS2CCALL_GET_COBJ;
            JS2CCALL_GET_PARAMS2;
            (pObj->*funcInfo->func)(p1, p2);
            resetJsStrBuf();
        }
    };

    //3
    template<typename Cls, typename P1, typename P2, typename P3 >
    struct imp_JS2CFunc<void(Cls::*)(P1, P2, P3)> {
        typedef void(Cls::*funcType)(P1, P2, P3);
        static void call(const FunctionCallbackInfo<Value>& args) {
            JS2CCALL_INIT;
            JS2CCALL_GET_COBJ;
            JS2CCALL_GET_PARAMS3;
            (pObj->*funcInfo->func)(p1, p2, p3);
            resetJsStrBuf();
        }
    };
    //4
    template< typename Cls, typename P1, typename P2, typename P3, typename P4 >
    struct imp_JS2CFunc<void(Cls::*)(P1, P2, P3, P4)> {
        typedef void(Cls::*funcType)(P1, P2, P3, P4);
        static void call(const FunctionCallbackInfo<Value>& args) {
            JS2CCALL_INIT;
            JS2CCALL_GET_COBJ;
            JS2CCALL_GET_PARAMS4;
            (pObj->*funcInfo->func)(p1, p2, p3, p4);
            resetJsStrBuf();
        }
    };
    //5
    template< typename Cls, typename P1, typename P2, typename P3, typename P4, typename P5 >
    struct imp_JS2CFunc<void(Cls::*)(P1, P2, P3, P4, P5)> {
        typedef void(Cls::*funcType)(P1, P2, P3, P4, P5);
        static void call(const FunctionCallbackInfo<Value>& args) {
            JS2CCALL_INIT;
            JS2CCALL_GET_COBJ;
            JS2CCALL_GET_PARAMS5;
            (pObj->*funcInfo->func)(p1, p2, p3, p4, p5);
            resetJsStrBuf();
        }
    };
    //6
    template< typename Cls, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6 >
    struct imp_JS2CFunc<void(Cls::*)(P1, P2, P3, P4, P5, P6)> {
        typedef void(Cls::*funcType)(P1, P2, P3, P4, P5, P6);
        static void call(const FunctionCallbackInfo<Value>& args) {
            JS2CCALL_INIT;
            JS2CCALL_GET_COBJ;
            JS2CCALL_GET_PARAMS6;
            (pObj->*funcInfo->func)(p1, p2, p3, p4, p5, p6);
            resetJsStrBuf();
        }
    };
    //7
    template< typename Cls, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7 >
    struct imp_JS2CFunc<void(Cls::*)(P1, P2, P3, P4, P5, P6, P7)> {
        typedef void(Cls::*funcType)(P1, P2, P3, P4, P5, P6, P7);
        static void call(const FunctionCallbackInfo<Value>& args) {
            JS2CCALL_INIT;
            JS2CCALL_GET_COBJ;
            JS2CCALL_GET_PARAMS7;
            (pObj->*funcInfo->func)(p1, p2, p3, p4, p5, p6, p7);
            resetJsStrBuf();
        }
    };
    //9
    template<typename Cls, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
    struct imp_JS2CFunc<void(Cls::*)(P1, P2, P3, P4, P5, P6, P7, P8, P9)> {
        typedef void(Cls::*funcType)(P1, P2, P3, P4, P5, P6, P7, P8, P9);
        static void call(const FunctionCallbackInfo<Value>& args) {
            JS2CCALL_INIT;
            JS2CCALL_GET_COBJ;
            JS2CCALL_GET_PARAMS9;
            (pObj->*funcInfo->func)(p1, p2, p3, p4, p5, p6, p7, p8, p9);
            resetJsStrBuf();
        }
    };
    template<typename Cls, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10>
    struct imp_JS2CFunc<void(Cls::*)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10)> {
        typedef void(Cls::*funcType)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10);
        static void call(const FunctionCallbackInfo<Value>& args) {
            JS2CCALL_INIT;
            JS2CCALL_GET_COBJ;
            JS2CCALL_GET_PARAMS10;
            (pObj->*funcInfo->func)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
            resetJsStrBuf();
        }
    };
    //11
    template< typename Cls, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10, typename P11 >
    struct imp_JS2CFunc<void(Cls::*)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11)> {
        typedef void(Cls::*funcType)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11);
        static void call(const FunctionCallbackInfo<Value>& args) {
            JS2CCALL_INIT;
            JS2CCALL_GET_COBJ;
            JS2CCALL_GET_PARAMS11;
            (pObj->*funcInfo->func)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);
            resetJsStrBuf();
        }
    };
    template<typename Cls, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10, typename P11, typename P12>
    struct imp_JS2CFunc<void(Cls::*)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12)> {
        typedef void(Cls::*funcType)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12);
        static void call(const FunctionCallbackInfo<Value>& args) {
            JS2CCALL_INIT;
            JS2CCALL_GET_COBJ;
            JS2CCALL_GET_PARAMS12;
            (pObj->*funcInfo->func)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);
            resetJsStrBuf();
        }
    };
    //非类函数
    template<> //void(void)
    struct imp_JS2CFunc<void(*)(void)> {
        typedef void(*funcType)(void);
        static void call(const FunctionCallbackInfo<Value>& args) {
            JS2CCALL_INIT;
            funcInfo->func();
        }
    };
    template<typename R>//R(void)
    struct imp_JS2CFunc<R(*)(void)> {
        typedef R(*funcType)(void);
        static void call(const FunctionCallbackInfo<Value>& args) {
            JS2CCALL_INIT;
            args.GetReturnValue().Set(ToJSValue<R>(funcInfo->func()));
        }
    };
    template<typename R, typename P1>//R(p1)
    struct imp_JS2CFunc<R(*)(P1)> {
        typedef R(*funcType)(P1);
        static void call(const FunctionCallbackInfo<Value>& args) {
            JS2CCALL_INIT;
            JS2CCALL_GET_PARAMS1;
            args.GetReturnValue().Set(ToJSValue<R>(funcInfo->func(p1)));
            resetJsStrBuf();
        }
    };
    template<typename R, typename P1>//const R(p1)
    struct imp_JS2CFunc<const R(*)(P1)> {
        typedef const R(*funcType)(P1);
        static void call(const FunctionCallbackInfo<Value>& args) {
            JS2CCALL_INIT;
            JS2CCALL_GET_PARAMS1;
            args.GetReturnValue().Set(ToJSValue<R>(funcInfo->func(p1)));
            resetJsStrBuf();
        }
    };

    template<typename P1>
    struct imp_JS2CFunc<void(*)(P1)> {//void(p1)
        typedef void(*funcType)(P1);
        static void call(const FunctionCallbackInfo<Value>& args) {
            JS2CCALL_INIT;
            JS2CCALL_GET_PARAMS1;
            funcInfo->func(p1);
            resetJsStrBuf();
        }
    };

    template<typename R, typename P1, typename P2>//R(p1,p2)
    struct imp_JS2CFunc<R(*)(P1, P2)> {
        typedef R(*funcType)(P1, P2);
        static void call(const FunctionCallbackInfo<Value>& args) {
            JS2CCALL_INIT;
            JS2CCALL_GET_PARAMS2;
            args.GetReturnValue().Set(ToJSValue<R>(funcInfo->func(p1, p2)));
            resetJsStrBuf();
        }
    };

    template<typename P1, typename P2>//void(p1,p2)
    struct imp_JS2CFunc<void(*)(P1, P2)> {
        typedef void(*funcType)(P1, P2);
        static void call(const FunctionCallbackInfo<Value>& args) {
            JS2CCALL_INIT;
            JS2CCALL_GET_PARAMS2;
            funcInfo->func(p1, p2);
            resetJsStrBuf();
        }
    };

    template<typename R, typename P1, typename P2, typename P3>//3
    struct imp_JS2CFunc<R(*)(P1, P2, P3)> {
        typedef R(*funcType)(P1, P2, P3);
        static void call(const FunctionCallbackInfo<Value>& args) {
            JS2CCALL_INIT;
            JS2CCALL_GET_PARAMS3;
            args.GetReturnValue().Set(ToJSValue<R>(funcInfo->func(p1, p2, p3)));
            resetJsStrBuf();
        }
    };
    template<typename R, typename P1, typename P2, typename P3, typename P4>//4
    struct imp_JS2CFunc<R(*)(P1, P2, P3, P4)> {
        typedef R(*funcType)(P1, P2, P3, P4);
        static void call(const FunctionCallbackInfo<Value>& args) {
            JS2CCALL_INIT;
            JS2CCALL_GET_PARAMS4;
            args.GetReturnValue().Set(ToJSValue<R>(funcInfo->func(p1, p2, p3, p4)));
            resetJsStrBuf();
        }
    };
    template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5>//5
    struct imp_JS2CFunc<R(*)(P1, P2, P3, P4, P5)> {
        typedef R(*funcType)(P1, P2, P3, P4, P5);
        static void call(const FunctionCallbackInfo<Value>& args) {
            JS2CCALL_INIT;
            JS2CCALL_GET_PARAMS5;
            args.GetReturnValue().Set(ToJSValue<R>(funcInfo->func(p1, p2, p3, p4, p5)));
            resetJsStrBuf();
        }
    };
    template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>//6
    struct imp_JS2CFunc<R(*)(P1, P2, P3, P4, P5, P6)> {
        typedef R(*funcType)(P1, P2, P3, P4, P5, P6);
        static void call(const FunctionCallbackInfo<Value>& args) {
            JS2CCALL_INIT;
            JS2CCALL_GET_PARAMS6;
            args.GetReturnValue().Set(ToJSValue<R>(funcInfo->func(p1, p2, p3, p4, p5, p6)));
            resetJsStrBuf();
        }
    };
    template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>//7
    struct imp_JS2CFunc<R(*)(P1, P2, P3, P4, P5, P6, P7)> {
        typedef R(*funcType)(P1, P2, P3, P4, P5, P6, P7);
        static void call(const FunctionCallbackInfo<Value>& args) {
            JS2CCALL_INIT;
            JS2CCALL_GET_PARAMS7;
            args.GetReturnValue().Set(ToJSValue<R>(funcInfo->func(p1, p2, p3, p4, p5, p6, p7)));
            resetJsStrBuf();
        }
    };
    template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>//8
    struct imp_JS2CFunc<R(*)(P1, P2, P3, P4, P5, P6, P7, P8)> {
        typedef R(*funcType)(P1, P2, P3, P4, P5, P6, P7, P8);
        static void call(const FunctionCallbackInfo<Value>& args) {
            JS2CCALL_INIT;
            JS2CCALL_GET_PARAMS8;
            args.GetReturnValue().Set(ToJSValue<R>(funcInfo->func(p1, p2, p3, p4, p5, p6, p7, p8)));
            resetJsStrBuf();
        }
    };
    template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>//9
    struct imp_JS2CFunc<R(*)(P1, P2, P3, P4, P5, P6, P7, P8, P9)> {
        typedef R(*funcType)(P1, P2, P3, P4, P5, P6, P7, P8, P9);
        static void call(const FunctionCallbackInfo<Value>& args) {
            JS2CCALL_INIT;
            JS2CCALL_GET_PARAMS9;
            args.GetReturnValue().Set(ToJSValue<R>(funcInfo->func(p1, p2, p3, p4, p5, p6, p7, p8, p9)));
            resetJsStrBuf();
        }
    };

    //void
    template< typename P1, typename P2, typename P3>//3
    struct imp_JS2CFunc<void(*)(P1, P2, P3)> {
        typedef void(*funcType)(P1, P2, P3);
        static void call(const FunctionCallbackInfo<Value>& args) {
            JS2CCALL_INIT;
            JS2CCALL_GET_PARAMS3;
            funcInfo->func(p1, p2, p3);
            resetJsStrBuf();
        }
    };
    template<typename P1, typename P2, typename P3, typename P4>//4
    struct imp_JS2CFunc<void(*)(P1, P2, P3, P4)> {
        typedef void(*funcType)(P1, P2, P3, P4);
        static void call(const FunctionCallbackInfo<Value>& args) {
            JS2CCALL_INIT;
            JS2CCALL_GET_PARAMS4;
            funcInfo->func(p1, p2, p3, p4);
            resetJsStrBuf();
        }
    };
    template<typename P1, typename P2, typename P3, typename P4, typename P5>//5
    struct imp_JS2CFunc<void(*)(P1, P2, P3, P4, P5)> {
        typedef void(*funcType)(P1, P2, P3, P4, P5);
        static void call(const FunctionCallbackInfo<Value>& args) {
            JS2CCALL_INIT;
            JS2CCALL_GET_PARAMS5;
            funcInfo->func(p1, p2, p3, p4, p5);
            resetJsStrBuf();
        }
    };
    template<typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>//6
    struct imp_JS2CFunc<void(*)(P1, P2, P3, P4, P5, P6)> {
        typedef void(*funcType)(P1, P2, P3, P4, P5, P6);
        static void call(const FunctionCallbackInfo<Value>& args) {
            JS2CCALL_INIT;
            JS2CCALL_GET_PARAMS6;
            funcInfo->func(p1, p2, p3, p4, p5, p6);
            resetJsStrBuf();
        }
    };
    template< typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>//7
    struct imp_JS2CFunc<void(*)(P1, P2, P3, P4, P5, P6, P7)> {
        typedef void(*funcType)(P1, P2, P3, P4, P5, P6, P7);
        static void call(const FunctionCallbackInfo<Value>& args) {
            JS2CCALL_INIT;
            JS2CCALL_GET_PARAMS7;
            funcInfo->func(p1, p2, p3, p4, p5, p6, p7);
            resetJsStrBuf();
        }
    };
    template<typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>//8
    struct imp_JS2CFunc<void(*)(P1, P2, P3, P4, P5, P6, P7, P8)> {
        typedef void(*funcType)(P1, P2, P3, P4, P5, P6, P7, P8);
        static void call(const FunctionCallbackInfo<Value>& args) {
            JS2CCALL_INIT;
            JS2CCALL_GET_PARAMS8;
            funcInfo->func(p1, p2, p3, p4, p5, p6, p7, p8);
            resetJsStrBuf();
        }
    };
    template<typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>//9
    struct imp_JS2CFunc<void(*)(P1, P2, P3, P4, P5, P6, P7, P8, P9)> {
        typedef void(*funcType)(P1, P2, P3, P4, P5, P6, P7, P8, P9);
        static void call(const FunctionCallbackInfo<Value>& args) {
            JS2CCALL_INIT;
            JS2CCALL_GET_PARAMS9;
            funcInfo->func(p1, p2, p3, p4, p5, p6, p7, p8, p9);
            resetJsStrBuf();
        }
    };
    template<typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10, typename P11, typename P12>//12
    struct imp_JS2CFunc<void(*)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12)> {
        typedef void(*funcType)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12);
        static void call(const FunctionCallbackInfo<Value>& args) {
            JS2CCALL_INIT;
            JS2CCALL_GET_PARAMS12;
            funcInfo->func(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);
            resetJsStrBuf();
        }
    };
    //构造函数相关
    struct IJSCCallback
    {
        virtual ~IJSCCallback() {}
        virtual void* constructor(const FunctionCallbackInfo<Value>& args) { throw "not implemented"; }
        virtual uint16_t getNumArgs() = 0;
    };

    #define JS2CCONSTRUCTOR_GET_PARAMS1 \
        P1 p1 = __TransferToCpp<P1>::ToCpp(args[0]);

    #define JS2CCONSTRUCTOR_GET_PARAMS2 \
        P1 p1 = __TransferToCpp<P1>::ToCpp(args[0]);\
        P2 p2 = __TransferToCpp<P2>::ToCpp(args[1]);

    #define JS2CCONSTRUCTOR_GET_PARAMS3 \
        P1 p1 = __TransferToCpp<P1>::ToCpp(args[0]);\
        P2 p2 = __TransferToCpp<P2>::ToCpp(args[1]);\
        P3 p3 = __TransferToCpp<P3>::ToCpp(args[2]);

    template<typename Cls>
    struct JSCConstructor0 : public IJSCCallback {
        virtual void* constructor(const FunctionCallbackInfo<Value>& args) {
            return new Cls();
        }
        virtual uint16_t getNumArgs() { return 0; }
    };

    template<typename Cls>
    IJSCCallback* regConstructor() {
        return new JSCConstructor0<Cls>();
    }

    template<typename Cls, typename P1>
    struct JSCConstructor1 :public IJSCCallback {
        virtual void*  constructor(const FunctionCallbackInfo<Value>& args) {
            JS2CCONSTRUCTOR_GET_PARAMS1;
            return new Cls(p1);
        }
        virtual uint16_t getNumArgs() { return 1; }
    };

    template<typename Cls, typename P1>
    IJSCCallback* regConstructor() {
        return new JSCConstructor1<Cls, P1>();
    }

    template<typename Cls, typename P1, typename P2>
    struct JSCConstructor2 :public IJSCCallback {
        virtual void* constructor(const FunctionCallbackInfo<Value>& args) {
            JS2CCONSTRUCTOR_GET_PARAMS2;
            return new Cls(p1, p2);
        }
        virtual uint16_t getNumArgs() { return 2; }
    };

    template<typename Cls, typename P1, typename P2, typename P3>
    struct JSCConstructor3 :public IJSCCallback {
        virtual void* constructor(const FunctionCallbackInfo<Value>& args) {
            JS2CCONSTRUCTOR_GET_PARAMS3;
            return new Cls(p1, p2, p3);
        }
        virtual uint16_t getNumArgs() { return 3; }
    };

    template<typename Cls, typename P1, typename P2>
    IJSCCallback* regConstructor() {
        return new JSCConstructor2<Cls, P1, P2>();
    }

    template<typename Cls, typename P1, typename P2, typename P3>
    IJSCCallback* regConstructor() {
        return new JSCConstructor3<Cls, P1, P2, P3>();
    }

    template<typename T>
    class JSCClass
    {
    public:
        class FuncEntry
        {
            enum
            {
                Max_Arg_Size = 12,
                Invalid_size = -1,
            };
            IJSCCallback *m_funcs[Max_Arg_Size + 1];
            int m_iMaxArgSize;

        public:
            FuncEntry() {
                m_iMaxArgSize = Invalid_size;
                memset(m_funcs, 0, sizeof(m_funcs));
            }

            ~FuncEntry() {
                reset();
            }

            void reset() {
                for (int i = 0; i <= Max_Arg_Size; ++i) {
                    if (0 != m_funcs[i]) {
                        delete m_funcs[i];
                        m_funcs[i] = 0;
                    }
                }
            }

            void add(IJSCCallback *p_fn) {
                assert(p_fn != nullptr && p_fn->getNumArgs() <= Max_Arg_Size && m_funcs[p_fn->getNumArgs()] == nullptr);
                if (m_iMaxArgSize < p_fn->getNumArgs())
                    m_iMaxArgSize = p_fn->getNumArgs();
                m_funcs[p_fn->getNumArgs()] = p_fn;
            }

            IJSCCallback *get(int p_iArgNum) {
                if (Invalid_size == m_iMaxArgSize) {
                    return 0;
                }

                if (p_iArgNum > m_iMaxArgSize)
                    p_iArgNum = m_iMaxArgSize;

                for (; p_iArgNum >= 0; p_iArgNum--) {
                    if (0 != m_funcs[p_iArgNum])
                        return m_funcs[p_iArgNum];
                }

                return 0;
            }
        };

        JSCClass() {
        }
        ~JSCClass() {
            _reset();
        }
        static JSCClass* getInstance() {
            static JSCClass<T> instance;
            return &instance;
        }
        void addConstructor(IJSCCallback* constructor) {
            m_Constructor.add(constructor);
        }
        static void reset() {
            JSCClass<T>::getInstance()->_reset();
        }

        static void JsConstructor(const FunctionCallbackInfo<Value>& args) {
            if (!args.IsConstructCall()) {
                printf("not a Construct Call)\n");
                return;
            }

            IJSCCallback *pfn = JSCClass<T>::getInstance()->m_Constructor.get(args.Length());
            Isolate* pIso = Isolate::GetCurrent();
            HandleScope handle_scope(pIso);
            Local<Object> pthis = args.This();

            T* obj = nullptr;

            if (0 == pfn) {
                obj = new T;
            }
            else {
                obj = (T*)pfn->constructor(args);
            }

            obj->mpJsIso = pIso;
            pthis->SetAlignedPointerInInternalField(0, obj);
            pthis->SetAlignedPointerInInternalField(1, (void*)&T::JSCLSINFO);
            Persistent<Object>* pPersist = new Persistent<Object>(pIso, pthis);
            obj->mpJsThis = pPersist;
            pPersist->SetWeak(pPersist, JsDestructor<T>, v8::WeakCallbackType::kInternalFields);
            obj->createRefArray();
        }
    private:
        void _reset() {
            m_Constructor.reset();
        }
    private:
        FuncEntry m_Constructor;
    };
   
    class JSClassMgr {
    public:
        typedef void(*RESETFUNC)();
        static JSClassMgr*   GetThreadInstance() {
            return &__Ins;
        }
        std::vector<RESETFUNC> allCls;
        void resetAllRegClass() {
            for (int i = 0, sz = allCls.size(); i < sz; i++) {
                allCls[i]();
            }
            allCls.clear();
        }
    private:
        static thread_local JSClassMgr __Ins;
    };
}


#endif