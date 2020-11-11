
#ifndef _JSCUtil_h
#define _JSCUtil_h

#include <unordered_map>
#include <string>
#include <JavaScriptCore/JavaScriptCore.h>

namespace laya
{
    class JSCUtil
    {
    public:
        static JSContextRef s_ctx;
        static bool extractJSAB(JSContextRef ctx,JSValueRef jsval, char*& data, int& len);
        static char* toCString(JSContextRef ctx,JSValueRef value);
        static char* toCString(JSContextRef ctx,JSStringRef value);
    };
    
    template<typename T>
    class JSCBinder
    {
    public:
        inline unsigned int __hash_BKDR(const char *p_str)
        {
            if(0 == p_str)
                return 0;
        
            unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
            unsigned int hash = 0;
        
            for(;0!=*p_str;)
            {
                hash = hash * seed + (*p_str++);
            }
        
            return (hash & 0x7FFFFFFF);
        }
        JSCBinder()
        {
            m_bIsGlobal = false;
            m_ClassDefine = kJSClassDefinitionEmpty;
            m_ClassObject = nullptr;
            m_iTypeID = __hash_BKDR(typeid(T).name());
        }
        ~JSCBinder()
        {
            _reset();
        }
        unsigned int getTypeID()
        {
            return m_iTypeID;
        }
	
