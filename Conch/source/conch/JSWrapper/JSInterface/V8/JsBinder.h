
#ifndef JS_BINDER_V8__H__
#define JS_BINDER_V8__H__

#include <v8.h>
#include <type_traits> 
#include "JSCProxyType.h"
#include <util/Log.h>
#include "../JSObjBase.h"
#include "JSCProxyTrnasfer.h"
#include <cassert>
#include "IsolateData.h"

using namespace v8;

namespace laya {

    template<class T>
    Local<Object> createJsObjAttachCObj(T* cobj) 
	{
        Isolate* isolate = Isolate::GetCurrent();
        EscapableHandleScope handle_scope(isolate);
		IsolateData* data = IsolateData::From(isolate);
		v8::Local<v8::ObjectTemplate> pTemp = data->GetObjectTemplate((JsObjClassInfo*)&T::JSCLSINFO);
		assert(!pTemp.IsEmpty());
		Local<Object> pNewIns = pTemp->NewInstance(isolate->GetCurrentContext()).ToLocalChecked();
        pNewIns->SetAlignedPointerInInternalField(0, cobj);
        pNewIns->SetAlignedPointerInInternalField(1, (void*)&T::JSCLSINFO);
		cobj->initialize(isolate, pNewIns);
		cobj->makeWeak();
        return handle_scope.Escape(pNewIns);
    }

    typedef Local<Value> JsValue;
    typedef Local<String> JsString;
    typedef FunctionCallbackInfo<Value> JsFuncArgs;
    typedef Local<Function>		JsFunction;

