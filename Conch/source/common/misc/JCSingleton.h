/**
@file			JCSingleton.h
@brief			
@author			James
@version		1.0
@date			2016_5_12
*/

#ifndef __JCSingleton_H__
#define __JCSingleton_H__

namespace laya
{
	template<class T>
	class JCSingleton
	{
	private:
		struct _ObjectCreator
		{
			_ObjectCreator()
			{
                JCSingleton<T>::getInstance();
			}

			inline void do_nothing() const
			{}
		};
		static _ObjectCreator m_CreateObject;
	protected:
		typedef T ObjectType;
        JCSingleton()
		{
		}
	public:
		static ObjectType *getInstance()
		{
			static ObjectType _l_Obj;

			m_CreateObject.do_nothing();

			return &_l_Obj;
		}
		//不允许有delInstance。如果内容对象确实需要释放某些资源，则自己在内容类中添加释放的函数
	};
	template <typename T> typename JCSingleton<T>::_ObjectCreator JCSingleton<T>::m_CreateObject;
}

//------------------------------------------------------------------------------


#endif //__JCSingleton_H__

//-----------------------------END FILE--------------------------------