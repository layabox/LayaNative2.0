
#ifndef _JSCProxyClass_h
#define _JSCProxyClass_h

#include <unordered_map>
#include "JSCProxyTransfer.h"
#include "JSCProxyFunction.h"
#include "JSCProxyType.h"


namespace laya
{
    
class JSCGlobal
{
public:
    JSCGlobal(){}
    ~JSCGlobal(){
        reset();
    }
    template <typename T>
    void  addProperty( const std::string& name, T value ){
        assert( !name.empty());
        JSContextRef pCtx = __TlsData::GetInstance()->GetCurContext();
        JSStringRef pJSStrName = JSStringCreateWithUTF8CString( name.c_str() );
        JSValueRef pVal = __TransferToJs<T>::ToJs( value );
        JSObjectSetProperty( pCtx, JSContextGetGlobalObject( pCtx ), pJSStrName, pVal, kJSPropertyAttributeDontDelete|kJSPropertyAttributeReadOnly, nullptr );
        JSStringRelease( pJSStrName );
    }
    template <typename T>
    void addFunction( const std::string& name, T fun ){
        assert( !name.empty() );
        IJSCFunction* pJSCFunction = new JSCFunction<T>( fun );
        JSContextRef pCtx = __TlsData::GetInstance()->GetCurContext();
        JSStringRef pjsName = JSStringCreateWithUTF8CString( name.c_str() );
        JSObjectRef pFunc = JSObjectMakeFunctionWithCallback(pCtx, pjsName, JSObjectCallAsFunctionCallback);
        JSObjectSetProperty(pCtx, JSContextGetGlobalObject(pCtx), pjsName, pFunc, kJSPropertyAttributeDontDelete|kJSPropertyAttributeReadOnly, nullptr);
        JSStringRelease( pjsName );
        FunctionMapRes rs = m_FunctionMap.insert(FunctionMapValue((unsigned long)pFunc, pJSCFunction));
        assert( rs.second );
    }
    IJSCFunction* getFunction(unsigned long func){
        FunctionMapItr itrFun = m_FunctionMap.find( func );
        if ( itrFun != m_FunctionMap.end() ){
            return itrFun->second;
        }
        else {
            return nullptr;
        }
    }
    static JSValueRef JSObjectCallAsFunctionCallback(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        IJSCFunction* pJSCFunction = JSCGlobal::getInstance()->getFunction( (unsigned long)function );
        if ( pJSCFunction == nullptr ){
            __JsThrow::Throw(exception,"JSCGlobal::JSObjectCallAsFunctionCallback can't find function");
            return nullptr;
        }
        return pJSCFunction->call(ctx, function, thisObject, argumentCount, arguments, exception);
    }
    static JSCGlobal* getInstance(){
        static JSCGlobal instance;
        return &instance;
    }
    void reset(){
        FunctionMapItr iter = m_FunctionMap.begin();
        for (; iter != m_FunctionMap.end(); iter++){
            delete iter->second;
        }
        m_FunctionMap.clear();
    }
private:
    typedef std::unordered_map<unsigned long,IJSCFunction*> FunctionMap;
    typedef FunctionMap::value_type FunctionMapValue;
    typedef FunctionMap::iterator FunctionMapItr;
    typedef std::pair<FunctionMapItr,bool> FunctionMapRes;
    FunctionMap m_FunctionMap;
};

template<typename T>
class JSCClass
{
public:
    inline unsigned int __hash_BKDR(const char *p_str){
        if( 0 == p_str )
            return 0;
        
        unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
        unsigned int hash = 0;
        
        for(;0!=*p_str;){
            hash = hash * seed + (*p_str++);
        }
        
        return (hash & 0x7FFFFFFF);
    }
    class FuncEntry
    {
        enum
        {
            Max_Arg_Size  = 12,
            Invalid_size = -1,
        };
        IJSCCallback *m_funcs[Max_Arg_Size+1];
        int m_iMaxArgSize;
        
    public:
        FuncEntry(){
            m_iMaxArgSize = Invalid_size;
            memset( m_funcs, 0, sizeof(m_funcs) );
        }
        
        ~FuncEntry(){
            reset();
        }
        
        void reset(){
            for(int i=0;i<=Max_Arg_Size;++i){
                if( 0 != m_funcs[i] ){
                    delete m_funcs[i];
                    m_funcs[i] = 0;
                }
            }
        }
        
