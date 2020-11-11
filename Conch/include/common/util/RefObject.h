/**
@file			RefObject.h
@brief			
@author			James
@version		1.0
@date			2017_8_8
*/

#ifndef __RefObject_H__
#define __RefObject_H__

namespace laya
{
    class RefObject 
    {
    public:
        RefObject()
	    {
		    _ref_ = 0;
	    }
        virtual ~RefObject()
        {

        }
	    int addRef()
	    {
		    return _ref_++;
	    }
	    virtual void destroy()
	    {
		    delete this;
	    }
	    void release()
	    {
		    if((--_ref_ )<1)
			    destroy();
	    }
    protected:
        int _ref_;
    };
}
//------------------------------------------------------------------------------


#endif //__RefObject_H__

//-----------------------------END FILE--------------------------------