/**
@file			JCKeyframeNodeList.cpp
@brief			
@author			James
@version		1.0
@date			2018_7_12
*/

#include "JCKeyframeNodeList.h"
#include "../Log.h"

namespace laya
{
    //------------------------------------------------------------------------------
    JCKeyframeNodeList::JCKeyframeNodeList()
    {

    }
    //------------------------------------------------------------------------------
    JCKeyframeNodeList::~JCKeyframeNodeList()
    {
        m_vNodes.clear();
    }

	void JCKeyframeNodeList::evaluateClipDatasRealTime(JCKeyframeNodeList* nodes, float playCurTime, short* realTimeCurrentFrameIndexs, int indexSize, bool addtive, bool frontPlay)
	{
        int nNodeCount = nodes->getCount();
        if (nNodeCount != indexSize)
        {
            LOGE("evaluateClipDatasRealTime error");
            return;
        }
		for (int i = 0; i < nNodeCount; i++) {
			JCKeyframeNode* node = nodes->m_vNodes[i];
			int type = node->m_nType;
			short nextFrameIndex = 0;
			std::vector<JCKeyFrame*> keyFrames = node->m_vKeyFrames;
			int keyFramesCount = keyFrames.size();
			short frameIndex = realTimeCurrentFrameIndexs[i];

			if (true)
			{
				if (frameIndex != -1 && playCurTime < keyFrames[frameIndex]->m_nTime)
				{
					frameIndex = -1;
					realTimeCurrentFrameIndexs[i] = frameIndex;
				}

				nextFrameIndex = frameIndex + 1;
				while (nextFrameIndex < keyFramesCount) {
					if (keyFrames[nextFrameIndex]->m_nTime > playCurTime)
						break;
					frameIndex++;
					nextFrameIndex++;
					realTimeCurrentFrameIndexs[i] = frameIndex;
				}
			}
			else
			{
				nextFrameIndex = frameIndex + 1;
				if ((nextFrameIndex != keyFramesCount) &&
					(playCurTime > keyFrames[nextFrameIndex]->m_nTime))
				{
					frameIndex = keyFramesCount - 1;
					realTimeCurrentFrameIndexs[i] = frameIndex;
				}

				nextFrameIndex = frameIndex + 1;
				while (frameIndex > -1)
				{
					if (keyFrames[frameIndex]->m_nTime < playCurTime)
						break;
					frameIndex--;
					nextFrameIndex--;
					realTimeCurrentFrameIndexs[i] = frameIndex;
				}
			}

			bool isEnd = (nextFrameIndex == keyFramesCount);		

			switch (type) {			
			case 0:
			{
				node->m_nDataType = 0;
                JCFloatKeyframe* pKeyframe = (JCFloatKeyframe*)keyFrames[0];
                if (frameIndex != -1)
                {
                    JCFloatKeyframe* frame = (JCFloatKeyframe*)keyFrames[frameIndex];
                    if (isEnd) {//如果nextFrame为空，不修改数据，保持上一帧
                        node->m_pDataFloat = frame->m_nValue;
                    }
                    else {
                        JCFloatKeyframe* nextFarme = (JCFloatKeyframe*)keyFrames[nextFrameIndex];
                        float d = nextFarme->m_nTime - frame->m_nTime;
                        float t;
                        if (d != 0)
                            t = (playCurTime - frame->m_nTime) / d;
                        else
                            t = 0;
                        node->m_pDataFloat = _hermiteInterpolate(frame, nextFarme, t, d);
                    }
                }
				else {
					node->m_pDataFloat = pKeyframe->m_nValue;
				}
				if (addtive) {
					node->m_pDataFloat -= pKeyframe->m_nValue;
				}
				break;
			}
			case 1:
			case 4:
			{
				node->m_nDataType = 1;
				JCFloatArrayKeyframe::FloatArrayBuffer* clipData = &node->m_pDataFloatArray;
				_evaluateFrameNodeArrayDatasRealTime(keyFrames, frameIndex, isEnd, playCurTime, 3, *clipData);
				if (addtive) {
					JCFloatArrayKeyframe* pKeyframe = (JCFloatArrayKeyframe*)keyFrames[0];
					JCFloatArrayKeyframe::FloatArrayBuffer* firstFrameValue = &pKeyframe->m_pData;
                    float* clipVData = (float*)clipData->data;
                    float* firstValue = (float*)firstFrameValue->data;
                    clipVData[0] -= firstValue[6];
                    clipVData[1] -= firstValue[7];
                    clipVData[2] -= firstValue[8];
				}
				break;
			}
			case 2:
			{
				node->m_nDataType = 1;
				JCFloatArrayKeyframe::FloatArrayBuffer* clipData = &node->m_pDataFloatArray;
				_evaluateFrameNodeArrayDatasRealTime(keyFrames, frameIndex, isEnd, playCurTime, 4, *clipData);
				if (addtive) {
                    float temp[4];
					JCFloatArrayKeyframe* pKeyframe = (JCFloatArrayKeyframe*)keyFrames[0];
					JCFloatArrayKeyframe::FloatArrayBuffer* firstFrameValue = &pKeyframe->m_pData;
					quaternionConjugate((float*)firstFrameValue->data,8,temp);
					quaternionMultiply(temp, (float*)clipData->data, (float*)clipData->data);
				}
				break;
			}
			case 3:
			{
				node->m_nDataType = 1;
				JCFloatArrayKeyframe::FloatArrayBuffer* clipData = &node->m_pDataFloatArray;
				_evaluateFrameNodeArrayDatasRealTime(keyFrames, frameIndex, isEnd, playCurTime, 3, *clipData);
				if (addtive) {
					JCFloatArrayKeyframe* pKeyframe = (JCFloatArrayKeyframe*)keyFrames[0];
					JCFloatArrayKeyframe::FloatArrayBuffer* firstFrameValue = &pKeyframe->m_pData;
                    float* clipVData = (float*)clipData->data;
                    float* firstValue = (float*)firstFrameValue->data;
                    clipVData[0] /= firstValue[6];
                    clipVData[1] /= firstValue[7];
                    clipVData[2] /= firstValue[8];
				}
				break;
			}
			default:
				throw "AnimationClip:unknown node type.";
			}
		}
	}