    inline JsString Js_Str(Isolate* pIso, const char* str) {
        return String::NewFromUtf8(pIso, str).ToLocalChecked();
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

    //函数无法偏特化，所以用类封装一下
    template <typename T>
    struct imp_JS2CFunc
	{
        static void call(const FunctionCallbackInfo<Value>& args) 
		{
			args.GetIsolate()->ThrowException(v8::String::NewFromUtf8(args.GetIsolate(), "imp_JS2CFunc function not handled").ToLocalChecked());
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
        static void call(Local<String> property, const PropertyCallbackInfo<Value>& info) { assert(false); }
    };

    template <class Cls, class R, class S>
    struct imp_JsGetProp<R(Cls::*)(void), S> {
        static void call(Local<String> property, const PropertyCallbackInfo<Value>& info) {
            PropFuncInfo< R(Cls::*)(void), S>* funcInfo = (PropFuncInfo< R(Cls::*)(void), S>*)External::Cast(*info.Data())->Value();
            Local<Object> pthis = info.This();
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
    //	v8::Local<Value> data = External::New(pIso, (void*)*(int*)&cfunc);
    //	func->SetCallHandler(imp_bindv8cCB<T>::call, data);
    //	//return imp_bindv8c<T>::call(func, cfunc, ptr);
    //	return func->GetFunction();
    //}

    //由於v8在js對象的0的位置存了c對象了，所以只要傳函數地址就行了
    template <class T>
    Local<Function> createJSMethod(const char* name, T cfunc) {
        Isolate* pIso = Isolate::GetCurrent();
        Local<FunctionTemplate> method = FunctionTemplate::New(pIso);
        FuncInfo<T>* pData = new FuncInfo<T>(cfunc);	//怎么释放呢
#if 1
		pData->name = name;
#endif
        //函數不能轉成數字，只好取地址再轉，但是取的地址是堆栈地址，所以再取*转成int,表示实际地址了，再强转void*
		v8::Local<Value> data = External::New(pIso, pData);// (void*)*(int*)&cfunc);
        method->SetCallHandler(imp_JS2CFunc<T>::call, data);

        return method->GetFunction(pIso->GetCurrentContext()).ToLocalChecked();
    }
    template <class T>
    void addJSMethod(const char* name, v8::Local<v8::FunctionTemplate> that, T cfunc) {
        Isolate* pIso = Isolate::GetCurrent();
        FuncInfo<T>* pData = new FuncInfo<T>(cfunc);	//怎么释放呢
#if 1
		pData->name = name;
#endif
         //函數不能轉成數字，只好取地址再轉，但是取的地址是堆栈地址，所以再取*转成int,表示实际地址了，再强转void*
		v8::Local<Value> data = External::New(pIso, pData);// (void*)*(int*)&cfunc);
        //instancTemp->SetNativeDataProperty(Js_Str(pIso, name), imp_JS2CFunc<T>::call, 0, data);

        v8::Local<v8::Signature> signature = v8::Signature::New(pIso, that);
        v8::Local<v8::FunctionTemplate> t = v8::FunctionTemplate::New(pIso, imp_JS2CFunc<T>::call, data, signature);
        // kInternalized strings are created in the old space.
        const v8::NewStringType type = v8::NewStringType::kInternalized;
        v8::Local<v8::String> name_string = v8::String::NewFromUtf8(pIso, name, type).ToLocalChecked();
        that->PrototypeTemplate()->Set(name_string, t);
        t->SetClassName(name_string);  // NODE_SET_PROTOTYPE_METHOD() compatibility.
    }
	template <class T>
	void SetMethod(const char* name, v8::Local<v8::Object> that, T callback, v8::Local<v8::FunctionTemplate> functionTemplate)
	{
		Isolate* isolate = Isolate::GetCurrent();
		v8::Local<v8::Context> context = isolate->GetCurrentContext();

		FuncInfo<T>* pData = new FuncInfo<T>(callback);
#if 1
		pData->name = name;
#endif
		v8::Local<Value> data = External::New(isolate, pData);
		v8::Local<v8::Signature> signature = v8::Signature::New(isolate, functionTemplate);

		v8::Local<v8::Function> function = v8::FunctionTemplate::New(isolate, imp_JS2CFunc<T>::call, data, signature, 0, v8::ConstructorBehavior::kThrow, v8::SideEffectType::kHasSideEffect)->GetFunction(context).ToLocalChecked();
		// kInternalized strings are created in the old space.
		const v8::NewStringType type = v8::NewStringType::kInternalized;
		v8::Local<v8::String> name_string = v8::String::NewFromUtf8(isolate, name, type).ToLocalChecked();
		that->Set(context, name_string, function).Check();
		function->SetName(name_string);  // NODE_SET_METHOD() compatibility.
	}
	template <class T>
	void SetInstanceMethod(const char* name, v8::Local<v8::FunctionTemplate> that, T callback)
	{
		Isolate* isolate = Isolate::GetCurrent();
		v8::Local<v8::Context> context = isolate->GetCurrentContext();

		FuncInfo<T>* pData = new FuncInfo<T>(callback);
#if 1
		pData->name = name;
#endif
		v8::Local<Value> data = External::New(isolate, pData);
		v8::Local<v8::Signature> signature = v8::Signature::New(isolate, that);

		
		v8::Local<v8::FunctionTemplate> t = v8::FunctionTemplate::New(isolate, imp_JS2CFunc<T>::call, data, signature, 0, v8::ConstructorBehavior::kThrow, v8::SideEffectType::kHasSideEffect);
		// kInternalized strings are created in the old space.
		const v8::NewStringType type = v8::NewStringType::kInternalized;
		v8::Local<v8::String> name_string = v8::String::NewFromUtf8(isolate, name, type).ToLocalChecked();
		that->InstanceTemplate()->Set(name_string, t);
		t->SetClassName(name_string);
	}
    //直接封装符合v8标准的指定函数
    Local<Function> createJSMethodRaw(FunctionCallback func, JsValue data);

    /////////js对象的基类////////////////
    class JSObjBaseV8 {
        friend class JsObjHandle;
	protected:
        JSObjBaseV8();   
	public:
        virtual ~JSObjBaseV8();

        void makeStrong();			//防止自己被刪除，例如做異步任務的時候。

        void makeWeak();			//可以被刪除了。

		void initialize(Isolate* isolate, v8::Local<v8::Object> object);

		static void WeakCallback(const v8::WeakCallbackInfo<JSObjBaseV8>& data)
		{
			JSObjBaseV8* obj = data.GetParameter();
			obj->m_persistent_handle.Reset();
			delete obj;
		}

        //检查js环境是否变化了，一般用来异步回调的保护
        bool IsMyJsEnv() 
		{
            return Isolate::GetCurrent() == m_isolate;
        }

        //通过jsthis来调用函数
        inline JsValue _callJsFunc(JsFunction& func, int argc, JsValue argv[])
		{
            EscapableHandleScope sc(m_isolate);
            Local<Object> localthis = Local<Object>::New(m_isolate, m_persistent_handle);
			return sc.Escape(func->Call(m_isolate->GetCurrentContext(), localthis, argc, argv).FromMaybe(Local<Value>()));
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
	protected:
        //js调用当前函数的时候的参数。主要是有时候想要获得一些具体的变量，
        //所有的公用这一个，所以是非线程安全的
        Isolate*				m_isolate;
		v8::Global<v8::Object>	m_persistent_handle;
    };

    class JsObjHandle
	{
    public:
        JsObjHandle()
		{
            m_pObj = NULL;
            m_nID = 0;
        }
        JSObjBaseV8*	m_pObj;
        int				m_nID;		//數組下標
		v8::Global<v8::Object>	m_persistent_handle;

        bool Empty();

        JsValue getJsObj()
		{
			return v8::Local<v8::Object>::New(Isolate::GetCurrent(), m_persistent_handle);
        }

		JsObjHandle(const JsObjHandle&) = delete;

		JsObjHandle& operator=(const JsObjHandle&) = delete;

        void set(int id, JSObjBaseV8* pobj, JsValue value)
		{
            m_pObj = pobj;
            m_nID = id;
			this->m_persistent_handle.Reset(Isolate::GetCurrent(), value.As<v8::Object>());
			this->m_persistent_handle.ClearWeak();
        }

		void set(JsValue value)
		{
			this->m_persistent_handle.Reset(Isolate::GetCurrent(), value.As<v8::Object>());
			this->m_persistent_handle.ClearWeak();
		}

        template <typename _Tp>
        static bool IsTypeof(JsValue val)
		{
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

        /*template <typename _Tp>
        bool IsTypeof() {
            JsValue val = m_pObj->getRefObj(m_nID);
            return IsTypeof<_Tp>(val);
        }*/


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

        /*static bool tryGetStrVector(JsValue& val, std::vector<std::string>& pRet) {
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
        }*/

        void Reset() {
			if (!m_persistent_handle.IsEmpty())
			{
				m_persistent_handle.ClearWeak();
				m_persistent_handle.Reset();
			}
        }

        bool IsFunction() {
            return getJsObj()->IsFunction();
        }

		#define CALLJSPRE \
			if (!m_pObj)return false;\
			HandleScope sc(m_pObj->m_isolate);\
			JsValue func = getJsObj();\
			if (!func->IsFunction())\
			return false;\
			Local<Function> jsfun = Local<Function>::Cast(func);

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
		bool Call(v8::Local<v8::Object> objTHIS, P1 p1, P2 p2){
			v8::HandleScope sc(v8::Isolate::GetCurrent());
			JsValue func = getJsObj(); 
			if (!func->IsFunction())
				return false; 
			v8::Local<v8::Function> jsfun = v8::Local<v8::Function>::Cast(func);
		
			int argc = 2;
			v8::Local<Value> argv[2];
			argv[0] = __TransferToJs<P1>::ToJs(p1);
			argv[1] = __TransferToJs<P2>::ToJs(p2);

			jsfun->Call(v8::Isolate::GetCurrent()->GetCurrentContext(), objTHIS, argc, argv).ToLocalChecked();
			return true;
		}

		template <typename P1, typename P2, typename P3>
		bool Call(v8::Local<v8::Object> objTHIS, P1 p1, P2 p2, P3 p3)
		{
			v8::HandleScope sc(v8::Isolate::GetCurrent());
			JsValue func = getJsObj();
			if (!func->IsFunction())
				return false;
			v8::Local<v8::Function> jsfun = v8::Local<v8::Function>::Cast(func);

			int argc = 3;
			Local<Value> argv[3];
			argv[0] = __TransferToJs<P1>::ToJs(p1);
			argv[1] = __TransferToJs<P2>::ToJs(p2);
			argv[2] = __TransferToJs<P2>::ToJs(p3);

			jsfun->Call(v8::Isolate::GetCurrent()->GetCurrentContext(), objTHIS, argc, argv).ToLocalChecked();
			return true;
		}
		template <typename P1, typename P2, typename P3, typename R>
		bool CallWithReturn(v8::Local<v8::Object> objTHIS, P1 p1, P2 p2, P3 p3, R& r)
		{
			v8::HandleScope sc(v8::Isolate::GetCurrent());
			JsValue func = getJsObj();
			if (!func->IsFunction())
				return false;
			v8::Local<v8::Function> jsfun = v8::Local<v8::Function>::Cast(func);

			int argc = 3;
			v8::Local<v8::Value> argv[3];
			argv[0] = __TransferToJs<P1>::ToJs(p1);
			argv[1] = __TransferToJs<P2>::ToJs(p2);
			argv[2] = __TransferToJs<P2>::ToJs(p3);

			r = __TransferToCpp<R>::ToCpp(jsfun->Call(v8::Isolate::GetCurrent()->GetCurrentContext(), objTHIS, argc, argv).ToLocalChecked());
			return true;
		}
	};

#define JSP_GLOBAL_OBJECT \
	v8::Isolate::GetCurrent()->GetCurrentContext()->Global()

	//为了与原来的差不多的宏。 只能在对象的实例中使用。必须按照下面的顺序调用。
	//第一个name是类名，统计用的
#define JSP_CLASS(name,cls) \
	v8::Isolate* pJsIso = v8::Isolate::GetCurrent(); \
	v8::HandleScope sc(pJsIso);	\
	v8::Local<v8::FunctionTemplate> xhrtemp = v8::FunctionTemplate::New(pJsIso, JSCClass<cls>::JsConstructor);	\
	xhrtemp->SetClassName(Js_Str(pJsIso, name));	\
	v8::Local<v8::ObjectTemplate> instancTemp = xhrtemp->InstanceTemplate();	\
	instancTemp->SetInternalFieldCount(2); \
	IsolateData* data = IsolateData::From(pJsIso); \
	data->SetObjectTemplate(&cls::JSCLSINFO, instancTemp); \
	v8::Local<v8::Context> pCtx = pJsIso->GetCurrentContext(); 

//这个的区别是不能直接创建c对象，而是绑定一个已经存在的对象。
#define JSP_GLOBAL_CLASS(name,cls,inst) \
	v8::Isolate* pJsIso = Isolate::GetCurrent(); \
	v8::HandleScope sc(pJsIso);	\
	v8::Local<v8::Context> pCtx = pJsIso->GetCurrentContext(); \
	v8::Local<v8::FunctionTemplate> xhrtemp = v8::FunctionTemplate::New(pJsIso);	\
	xhrtemp->SetClassName(Js_Str(pJsIso, name));	\
    v8::Local<v8::ObjectTemplate> objectTemplate = xhrtemp->InstanceTemplate();	\
	objectTemplate->SetInternalFieldCount(2); \
	IsolateData* data = IsolateData::From(pJsIso); \
	data->SetObjectTemplate(&cls::JSCLSINFO, objectTemplate); \
	v8::Local<v8::Object> instance = objectTemplate->NewInstance(pCtx).ToLocalChecked(); \
	instance->SetAlignedPointerInInternalField(0, inst);	\
	instance->SetAlignedPointerInInternalField(1, &cls::JSCLSINFO);	\
	inst->initialize(pJsIso, instance);
	


#define JSP_REG_CONSTRUCTOR(cls,...) \
    JSCClass<cls>::getInstance()->addConstructor(regConstructor<cls,##__VA_ARGS__>());

#define JSP_ADD_FIXED_PROPERTY(name,cls,val) \
    instancTemp->Set(Js_Str(pJsIso, #name), __TransferToJs<decltype(val)>::ToJs(val));

#define JSP_ADD_METHOD(name,fn)    \
	addJSMethod(name,xhrtemp, &fn);

/*#define JSP_GLOBAL_ADD_METHOD(name,fn)    \
	SetInstanceMethod(name, xhrtemp, &fn);*/

#define JSP_GLOBAL_ADD_METHOD(name,fn)    \
	SetMethod(name, instance, &fn, xhrtemp);

#define JSP_ADD_METHODRAW(name, func, data) \
	instancTemp->Set(Js_Str(pJsIso, name), createJSMethodRaw(func,data));


#define JSP_INSTALL_CLASS(name,cls)    \
	pCtx->Global()->Set(pCtx, Js_Str(pJsIso, name), xhrtemp->GetFunction(pCtx).ToLocalChecked()); \
    JSClassMgr::GetInstance()->registerResetFunction(JSCClass<cls>::reset);

//注意，下面的inst是返回的。
#define JSP_INSTALL_GLOBAL_CLASS(name,cls,inst)	\
	pCtx->Global()->Set(pCtx, Js_Str(pJsIso, name), instance);

//添加一个只读属性。get是函数地址，funcplace是一个存放get和set函数地址的结构的指针
#define JSP_ADD_PROPERTY_RO(name,cls,get)	\
	static PropFuncInfo<decltype(&cls::get),decltype(&cls::get)> propFunc_R##cls##name(&cls::get);	\
	instancTemp->SetAccessor(Js_Str(pJsIso,#name), \
		imp_JsGetProp<decltype(&cls::get),decltype(&cls::get)>::call,0,	\
		External::New(pJsIso, (void*)&propFunc_R##cls##name),v8::DEFAULT, v8::ReadOnly);

#define JSP_GLOBAL_ADD_PROPERTY_RO(name,cls,get)	\
	instance->SetAccessorProperty(Js_Str(pJsIso,#name), createJSMethod(#name, &cls::get));


#define JSP_ADD_PROPERTY(name,cls,get,set)	\
	static PropFuncInfo<decltype(&cls::get),decltype(&cls::set)> propFunc_##cls##name(&cls::get, &cls::set);	\
	instancTemp->SetAccessor(Js_Str(pJsIso,#name),		\
			imp_JsGetProp<decltype(&cls::get),decltype(&cls::set)>::call,imp_JsSetProp<decltype(&cls::get),decltype(&cls::set)>::call,	\
			External::New(pJsIso, (void*)&propFunc_##cls##name));


#define JSP_GLOBAL_ADD_PROPERTY(name,cls,get,set)	\
	instance->SetAccessorProperty(Js_Str(pJsIso,#name), createJSMethod(#name, &cls::get), createJSMethod(#name, &cls::set));


#define JSP_GLOBAL_START1()	\
	v8::Isolate* pIso = v8::Isolate::GetCurrent();	\
	v8::HandleScope sc(pIso);	\
	v8::Local<v8::Context> pCtx = pIso->GetCurrentContext(); \
	v8::Local<v8::Object> gctx = pCtx->Global();

//func必须是全局函数，不允许成员函数。为了简单，还是与原来的兼容把
#define JSP_ADD_GLOBAL_FUNCTION(name, func, ...) \
	gctx->Set(pCtx, Js_Str(pIso, #name), createJSMethod(#name, func));

#define JSP_ADD_GLOBAL_PROPERTY(name, v)	\
	gctx->Set(pCtx, Js_Str(pIso, #name), ToJSValue(v));


/////////////////////////////////////////////////////////

//检查参数是否>=num个
#define CHECK_ARGUMENTS_COUNT(num) \
	int len = args.Length(); \
	if (len < num) \
	{ \
		args.GetIsolate()->ThrowException(v8::String::NewFromUtf8(args.GetIsolate(), "arguments count error").ToLocalChecked()); \
		return; \
	}


#define JS2CCALL_INIT \
	void* pExData = External::Cast(*args.Data())->Value();\
	FuncInfo<funcType>* funcInfo = (FuncInfo<funcType>*)pExData;\
	Local<Object> pthis = args.This();

#define JS2CCALL_GET_COBJ \
	Cls *pObj = (Cls*)pthis->GetAlignedPointerFromInternalField(0);

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
		static void call(const FunctionCallbackInfo<Value>& args) {
			JS2CCALL_INIT;
			JS2CCALL_GET_COBJ;
			args.GetReturnValue().Set(ToJSValue<R>((pObj->*funcInfo->func)()));
		}
	};

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

            pthis->SetAlignedPointerInInternalField(0, obj);
            pthis->SetAlignedPointerInInternalField(1, (void*)&T::JSCLSINFO);
			obj->initialize(pIso, pthis);
			obj->makeWeak();
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