/**
@file			JCIOSFTInterface.h
@brief			
@author			James
@version		1.0
@date			2016_6_11
*/

#ifndef __JCIOSFTInterface_H__
#define __JCIOSFTInterface_H__

#include <string>
namespace laya
{
	class JCIOSFTInterface
	{
	public:
		
        virtual std::string getIOSFontTTFPath() = 0;
        
        virtual std::string writeIOSFontTTF() = 0;

	};

}
//------------------------------------------------------------------------------


#endif //__JCIOSFTInterface_H__

//-----------------------------END FILE--------------------------------