	float JCKeyframeNodeList::_hermiteInterpolate(JCFloatKeyframe* frame, JCFloatKeyframe* nextFrame, float t, float dur) {
		float t0 = frame->m_nOutTangent, t1 = nextFrame->m_nInTangent;
		float t2 = t * t;
		float t3 = t2 * t;
		float a = (float)(2.0 * t3 - 3.0 * t2 + 1.0);
		float b = (float)(t3 - 2.0 * t2 + t);
		float c = (float)(t3 - t2);
		float d = (float)(-2.0 * t3 + 3.0 * t2);
		return a * frame->m_nValue + b * t0 * dur + c * t1 * dur + d * nextFrame->m_nValue;
	}

	void JCKeyframeNodeList::_evaluateFrameNodeArrayDatasRealTime(std::vector<JCKeyFrame*>& keyFrames, int frameIndex, bool isEnd, float playCurTime, int width, JCFloatArrayKeyframe::FloatArrayBuffer& outDatas)
    {
        int dataOffset = width * 2;
        float* outVData = (float*)outDatas.data;
        if (frameIndex != -1)
        {
            JCFloatArrayKeyframe* frame = (JCFloatArrayKeyframe*)keyFrames[frameIndex];
            if (isEnd)
            {
                float* frameData = (float*)frame->m_pData.data;
                for (int j = 0; j < width; j++)
                    outVData[j] = frameData[dataOffset+j];//不能设为null，会造成跳过当前帧数据
            }
            else
            {
                JCFloatArrayKeyframe* nextKeyFrame = (JCFloatArrayKeyframe*)keyFrames[frameIndex + 1];
                float t;
                float startTime = frame->m_nTime;
                float d = nextKeyFrame->m_nTime - startTime;
                if (d != 0)
                    t = (playCurTime - startTime) / d;
                else
                    t = 0;

                _hermiteInterpolateArray(frame, nextKeyFrame, t, d, outDatas);
            }
        }
		else 
        {
			JCFloatArrayKeyframe::FloatArrayBuffer* firstFrameDatas = &((JCFloatArrayKeyframe*)keyFrames[0])->m_pData;
            float* frameData = (float*)firstFrameDatas->data;
			for (int j = 0; j < width; j++)
                outVData[j] = frameData[dataOffset+j];
		}
	}

	void JCKeyframeNodeList::_hermiteInterpolateArray(JCFloatArrayKeyframe* frame, JCFloatArrayKeyframe* nextFrame, float t, float dur, JCFloatArrayKeyframe::FloatArrayBuffer& out) {
		bool isComputeParams = false;
		float a, b, c, d;
		float* out_data = (float*)out.data;
		float* p0_data = (float*)frame->m_pData.data;
		float* p1_data = (float*)nextFrame->m_pData.data;
        int nOutTanOffset = out.byteSize / sizeof(float);
        int nOffset = nOutTanOffset * 2;
		for (int i = 0; i < nOutTanOffset; i++) {
			float t0 = p0_data[nOutTanOffset +i], t1 = p1_data[i];
			if (!isComputeParams) {
				float t2 = t * t;
				float t3 = t2 * t;
				a = (float)(2.0 * t3 - 3.0 * t2 + 1.0);
				b = (float)(t3 - 2.0 * t2 + t);
				c = (float)(t3 - t2);
				d = (float)(-2.0 * t3 + 3.0 * t2);
				isComputeParams = true;
			}
			out_data[i] = a * p0_data[nOffset+i] + b * t0 * dur + c * t1 * dur + d * p1_data[nOffset+i];
		}
	}

	void JCKeyframeNodeList::quaternionConjugate(float* value,int nOffset, float* result) {
		result[0] = -value[nOffset];
		result[1] = -value[nOffset+1];
		result[2] = -value[nOffset+2];
		result[3] = value[nOffset+3];
	}

	void JCKeyframeNodeList::quaternionMultiply(float* le, float* re, float* oe) {
		/*[DISABLE-ADD-VARIABLE-DEFAULT-VALUE]*/
		float lx = le[0];
		float ly = le[1];
		float lz = le[2];
		float lw = le[3];
		float rx = re[0];
		float ry = re[1];
		float rz = re[2];
		float rw = re[3];

		float a  = ly * rz - lz * ry;
		float b  = lz * rx - lx * rz;
		float c  = lx * ry - ly * rx;
		float d  = lx * rx + ly * ry + lz * rz;
        oe[0] = lx * rw + rx * lw + a;
        oe[1] = ly * rw + ry * lw + b;
        oe[2] = lz * rw + rz * lw + c;
        oe[3] = lw * rw - d;
	}
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
