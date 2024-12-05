#ifndef IsolateData_H
#define IsolateData_H

#include <ark_runtime/jsvm_types.h>
#include <map>
#include <string>

namespace laya
{
	struct JsObjClassInfo;
    class JsObjBase;
    
    class JSClass
    {
        public:
            void            init(const std::string &clsName, JSVM_Callback ctor);
            bool            install(JSVM_Env env);
            void            defineFunction(const char* name, JSVM_Callback func);
            void            defineProperty(const char* name, JSVM_Callback g, JSVM_Callback s, JSVM_Value value);
            void            defineStaticFunction(const char* name, JSVM_Callback func);
            static JSClass  *create(const std::string &clsName, JSVM_Callback ctor);
            JSVM_Ref        getConRef();
        private:
            JSClass();
            ~JSClass();
            static JSVM_Value _defaultCtor(JSVM_Env env, JSVM_CallbackInfo info);
    
        private:
        std::string                          _name;
        JSVM_CallbackStruct                  _ctorStruct = {JSClass::_defaultCtor, nullptr};
        JSVM_Callback                        _ctorFunc = &_ctorStruct;
        JSVM_Ref                             _constructor = nullptr;
        std::vector<JSVM_PropertyDescriptor> _properties;
    };

	class IsolateData
	{
	public:

		IsolateData(JSVM_Env env);

		~IsolateData();

		static IsolateData* getInstance();
    
        void _setNeedCallConstructor(bool need);
    
        bool _needCallConstructor();

		IsolateData(const IsolateData&) = delete;

		IsolateData& operator=(const IsolateData&) = delete;

		void SetObjectTemplate(JsObjClassInfo* info, JSClass* cls);

		JSVM_Value GetObjectTemplate(JsObjClassInfo* info);
    
	private:

		static IsolateData* instance;

		JSVM_Env _env;

		typedef std::map<JsObjClassInfo*, JSVM_Ref> ClsMap;

		ClsMap m_class_map;
    
        bool _isNeedCallConstructor = true;
	};
}
#endif
