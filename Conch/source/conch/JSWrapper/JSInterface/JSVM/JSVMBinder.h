#ifndef JS_BINDER_JSVM__H__
#define JS_BINDER_JSVM__H__

#include <ark_runtime/jsvm_types.h>
#include <type_traits> 
#include "JSVMProxyType.h"
#include <util/Log.h>
#include "../JSObjBase.h"
#include "JSVMProxyTransfer.h"
#include <cassert>
#include "IsolateData.h"
#include "JSVMEnv.h"
#include "set"
namespace laya {

#ifdef __GNUC__
#define JSVM_UNUSED __attribute__((unused))
#define JSVM_HOT    __attribute__((hot))
#else
#define JSVM_UNUSED
#define JSVM_HOT
#endif

    template<class T>
    JSVM_Value createJsObjAttachCObj(T* cobj) 
	{
        AutoEscHandleScope esc;
        auto env = Javascript::getEnv();
    
        JSVM_Value constructor = IsolateData::getInstance()->GetObjectTemplate((JsObjClassInfo*)&T::JSCLSINFO);
        assert(constructor != nullptr);
        JSVM_Value obj = nullptr;
        JSVM_Status status;
    
        IsolateData::getInstance()->_setNeedCallConstructor(false);
        JSVM_API_CALL(status, env, OH_JSVM_NewInstance(env, constructor, 0, nullptr, &obj));
        IsolateData::getInstance()->_setNeedCallConstructor(true);
		cobj->initialize(obj);
		cobj->makeWeak();
        JSVM_Value newIns;
        JSVM_API_CALL(status, env, OH_JSVM_EscapeHandle(env, esc.getScope(), obj, &newIns));
        return newIns;
    }

    typedef JSVM_Value JsValue;
    typedef JSVM_Value JsString;
    typedef JSVM_CallbackInfo JsFuncArgs;
    typedef JSVM_Value		JsFunction;

