/**
@file			JCImageManager.h
@brief			
@author			James
@version		1.0
@date			2016_5_13
*/

#ifndef __JCImageManager_H__
#define __JCImageManager_H__

#include "../Image/JCImage.h"
#include <map>
#include <vector>
#include <thread>
#include <mutex>

/** 
 * @brief 
*/
namespace laya
{
    class JCImageManager
    {
    public:

        typedef std::vector<JCImage*>                   VectorImage;
        typedef VectorImage::iterator                   VectorImageIter;

        typedef std::vector<int>                        VectorImageID;
        typedef VectorImageID::iterator                 VectorImageIDIter;

        typedef std::map<int, JCImage*>                 MapImage;
        typedef MapImage::iterator                      MapImageIter;

    public:

        /** @brief构造函数
        */
        JCImageManager();

        /** @brief析构函数
        */
        ~JCImageManager();

        void setImage( int nID,JCImage* pImage );

        bool deleteImage( int nID );
        
        void clearAllImage();

        JCImage* getImage( int nID );

        void resetRenderThread();

        void printCorpseImages( const char* sFileName );

    public:

        int getImageID();

        void resetJSThread();

        void removeImageID(int nID);

        void clearImageIDs();

        void removeImageFromMap( int nID );

        void update(int nFrameCount);

        void setReleaseSpaceTime(int nTime);

    private:

        void _batchHandleDeleteImage();

        bool _deleteImage(int nID);

    public:

        VectorImageID           m_vImageIDs;            ///<imageID

        VectorImage             m_vImages;              ///<完整的Image的容器

        //当Image把数据merge到显卡上，Image的内存数据就会删除掉，如果迟迟没有merge到显卡，这部分数据只会保留一段时间
        MapImage                m_vOccupiedMemImages;   ///<占用内存的Image容器

        int                     m_nCountImageID;        ///<imageID

        int                     m_nReleaseSpaceTime;    ///<删除的间隔时间
		
		std::recursive_mutex    m_kMutex;               ///<线程锁

    private:

        int                     m_nDeleteMaxNum;        ///<为了效率保存最大的值
        std::vector<int>        m_vDeleteList;          ///<删除列表

    };
}
#endif //__JCImageManager_H__

//-----------------------------END FILE--------------------------------
