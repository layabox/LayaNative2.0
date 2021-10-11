#ifndef __IsolateData _H__
#define __IsolateData _H__

#include <v8.h>
#include <map>

namespace laya
{
	enum Embedder : uint16_t {
		kEmbedderNative,
	};
	struct JsObjClassInfo;
	class IsolateData
	{
	public:

		IsolateData(v8::Isolate* isolate, v8::ArrayBuffer::Allocator* allocator);

		~IsolateData();

		static IsolateData* From(v8::Isolate* isolate);

		IsolateData(const IsolateData&) = delete;

		IsolateData& operator=(const IsolateData&) = delete;

		void SetObjectTemplate(JsObjClassInfo* info, v8::Local<v8::ObjectTemplate> templ);

		v8::Local<v8::ObjectTemplate> GetObjectTemplate(JsObjClassInfo* info);

	private:

		v8::Isolate* m_isolate;

		v8::ArrayBuffer::Allocator* m_allocator;

		typedef std::map<JsObjClassInfo*, v8::Eternal<v8::ObjectTemplate> > ObjectTemplateMap;

		ObjectTemplateMap m_object_templates;
	};
}
#endif