        void add( IJSCCallback *p_fn ){
            assert(p_fn != nullptr && p_fn->getNumArgs() <= Max_Arg_Size && m_funcs[p_fn->getNumArgs()] == nullptr );
            if( m_iMaxArgSize < p_fn->getNumArgs() )
                m_iMaxArgSize = p_fn->getNumArgs();
            m_funcs[p_fn->getNumArgs()] = p_fn;
        }
        
        IJSCCallback *get( int p_iArgNum ){
            if( Invalid_size == m_iMaxArgSize ){
                return 0;
            }
            
            if( p_iArgNum > m_iMaxArgSize )
                p_iArgNum = m_iMaxArgSize;
            
            for(;p_iArgNum>=0;p_iArgNum--){
                if( 0 != m_funcs[p_iArgNum] )
                    return m_funcs[p_iArgNum];
            }
            
            return 0;
        }
    };
    enum
    {
        CallbackType_Property,
        CallbackType_Function,
    };
    struct CallbackDefine
    {
        int m_iType;
        std::string m_name;
        IJSCCallback* m_pSetter;
        IJSCCallback* m_pGetter;
        
        FuncEntry m_Method;
        JSValueRef m_pCallAsFunction;
        
        CallbackDefine(const std::string& name, IJSCCallback* setter, IJSCCallback* getter)
        :m_name(name), m_iType(CallbackType_Property), m_pSetter(setter), m_pGetter(getter),m_pCallAsFunction(nullptr){
        }
        
        CallbackDefine(const std::string& name,IJSCCallback* method)
        :m_name(name), m_iType(CallbackType_Function), m_pSetter(nullptr), m_pGetter(nullptr),m_pCallAsFunction(nullptr){
            m_Method.add(method);
        }
        
        void addMethod( IJSCCallback *p_pfn ){
            m_Method.add(p_pfn);
        }
        
        IJSCCallback *getMethod( size_t p_iArgNum ){
            return m_Method.get( (int)p_iArgNum );
        }
        ~CallbackDefine(){
            
            if (m_pGetter){
                delete m_pGetter;
                m_pGetter = nullptr;
            }
            
            if (m_pSetter){
                delete m_pSetter;
                m_pSetter = nullptr;
            }
            
            m_Method.reset();
            JSContextRef ctx = __TlsData::GetInstance()->GetCurContext();
            if (ctx != nullptr && m_pCallAsFunction != nullptr){
                JSValueUnprotect(ctx, m_pCallAsFunction);
            }
        }
        
    };
    struct FixedProperty
    {
    private:
        JSValueRef m_pszName;
        JSValueRef m_pValue;
        
    public:
        template <typename _Tp>
        explicit FixedProperty( const char *p_pszName, _Tp p_Val )
        {
            JSContextRef pCtx = __TlsData::GetInstance()->GetCurContext();
            
            JSStringRef pszName = JSStringCreateWithUTF8CString(p_pszName);
            m_pszName = JSValueMakeString(pCtx, pszName);
            JSStringRelease(pszName);
            
            m_pValue = __TransferToJs<_Tp>::ToJs(p_Val);
            
            JSValueProtect( pCtx, m_pszName );
            JSValueProtect( pCtx, m_pValue );
        }
        
        ~FixedProperty()
        {
            JSContextRef pCtx = __TlsData::GetInstance()->GetCurContext();
            if( 0 != pCtx )
            {
                if( 0 != m_pszName )
                {
                    JSValueUnprotect( pCtx, m_pszName );
                }
                if( 0 != m_pValue )
                {
                    JSValueUnprotect( pCtx, m_pValue );
                }
            }
        }
        
        JSStringRef GetName()
        {
            return JSValueToStringCopy(__TlsData::GetInstance()->GetCurContext(), m_pszName, 0);
        }
        
        JSValueRef GetValue()
        {
            return m_pValue;
        }
    };
    typedef std::vector<FixedProperty *> FixedProperties;
    typedef typename FixedProperties::iterator FixedPropertiesIter;
        
    FixedProperties m_FixedProperties;
    
