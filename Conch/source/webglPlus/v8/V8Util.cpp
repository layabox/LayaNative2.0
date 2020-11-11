/**
@file			V8Util.cpp
@brief			
@author			James
@version		1.0
@date			2017_11_29
*/

#include "V8Util.h"

namespace laya
{
    void V8Util::SET_PROTOTYPE_METHOD(v8::Local<v8::FunctionTemplate> recv, const char* name, v8::FunctionCallback callback) 
    {
        v8::Isolate* isolate = v8::Isolate::GetCurrent();
        v8::HandleScope handle_scope(isolate);
        v8::Local<v8::Signature> s = v8::Signature::New(isolate, recv);
        v8::Local<v8::FunctionTemplate> t =
            v8::FunctionTemplate::New(isolate, callback, v8::Local<v8::Value>(), s);
        v8::Local<v8::String> fn_name = v8::String::NewFromUtf8(isolate, name,
            v8::NewStringType::kInternalized).ToLocalChecked();
        t->SetClassName(fn_name);
        recv->PrototypeTemplate()->Set(fn_name, t);
    }

   bool V8Util::extractJSAB(v8::Local<v8::Value> jsval, char*& data, int& len)
    {
        v8::Local<v8::ArrayBuffer> ab;
        if (jsval->IsArrayBufferView())
        {
            v8::Local<v8::ArrayBufferView> abv = v8::Local<v8::ArrayBufferView>::Cast(jsval);
            len = abv->ByteLength();
            ab = abv->Buffer();
            v8::ArrayBuffer::Contents contents = ab->GetContents();
            data = (char*)contents.Data() + abv->ByteOffset();
        }
        else if (jsval->IsArrayBuffer())
        {
            ab = v8::Local<v8::ArrayBuffer>::Cast(jsval);
            v8::ArrayBuffer::Contents contents = ab->GetContents();
            len = contents.ByteLength();
            data = (char*)contents.Data();
        }
        else
        {
            data = NULL;
            len = 0;
            return false;
        }
        return true;
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
