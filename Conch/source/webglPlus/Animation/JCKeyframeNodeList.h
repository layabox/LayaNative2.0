/**
@file			JCKeyframeNodeList.h
@brief			
@author			James
@version		1.0
@date			2018_7_12
*/

#ifndef __JCKeyframeNodeList_H__
#define __JCKeyframeNodeList_H__

#include <stdio.h>
#include <string>
#include <map>
#include "JCKeyframeNode.h"
#include <vector>

namespace laya
{
    class JCKeyframeNodeList
    {
    public:

		static void evaluateClipDatasRealTime(JCKeyframeNodeList* nodes, float playCurTime, short* realTimeCurrentFrameIndexs,int indexSize, bool addtive, bool frontPlay);

        JCKeyframeNodeList();

        virtual  ~JCKeyframeNodeList();

        int getCount()
        {
            return m_vNodes.size();
        }

        void setCount(int value)
        {
            m_vNodes.resize(value);
        }

    protected:

        static float _hermiteInterpolate(JCFloatKeyframe* frame, JCFloatKeyframe* nextFrame, float t, float dur);

        static void _evaluateFrameNodeArrayDatasRealTime(std::vector<JCKeyFrame*>& keyFrames, int frameIndex, bool isEnd, float playCurTime, int width, JCFloatArrayKeyframe::FloatArrayBuffer& outDatas);

        static void _hermiteInterpolateArray(JCFloatArrayKeyframe* frame, JCFloatArrayKeyframe* nextFrame, float t, float dur, JCFloatArrayKeyframe::FloatArrayBuffer& out);

        static void quaternionConjugate(float* value, int offset, float* result);

        static void quaternionMultiply(float* le, float* re, float* oe);

    public:

        std::vector<JCKeyframeNode*>    m_vNodes;

    };
}
//------------------------------------------------------------------------------


#endif //__JCKeyframeNodeList_H__

//-----------------------------END FILE--------------------------------