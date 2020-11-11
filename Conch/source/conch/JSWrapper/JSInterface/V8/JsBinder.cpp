#ifdef JS_V8
#include "JsBinder.h"
#include "../JSInterface.h"

using namespace v8;

namespace laya {

    thread_local JSClassMgr JSClassMgr::__Ins;

    void imp_js2cfunc_common(const FunctionCallbackInfo<Value>& args) {
        void* pExData = External::Cast(*args.Data())->Value();
        Local<Object> pthis = args.This();
        if (pthis->InternalFieldCount() > 1) {
            JsObjClassInfo* id = (JsObjClassInfo*)Handle<External>::Cast(pthis->GetInternalField(1))->Value();
            void *pObj = Handle<External>::Cast(pthis->GetInternalField(0))->Value();
            int a = 0;
        }
        JSP_RUN_SCRIPT("printstack();");
        throw - 1;//不能调到这里									//支持基本类型，或者Value
    }

    bool checkJSToCArgs(const FunctionCallbackInfo<Value>& args, int num) {
        int len = args.Length();
        if (len >= num)
            return true;
        static char buff[512];
        /*算了，别alert了，log把
        sprintf(buff, "alert('arguments number err: %d:%d');printstack();", num, len);
        JSP_RUN_SCRIPT(buff);
        throw - 1;//不能调到这里									//支持基本类型，或者Value
        */
        sprintf(buff, "console.log('arguments number err: %d:%d');var e = new Error();console.log(e.stack);", num, len);
        JSP_RUN_SCRIPT(buff);
        return false;
    }

    Local<Function> createJSMethodRaw(FunctionCallback func, JsValue data) {
        Isolate* pIso = Isolate::GetCurrent();
        Local<FunctionTemplate> method = FunctionTemplate::New(pIso);
        method->SetCallHandler(func, data);
        return method->GetFunction();
    }


    JSObjBaseV8::JSObjBaseV8() {
        mpRefArray = NULL;
        mpCurJsArgs = NULL;
        mpJsThis = NULL;
        mnRefArrSize = 0;
        mbWeakThis = true;
        //放在构造里面，容易导致没有js环境而非法。还是往后放放吧。
        mpJsIso = NULL;
        //mpJsIso = Isolate::GetCurrent();
    }

    JSObjBaseV8::~JSObjBaseV8() {
        if (!mbWeakThis) {
            mpJsThis->Reset();
            delete mpJsThis;
            mpJsThis = NULL;
        }
    }

    void JSObjBaseV8::destroyWeakCB(const WeakCallbackInfo<Persistent<Object> >& data) {
        HandleScope __lHandleScope(Isolate::GetCurrent());
        Persistent<Object> *pPersistentVal = data.GetParameter();
        //Handle<Object> pNewIns = Local<Object>::New(Isolate::GetCurrent(), *pPersistentVal);
        //T *ptr = (T *)Handle<External>::Cast(pNewIns->GetInternalField(0))->Value();
        //delete ptr;
        pPersistentVal->Reset();
        delete pPersistentVal;
    }

    void JSObjBaseV8::destroyWeakRefArray(const WeakCallbackInfo<Persistent<Array> >& data) {
        Persistent<Array>* pData = data.GetParameter();
        //JSObjBaseV8*  pObj = pData->pObj;
        if (pData == NULL)
            return;
        //Isolate* piso = Isolate::GetCurrent();
        //HandleScope __lHandleScope(piso);
        /*
        Local<Array> refArr = pData->Get(piso);
        int sz = refArr->Length();
        for (int i = 0; i < sz; i++) {
            refArr->Delete(piso->GetCurrentContext(), i);
        }
        */
        pData->Reset();
        delete pData;
    }

    void JSObjBaseV8::retainThis() {
        //我觉得这个其实应该不用。
        mRetainedThis.Reset(mpJsIso, mpJsThis->Get(mpJsIso));
    }

    void JSObjBaseV8::releaseThis() {
        mRetainedThis.Reset();
    }

    void JSObjBaseV8::createJSObj() {
        if (!mpJsIso)mpJsIso = Isolate::GetCurrent();
        Local<Object> jsobj = Local<Object>::New(mpJsIso, Object::New(mpJsIso));
        mpJsThis = new Persistent<Object>(mpJsIso, jsobj);
        mbWeakThis = false;
        createRefArray();
    }

    /*
    void JSObjBaseV8::addRefHandle(JsObjHandle2* handle) {
        handle->m_pObj = this;
        handle->m_nID = mnRefArrSize++;
    }
    */
    //創建一個數組，用於保存引用的js對象。
    void JSObjBaseV8::createRefArray() {
        mpJsIso = Isolate::GetCurrent();
        Local<Array> refarr = Array::New(mpJsIso, mnRefArrSize);
        int le = refarr->Length();
        Local<Object> jsthis = Local<Object>::New(mpJsIso, *mpJsThis);
        //需要把它加到this的引用中，防止这个数组被删除掉。
        jsthis->Set(Js_Str(mpJsIso, "__internal_refArray"), refarr);
        //这个是为了访问方便。可能去掉更好。
        mpRefArray = new Persistent<Array>(mpJsIso, refarr);
        //由于自己有引用，所以不用再在回调中删掉了。
        mpRefArray->SetWeak(mpRefArray, JSObjBaseV8::destroyWeakRefArray, v8::WeakCallbackType::kInternalFields);
    }

    void JSObjBaseV8::setRefObj(int idx, JsValue obj) {
        if (!mpRefArray)
            return;
        if (mnRefArrSize <= idx)mnRefArrSize = idx + 1;
        Local<Array> refArr = mpRefArray->Get(mpJsIso);
        refArr->Set(idx, obj);
    }

    Local<Object> JSObjBaseV8::getRefObj(int idx) {
        Local<Array> refArr = mpRefArray->Get(mpJsIso);
        return refArr->Get(idx).As<Object>();
    }

    Persistent<Object>* JSObjBaseV8::weakHoldJsObj(Local<Object>& pobj) {
        Persistent<Object>* pObj = new Persistent<Object>(mpJsIso, pobj);
        pObj->SetWeak(pObj, destroyWeakCB, v8::WeakCallbackType::kInternalFields);
        return pObj;
    }

    JsValue JSObjBaseV8::callJsFunc(JsFunction& func) {
        int argc = 0;
        Local<Value> argv[1];
        return _callJsFunc(func, argc, argv);
    }

    bool JsObjHandle2::Empty() {
        if (!m_pObj)
            return true;
        return m_pObj->getRefObj(m_nID).IsEmpty();
    }

    void JsObjHandle2::set(JsValue& v) {
        if (m_pObj == NULL) {
            LOGE("throw set jsvalue m_pObj==null ");
            throw - 4;
        }
        m_pObj->setRefObj(m_nID, v);
    }
}
#endif