    JSCClass(){
        m_bIsGlobal = false;
        m_ClassDefine = kJSClassDefinitionEmpty;
        m_ClassObject = nullptr;
        m_iTypeID = __hash_BKDR(typeid(T).name());
    }
    ~JSCClass(){
        _reset();
    }
    unsigned int getTypeID(){
        return m_iTypeID;
    }
    static JSCClass* getInstance(){
        static JSCClass<T> instance;
        return &instance;
    }
    template <typename P>
    void addFixedProperty(const std::string& name,const P& val){
        assert(!name.empty());
        m_FixedProperties.push_back(new FixedProperty(name.c_str(),val));
    }
    template <typename G,typename S>
    void addProperty( const std::string& name, G getter, S setter){
        assert( !name.empty() );
        IJSCCallback* pGetter = new JSCCallback<G>(getter);
        IJSCCallback* pSetter = nullptr;
        if (setter != nullptr){
            pSetter = new JSCCallback<S>(setter);
        }
        PropertyMapRes rs = m_PropertyMap.insert(PropertyMapValue(name,new CallbackDefine(name,pSetter,pGetter)));
        assert(rs.second && "JSCClass::addProperty name is dumplicate value");
    }
    void addConstructor( IJSCCallback* constructor ){
        m_Constructor.add(constructor);
    }
    template<typename M>
    void addMethod( const std::string& name, M method ){
        
        PropertyMapItr iter = m_PropertyMap.find(name);
        
        if ( iter != m_PropertyMap.end() ){
            assert( iter->second->m_iType == CallbackType_Function );
            iter->second->addMethod(new JSCCallback<M>(method));
            return;
        }
        CallbackDefine* pProperty = new CallbackDefine(name,new JSCCallback<M>(method));
        JSStringRef pName = JSStringCreateWithUTF8CString(name.c_str());
        JSContextRef pCtx = __TlsData::GetInstance()->GetCurContext();
        pProperty->m_pCallAsFunction = JSObjectMakeFunctionWithCallback(pCtx, pName, JSCClass<T>::callAsFunctionCallback);
        JSStringRelease(pName);
        JSValueProtect(pCtx, pProperty->m_pCallAsFunction);
        
        
        PropertyMapRes rsp = m_PropertyMap.insert(PropertyMapValue(name,pProperty));
        assert(rsp.second);
        
        FunctionMapRes rsf = m_FunctionMap.insert(FunctionMapValue((unsigned long)pProperty->m_pCallAsFunction,pProperty));
        assert(rsf.second);
        
    }
    void finish( const std::string& name ){
        m_bIsGlobal = false;
        finishImpl( name, nullptr);
    }
    void finishToGlobal( const std::string& name, T* pIns){
        assert(pIns != nullptr);
        m_bIsGlobal = false;
        finishImpl( name, pIns);
        m_bIsGlobal = true;
    }
        JSObjectRef transferObjPtrToJS( T *p_pIns ){
        assert( !m_bIsGlobal );
        JSContextRef pCtx = __TlsData::GetInstance()->GetCurContext();
        JSObjectRef pRet = JSObjectMake( pCtx, m_ClassObject, p_pIns );
        p_pIns->mpJsThis = pRet;
        JSValueRef pProperty = JSValueMakeNumber(pCtx, (double)getTypeID());
        JSStringRef pszName = JSStringCreateWithUTF8CString(__Js_class_typeid_property);
        JSObjectSetProperty(pCtx, pRet, pszName, pProperty, kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete, 0);
        JSStringRelease(pszName);
        
        if( m_FixedProperties.size() ){
            FixedPropertiesIter iter;
            for(iter=m_FixedProperties.begin();iter!=m_FixedProperties.end();iter++){
                JSStringRef sName = (*iter)->GetName();
                JSObjectSetProperty(pCtx, pRet, sName, (*iter)->GetValue(), kJSPropertyAttributeReadOnly/*|kJSPropertyAttributeDontDelete*/, 0);
                //kJSPropertyAttributeDontDelete 就意味着这个值不能改了 fuck
               JSStringRelease(sName);
            }
        }
        
        return pRet;
    }
    static void reset(){
        JSCClass<T>::getInstance()->_reset();
    }
private:
    static JSObjectRef newWrap(JSContextRef ctx, JSObjectRef constructor, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        if( JSCClass<T>::getInstance()->m_bIsGlobal ){
            __JsThrow::Throw(exception,"can't new a global object");
            return NULL;
        }
        
        IJSCCallback *pfn = JSCClass<T>::getInstance()->m_Constructor.get( (int)argumentCount );
        
        if( 0 == pfn ){
            return JSCClass<T>::getInstance()->transferObjPtrToJS(new T);
        }
        else{
            return pfn->constructor(ctx, constructor, argumentCount, arguments, exception);
        }
    }
    
