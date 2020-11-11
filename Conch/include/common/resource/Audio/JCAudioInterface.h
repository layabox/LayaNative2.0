/**
@file			JCAudioInterface.h
@brief			
@author			James
@version		1.0
@date			2014_11_26
*/

#ifndef __JCAudioInterface_H__
#define __JCAudioInterface_H__

#include <stdio.h>

/** 
 * @brief JCAudioInterface
 * 纯接口类
*/
namespace laya
{
class JCAudioInterface
{
public:

	virtual void onPlayEnd() = 0;

};
}

#endif //__JCAudioInterface_H__

//-----------------------------END FILE--------------------------------