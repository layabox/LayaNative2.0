/**
@file			JSWebGLPlus.h
@brief			
@author			James
@version		1.0
@date			2019_8_24
*/

#ifndef __JSWebGLPlus_H__
#define __JSWebGLPlus_H__

#include "../JSInterface/JSInterface.h"
#include <webglplus/JCWebGLPlus.h>

/** 
 * @brief 
*/
namespace laya 
{
    class JSWebGLPlus:public JsObjBase, public JSObjNode
    {
    public:

	    static JsObjClassInfo JSCLSINFO;

        void exportJS();

        static JSWebGLPlus* getInstance();

        JSWebGLPlus();

        ~JSWebGLPlus();

    public:

        JsValue createArrayBufferRef(JSValueAsParam pArrayBuffer, int nType, bool bSyncToRender, int nRefType);

        bool updateArrayBufferRef(int nID, bool bSyncToRender, JSValueAsParam pArrayBuffer);

        bool syncArrayBufferDataToRuntime(int nID, bool bSyncToRender, JSValueAsParam pArrayBuffer);

        /*(void evaluateClipDatasRealTime(JSValueAsParam nodes, float playCurTime, JSValueAsParam realTimeCurrentFrameIndexs, bool addtive, bool frontPlay);

        bool updateAnimationNodeWorldMatix(JSValueAsParam locPosition, JSValueAsParam locRotation, JSValueAsParam locScaling, JSValueAsParam parentIndices, JSValueAsParam outWorldMatrix);

        bool computeSubSkinnedData(JSValueAsParam worldMatrixs, JSValueAsParam worldMatrixIndex, JSValueAsParam inverseBindPosesBuffer, JSValueAsParam boneIndices, JSValueAsParam bindPoseInices, JSValueAsParam data);

        int culling(JSValueAsParam boundFrustumBuffer, JSValueAsParam cullingBuffer, JSValueAsParam cullingBufferIndices, int cullingCount, JSValueAsParam cullingBufferResult);
		*/
    public:

        static JSWebGLPlus*         s_pWebGLPlus;
        
    };
}
//------------------------------------------------------------------------------


#endif //__JSWebGLPlus_H__

//-----------------------------END FILE--------------------------------