        void addProperty(const std::string& name, JSObjectGetPropertyCallback getter, JSObjectSetPropertyCallback setter = NULL)
        {
            assert(!name.empty());
            if (getter)
            {
                GetPropertyMapRes rs = m_GetPropertyMap.insert(GetPropertyMapValue(name, getter));
                assert(rs.second);
            }
            if (setter)
		    {
                SetPropertyMapRes rs = m_SetPropertyMap.insert(SetPropertyMapValue(name, setter));
			    assert(rs.second);
		    }
        }
        void begin(JSContextRef ctx)
        {
            JSCUtil::s_ctx = ctx;
        }
        void addMethod(const std::string& name, JSObjectCallAsFunctionCallback method)
        {
            JSStringRef pName = JSStringCreateWithUTF8CString(name.c_str());
            JSContextRef pCtx = JSCUtil::s_ctx;
            JSValueRef callAsFunction = JSObjectMakeFunctionWithCallback(pCtx, pName, method);
            JSStringRelease(pName);
            JSValueProtect(pCtx, callAsFunction);
            FunctionMapRes rsf = m_FunctionMap.insert(FunctionMapValue(name, callAsFunction));
            assert(rsf.second);
        
        }
        void end(JSObjectRef object, const std::string& name)
        {
            m_bIsGlobal = false;
            endImpl(object, name, nullptr);
        }
        void endToGlobal(JSObjectRef object, const std::string& name, T* pIns)
	    {
            assert(pIns != nullptr);
            m_bIsGlobal = false;
            endImpl(object, name, pIns);
            m_bIsGlobal = true;
        }
        JSObjectRef transferObjPtrToJS(JSContextRef pCtx, T* p_pIns)
        {
            assert( !m_bIsGlobal );
            JSObjectRef pRet = JSObjectMake(pCtx, m_ClassObject, p_pIns);
            JSValueRef pProperty = JSValueMakeNumber(pCtx, (double)getTypeID());
            JSStringRef pszName = JSStringCreateWithUTF8CString("jsc__cppclstypeid");
            JSObjectSetProperty(pCtx, pRet, pszName, pProperty, kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete, 0);
            JSStringRelease(pszName);
            return pRet;
        }
        static JSCBinder<T>* GetInstance()
        {
            if (!s_instance)
            {
                s_instance = new JSCBinder<T>();
            }
            return s_instance;
        }
        static void ReleaseInstance()
        {
            if (s_instance)
            {
                delete s_instance;
                s_instance = NULL;
            }
        }
    private:
        static JSObjectRef newWrap(JSContextRef ctx, JSObjectRef constructor, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
	    {
            if( JSCBinder<T>::GetInstance()->m_bIsGlobal )
		    {
                return NULL;
            }
            return JSCBinder<T>::GetInstance()->transferObjPtrToJS(ctx, new T);
        }
    
        static void destroyWrap(JSObjectRef object)
        {
            T *p = (T *)JSObjectGetPrivate(object);
            delete p;
        }
    
        static bool isInstanceOf(JSContextRef ctx, JSObjectRef constructor, JSValueRef possibleInstance, JSValueRef* exception)
        {
            if(!JSValueIsObject(ctx, possibleInstance))
            {
                return false;
            }
		
            JSObjectRef p_pObj = JSValueToObject(ctx, possibleInstance ,NULL);
            JSStringRef pszName = JSStringCreateWithUTF8CString("jsc__cppclstypeid");
            JSValueRef pProperty = JSObjectGetProperty(ctx, p_pObj, pszName, 0);
            JSStringRelease(pszName);
            
            if(0 == pProperty)
            {
                return false;
            }
            else
            {
                
                int nID = (int)JSValueToNumber(ctx,pProperty, 0);
                return (nID == (JSCBinder<T>::GetInstance()->getTypeID()));
            }
        }

        static bool hasProperty(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName)
        {
            char* strPropertyName = JSCUtil::toCString(ctx, propertyName);
        
            if (JSCBinder<T>::GetInstance()->findFunction(strPropertyName))
            {
                return true;
            }
		
            if (JSCBinder<T>::GetInstance()->findGetProperty(strPropertyName))
            {
                return true;
            }
		
            if (JSCBinder<T>::GetInstance()->findSetProperty(strPropertyName))
            {
                return true;
            }
            return false;
        }
        static JSValueRef getProperty(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception)
        {
            T* pThis = (T*)JSObjectGetPrivate(object);
        
            if(NULL == pThis )
            {
                return NULL;
            }
        
            char* strPropertyName = JSCUtil::toCString(ctx,  propertyName);
            
        
            JSObjectGetPropertyCallback callAsFunction = JSCBinder<T>::GetInstance()->findGetProperty(strPropertyName);
        
            if(callAsFunction)
            {
                return callAsFunction(ctx, object, propertyName, exception);
            }
        
            return JSCBinder<T>::GetInstance()->findFunction(strPropertyName);
        }
        static bool setProperty(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef value, JSValueRef* exception)
        {
            T* pThis = (T*)JSObjectGetPrivate(object);
        
            if(0 == pThis)
            {
                return false;
            }
            
            char* strPropertyName = JSCUtil::toCString(ctx, propertyName);
            
            JSObjectSetPropertyCallback callAsFunction = JSCBinder<T>::GetInstance()->findSetProperty(strPropertyName);
        
        
            if( NULL == callAsFunction)
            {
                return false;
            }
   
            JSValueRef arguments[1];
            arguments[0] = value;
            callAsFunction(ctx, object, propertyName, value, exception);
            return true;
        }
    private:
        void _reset()
        {
            JSContextRef ctx = JSCUtil::s_ctx;
            for (FunctionMapItr itr = m_FunctionMap.begin(); itr != m_FunctionMap.end(); itr++)
            {
                JSValueUnprotect(ctx, itr->second);
            }
            m_FunctionMap.clear();
		
            m_SetPropertyMap.clear();
            m_GetPropertyMap.clear();
		
            m_bIsGlobal = false;
            m_ClassDefine = kJSClassDefinitionEmpty;
        
            if (m_ClassObject != nullptr)
            {
                JSClassRelease(m_ClassObject);
                m_ClassObject = nullptr;
            }
        }
        void endImpl(JSObjectRef object, const std::string& name, T *p_pIns)
        {
            assert(!name.empty());
            m_ClassDefine = kJSClassDefinitionEmpty;
            m_ClassDefine.attributes = kJSClassAttributeNone;
            m_ClassDefine.className = name.c_str();
            m_ClassDefine.callAsConstructor = JSCBinder<T>::newWrap;
            m_ClassDefine.finalize = JSCBinder<T>::destroyWrap;
            m_ClassDefine.hasProperty = JSCBinder<T>::hasProperty;
            m_ClassDefine.hasInstance = JSCBinder<T>::isInstanceOf;
            m_ClassDefine.getProperty = JSCBinder<T>::getProperty;
            m_ClassDefine.setProperty = JSCBinder<T>::setProperty;
            //m_ClassDefine.callAsFunction = JSCBinder<T>::callAsFunctionCallback;
        
            m_ClassObject = JSClassCreate(&m_ClassDefine);
            JSContextRef pCtx = JSCUtil::s_ctx;
            JSStringRef jsName = JSStringCreateWithUTF8CString(name.c_str());
            JSObjectRef myObject;
            if( 0 != p_pIns )
            {
                myObject = transferObjPtrToJS(pCtx, p_pIns);
            }
            else
            {
                myObject = JSObjectMake(pCtx, m_ClassObject, 0);
            }
   
            JSObjectSetProperty( pCtx, object, jsName, myObject, kJSPropertyAttributeNone, NULL );
            JSStringRelease(jsName);
        }
        JSValueRef findFunction(const std::string& name)
        {
            FunctionMapItr iter = m_FunctionMap.find(name);
            if(iter == m_FunctionMap.end())
            {
                return nullptr;
            }
            else
            {
                return (*iter).second;
            }
        }
        JSObjectGetPropertyCallback findGetProperty(const std::string& name)
        {
            GetPropertyMapItr iter = m_GetPropertyMap.find(name);
            if(iter == m_GetPropertyMap.end())
            {
                return nullptr;
            }
            else
            {
                return (*iter).second;
            }
        }
        JSObjectSetPropertyCallback findSetProperty(const std::string& name)
        {
            SetPropertyMapItr iter = m_SetPropertyMap.find(name);
            if(iter == m_SetPropertyMap.end())
            {
                return nullptr;
            }
            else
            {
                return (*iter).second;
            }
        }
    private:
                                                            
        typedef std::unordered_map<std::string, JSObjectGetPropertyCallback> GetPropertyMap;
        typedef typename GetPropertyMap::value_type GetPropertyMapValue;
        typedef typename GetPropertyMap::iterator GetPropertyMapItr;
        typedef std::pair<GetPropertyMapItr, bool> GetPropertyMapRes;
        GetPropertyMap m_GetPropertyMap;
        
        typedef std::unordered_map<std::string, JSObjectSetPropertyCallback> SetPropertyMap;
        typedef typename SetPropertyMap::value_type SetPropertyMapValue;
        typedef typename SetPropertyMap::iterator SetPropertyMapItr;
        typedef std::pair<SetPropertyMapItr, bool> SetPropertyMapRes;
        SetPropertyMap m_SetPropertyMap;
	
        typedef std::unordered_map<std::string, JSValueRef> FunctionMap;
        typedef typename FunctionMap::value_type FunctionMapValue;
        typedef typename FunctionMap::iterator FunctionMapItr;
        typedef std::pair<FunctionMapItr, bool> FunctionMapRes;
        FunctionMap m_FunctionMap;
    
        bool m_bIsGlobal;
        JSClassDefinition m_ClassDefine;
        JSClassRef m_ClassObject;
        unsigned int m_iTypeID;
        static JSCBinder<T>* s_instance;
    };

    template<typename T> JSCBinder<T>* JSCBinder<T>::s_instance = NULL;
}
#endif
