#include "IsolateData.h"
namespace laya
{
	IsolateData::IsolateData(v8::Isolate* isolate, v8::ArrayBuffer::Allocator* allocator): m_isolate(isolate), m_allocator(allocator)
	{
		m_isolate->SetData(kEmbedderNative, this);
	}

	IsolateData::~IsolateData()
	{
		m_isolate->SetData(kEmbedderNative, NULL);
	}

	IsolateData* IsolateData::From(v8::Isolate* isolate)
	{
		return static_cast<IsolateData*>(isolate->GetData(kEmbedderNative));
	}
	void IsolateData::SetObjectTemplate(JsObjClassInfo* info, v8::Local<v8::ObjectTemplate> templ)
	{
		m_object_templates[info] = v8::Eternal<v8::ObjectTemplate>(m_isolate, templ);
	}
	v8::Local<v8::ObjectTemplate> IsolateData::GetObjectTemplate(JsObjClassInfo* info)
	{
		ObjectTemplateMap::iterator it = m_object_templates.find(info);
		if (it == m_object_templates.end())
			return v8::Local<v8::ObjectTemplate>();
		return it->second.Get(m_isolate);
	}

}

