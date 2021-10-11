#include "JCSystemConfig.h"
#include <util/JCCommonMethod.h>

namespace laya
{
	JCSystemConfig g_kSystemConfig;
	JCSystemConfig::JCSystemConfig()
    {
		reset();
	}
    bool JCSystemConfig::s_bIsPlug = true;	//插件是主流。同一进程也不会改变。
    bool JCSystemConfig::s_bLocalizable = false;
	void JCSystemConfig::reset()
    {
		m_bPerfStat=false;
        m_nFrameType = FT_FAST;
        m_nFrameThreshold = 2000;
        m_nSleepTime = 0;
		m_strStartURL = "";
		m_nPerf_UpdateNum=500;
		m_jsonparamExt="";
        s_bLocalizable = false;
        m_bShowInternalPerBar = false;
        m_bUseChoreographer = false;
        m_bUseAndroidSystemFont = false;
        m_nThreadMODE = THREAD_MODE_DOUBLE;
	}
}