    static void destroyWrap(JSObjectRef object){
        T *p = (T *)JSObjectGetPrivate(object);
        
        delete p;
    }
    
    static bool isInstanceOf(JSContextRef ctx, JSObjectRef constructor, JSValueRef possibleInstance, JSValueRef* exception){
        if( !JSValueIsObject(ctx, possibleInstance) ){
            return false;
        }
        
        bool bRet = __CheckClassType::IsTypeOf<T>((JSObjectRef)possibleInstance);
        
        return bRet;
    }

    
    
    static bool hasProperty(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName){
        std::string strPropertyName = __TransferToCpp<char *>::ToCpp(propertyName);
        resetJsStrBuf();
        if( !strPropertyName.length() ){
            return false;
        }
        
        return (0 != JSCClass<T>::getInstance()->findProperty(strPropertyName.c_str()));
    }
    static JSValueRef getProperty(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception){
        T *pThis = (T *)JSObjectGetPrivate( object );
        
        if( 0 == pThis ){
            __JsThrow::Throw(exception,"not a global object");
            return NULL;
        }
        
        std::string strPropertyName = __TransferToCpp<char *>::ToCpp(propertyName);
        resetJsStrBuf();
        if( !strPropertyName.length() ){
            __JsThrow::Throw(exception,"GetProperty PropertyName is null");
            return NULL;
        }
        
        CallbackDefine *pProperty = JSCClass<T>::getInstance()->findProperty(strPropertyName.c_str());
        
        if( 0 == pProperty ){
            __JsThrow::Throw(exception,"GetProperty can't find property");
            return NULL;
        }
        
        JSValueRef pRet = NULL;
        if( CallbackType_Property == pProperty->m_iType ){
            // is property
            pRet = pProperty->m_pGetter->call(ctx, nullptr, object, 0, nullptr, nullptr);//?????????????????????
        }
        else
        {   // is function
            pRet = pProperty->m_pCallAsFunction;
        }
        
        return pRet;
    }
    static bool setProperty(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef value, JSValueRef* exception){
        T *pThis = (T *)JSObjectGetPrivate( object );
        
        if( 0 == pThis ){
            __JsThrow::Throw(exception,"not a global object");
            return false;
        }
        
        std::string sPropertyName = __TransferToCpp<char *>::ToCpp(propertyName);
        resetJsStrBuf();
        if( sPropertyName.empty() ){
            __JsThrow::Throw(exception,"SetProperty PropertyName is null");
            return false;
        }
        
        CallbackDefine *pProperty = JSCClass<T>::getInstance()->findProperty(sPropertyName);
        
        
        if( 0 == pProperty ){
            return false;
        }
        
    
         if( CallbackType_Property != pProperty->m_iType )
         {
         if( pProperty->m_iType == CallbackType_Function ){
         JSObjectRef _obj = JSValueToObject(ctx, value, 0);
         if( JSObjectIsFunction(ctx,_obj) ){
         getInstance()->JSDefineFunction[sPropertyName] = _obj;
         return true;
         }
         }
         __JsThrow::Throw(exception,"SetProperty property is function object");
         return false;
         }
        
        if( 0 == pProperty->m_pSetter ){
            __JsThrow::Throw(exception,"SetProperty property is read only");
            return false;
        }
        
        JSValueRef arguments[1];
        arguments[0] = value;
        pProperty->m_pSetter->call(ctx, nullptr, object, 1, arguments, nullptr);
        return true;
    }
         
    
    static JSValueRef callAsFunctionCallback(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
        
        T *pThis = (T *)JSObjectGetPrivate( thisObject );
        
        if( nullptr == pThis ){
            __JsThrow::Throw(exception,"CallAsFunctionCallback this is null");
            return JSValueMakeUndefined(ctx);
        }
        
        CallbackDefine *pProperty = JSCClass<T>::getInstance()->findFunction((unsigned long)function);
        
        if( pProperty != NULL ){
         JSDefineFunctionIter it = getInstance()->JSDefineFunction.find(pProperty->m_name);
         if( it != getInstance()->JSDefineFunction.end() ){
         return JSObjectCallAsFunction(ctx,it->second,thisObject,argumentCount,arguments,0);
         }
         }
        
        
        if( nullptr == pProperty ){
            __JsThrow::Throw(exception,"CallAsFunctionCallback can't find function");
            return JSValueMakeUndefined(ctx);
        }
        
        IJSCCallback *pMethod = pProperty->getMethod(argumentCount);
        if( 0 != pMethod ){
            //__JsThrow::GetInstance()->UpdateException(exception);
            return pMethod->call(ctx, function, thisObject, argumentCount, arguments, exception);
        }
        else
        {
            __JsThrow::Throw(exception,"callAsFunctionCallback can't find function");
            return JSValueMakeUndefined(ctx);
        }
        
    }
    
    
private:
    void _reset(){
        
		if( m_FixedProperties.size() ){
            FixedPropertiesIter iter;
            for(iter=m_FixedProperties.begin();iter!=m_FixedProperties.end();iter++){
                delete *iter;
            }
            m_FixedProperties.clear();
        }
			
        for (PropertyMapItr itr = m_PropertyMap.begin(); itr != m_PropertyMap.end(); itr++ ){
            delete itr->second;
        }
        m_PropertyMap.clear();
        
        m_FunctionMap.clear();
        
        m_bIsGlobal = false;
        m_ClassDefine = kJSClassDefinitionEmpty;
        
        if ( m_ClassObject != nullptr ){
            JSClassRelease(m_ClassObject);
            m_ClassObject = nullptr;
        }
        JSDefineFunction.clear();
        
        m_Constructor.reset();
    }
    void finishImpl( const std::string& name, T *p_pIns ){
        
        assert(!name.empty());
        m_ClassDefine = kJSClassDefinitionEmpty;
        m_ClassDefine.attributes = kJSClassAttributeNone;
        m_ClassDefine.className = name.c_str();
        m_ClassDefine.callAsConstructor = JSCClass<T>::newWrap;
        m_ClassDefine.finalize = JSCClass<T>::destroyWrap;
        m_ClassDefine.hasProperty = JSCClass<T>::hasProperty;
        m_ClassDefine.hasInstance = JSCClass<T>::isInstanceOf;
        m_ClassDefine.getProperty = JSCClass<T>::getProperty;
        m_ClassDefine.setProperty = JSCClass<T>::setProperty;
        m_ClassDefine.callAsFunction = JSCClass<T>::callAsFunctionCallback;
        
        m_ClassObject = JSClassCreate(&m_ClassDefine);
        JSContextRef pCtx = __TlsData::GetInstance()->GetCurContext();
        JSStringRef jsName = JSStringCreateWithUTF8CString(name.c_str());
        JSObjectRef myObject;
        if( 0 != p_pIns ){
            myObject = transferObjPtrToJS( p_pIns );
            p_pIns->mpJsThis = myObject;
        }
        else
        {
            myObject = JSObjectMake(pCtx, m_ClassObject, 0);
        }
   
        JSObjectSetProperty( pCtx, JSContextGetGlobalObject(pCtx), jsName, myObject, kJSPropertyAttributeNone, NULL );//???
        JSStringRelease(jsName);
    }
    CallbackDefine *findFunction( unsigned long p_ulObj ){
        FunctionMapItr iter = m_FunctionMap.find(p_ulObj);
        if( iter == m_FunctionMap.end() ){
            return nullptr;
        }
        else
        {
            return (*iter).second;
        }
    }
    CallbackDefine *findProperty( const std::string& name ){
        PropertyMapItr iter = m_PropertyMap.find(name);
        if( iter == m_PropertyMap.end() ){
            return nullptr;
        }
        else
        {
            return (*iter).second;
        }
    }
    
    
private:
    typedef std::unordered_map<std::string,CallbackDefine*> PropertyMap;
    typedef typename PropertyMap::value_type PropertyMapValue;
    typedef typename PropertyMap::iterator PropertyMapItr;
    typedef std::pair<PropertyMapItr,bool> PropertyMapRes;
    PropertyMap m_PropertyMap;
    
    typedef std::unordered_map<unsigned long,CallbackDefine*> FunctionMap;
    typedef typename FunctionMap::value_type FunctionMapValue;
    typedef typename FunctionMap::iterator FunctionMapItr;
    typedef std::pair<FunctionMapItr,bool> FunctionMapRes;
    FunctionMap m_FunctionMap;
    
    bool m_bIsGlobal;
    JSClassDefinition m_ClassDefine;
    JSClassRef m_ClassObject;
    typedef std::unordered_map<std::string,JSObjectRef>::iterator JSDefineFunctionIter;
    std::unordered_map<std::string, JSObjectRef> JSDefineFunction;
    unsigned int m_iTypeID;
    
    FuncEntry m_Constructor;
};
}

#endif