    inline JsString Js_Str(const char* str) {
        auto env = Javascript::getEnv();
        JSVM_Status status;
        JSVM_Value outJsVal;
        if (str == nullptr) {
            JSVM_API_CALL(status, env, OH_JSVM_GetNull(env, &outJsVal));
            return outJsVal;
        }
        JSVM_API_CALL(status, env, OH_JSVM_CreateStringUtf8(env, str, strlen(str), &outJsVal));
        return outJsVal;
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
            JSVM_Status status;
            JSVM_Value result;
            JSVM_API_CALL(status, ENV, OH_JSVM_GetNull(ENV, &result));
            return result;
        }
    };

    template<>
    struct imp_ToJSValue<JsValue> {
        static JsValue trans(JsValue t) {
            return t;
        }
    };

    template<>
    struct imp_ToJSValue<int> {
        static JsValue trans(int t) {
            JSVM_Status status;
            JSVM_Value result;
            JSVM_API_CALL(status, ENV, OH_JSVM_CreateInt64(ENV, t, &result));
            return result;
        }
    };

    //函数无法偏特化，所以用类封装一下
    template <typename T>
    struct imp_JS2CFunc
	{
        static void call(JSVM_Value _this, void* data, size_t argc, JSVM_Value* argv, JSVM_Value* re_val) 
		{
            auto env = ENV;
            JSVM_Status status;
            JSVM_API_CALL(status, env, OH_JSVM_ThrowError(env, nullptr, "imp_JS2CFunc function not handled"));
			return;
        }
    };

    //保存函数指针，是为了给js回调用。
    template <typename T>
    struct FuncInfo {
        T func;
#if 1
		std::string name;
#endif
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
        static void call(void* pthis, void * data, JSVM_Value * re_val) { assert(false); }
    };

    template <class Cls, class R, class S>
    struct imp_JsGetProp<R(Cls::*)(void), S> {
        static void call(void* pthis, void * data, JSVM_Value * re_val) {
            PropFuncInfo< R(Cls::*)(void), S>* funcInfo = (PropFuncInfo< R(Cls::*)(void), S>*)data;
            Cls *pObj = (Cls*)pthis;
            *re_val = ToJSValue<R>((pObj->*funcInfo->fGet)());
        }
    };

    template <class G, class S>
    struct imp_JsSetProp {
        static void call(void* pthis, void * data, JSVM_Value arg, JSVM_Value * re_val) { assert(false); }
    };

    template <class G, class Cls, class T>
    struct imp_JsSetProp<G, void(Cls::*)(T)> {
        static void call(void* pthis, void * data, JSVM_Value arg, JSVM_Value * re_val) {
            PropFuncInfo<G, void(Cls::*)(T)>* funcInfo = (PropFuncInfo<G, void(Cls::*)(T) >*)data;
            Cls *pObj = (Cls*)pthis;
            (pObj->*funcInfo->fSet)(__TransferToCpp<T>::ToCpp(arg));
        }
    };
    //Set还带返回值的情况
    template <class G, class Cls, class T>
    struct imp_JsSetProp<G, T(Cls::*)(T)> {
        static void call(void* pthis, void * data, JSVM_Value arg, JSVM_Value * re_val) {
            PropFuncInfo<G, T(Cls::*)(T)>* funcInfo = (PropFuncInfo<G, T(Cls::*)(T)>*)data;
            Cls *pObj = (Cls*)pthis;
            T ret = (pObj->*funcInfo->fSet)(__TransferToCpp<T>::ToCpp(arg));
            *re_val = ToJSValue<T>(ret);
        }
    };
    ///////////////////////////////////////////
    
    template <class T>
    JSVM_HOT JSVM_Value JSMethodCall(JSVM_Env env, JSVM_CallbackInfo info) {
        JSVM_Status status;
        JSVM_Value _this;
        size_t argc = 15;
        JSVM_Value args[15];
        void* data;
        JSVM_API_CALL(status, env, OH_JSVM_GetCbInfo(env, info, &argc, args, &_this, &data));

        JSVM_Value re_val = nullptr;
        imp_JS2CFunc<T>::call(_this, data, argc, args, &re_val);

        return re_val;
    }

    template <class T>
    JSVM_Value createJSMethod(const char* name, T cfunc) {

        FuncInfo<T>* pData = new FuncInfo<T>(cfunc);
        JSVM_Value fn;
        JSVM_Status status;
        auto env = Javascript::getEnv();
        JSVM_Callback cb = new JSVM_CallbackStruct;
        cb->data = pData;
        cb->callback = JSMethodCall<T>;
        JSVM_API_CALL(status, env, OH_JSVM_CreateFunction(env, name, JSVM_AUTO_LENGTH, cb, &fn));
        return fn;
    }

    template <class T>
    JSVM_Callback createPropMethod(T cfunc) {

        FuncInfo<T>* pData = new FuncInfo<T>(cfunc);
        JSVM_Callback prop_cb = new JSVM_CallbackStruct;
        prop_cb->data = pData;
        prop_cb->callback = JSMethodCall<T>;

        return prop_cb;
    }

    template <class T>
    void addJSMethod(JSClass *jsCls, const char* name, T cfunc) {

        FuncInfo<T>* pData = new FuncInfo<T>(cfunc);
        JSVM_Callback cb = new JSVM_CallbackStruct;
        cb->data = pData;
        cb->callback = JSMethodCall<T>;
        jsCls->defineFunction(name, cb);
    }

    template <class T>
    JSVM_HOT JSVM_Value getPropertyJSCall(JSVM_Env env, JSVM_CallbackInfo info) {
        JSVM_Status status;
        JSVM_Value _this;
        void* data;
        JSVM_API_CALL(status, env, OH_JSVM_GetCbInfo(env, info, nullptr, nullptr, &_this, &data));

        void* nativeThisObject = nullptr;
        JSVM_API_CALL(status, env, OH_JSVM_Unwrap(env, _this, &nativeThisObject));

        JSVM_Value re_val = nullptr;
        imp_JsGetProp<T, T>::call(nativeThisObject, data, &re_val);// 第二个参数签名不重要，因为用不到
        return re_val;
    }

    template <class T>
    JSVM_Callback getPropertyCall(void * data, T cget) {

        JSVM_Callback get_cb = new JSVM_CallbackStruct;
        get_cb->data = data;
        get_cb->callback = getPropertyJSCall<T>;

        return get_cb;
    }

    template <class T>
    JSVM_HOT JSVM_Value setPropertyJSCall(JSVM_Env env, JSVM_CallbackInfo info) {
        JSVM_Status status;
        JSVM_Value _this;
        size_t argc = 1;
        JSVM_Value args[1];
        void* data;
        JSVM_API_CALL(status, env, OH_JSVM_GetCbInfo(env, info, &argc, args, &_this, &data));

        void* nativeThisObject = nullptr;
        JSVM_API_CALL(status, env, OH_JSVM_Unwrap(env, _this, &nativeThisObject));

        JSVM_Value re_val = nullptr;
        imp_JsSetProp<T, T>::call(nativeThisObject, data, args[0], &re_val);// 第一个参数签名不重要，因为用不到
        return re_val;
    }

    template <class T>
    JSVM_Callback setPropertyCall(void * data, T cset) {

        JSVM_Callback set_cb = new JSVM_CallbackStruct;
        set_cb->data = data;
        set_cb->callback = setPropertyJSCall<T>;

        return set_cb;
    }

    /////////js对象的基类////////////////
    class JSObjBaseJSVM {
        friend class JsObjHandle;
	protected:
        JSObjBaseJSVM();   
	public:
        virtual ~JSObjBaseJSVM();

        static  std::set<JSObjBaseJSVM*> objBaseSet;
        static bool restarting;
        static void resetBaseSet() {
            objBaseSet.erase(objBaseSet.begin(), objBaseSet.end());
        }

        void makeStrong();			//防止自己被刪除，例如做異步任務的時候。

        void makeWeak();			//可以被刪除了。

		void initialize(JSVM_Value obj);

		static void WeakCallback(JSVM_Env env, void* nativeObject, void* finalizeHint /*finalize_hint*/)
		{
            if (finalizeHint) {
                if (nativeObject == nullptr) {
                    return;
                }
                JSObjBaseJSVM* obj = reinterpret_cast<JSObjBaseJSVM*>(finalizeHint);
                auto it = objBaseSet.find(obj);
                if (it != objBaseSet.end()) {
                    return;
                }
                delete obj;
            }
		}

        //检查js环境是否变化了，一般用来异步回调的保护
        bool IsMyJsEnv() 
		{
            return Javascript::getEnv() == _env;
        }

        //通过jsthis来调用函数
        inline JsValue _callJsFunc(JsFunction& func, int argc, JsValue argv[])
		{
            AutoEscHandleScope esc;
            JSVM_Status status;
            JSVM_Value jthis;
            JSVM_API_CALL(status, _env, OH_JSVM_GetReferenceValue(_env, _ref, &jthis));
            JSVM_Value result;
            JSVM_API_CALL(status, _env, 
                OH_JSVM_CallFunction(_env, jthis, func, argc, argv, &result));
            JSVM_Value newIns;
            JSVM_API_CALL(status, _env, OH_JSVM_EscapeHandle(_env, esc.getScope(), result, &newIns));
            return newIns;
        }

        JsValue callJsFunc(JsFunction& func);

        template<class P1>
        JsValue callJsFunc(JsFunction& func, P1 p1) {
            int argc = 1;
            JSVM_Value argv[1];
            argv[0] = __TransferToJs<P1>::ToJs(p1);
            return _callJsFunc(func, argc, argv);
        }

        template<class P1, class P2>
        JsValue callJsFunc(JsFunction& func, P1 p1, P2 p2) {
            int argc = 2;
            JSVM_Value argv[2];
            argv[0] = __TransferToJs<P1>::ToJs(p1);
            argv[1] = __TransferToJs<P2>::ToJs(p2);
            return _callJsFunc(func, argc, argv);
        }
        template<class P1, class P2, class P3>
        JsValue callJsFunc(JsFunction& func, P1 p1, P2 p2, P3 p3) {
            int argc = 3;
            JSVM_Value argv[3];
            argv[0] = __TransferToJs<P1>::ToJs(p1);
            argv[1] = __TransferToJs<P2>::ToJs(p2);
            argv[2] = __TransferToJs<P3>::ToJs(p3);
            return _callJsFunc(func, argc, argv);
        }
        template<class P1, class P2, class P3, class P4>
        JsValue callJsFunc(JsFunction& func, P1 p1, P2 p2, P3 p3, P4 p4) {
            int argc = 4;
            JSVM_Value argv[4];
            argv[0] = __TransferToJs<P1>::ToJs(p1);
            argv[1] = __TransferToJs<P2>::ToJs(p2);
            argv[2] = __TransferToJs<P3>::ToJs(p3);
            argv[3] = __TransferToJs<P4>::ToJs(p4);
            return _callJsFunc(func, argc, argv);
        }
        template<class P1, class P2, class P3, class P4, class P5>
        JsValue callJsFunc(JsFunction& func, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5) {
            int argc = 5;
            JSVM_Value argv[5];
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
            JSVM_Value argv[6];
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
            JSVM_Value argv[7];
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
            JSVM_Value argv[8];
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
            JSVM_Value argv[12];
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
	protected:
        JSVM_Env _env = nullptr;
		JSVM_Ref _ref = nullptr;
    };

    class JsObjHandle
	{
    public:
        JsObjHandle()
		{
            m_pObj = NULL;
            m_nID = 0;
        }
        JSObjBaseJSVM*	m_pObj;
        int				m_nID;		//數組下標
		JSVM_Ref	    _ref = nullptr;

        bool Empty();

        JsValue getJsObj()
		{
			if (_ref == nullptr) {
                return nullptr;
            }
            JSVM_Value result;
            JSVM_Status status;
            JSVM_API_CALL(status, ENV, OH_JSVM_GetReferenceValue(ENV, _ref, &result));
            assert(status == JSVM_OK);
            assert(result != nullptr);
            return result;
        }

		JsObjHandle(const JsObjHandle&) = delete;

		JsObjHandle& operator=(const JsObjHandle&) = delete;

        void set(int id, JSObjBaseJSVM* pobj, JsValue value)
		{
            m_pObj = pobj;
            m_nID = id;
            auto env = ENV;
            JSVM_Status status;
			JSVM_API_CALL(status, env, OH_JSVM_CreateReference(env, value, 1, &_ref));
        }

		void set(JsValue value)
		{
            auto env = ENV;
            JSVM_Status status;
			JSVM_API_CALL(status, env, OH_JSVM_CreateReference(env, value, 1, &_ref));
		}

        static bool tryGetStr(JsValue& val, char** ppRet) {
            JSVM_Status status;
            bool result;
            JSVM_API_CALL(status, ENV, OH_JSVM_IsString(ENV, val, &result));
            if (result) {
                *ppRet = JsCharToC(val);// __TransferToCpp<char*>::ToCpp(val);
                return true;
            }
            return false;
        }

        void Reset() {
			if (_ref != nullptr)
			{
				JSVM_Status status;
                JSVM_API_CALL(status, ENV, OH_JSVM_DeleteReference(ENV, _ref));
                _ref = nullptr;
			}
        }

        bool IsFunction(JSVM_Value obj) {
            JSVM_Status status;
            bool result;
            JSVM_API_CALL(status, ENV, OH_JSVM_IsFunction(ENV, obj, &result));
            return result;
        }

		#define CALLJSPRE \
			if (!m_pObj)return false;\
			laya::AutoHandleScope hs;\
			JsValue jsfun = getJsObj();\
			if (!IsFunction(jsfun))\
			return false;

        bool Call() {
			CALLJSPRE
            m_pObj->callJsFunc(jsfun);
            return true;
        }
        template <typename P1>
        bool Call(P1 p1) {
			CALLJSPRE
            m_pObj->callJsFunc(jsfun, p1);
            return true;
        }
		template <typename P1, typename R>
		bool CallWithReturn(P1 p1, R& r) {
			CALLJSPRE
			r = __TransferToCpp<R>::ToCpp(m_pObj->callJsFunc(jsfun, p1));
			return true;
		}

        template <typename P1, typename P2>
        bool Call(P1 p1, P2 p2) {
            CALLJSPRE
            m_pObj->callJsFunc(jsfun, p1, p2);
            return true;
        }
        template <typename P1, typename P2, typename P3>
        bool Call(P1 p1, P2 p2, P3 p3) {
            CALLJSPRE
            m_pObj->callJsFunc(jsfun, p1, p2, p3);
            return true;
        }
        template <typename P1, typename P2, typename P3, typename P4>
        bool Call(P1 p1, P2 p2, P3 p3, P4 p4) {
            CALLJSPRE
            m_pObj->callJsFunc(jsfun, p1, p2, p3, p4);
            return true;
        }
        template <typename P1, typename P2, typename P3, typename P4, typename P5>
        bool Call(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5) {
            CALLJSPRE
            m_pObj->callJsFunc(jsfun, p1, p2, p3, p4, p5);
            return true;
        }
        template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
        bool Call(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6) {
            CALLJSPRE
            m_pObj->callJsFunc(jsfun, p1, p2, p3, p4, p5, p6);
            return true;
        }
        template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
        bool Call(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7) {
            CALLJSPRE
            m_pObj->callJsFunc(jsfun, p1, p2, p3, p4, p5, p6, p7);
            return true;
        }
        template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
        bool Call(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8) {
            CALLJSPRE
            m_pObj->callJsFunc(jsfun, p1, p2, p3, p4, p5, p6, p7, p8);
            return true;
        }
        template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
        bool Call(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9) {
            CALLJSPRE
            m_pObj->callJsFunc(jsfun, p1, p2, p3, p4, p5, p6, p7, p8, p9);
            return true;
        }
		template <typename P1, typename P2>
		bool Call(JSVM_Value objTHIS, P1 p1, P2 p2){
			AutoHandleScope scope;
			JSVM_Value func = getJsObj(); 
			if (!IsFunction(func)) {
                return false;
            } 
		
            JSVM_Status status;
            JSVM_Value result;
			size_t argc = 2;
			JSVM_Value argv[2];
			argv[0] = __TransferToJs<P1>::ToJs(p1);
			argv[1] = __TransferToJs<P2>::ToJs(p2);
            JSVM_API_CALL(status, ENV,
                OH_JSVM_CallFunction(ENV, objTHIS, func, argc, argv, &result));
			return true;
		}

		template <typename P1, typename P2, typename P3>
		bool Call(JSVM_Value objTHIS, P1 p1, P2 p2, P3 p3)
		{
			AutoHandleScope scope;
			JSVM_Value func = getJsObj(); 
			if (!IsFunction(func)) {
                return false;
            } 
		
            JSVM_Status status;
            JSVM_Value result;
			size_t argc = 3;
			JSVM_Value argv[3];
			argv[0] = __TransferToJs<P1>::ToJs(p1);
			argv[1] = __TransferToJs<P2>::ToJs(p2);
			argv[2] = __TransferToJs<P2>::ToJs(p3);
            JSVM_API_CALL(status, ENV,
                OH_JSVM_CallFunction(ENV, objTHIS, func, argc, argv, &result));
			return true;
		}
		template <typename P1, typename P2, typename P3, typename R>
		bool CallWithReturn(JSVM_Value objTHIS, P1 p1, P2 p2, P3 p3, R& r)
		{
			AutoHandleScope scope;
			JSVM_Value func = getJsObj(); 
			if (!IsFunction(func)) {
                return false;
            } 
		
            JSVM_Status status;
            JSVM_Value result;
			size_t argc = 3;
			JSVM_Value argv[3];
			argv[0] = __TransferToJs<P1>::ToJs(p1);
			argv[1] = __TransferToJs<P2>::ToJs(p2);
			argv[2] = __TransferToJs<P2>::ToJs(p3);
            JSVM_API_CALL(status, ENV,
                OH_JSVM_CallFunction(ENV, objTHIS, func, argc, argv, &result));
			r = __TransferToCpp<R>::ToCpp(result);
			return true;
		}
	};

#define JSP_GLOBAL_OBJECT \
	Javascript::getJsGlobal()

	//为了与原来的差不多的宏。 只能在对象的实例中使用。必须按照下面的顺序调用。
	//第一个name是类名，统计用的
#define JSP_CLASS(name,cls) \
	AutoHandleScope scope; \
	JSClass *jsCls = JSClass::create(name, JSCClass<cls>::JsConstructor()); 

//这个的区别是不能直接创建c对象，而是绑定一个已经存在的对象。
#define JSP_GLOBAL_CLASS(name,cls,inst) \
	AutoHandleScope scope; \
	JSVM_Value gctx = Javascript::getJsGlobal();	\
	JSVM_Value instance; \
	OH_JSVM_CreateObject(Javascript::getEnv(), &instance);	\
	inst->initialize(instance);

#define JSP_REG_CONSTRUCTOR(cls,...) \
    JSCClass<cls>::getInstance()->addConstructor(regConstructor<cls,##__VA_ARGS__>());

#define JSP_ADD_FIXED_PROPERTY(name,cls,val) \
    jsCls->defineProperty(#name, nullptr, nullptr, __TransferToJs<decltype(val)>::ToJs(val));

#define JSP_ADD_METHOD(name,fn)    \
	addJSMethod(jsCls, name, &fn);

#define JSP_GLOBAL_ADD_METHOD(name,fn)    \
	OH_JSVM_SetNamedProperty(Javascript::getEnv(), instance, name, createJSMethod(name, &fn));

#define JSP_INSTALL_CLASS(name,cls)    \
	jsCls->install(Javascript::getEnv()); \
    IsolateData::getInstance()->SetObjectTemplate(&cls::JSCLSINFO, jsCls); \
    JSClassMgr::GetInstance()->registerResetFunction(JSCClass<cls>::reset);

//注意，下面的inst是返回的。
#define JSP_INSTALL_GLOBAL_CLASS(name,cls,inst)	\
    OH_JSVM_SetNamedProperty(Javascript::getEnv(), gctx, name, instance);

//添加一个只读属性。get是函数地址，funcplace是一个存放get和set函数地址的结构的指针
#define JSP_ADD_PROPERTY_RO(name,cls,get)	\
	static PropFuncInfo<decltype(&cls::get),decltype(&cls::get)> propFunc_R##cls##name(&cls::get);	\
    jsCls->defineProperty(#name, getPropertyCall((void*)&propFunc_R##cls##name, &cls::get), nullptr, 0);

#define JSP_GLOBAL_ADD_PROPERTY_RO(name,cls,get)	\
    JSVM_PropertyDescriptor properties##cls##name[] = {{#name, nullptr, nullptr, \
        createPropMethod(&cls::get), nullptr, 0, JSVM_DEFAULT}}; \
    OH_JSVM_DefineProperties(Javascript::getEnv(), instance, sizeof(properties##cls##name) / sizeof(JSVM_PropertyDescriptor), properties##cls##name);

#define JSP_ADD_PROPERTY(name,cls,get,set)	\
	static PropFuncInfo<decltype(&cls::get),decltype(&cls::set)> propFunc_##cls##name(&cls::get, &cls::set);	\
    jsCls->defineProperty(#name, getPropertyCall((void*)&propFunc_##cls##name, &cls::get), \
        setPropertyCall((void*)&propFunc_##cls##name, &cls::set), 0);

#define JSP_GLOBAL_ADD_PROPERTY(name,cls,get,set)	\
    JSVM_PropertyDescriptor properties##cls##name[] = {{#name, nullptr, nullptr, \
        createPropMethod(&cls::get), createPropMethod(&cls::set), 0, JSVM_DEFAULT}}; \
    OH_JSVM_DefineProperties(Javascript::getEnv(), instance, sizeof(properties##cls##name) / sizeof(JSVM_PropertyDescriptor), properties##cls##name);


#define JSP_GLOBAL_START1()	\
	AutoHandleScope scope; \
	JSVM_Value gctx;	\
    OH_JSVM_GetGlobal(Javascript::getEnv(), &gctx);

//func必须是全局函数，不允许成员函数。为了简单，还是与原来的兼容把
#define JSP_ADD_GLOBAL_FUNCTION(name, func, ...) \
    OH_JSVM_SetNamedProperty(Javascript::getEnv(), gctx, #name, createJSMethod(#name, func));

#define JSP_ADD_GLOBAL_PROPERTY(name, v)	\
    OH_JSVM_SetNamedProperty(Javascript::getEnv(), gctx, #name, ToJSValue(v));


/////////////////////////////////////////////////////////

//检查参数是否>=num个
#define CHECK_ARGUMENTS_COUNT(num) \
	if (argc < num) \
	{ \
        OH_JSVM_ThrowError(ENV, nullptr, "arguments count error"); \
		return; \
	}

#define JS2CCALL_GET_COBJ \
    void* pthis = nullptr; \
    OH_JSVM_Unwrap(Javascript::getEnv(), _this, &pthis); \
	Cls *pObj = (Cls*)pthis;

#define JS2CCALL_GET_PARAMS1 \
	CHECK_ARGUMENTS_COUNT(1)\
	P1 p1 = __TransferToCpp<P1>::ToCpp(args[0]);

#define JS2CCALL_GET_PARAMS2 \
	CHECK_ARGUMENTS_COUNT(2)\
	P1 p1 = __TransferToCpp<P1>::ToCpp(args[0]);\
	P2 p2 = __TransferToCpp<P2>::ToCpp(args[1]);

#define JS2CCALL_GET_PARAMS3 \
	CHECK_ARGUMENTS_COUNT(3)\
	P1 p1 = __TransferToCpp<P1>::ToCpp(args[0]);\
	P2 p2 = __TransferToCpp<P2>::ToCpp(args[1]);\
	P3 p3 = __TransferToCpp<P3>::ToCpp(args[2]);

#define JS2CCALL_GET_PARAMS4 \
	CHECK_ARGUMENTS_COUNT(4)\
	P1 p1 = __TransferToCpp<P1>::ToCpp(args[0]);\
	P2 p2 = __TransferToCpp<P2>::ToCpp(args[1]);\
	P3 p3 = __TransferToCpp<P3>::ToCpp(args[2]);\
	P4 p4 = __TransferToCpp<P4>::ToCpp(args[3]);

#define JS2CCALL_GET_PARAMS5 \
	CHECK_ARGUMENTS_COUNT(5);\
	P1 p1 = __TransferToCpp<P1>::ToCpp(args[0]);\
	P2 p2 = __TransferToCpp<P2>::ToCpp(args[1]);\
	P3 p3 = __TransferToCpp<P3>::ToCpp(args[2]);\
	P4 p4 = __TransferToCpp<P4>::ToCpp(args[3]);\
	P5 p5 = __TransferToCpp<P5>::ToCpp(args[4]);

#define JS2CCALL_GET_PARAMS6 \
	CHECK_ARGUMENTS_COUNT(6)\
	P1 p1 = __TransferToCpp<P1>::ToCpp(args[0]);\
	P2 p2 = __TransferToCpp<P2>::ToCpp(args[1]);\
	P3 p3 = __TransferToCpp<P3>::ToCpp(args[2]);\
	P4 p4 = __TransferToCpp<P4>::ToCpp(args[3]);\
	P5 p5 = __TransferToCpp<P5>::ToCpp(args[4]);\
	P6 p6 = __TransferToCpp<P6>::ToCpp(args[5]);

#define JS2CCALL_GET_PARAMS7 \
	CHECK_ARGUMENTS_COUNT(7)\
	P1 p1 = __TransferToCpp<P1>::ToCpp(args[0]);\
	P2 p2 = __TransferToCpp<P2>::ToCpp(args[1]);\
	P3 p3 = __TransferToCpp<P3>::ToCpp(args[2]);\
	P4 p4 = __TransferToCpp<P4>::ToCpp(args[3]);\
	P5 p5 = __TransferToCpp<P5>::ToCpp(args[4]);\
	P6 p6 = __TransferToCpp<P6>::ToCpp(args[5]);\
	P7 p7 = __TransferToCpp<P7>::ToCpp(args[6]);

#define JS2CCALL_GET_PARAMS8 \
	CHECK_ARGUMENTS_COUNT(8)\
	P1 p1 = __TransferToCpp<P1>::ToCpp(args[0]);\
	P2 p2 = __TransferToCpp<P2>::ToCpp(args[1]);\
	P3 p3 = __TransferToCpp<P3>::ToCpp(args[2]);\
	P4 p4 = __TransferToCpp<P4>::ToCpp(args[3]);\
	P5 p5 = __TransferToCpp<P5>::ToCpp(args[4]);\
	P6 p6 = __TransferToCpp<P6>::ToCpp(args[5]);\
	P7 p7 = __TransferToCpp<P7>::ToCpp(args[6]);\
	P8 p8 = __TransferToCpp<P8>::ToCpp(args[7]);

#define JS2CCALL_GET_PARAMS9 \
	CHECK_ARGUMENTS_COUNT(9)\
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
	CHECK_ARGUMENTS_COUNT(10)\
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
	CHECK_ARGUMENTS_COUNT(11)\
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
	CHECK_ARGUMENTS_COUNT(12)\
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


	//回调的偏特化
	//没有参数
	template<typename Cls, typename R>
	struct imp_JS2CFunc<R(Cls::*)(void)> {
		typedef R(Cls::*funcType)(void);
		static void call(JSVM_Value _this, void* data, size_t argc, JSVM_Value* args, JSVM_Value* re_val) {
            FuncInfo<funcType>* funcInfo = (FuncInfo<funcType>*)data;
			JS2CCALL_GET_COBJ;
			*re_val = ToJSValue<R>((pObj->*funcInfo->func)());
		}
	};

    //一个参数
    template<typename Cls, typename R, typename P1 >
    struct imp_JS2CFunc<R(Cls::*)(P1)> {
        typedef R(Cls::*funcType)(P1);
        static void call(JSVM_Value _this, void* data, size_t argc, JSVM_Value* args, JSVM_Value* re_val) {
            FuncInfo<funcType>* funcInfo = (FuncInfo<funcType>*)data;
            JS2CCALL_GET_COBJ;
            JS2CCALL_GET_PARAMS1;
            *re_val = ToJSValue<R>((pObj->*funcInfo->func)(p1));
            resetJsStrBuf();
        }
    };

    //2
    template<typename Cls, typename R,  typename P1, typename P2 >
    struct imp_JS2CFunc<R(Cls::*)(P1, P2)> {
        typedef R(Cls::*funcType)(P1, P2);
        static void call(JSVM_Value _this, void* data, size_t argc, JSVM_Value* args, JSVM_Value* re_val) {
            FuncInfo<funcType>* funcInfo = (FuncInfo<funcType>*)data;
            JS2CCALL_GET_COBJ;
            JS2CCALL_GET_PARAMS2;
            *re_val = ToJSValue<R>((pObj->*funcInfo->func)(p1, p2));
            resetJsStrBuf();
        }
    };

    //3
    template<typename Cls, typename R,  typename P1, typename P2, typename P3 >
    struct imp_JS2CFunc<R(Cls::*)(P1, P2, P3)> {
        typedef R(Cls::*funcType)(P1, P2, P3);
        static void call(JSVM_Value _this, void* data, size_t argc, JSVM_Value* args, JSVM_Value* re_val) {
            FuncInfo<funcType>* funcInfo = (FuncInfo<funcType>*)data;
            JS2CCALL_GET_COBJ;
            JS2CCALL_GET_PARAMS3;
            *re_val = ToJSValue<R>((pObj->*funcInfo->func)(p1, p2, p3));
            resetJsStrBuf();
        }
    };

    //4
    template<typename Cls, typename R,  typename P1, typename P2, typename P3, typename P4 >
    struct imp_JS2CFunc<R(Cls::*)(P1, P2, P3, P4)> {
        typedef R(Cls::*funcType)(P1, P2, P3, P4);
        static void call(JSVM_Value _this, void* data, size_t argc, JSVM_Value* args, JSVM_Value* re_val) {
            FuncInfo<funcType>* funcInfo = (FuncInfo<funcType>*)data;
            JS2CCALL_GET_COBJ;
            JS2CCALL_GET_PARAMS4;
            *re_val = ToJSValue<R>((pObj->*funcInfo->func)(p1, p2, p3, p4));
            resetJsStrBuf();
        }
    };
    //5
    template<typename Cls, typename R,  typename P1, typename P2, typename P3, typename P4, typename P5>
    struct imp_JS2CFunc<R(Cls::*)(P1, P2, P3, P4, P5)> {
        typedef R(Cls::*funcType)(P1, P2, P3, P4, P5);
        static void call(JSVM_Value _this, void* data, size_t argc, JSVM_Value* args, JSVM_Value* re_val) {
            FuncInfo<funcType>* funcInfo = (FuncInfo<funcType>*)data;
            JS2CCALL_GET_COBJ;
            JS2CCALL_GET_PARAMS5;
            *re_val = ToJSValue<R>((pObj->*funcInfo->func)(p1, p2, p3, p4, p5));
            resetJsStrBuf();
        }
    };
    //6
    template<typename Cls, typename R,  typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
    struct imp_JS2CFunc<R(Cls::*)(P1, P2, P3, P4, P5, P6)> {
        typedef R(Cls::*funcType)(P1, P2, P3, P4, P5, P6);
        static void call(JSVM_Value _this, void* data, size_t argc, JSVM_Value* args, JSVM_Value* re_val) {
            FuncInfo<funcType>* funcInfo = (FuncInfo<funcType>*)data;
            JS2CCALL_GET_COBJ;
            JS2CCALL_GET_PARAMS6;
            *re_val = ToJSValue<R>((pObj->*funcInfo->func)(p1, p2, p3, p4, p5, p6));
            resetJsStrBuf();
        }
    };
    //7
    template<typename Cls, typename R,  typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
    struct imp_JS2CFunc<R(Cls::*)(P1, P2, P3, P4, P5, P6, P7)> {
        typedef R(Cls::*funcType)(P1, P2, P3, P4, P5, P6, P7);
        static void call(JSVM_Value _this, void* data, size_t argc, JSVM_Value* args, JSVM_Value* re_val) {
            FuncInfo<funcType>* funcInfo = (FuncInfo<funcType>*)data;
            JS2CCALL_GET_COBJ;
            JS2CCALL_GET_PARAMS7;
            *re_val = ToJSValue<R>((pObj->*funcInfo->func)(p1, p2, p3, p4, p5, p6, p7));
            resetJsStrBuf();
        }
    };
    template<typename Cls, typename R,  typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
    struct imp_JS2CFunc<R(Cls::*)(P1, P2, P3, P4, P5, P6, P7, P8)> {
        typedef R(Cls::*funcType)(P1, P2, P3, P4, P5, P6, P7, P8);
        static void call(JSVM_Value _this, void* data, size_t argc, JSVM_Value* args, JSVM_Value* re_val) {
            FuncInfo<funcType>* funcInfo = (FuncInfo<funcType>*)data;
            JS2CCALL_GET_COBJ;
            JS2CCALL_GET_PARAMS8;
            *re_val = ToJSValue<R>((pObj->*funcInfo->func)(p1, p2, p3, p4, p5, p6, p7, p8));
            resetJsStrBuf();
        }
    };

    template<typename Cls, typename R,  typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
    struct imp_JS2CFunc<R(Cls::*)(P1, P2, P3, P4, P5, P6, P7, P8, P9)> {
        typedef R(Cls::*funcType)(P1, P2, P3, P4, P5, P6, P7, P8, P9);
        static void call(JSVM_Value _this, void* data, size_t argc, JSVM_Value* args, JSVM_Value* re_val) {
            FuncInfo<funcType>* funcInfo = (FuncInfo<funcType>*)data;
            JS2CCALL_GET_COBJ;
            JS2CCALL_GET_PARAMS9;
            *re_val = ToJSValue<R>((pObj->*funcInfo->func)(p1, p2, p3, p4, p5, p6, p7, p8, p9));
            resetJsStrBuf();
        }
    };
    template<typename Cls, typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10, typename P11, typename P12>
    struct imp_JS2CFunc<R(Cls::*)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12)> {
        typedef R(Cls::*funcType)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12);
        static void call(JSVM_Value _this, void* data, size_t argc, JSVM_Value* args, JSVM_Value* re_val) {
            FuncInfo<funcType>* funcInfo = (FuncInfo<funcType>*)data;
            JS2CCALL_GET_COBJ;
            JS2CCALL_GET_PARAMS12;
            *re_val = ToJSValue<R>((pObj->*funcInfo->func)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12));
            resetJsStrBuf();
        }
    };

    //下面是void的，没想到好办法
    //没有参数
    template<typename Cls>
    struct imp_JS2CFunc<void(Cls::*)(void)> {
        typedef void(Cls::*funcType)(void);
        static void call(JSVM_Value _this, void* data, size_t argc, JSVM_Value* args, JSVM_Value* re_val) {
            FuncInfo<funcType>* funcInfo = (FuncInfo<funcType>*)data;
            JS2CCALL_GET_COBJ;
            (pObj->*funcInfo->func)();
        }
    };

    //一个参数
    template< typename Cls, typename P1 >
    struct imp_JS2CFunc<void(Cls::*)(P1)> {
        typedef void(Cls::*funcType)(P1);
        static void call(JSVM_Value _this, void* data, size_t argc, JSVM_Value* args, JSVM_Value* re_val) {
            FuncInfo<funcType>* funcInfo = (FuncInfo<funcType>*)data;
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
        static void call(JSVM_Value _this, void* data, size_t argc, JSVM_Value* args, JSVM_Value* re_val) {
            FuncInfo<funcType>* funcInfo = (FuncInfo<funcType>*)data;
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
        static void call(JSVM_Value _this, void* data, size_t argc, JSVM_Value* args, JSVM_Value* re_val) {
            FuncInfo<funcType>* funcInfo = (FuncInfo<funcType>*)data;
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
        static void call(JSVM_Value _this, void* data, size_t argc, JSVM_Value* args, JSVM_Value* re_val) {
            FuncInfo<funcType>* funcInfo = (FuncInfo<funcType>*)data;
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
        static void call(JSVM_Value _this, void* data, size_t argc, JSVM_Value* args, JSVM_Value* re_val) {
            FuncInfo<funcType>* funcInfo = (FuncInfo<funcType>*)data;
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
        static void call(JSVM_Value _this, void* data, size_t argc, JSVM_Value* args, JSVM_Value* re_val) {
            FuncInfo<funcType>* funcInfo = (FuncInfo<funcType>*)data;
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
        static void call(JSVM_Value _this, void* data, size_t argc, JSVM_Value* args, JSVM_Value* re_val) {
            FuncInfo<funcType>* funcInfo = (FuncInfo<funcType>*)data;
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
        static void call(JSVM_Value _this, void* data, size_t argc, JSVM_Value* args, JSVM_Value* re_val) {
            FuncInfo<funcType>* funcInfo = (FuncInfo<funcType>*)data;
            JS2CCALL_GET_COBJ;
            JS2CCALL_GET_PARAMS9;
            (pObj->*funcInfo->func)(p1, p2, p3, p4, p5, p6, p7, p8, p9);
            resetJsStrBuf();
        }
    };
    template<typename Cls, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10>
    struct imp_JS2CFunc<void(Cls::*)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10)> {
        typedef void(Cls::*funcType)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10);
        static void call(JSVM_Value _this, void* data, size_t argc, JSVM_Value* args, JSVM_Value* re_val) {
            FuncInfo<funcType>* funcInfo = (FuncInfo<funcType>*)data;
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
        static void call(JSVM_Value _this, void* data, size_t argc, JSVM_Value* args, JSVM_Value* re_val) {
            FuncInfo<funcType>* funcInfo = (FuncInfo<funcType>*)data;
            JS2CCALL_GET_COBJ;
            JS2CCALL_GET_PARAMS11;
            (pObj->*funcInfo->func)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);
            resetJsStrBuf();
        }
    };
    template<typename Cls, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10, typename P11, typename P12>
    struct imp_JS2CFunc<void(Cls::*)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12)> {
        typedef void(Cls::*funcType)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12);
        static void call(JSVM_Value _this, void* data, size_t argc, JSVM_Value* args, JSVM_Value* re_val) {
            FuncInfo<funcType>* funcInfo = (FuncInfo<funcType>*)data;
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
        static void call(JSVM_Value _this, void* data, size_t argc, JSVM_Value* args, JSVM_Value* re_val) {
            FuncInfo<funcType>* funcInfo = (FuncInfo<funcType>*)data;
            funcInfo->func();
        }
    };
    template<typename R>//R(void)
    struct imp_JS2CFunc<R(*)(void)> {
        typedef R(*funcType)(void);
        static void call(JSVM_Value _this, void* data, size_t argc, JSVM_Value* args, JSVM_Value* re_val) {
            FuncInfo<funcType>* funcInfo = (FuncInfo<funcType>*)data;
            *re_val = ToJSValue<R>(funcInfo->func());
        }
    };
    template<typename R, typename P1>//R(p1)
    struct imp_JS2CFunc<R(*)(P1)> {
        typedef R(*funcType)(P1);
        static void call(JSVM_Value _this, void* data, size_t argc, JSVM_Value* args, JSVM_Value* re_val) {
            FuncInfo<funcType>* funcInfo = (FuncInfo<funcType>*)data;
            JS2CCALL_GET_PARAMS1;
            *re_val = ToJSValue<R>(funcInfo->func(p1));
            resetJsStrBuf();
        }
    };
    template<typename R, typename P1>//const R(p1)
    struct imp_JS2CFunc<const R(*)(P1)> {
        typedef const R(*funcType)(P1);
        static void call(JSVM_Value _this, void* data, size_t argc, JSVM_Value* args, JSVM_Value* re_val) {
            FuncInfo<funcType>* funcInfo = (FuncInfo<funcType>*)data;
            JS2CCALL_GET_PARAMS1;
            *re_val = ToJSValue<R>(funcInfo->func(p1));
            resetJsStrBuf();
        }
    };

    template<typename P1>
    struct imp_JS2CFunc<void(*)(P1)> {//void(p1)
        typedef void(*funcType)(P1);
        static void call(JSVM_Value _this, void* data, size_t argc, JSVM_Value* args, JSVM_Value* re_val) {
            FuncInfo<funcType>* funcInfo = (FuncInfo<funcType>*)data;
            JS2CCALL_GET_PARAMS1;
            funcInfo->func(p1);
            resetJsStrBuf();
        }
    };

    template<typename R, typename P1, typename P2>//R(p1,p2)
    struct imp_JS2CFunc<R(*)(P1, P2)> {
        typedef R(*funcType)(P1, P2);
        static void call(JSVM_Value _this, void* data, size_t argc, JSVM_Value* args, JSVM_Value* re_val) {
            FuncInfo<funcType>* funcInfo = (FuncInfo<funcType>*)data;
            JS2CCALL_GET_PARAMS2;
            *re_val = ToJSValue<R>(funcInfo->func(p1, p2));
            resetJsStrBuf();
        }
    };

    template<typename P1, typename P2>//void(p1,p2)
    struct imp_JS2CFunc<void(*)(P1, P2)> {
        typedef void(*funcType)(P1, P2);
        static void call(JSVM_Value _this, void* data, size_t argc, JSVM_Value* args, JSVM_Value* re_val) {
            FuncInfo<funcType>* funcInfo = (FuncInfo<funcType>*)data;
            JS2CCALL_GET_PARAMS2;
            funcInfo->func(p1, p2);
            resetJsStrBuf();
        }
    };

    template<typename R, typename P1, typename P2, typename P3>//3
    struct imp_JS2CFunc<R(*)(P1, P2, P3)> {
        typedef R(*funcType)(P1, P2, P3);
        static void call(JSVM_Value _this, void* data, size_t argc, JSVM_Value* args, JSVM_Value* re_val) {
            FuncInfo<funcType>* funcInfo = (FuncInfo<funcType>*)data;
            JS2CCALL_GET_PARAMS3;
            *re_val = ToJSValue<R>(funcInfo->func(p1, p2, p3));
            resetJsStrBuf();
        }
    };
    template<typename R, typename P1, typename P2, typename P3, typename P4>//4
    struct imp_JS2CFunc<R(*)(P1, P2, P3, P4)> {
        typedef R(*funcType)(P1, P2, P3, P4);
        static void call(JSVM_Value _this, void* data, size_t argc, JSVM_Value* args, JSVM_Value* re_val) {
            FuncInfo<funcType>* funcInfo = (FuncInfo<funcType>*)data;
            JS2CCALL_GET_PARAMS4;
            *re_val = ToJSValue<R>(funcInfo->func(p1, p2, p3, p4));
            resetJsStrBuf();
        }
    };
    template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5>//5
    struct imp_JS2CFunc<R(*)(P1, P2, P3, P4, P5)> {
        typedef R(*funcType)(P1, P2, P3, P4, P5);
        static void call(JSVM_Value _this, void* data, size_t argc, JSVM_Value* args, JSVM_Value* re_val) {
            FuncInfo<funcType>* funcInfo = (FuncInfo<funcType>*)data;
            JS2CCALL_GET_PARAMS5;
            *re_val = ToJSValue<R>(funcInfo->func(p1, p2, p3, p4, p5));
            resetJsStrBuf();
        }
    };
    template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>//6
    struct imp_JS2CFunc<R(*)(P1, P2, P3, P4, P5, P6)> {
        typedef R(*funcType)(P1, P2, P3, P4, P5, P6);
        static void call(JSVM_Value _this, void* data, size_t argc, JSVM_Value* args, JSVM_Value* re_val) {
            FuncInfo<funcType>* funcInfo = (FuncInfo<funcType>*)data;
            JS2CCALL_GET_PARAMS6;
            *re_val = ToJSValue<R>(funcInfo->func(p1, p2, p3, p4, p5, p6));
            resetJsStrBuf();
        }
    };
    template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>//7
    struct imp_JS2CFunc<R(*)(P1, P2, P3, P4, P5, P6, P7)> {
        typedef R(*funcType)(P1, P2, P3, P4, P5, P6, P7);
        static void call(JSVM_Value _this, void* data, size_t argc, JSVM_Value* args, JSVM_Value* re_val) {
            FuncInfo<funcType>* funcInfo = (FuncInfo<funcType>*)data;
            JS2CCALL_GET_PARAMS7;
            *re_val = ToJSValue<R>(funcInfo->func(p1, p2, p3, p4, p5, p6, p7));
            resetJsStrBuf();
        }
    };
    template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>//8
    struct imp_JS2CFunc<R(*)(P1, P2, P3, P4, P5, P6, P7, P8)> {
        typedef R(*funcType)(P1, P2, P3, P4, P5, P6, P7, P8);
        static void call(JSVM_Value _this, void* data, size_t argc, JSVM_Value* args, JSVM_Value* re_val) {
            FuncInfo<funcType>* funcInfo = (FuncInfo<funcType>*)data;
            JS2CCALL_GET_PARAMS8;
            *re_val = ToJSValue<R>(funcInfo->func(p1, p2, p3, p4, p5, p6, p7, p8));
            resetJsStrBuf();
        }
    };
    template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>//9
    struct imp_JS2CFunc<R(*)(P1, P2, P3, P4, P5, P6, P7, P8, P9)> {
        typedef R(*funcType)(P1, P2, P3, P4, P5, P6, P7, P8, P9);
        static void call(JSVM_Value _this, void* data, size_t argc, JSVM_Value* args, JSVM_Value* re_val) {
            FuncInfo<funcType>* funcInfo = (FuncInfo<funcType>*)data;
            JS2CCALL_GET_PARAMS9;
            *re_val = ToJSValue<R>(funcInfo->func(p1, p2, p3, p4, p5, p6, p7, p8, p9));
            resetJsStrBuf();
        }
    };

    //void
    template< typename P1, typename P2, typename P3>//3
    struct imp_JS2CFunc<void(*)(P1, P2, P3)> {
        typedef void(*funcType)(P1, P2, P3);
        static void call(JSVM_Value _this, void* data, size_t argc, JSVM_Value* args, JSVM_Value* re_val) {
            FuncInfo<funcType>* funcInfo = (FuncInfo<funcType>*)data;
            JS2CCALL_GET_PARAMS3;
            funcInfo->func(p1, p2, p3);
            resetJsStrBuf();
        }
    };
    template<typename P1, typename P2, typename P3, typename P4>//4
    struct imp_JS2CFunc<void(*)(P1, P2, P3, P4)> {
        typedef void(*funcType)(P1, P2, P3, P4);
        static void call(JSVM_Value _this, void* data, size_t argc, JSVM_Value* args, JSVM_Value* re_val) {
            FuncInfo<funcType>* funcInfo = (FuncInfo<funcType>*)data;
            JS2CCALL_GET_PARAMS4;
            funcInfo->func(p1, p2, p3, p4);
            resetJsStrBuf();
        }
    };
    template<typename P1, typename P2, typename P3, typename P4, typename P5>//5
    struct imp_JS2CFunc<void(*)(P1, P2, P3, P4, P5)> {
        typedef void(*funcType)(P1, P2, P3, P4, P5);
        static void call(JSVM_Value _this, void* data, size_t argc, JSVM_Value* args, JSVM_Value* re_val) {
            FuncInfo<funcType>* funcInfo = (FuncInfo<funcType>*)data;
            JS2CCALL_GET_PARAMS5;
            funcInfo->func(p1, p2, p3, p4, p5);
            resetJsStrBuf();
        }
    };
    template<typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>//6
    struct imp_JS2CFunc<void(*)(P1, P2, P3, P4, P5, P6)> {
        typedef void(*funcType)(P1, P2, P3, P4, P5, P6);
        static void call(JSVM_Value _this, void* data, size_t argc, JSVM_Value* args, JSVM_Value* re_val) {
            FuncInfo<funcType>* funcInfo = (FuncInfo<funcType>*)data;
            JS2CCALL_GET_PARAMS6;
            funcInfo->func(p1, p2, p3, p4, p5, p6);
            resetJsStrBuf();
        }
    };
    template< typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>//7
    struct imp_JS2CFunc<void(*)(P1, P2, P3, P4, P5, P6, P7)> {
        typedef void(*funcType)(P1, P2, P3, P4, P5, P6, P7);
        static void call(JSVM_Value _this, void* data, size_t argc, JSVM_Value* args, JSVM_Value* re_val) {
            FuncInfo<funcType>* funcInfo = (FuncInfo<funcType>*)data;
            JS2CCALL_GET_PARAMS7;
            funcInfo->func(p1, p2, p3, p4, p5, p6, p7);
            resetJsStrBuf();
        }
    };
    template<typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>//8
    struct imp_JS2CFunc<void(*)(P1, P2, P3, P4, P5, P6, P7, P8)> {
        typedef void(*funcType)(P1, P2, P3, P4, P5, P6, P7, P8);
        static void call(JSVM_Value _this, void* data, size_t argc, JSVM_Value* args, JSVM_Value* re_val) {
            FuncInfo<funcType>* funcInfo = (FuncInfo<funcType>*)data;
            JS2CCALL_GET_PARAMS8;
            funcInfo->func(p1, p2, p3, p4, p5, p6, p7, p8);
            resetJsStrBuf();
        }
    };
    template<typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>//9
    struct imp_JS2CFunc<void(*)(P1, P2, P3, P4, P5, P6, P7, P8, P9)> {
        typedef void(*funcType)(P1, P2, P3, P4, P5, P6, P7, P8, P9);
        static void call(JSVM_Value _this, void* data, size_t argc, JSVM_Value* args, JSVM_Value* re_val) {
            FuncInfo<funcType>* funcInfo = (FuncInfo<funcType>*)data;
            JS2CCALL_GET_PARAMS9;
            funcInfo->func(p1, p2, p3, p4, p5, p6, p7, p8, p9);
            resetJsStrBuf();
        }
    };
    template<typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10, typename P11, typename P12>//12
    struct imp_JS2CFunc<void(*)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12)> {
        typedef void(*funcType)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12);
        static void call(JSVM_Value _this, void* data, size_t argc, JSVM_Value* args, JSVM_Value* re_val) {
            FuncInfo<funcType>* funcInfo = (FuncInfo<funcType>*)data;
            JS2CCALL_GET_PARAMS12;
            funcInfo->func(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);
            resetJsStrBuf();
        }
    };
    //构造函数相关
    struct IJSCCallback
    {
        virtual ~IJSCCallback() {}
        virtual void* constructor(JSVM_Value* args) { throw "not implemented"; }
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
        virtual void* constructor(JSVM_Value* args) {
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
        virtual void*  constructor(JSVM_Value* args) {
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
        virtual void* constructor(JSVM_Value* args) {
            JS2CCONSTRUCTOR_GET_PARAMS2;
            return new Cls(p1, p2);
        }
        virtual uint16_t getNumArgs() { return 2; }
    };

    template<typename Cls, typename P1, typename P2, typename P3>
    struct JSCConstructor3 :public IJSCCallback {
        virtual void* constructor(JSVM_Value* args) {
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

        JSVM_HOT static JSVM_Value JsConstructorCall(JSVM_Env env, JSVM_CallbackInfo info) {
            
            JSVM_Status status;

            JSVM_Value result;
            JSVM_API_CALL(status, env, OH_JSVM_GetNewTarget(env, info, &result));
            bool     isUndefined;
            JSVM_API_CALL(status, env, OH_JSVM_IsUndefined(env, result, &isUndefined));
            if (status != JSVM_OK || isUndefined) {
                printf("not a Construct Call)\n");
                return nullptr;
            }

            JSVM_Value _this;
            size_t argc = 10;
            JSVM_Value args[10];
            JSVM_API_CALL(status, env, OH_JSVM_GetCbInfo(env, info, &argc, args, &_this, NULL));

            if (!IsolateData::getInstance()->_needCallConstructor()) {
                return _this;
            }

            IJSCCallback *pfn = JSCClass<T>::getInstance()->m_Constructor.get(argc);
            T* cobj = nullptr;
            if (0 == pfn) {
                cobj = new T;
            }
            else {
                cobj = (T*)pfn->constructor(args);
            }
            cobj->initialize(_this);
			cobj->makeWeak();

            return _this;
        }

        static JSVM_Callback JsConstructor() {
            AutoHandleScope scope;
            JSVM_Callback cb = new JSVM_CallbackStruct;
            cb->data = nullptr;
            cb->callback = JsConstructorCall;
            return cb;
        }
    private:
        void _reset() {
            m_Constructor.reset();
        }
    private:
        FuncEntry m_Constructor;
    };
   
    class JSClassMgr
	{
    public:
        typedef void(*RESETFUNC)();

        static JSClassMgr* GetInstance() 
		{
            return &__Ins;
        }

		void registerResetFunction(RESETFUNC func)
		{
			allCls.push_back(func);
		}
        void resetAllRegClass()
		{
            for (int i = 0, sz = allCls.size(); i < sz; i++)
			{
                allCls[i]();
            }
            allCls.clear();
        }

    private:

		std::vector<RESETFUNC> allCls;

        static JSClassMgr __Ins;

    };
}


#endif