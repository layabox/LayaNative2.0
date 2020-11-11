
#ifndef _JS_ARRAYBUFFER_H_
#define _JS_ARRAYBUFFER_H_

#include <v8.h>
#include <vector>

/*
	如果有c这边的ArrayBufferView引用这个ArrayBuffer的话，需要增加引用计数，不能直接删除
*/

namespace laya{

	//这个要在v8线程外分配和释放，因为v8析构的时候会调用这个对象提供的 Free 接口
	class ArrayBufferAllocator : public v8::ArrayBuffer::Allocator {
	public:
        ArrayBufferAllocator();
        ~ArrayBufferAllocator();
		virtual void* Allocate(size_t length) ;
		virtual void* AllocateUninitialized(size_t length);
		virtual void Free(void* data, size_t length);
        //这个函数没有意义，v8本身会都释放掉 Heap::FreeDeadArrayBuffers
        //void FreeAllAlive();//释放所有的还没有释放的ArrayBuffer
		static ArrayBufferAllocator* getInstance();
        //int _testGetID(void* pdata);
        //int getAliveBufferNum() {
        //    return m_vAliveBuffer.size();
        //}
    protected:
        //std::vector<char*> m_vAliveBuffer;
	};

	v8::Local<v8::ArrayBuffer> createJSAB(char* pData, int len);
    v8::Local<v8::ArrayBuffer> createJSABAligned(char* pData, int len);
	bool extractJSAB(v8::Local<v8::Value> ab, char*& data, int& len);
}

#endif
