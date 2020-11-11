/**
@file			JSFileSystem.h
@brief			
@author			James
@version		1.0
@date			2017_11_28
*/

#ifndef __JSFileSystem_H__
#define __JSFileSystem_H__

#include "../JSInterface/JSInterface.h"

namespace laya
{
	class JSFileSystem
    {
	public:
		static void exportJS();
		static bool exists(const char* p_pszPath );
		static bool mkdir( const char* p_pszPath );
		static bool rm(const char* p_pszFile);
		static bool rmDir(const char* p_pszPath, JSValueAsParam onprogress, JSValueAsParam oncomplete, JSValueAsParam onerror);
		static bool rmDirSync(const char* p_pszPath);
		static JsValue readBinFileSync(const char* p_pszFile);
		static JsValue readdirSync(const char* pPath);
		static JsValue lstatSync(const char* pPath);
        static bool JSWriteFileSync(const char* p_sUrl, JSValueAsParam args);
	};
}
//------------------------------------------------------------------------------


#endif //__JSFileSystem_H__

//-----------------------------END FILE--------------------------------