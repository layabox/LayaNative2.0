/**
@file			CToJavaBridge.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_19
*/

#include "CToJavaBridge.h"
#include <android/log.h>
#include "util/Log.h"
#include <malloc.h>
#include <imageLib/JCImageRW.h>
#include "fontMgr/JCFontInfo.h"
#include <pthread.h>
CToJavaBridge*	g_pCToJava=NULL;
std::string     CToJavaBridge::JavaClass="layaair/game/browser/ExportJavaFunction";

const char* s_javaClasses[] =
{
	"layaair/game/browser/LayaVideoPlayer",
};

const int s_javaClassesSize = 1;

void OnCleanTLS(void* p_dt){
	CToJavaBridge::ThreadJNIData* pData = (CToJavaBridge::ThreadJNIData*)p_dt;
	pData->pJVM->DetachCurrentThread();
	pData->pJVM=0;
	pData->pThreadJNI=0;
	delete pData;
}
CToJavaBridge* CToJavaBridge::GetInstance(){
	if(g_pCToJava==NULL)
		g_pCToJava=new CToJavaBridge();
	return g_pCToJava;
}
void CToJavaBridge::DelInstance(){
	if(g_pCToJava!=NULL)
		delete g_pCToJava;
	g_pCToJava=NULL;
	LOGE("delete CToJavaBridge");
}
CToJavaBridge::CToJavaBridge(){
	m_pJVM = 0;
	m_pDefJNIEnv = 0;
    m_jMethodID = NULL;
	m_jMethodIDRefection = NULL;
    m_jNativeMethodStrId = NULL;
    m_jNativeMethodStrId1 = NULL;
    m_jNativeMethodStrId2 = NULL;
    m_jClass = NULL;
    m_jIntegerClass = NULL;
	pthread_key_create (&(m_kThreadkeyJNI), OnCleanTLS);
}
CToJavaBridge::~CToJavaBridge(){
	//m_StaticMethods.clear();
}
bool paserParamDesc( const char* p_sDesc,std::vector<int>& p_nParamNum,int& p_nReturnType )
{
	if( p_sDesc == NULL )return false;
	int nParamNum = 0;
	char* sDesc = (char*)p_sDesc;
	int nLen = strlen( sDesc );
	if( sDesc[0] != '(' )
	{
		LOGE("=======paserParamDesc格式错误");
		return false;
	}
	bool bPaserReturn = false;
	int i = 1;
	while( i < nLen )
	{
		if( sDesc[i] == ')' )
		{
			i++;
			break;
		}
		else
		{
			if( sDesc[i] == 'Z' )
			{
				p_nParamNum.push_back( PT_bool );
			}
			else if( sDesc[i] == 'B' )
			{
				p_nParamNum.push_back( PT_byte );
			}
			else if( sDesc[i] == 'C' )
			{
				p_nParamNum.push_back( PT_char );
			}
			else if( sDesc[i] == 'S' )
			{
				p_nParamNum.push_back( PT_short );
			}
			else if( sDesc[i] == 'I' )
			{
				p_nParamNum.push_back( PT_int );
			}
			else if( sDesc[i] == 'J' )
			{
				p_nParamNum.push_back( PT_long );
			}
			else if( sDesc[i] == 'F' )
			{
				p_nParamNum.push_back( PT_float );
			}
			else if( sDesc[i] == 'D' )
			{
				p_nParamNum.push_back( PT_double );
			}
			else if( sDesc[i] == 'L' )
			{
				char sTempBuffer[64];
				memset( sTempBuffer,0,sizeof(sTempBuffer) );
				int n = 0;
				while( sDesc[i] != ';' )
				{
					sTempBuffer[ n ]= sDesc[i];
					n++;
					i++;
				}
				if( strcmp( sTempBuffer,"Ljava/lang/String" ) == 0 )
				{
					p_nParamNum.push_back( PT_String );
				}else if(strcmp( sTempBuffer,"Ljava/lang/Object" ) == 0){
					p_nParamNum.push_back( PT_object );
				}
				else
				{
					LOGE("=======你是想传入Ljava/lang/String吗?格式错误了");
					return false;
				}
			}
			else if(sDesc[i] == '[')
			{
				p_nParamNum.push_back(PT_array);
			}
			else
			{
				LOGE("=======paserParamDesc格式暂不支持");
			}
		}
		i++;
	}
	//处理返回值的
	while( i < nLen )
	{
		if( sDesc[i] == '[' )
		{
			p_nReturnType = JRT_object;
			break;
		}
		else if( sDesc[i] == 'I' )
		{
			p_nReturnType = JRT_Int;
			break;
		}
		else if( sDesc[i] == 'L' )
		{
			char sTempBuffer[64];
			memset( sTempBuffer,0,sizeof(sTempBuffer) );
			int n = 0;
			while( sDesc[i] != ';' )
			{
				sTempBuffer[ n ]= sDesc[i];
				n++;
				i++;
			}
			if( strcmp( sTempBuffer,"Ljava/lang/String" ) == 0 )
			{
				p_nReturnType = JRT_string;
				break;
			}
			else
			{
				LOGE("=======你是想传入Ljava/lang/String吗?格式错误了");
				return false;
			}
		}
		else if( sDesc[i] == 'V' )
		{
			p_nReturnType = JRT_void;
			break;
		}
		else if( sDesc[i] == 'F' ){
		  p_nReturnType = JRT_Float;
		  break;
		}
		else
		{
			LOGE("=======paserParamDesc格式暂不支持");
		}
		i++;
	}
	return true;
}
//内部不会保存这些字符串的指针
jmethodID CToJavaBridge::addStaticMethod( JNIEnv* p_Env, const char* p_sCls){
	
    LOGE("=======================addStaticMethod=========zz%x",(long)p_Env);
//初始化
    CToJavaBridge::JavaClass = p_sCls;
	if( m_pJVM==0){
		p_Env->GetJavaVM(&m_pJVM);
		m_pDefJNIEnv = p_Env;

		// 初始化Java的Class
		for(int i = 0; i < s_javaClassesSize; i++)
		{
			jclass classTemp = p_Env->FindClass(s_javaClasses[i]);
			if (!classTemp)
			{
				LOGI("[Debug][JNI]find class %s failed", s_javaClasses[i]);
				continue;
			}

			jclass savedClass = (jclass)p_Env->NewGlobalRef(classTemp);
			m_classMap.insert(std::make_pair(s_javaClasses[i], savedClass));
		}
	}
    if (m_jMethodID == NULL)
    {
        jclass tmp = p_Env->FindClass(CToJavaBridge::JavaClass.c_str()); 
		LOGI("[########]  %s", CToJavaBridge::JavaClass.c_str());
    	m_jClass  =(jclass)p_Env->NewGlobalRef(tmp);
        m_jMethodID = p_Env->GetStaticMethodID(m_jClass, "callMethod", "(IZLjava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
		m_jMethodIDRefection = p_Env->GetStaticMethodID(m_jClass, "callMethodRefection", "(IZLjava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
        m_jNativeMethodStrId = p_Env->GetStaticMethodID(m_jClass, "callMethod", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
		
        m_jNativeMethodStrId1 = p_Env->GetStaticMethodID(m_jClass, "createTextBitmap", "(Ljava/lang/String;Ljava/lang/String;IIIF)[Ljava/lang/Object;");

        tmp = p_Env->FindClass("java/lang/Integer");
        m_jIntegerClass = (jclass)p_Env->NewGlobalRef(tmp);
        m_jNativeMethodStrId2 = p_Env->GetStaticMethodID(m_jClass, "measureText", "(Ljava/lang/String;Ljava/lang/String;I)[Ljava/lang/Object;");
    }

	//TODO 传过来的可能是 laya.game.browser.xx, 这里需要转换成 laya/game/browser/xx
	/*jmethodID methodid=NULL;
	try{
		jclass javacls = p_Env->FindClass(p_sCls);	
		jclass cls = reinterpret_cast<jclass>(p_Env->NewGlobalRef(javacls));	//TODO 引用问题,每添加一个函数就加一次，每删除 一个就减一次

		methodid = 	p_Env->GetStaticMethodID( cls, p_sMethod, p_sDesc);

		JavaMethodMap::iterator it = m_StaticMethods.find( p_nKey );
		if( it!=m_StaticMethods.end())
		{
			LOGE("注意：已经存在这个函数的定义了 %d,对应的java函数是 %s", p_nKey, p_sMethod);
			//删除对某个java类的引用
			p_Env->DeleteGlobalRef((*it).second.cls);
		}
		
		JavaMethod method(cls, methodid);
		paserParamDesc( p_sDesc,method.paramInfo,method.retureValueType );
		m_StaticMethods[p_nKey]=method;
	}catch(...){
		LOGE("addStaticMethod 失败，可能是函数名称或者参数写错了，请检查一下。\ncls=%s,method=%s", p_sCls, p_sMethod);
	}*/
	return NULL;
}
CToJavaBridge::ThreadJNIData* CToJavaBridge::checkThreadJNI(){
	if( m_pJVM == NULL ) return 0;
	void* pdata = pthread_getspecific(m_kThreadkeyJNI);
	//如果没有set，则会返回0
	if( !pdata ){
		ThreadJNIData* pThreadData = new ThreadJNIData();
		pThreadData->pJVM = m_pJVM;
		m_pJVM->AttachCurrentThread(&(pThreadData->pThreadJNI),0);
		if( pThreadData->pThreadJNI == 0 )
			return 0;

		//设置线程退出的时候的回调
		pthread_setspecific( m_kThreadkeyJNI, pThreadData );
		return pThreadData;
	}
	return (ThreadJNIData*)pdata;
}
/*
	返回的指针是重新分配内存的。
*/
int* CToJavaBridge::getJavaIntArray(JNIEnv* p_pJNI, jobject p_obj){
	jarray javaarr = (jarray)p_obj;
	if( javaarr==0 || p_pJNI == NULL )
		return 0;

	int len = p_pJNI->GetArrayLength(javaarr);
	jintArray * arr = reinterpret_cast<jintArray*>(&p_obj);
	int* pIntBuff = p_pJNI->GetIntArrayElements(*arr, 0);
	int* pRet = new int[len];
	if( pRet )
		memcpy(pRet, pIntBuff, len*sizeof(int));
	else
	{}
	//释放
	p_pJNI->ReleaseIntArrayElements(*arr, pIntBuff, 0);
	return pRet;
}
/*
	返回的指针是参数提供的
*/
int* CToJavaBridge::getJavaIntArray(JNIEnv* p_pJNI, jobject p_obj , char* p_pBuff, int& p_nByteLen){
	jarray javaarr = (jarray)p_obj;
	if( javaarr==0 || p_pJNI == NULL )
		return 0;

	int len = p_pJNI->GetArrayLength(javaarr);

	if(len*sizeof(int)>p_nByteLen)//提供的内存不足
		return NULL;

	jintArray * arr = reinterpret_cast<jintArray*>(&p_obj);
	int* pIntBuff = p_pJNI->GetIntArrayElements(*arr, 0);
	memcpy(p_pBuff, pIntBuff, len*sizeof(int));
	//释放
	p_pJNI->ReleaseIntArrayElements(*arr, pIntBuff, 0);
	p_nByteLen = len*sizeof(int);
	return (int*)p_pBuff;
}
std::string CToJavaBridge::getJavaString( JNIEnv* env, jstring jstr )
{
	if (jstr == NULL)return "";
	char* sBuffer = NULL;
	jclass clsstring = env->FindClass("java/lang/String");
	jstring strencode = env->NewStringUTF("utf-8");
	jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
	jbyteArray barr= (jbyteArray)env->CallObjectMethod(jstr, mid, strencode);
	jsize alen = env->GetArrayLength(barr);
	jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
	if (alen > 0)
	{
	    sBuffer = (char*)malloc(alen + 1);
		memcpy(sBuffer, ba, alen);
		sBuffer[alen] = 0;
	}
	env->ReleaseByteArrayElements(barr, ba, 0);
	env->DeleteLocalRef (barr);
	env->DeleteLocalRef (strencode);
	env->DeleteLocalRef (clsstring);
	std::string sRetString="";
    if( sBuffer )
    {
        sRetString = sBuffer;
        free( sBuffer );
    }
    return sRetString;
}
void correctUtfBytes(char* bytes)
{
	char three = 0;
	while (*bytes != '\0') {
		unsigned char utf8 = *(bytes++);
		three = 0;
		// Switch on the high four bits.
		switch (utf8 >> 4) {
		case 0x00:
		case 0x01:
		case 0x02:
		case 0x03:
		case 0x04:
		case 0x05:
		case 0x06:
		case 0x07:
			// Bit pattern 0xxx. No need for any extra bytes.
			break;
		case 0x08:
		case 0x09:
		case 0x0a:
		case 0x0b:
		case 0x0f:
			/*
			* Bit pattern 10xx or 1111, which are illegal start bytes.
			* Note: 1111 is valid for normal UTF-8, but not the
			* modified UTF-8 used here.
			*/
			*(bytes - 1) = '*';
			break;
		case 0x0e:
			// Bit pattern 1110, so there are two additional bytes.
			utf8 = *(bytes++);
			if ((utf8 & 0xc0) != 0x80) {
				--bytes;
				*(bytes - 1) = '*';
				break;
			}
			three = 1;
			// Fall through to take care of the final byte.
		case 0x0c:
		case 0x0d:
			// Bit pattern 110x, so there is one additional byte.
			utf8 = *(bytes++);
			if ((utf8 & 0xc0) != 0x80) {
				--bytes;
				if (three)--bytes;
				*(bytes - 1) = '*';
			}
			break;
		}
	}
}
void correctUtfBytes(char* bytes);
/*
void CToJavaBridge::getResultValue(JNIEnv* p_pJNI, jstring p_sJString, std::string &ret)
{
    ret = getJavaString(p_pJNI, p_sJString);
    LOGI("JSAndroidEditBoxgetResultValue::get_Value=%s", ret.c_str());
    laya::JCJson jcJson;
    int msglen = ret.length();
    char* ptmpMsg = new char[msglen + 1];
    ptmpMsg[msglen] = 0;
    memcpy(ptmpMsg, ret.c_str(), msglen);
    if (jcJson.paserJson((char*)ptmpMsg)) {
        laya::JsonObject* pRoot = (laya::JsonObject*)jcJson.getRoot();
        laya::JsonValue* pCmd = (laya::JsonValue*)pRoot->getNode("v");
        int jj = 0;
        if (pCmd)
        {
            ret = pCmd->m_sValue;
            LOGI("JSAndroidEditBoxgetResultValue::get_Value1=%s", ret.c_str());
            replace_all_distinct(ret, "\\/", "/");
            LOGI("JSAndroidEditBoxgetResultValue::get_Value2=%s", ret.c_str());
        }
        else
        {
            ret = "";
        }
    }
    else
    {
        ret = "";
    }
    delete[]ptmpMsg;
}
*/
bool CToJavaBridge::callMethodRefection(int objid,bool isSyn,const char* className, const char* methodName, const char* params, JavaRet& p_Ret)
{
    ThreadJNIData* pThreadData = checkThreadJNI();
    if (!pThreadData)return false;
    JNIEnv* pJNI = pThreadData->pThreadJNI;
    jstring jclassName = pJNI->NewStringUTF(className);
    jstring jmethodName = pJNI->NewStringUTF(methodName);
    jstring jparams = pJNI->NewStringUTF(params);
    jstring ret = (jstring)pJNI->CallStaticObjectMethod(m_jClass, m_jMethodIDRefection, objid,isSyn ,jclassName, jmethodName, jparams);
    p_Ret.pJNI = pJNI;
	p_Ret.retType = JavaRet::RT_String;
	p_Ret.strRet = ret;
	pJNI->DeleteLocalRef(jclassName);
	pJNI->DeleteLocalRef(jmethodName);
	pJNI->DeleteLocalRef(jparams);
    return true;
}
/** @brief
    通用反射函数
 *  @param[in]objid 对象id 如果是静态函数设为-1 
 *  @param[in]isSyn 是否是同步  c++
 *  @param[in]className 类名 如com.utils.test 或者 com/utils/test
 *  @param[in]methodName函数名
 *  @param[in]params  参数 jsonArray字符串
 *  @param[in]p_Ret  用来处理返回值
 *  @return[out]bool 是否成功

*/
bool CToJavaBridge::callMethod(int objid,bool isSyn,const char* className, const char* methodName, const char* params, JavaRet& p_Ret)
{
    ThreadJNIData* pThreadData = checkThreadJNI();
    if (!pThreadData)return false;
    JNIEnv* pJNI = pThreadData->pThreadJNI;
    jstring jclassName = pJNI->NewStringUTF(className);
    jstring jmethodName = pJNI->NewStringUTF(methodName);
    jstring jparams = pJNI->NewStringUTF(params);
    jstring ret = (jstring)pJNI->CallStaticObjectMethod(m_jClass, m_jMethodID, objid,isSyn ,jclassName, jmethodName, jparams);
    p_Ret.pJNI = pJNI;
	p_Ret.retType = JavaRet::RT_String;
	p_Ret.strRet = ret;
	pJNI->DeleteLocalRef(jclassName);
	pJNI->DeleteLocalRef(jmethodName);
	pJNI->DeleteLocalRef(jparams);
    return true;
}
/** @brief
*  C++直接调用 
   无参函数的调用 java层返回统一都是string 通过p_eReturnType来决定返回的类型
*/
bool CToJavaBridge::callMethod(const char* className, const char* methodName, JavaRet& p_Ret, JavaRet::RTType p_eReturnType)
{
    ThreadJNIData* pThreadData = checkThreadJNI();
    if (!pThreadData)return false;
    JNIEnv* pJNI = pThreadData->pThreadJNI;
    LOGE(">>>>>>>>>>>>>>>>>>>>>>>>>>>>className:%s,methodName：%s", className,methodName);
    jstring jclassName = pJNI->NewStringUTF(className);
    jstring jmethodName = pJNI->NewStringUTF(methodName);
    LOGE(">>>>>>>>>>>>before");
    jstring retstr = (jstring)pJNI->CallStaticObjectMethod(m_jClass, m_jNativeMethodStrId, jclassName, jmethodName, NULL);
    LOGE(">>>>>>>>>>>>after");
    p_Ret.pJNI = pJNI;
    p_Ret.retType = p_eReturnType;
    p_Ret.strRet = retstr;
    switch (p_eReturnType) {
    case JavaRet::RT_Object:
        break;
    case JavaRet::RT_String:
        break;
    case JavaRet::RT_Int:
    {
        std::string temp = getJavaString(pJNI, retstr);
        sscanf(temp.c_str(), "%d", &p_Ret.intRet);
    }
        break;
    case JavaRet::RT_Float:
    {
        std::string temp = getJavaString(pJNI, retstr);
        sscanf(temp.c_str(), "%f", &p_Ret.floatRet);
    }
    break;
    default:
        break;
    }

    pJNI->DeleteLocalRef(jclassName);
    pJNI->DeleteLocalRef(jmethodName);
    return true;
}
/** @brief
*  C++直接调用  参数为int 直接调用通用callMethod
*/
bool CToJavaBridge::callMethod(const char* className, const char* methodName,int n, JavaRet& ret)
{
    char buffer[20];
    sprintf(buffer, "[%d]", n);
    std::string params(buffer);
    return callMethod(-1, true, className, methodName, params.c_str(), ret);
}

/** @brief
*  C++直接调用  参数为const char*s1, const char*s2  直接调用通用callMethod
*/
bool CToJavaBridge::callMethod(const char* className, const char* methodName, const char*s1, const char*s2, JavaRet& ret)
{
    std::string param1 = s1;
    replace_all_distinct(param1, "\\", "\\\\");
    replace_all_distinct(param1, "\"", "\\\"");
    std::string param2 = s2;
    replace_all_distinct(param2, "\\", "\\\\");
    replace_all_distinct(param2, "\"", "\\\"");
    std::string result = "[\"" + param1 + "\",\"" + param2 + "\"]";
    return callMethod(-1, true, className, methodName, result.c_str(), ret);
}
/** @brief
  C++直接调用  参数为const char*s1, const char*s2  直接调用通用callMethod
*/
bool CToJavaBridge::callMethod(const char* className, const char* methodName, const char* s1, const char* s2,const char* s3, JavaRet& ret)
{
    std::string param1 = s1;
    replace_all_distinct(param1, "\\", "\\\\");
    replace_all_distinct(param1, "\"", "\\\"");
    std::string param2 = s2;
    replace_all_distinct(param2, "\\", "\\\\");
    replace_all_distinct(param2, "\"", "\\\"");
    std::string param3 = s3;
    replace_all_distinct(param3, "\\", "\\\\");
    replace_all_distinct(param3, "\"", "\\\"");
    std::string result = "[\"" + param1 + "\",\"" + param2 + "\",\"" + param3 +"\"]";
    return callMethod(-1, true, className, methodName, result.c_str(), ret);
}
/** @brief
*  C++直接调用  参数为const char*s1, int n,如playaAudio  直接调用通用callMethod
*/
bool CToJavaBridge::callMethod(const char* className, const char* methodName, const char*s, int n, JavaRet& ret)
{
    std::string param1 = s;
    replace_all_distinct(param1, "\\", "\\\\");
    replace_all_distinct(param1, "\"", "\\\"");
    char buffer[20];
    sprintf(buffer, "%d", n);
    std::string param2(buffer);
    std::string result = "[\"" + param1 + "\"," + param2 + "]";
    return callMethod(-1, true, className, methodName, result.c_str(), ret);
}
/** @brief
*  C++直接调用  参数为const char*s1, int n,如playaAudio  直接调用通用callMethod
*/
bool CToJavaBridge::callMethod(const char* className, const char* methodName, const char*s, int n, int n1,JavaRet& ret)
{
    std::string param1 = s;
    replace_all_distinct(param1, "\\", "\\\\");
    replace_all_distinct(param1, "\"", "\\\"");
    char buffer[20];
    sprintf(buffer, "%d,%d", n,n1);
    std::string param2(buffer);
    std::string result = "[\"" + param1 + "\"," + param2 + "]";
    return callMethod(-1, true, className, methodName, result.c_str(), ret);
}
/** @brief
*  C++直接调用  参数为int n1, int n2, const char*s1, const char*s2, const char*s3,目前只有setOnceNotify  直接调用通用callMethod
*/
bool CToJavaBridge::callMethod(const char* className, const char* methodName, int n1, int n2, const char*s1, const char*s2, const char*s3, JavaRet& ret)
{
    char buffer[40];
    sprintf(buffer, "%d,%d,", n1,n2);
    std::string param12(buffer);
    std::string param1 = s1;
    replace_all_distinct(param1, "\\", "\\\\");
    replace_all_distinct(param1, "\"", "\\\"");
    std::string param2 = s2;
    replace_all_distinct(param2, "\\", "\\\\");
    replace_all_distinct(param2, "\"", "\\\"");
    std::string param3 = s3;
    replace_all_distinct(param3, "\\", "\\\\");
    replace_all_distinct(param3, "\"", "\\\"");
    std::string result = "[" + param12 + "\"" + param1 + "\",\"" + param2 + "\",\"" + param3 + "\"]";
    return callMethod(-1, true, className, methodName, result.c_str(), ret);
}
/** @brief
*  C++直接调用  参数为int n1, int n2,int n3, const char*s1, const char*s2, const char*s3,目前只有setRepeatNotify  直接调用通用callMethod
*/
bool CToJavaBridge::callMethod(const char* className, const char* methodName, int n1, int n2, int n3, const char*s1, const char*s2, const char*s3, JavaRet& ret)
{
    char buffer[60];
    sprintf(buffer, "%d,%d,%d,", n1, n2,n3);
    std::string param123(buffer);
    std::string param1 = s1;
    replace_all_distinct(param1, "\\", "\\\\");
    replace_all_distinct(param1, "\"", "\\\"");
    std::string param2 = s2;
    replace_all_distinct(param2, "\\", "\\\\");
    replace_all_distinct(param2, "\"", "\\\"");
    std::string param3 = s3;
    replace_all_distinct(param3, "\\", "\\\\");
    replace_all_distinct(param3, "\"", "\\\"");
    std::string result = "[" + param123 + "\"" + param1 + "\",\"" + param2 + "\",\"" + param3 + "\"]";
    return callMethod(-1, true, className, methodName, result.c_str(), ret);

}
/** @brief
*  C++直接调用  参数为float f,如 setVolume  直接调用通用callMethod
*/
bool CToJavaBridge::callMethod(const char* className, const char* methodName, float f, JavaRet& ret)
{
    char buffer[40];
    sprintf(buffer, "[%f]", f);
    std::string params(buffer);
    return callMethod(-1, true, className, methodName, params.c_str(), ret);
}
/** @brief
*  C++直接调用  参数为float x float y,  直接调用通用callMethod
*/
bool CToJavaBridge::callMethod(const char* className, const char* methodName, float x,float y, JavaRet& ret)
{
	char buffer[40];
	sprintf(buffer, "[%f,%f]",x,y);
	std::string params(buffer);
	return callMethod(-1, true, className, methodName, params.c_str(), ret);
}
/** @brief
*  C++直接调用  参数为bool b,如 showLoadingviw  直接调用通用callMethod
*/
bool CToJavaBridge::callMethod(const char* className, const char* methodName, bool b, JavaRet& ret)
{
    return callMethod(-1, true, className, methodName, b?"[true]":"[false]", ret);
}
/** @brief
*  C++直接调用  参数为int x,int y,如  setEditBoxSize  直接调用通用callMethod
*/
bool CToJavaBridge::callMethod(const char* className, const char* methodName, int x,int y,JavaRet& ret)
{
    char buffer[40];
    sprintf(buffer, "[%d,%d]",x,y);
    std::string params(buffer);
    return callMethod(-1, true, className, methodName, params.c_str(), ret);
}

/** @brief
*  C++直接调用  参数为int x,int y,int w,int h 如直接调用通用callMethod
*/
bool CToJavaBridge::callMethod(const char* className, const char* methodName, int x, int y,int w,int h,JavaRet& ret)
{
	char buffer[64];
	sprintf(buffer, "[%d,%d,%d,%d]", x, y,w,h);
	std::string params(buffer);
	return callMethod(-1, true, className, methodName, params.c_str(), ret);
}

/** @brief
*  C++直接调用  参数为char* s,int x,int y,int w,int h,int bKeyCloseView 如直接调用通用callMethod
*/
bool CToJavaBridge::callMethod(const char* className, const char* methodName,const char* s, int x, int y,int w,int h,int bKeyCloseView,JavaRet& ret)
{
	std::string param1 = s;
    replace_all_distinct(param1, "\\", "\\\\");
    replace_all_distinct(param1, "\"", "\\\"");
    char buffer[64];
	sprintf(buffer, "%d,%d,%d,%d,%d", x, y,w,h,bKeyCloseView);
    std::string paramInt(buffer);
    std::string result = "[\"" + param1 + "\"," + paramInt + "]";
	return callMethod(-1, true, className, methodName, result.c_str(), ret);
}

/** @brief
*  C++直接调用
参数为一个字符串的调用 java层返回统一都是string 通过p_eReturnType来决定返回的类型
*/
bool CToJavaBridge::callMethod(const char* className, const char* methodName, const char*s, JavaRet& p_Ret, JavaRet::RTType p_eReturnType)
{
    ThreadJNIData* pThreadData = checkThreadJNI();
    if (!pThreadData)return false;
    JNIEnv* pJNI = pThreadData->pThreadJNI;
    jstring jclassName = pJNI->NewStringUTF(className);
    jstring jmethodName = pJNI->NewStringUTF(methodName);
    jstring jparams = pJNI->NewStringUTF(s);
    jstring retstr = (jstring)pJNI->CallStaticObjectMethod(m_jClass, m_jNativeMethodStrId, jclassName, jmethodName, jparams);
    p_Ret.pJNI = pJNI;
    p_Ret.retType = p_eReturnType;
    p_Ret.strRet = retstr;
    switch (p_eReturnType) {
    case JavaRet::RT_Object:
        break;
    case JavaRet::RT_String:
        break;
    case JavaRet::RT_Int:
    {
        std::string temp=getJavaString(pJNI, retstr);
        sscanf(temp.c_str(), "%d", &p_Ret.intRet);
    }
        break;
    case JavaRet::RT_Float:
    {
        std::string temp = getJavaString(pJNI, retstr);
        sscanf(temp.c_str(), "%f", &p_Ret.floatRet);
    }
    break;
    default:
        break;
    }
    pJNI->DeleteLocalRef(jclassName);
    pJNI->DeleteLocalRef(jmethodName);
    pJNI->DeleteLocalRef(jparams);
    return true;
    /*replace_all_distinct(s, "\\\"", "\\\\\"");
    replace_all_distinct(s, "\"", "\\\"");
    s = "[\"" + s + "\"]";
    callMethod(objid, isSyn, className, methodName, s.c_str(), ret);*/
}

void CToJavaBridge::replace_all_distinct(std::string&   str, const   std::string&   old_value, const   std::string&   new_value)
{
    for (std::string::size_type pos(0); pos != std::string::npos; pos += new_value.length()) {
        if ((pos = str.find(old_value, pos)) != std::string::npos)
            str.replace(pos, old_value.length(), new_value);
        else   break;
    }
}

bool CToJavaBridge::getTextBitmap(laya::BitmapData* bitmapData, const char*text, laya::JCFontInfo* pFontInfo, int fontColor, int borderSize, int nBorderColor)
{
    ThreadJNIData* pThreadData = checkThreadJNI();
    if (!pThreadData)return false;
    JNIEnv* pJNI = pThreadData->pThreadJNI;
	
    jstring jstrFont = pJNI->NewStringUTF(pFontInfo->m_sFamily);
    jstring jstrText = pJNI->NewStringUTF(text);
    jobjectArray objectArray = (jobjectArray)pJNI->CallStaticObjectMethod(m_jClass, m_jNativeMethodStrId1, jstrText, jstrFont, pFontInfo->m_nFontSize, fontColor, borderSize, nBorderColor);

    if (objectArray == NULL)
    {
        pJNI->DeleteLocalRef(jstrText);
        pJNI->DeleteLocalRef(jstrFont);
        return false;
    }
    int maxSize = bitmapData->m_nWidth * bitmapData->m_nHeight;
    jobject objInteger = pJNI->GetObjectArrayElement(objectArray, 0);
    jmethodID intValue = pJNI->GetMethodID(m_jIntegerClass, "intValue", "()I");
    bitmapData->m_nWidth = (int)pJNI->CallIntMethod( objInteger, intValue);
    pJNI->DeleteLocalRef(objInteger);

    objInteger = pJNI->GetObjectArrayElement(objectArray, 1);
    intValue = pJNI->GetMethodID(m_jIntegerClass, "intValue", "()I");
    bitmapData->m_nHeight = (int)pJNI->CallIntMethod(objInteger, intValue);
    pJNI->DeleteLocalRef(objInteger);

    if (maxSize < bitmapData->m_nWidth * bitmapData->m_nHeight)
    { 
        LOGW("CToJavaBridge::getTextBitmap the buffersize is insufficient.");
        return false;
    }
    jbyteArray newArray = (jbyteArray)pJNI->GetObjectArrayElement( objectArray, 2);
    jsize theArrayLen = pJNI->GetArrayLength(newArray);
    pJNI->GetByteArrayRegion(newArray, 0, theArrayLen, (jbyte*)bitmapData->m_pImageData);
    pJNI->DeleteLocalRef(newArray);

    pJNI->DeleteLocalRef(objectArray);
    pJNI->DeleteLocalRef(jstrText);
    pJNI->DeleteLocalRef(jstrFont);
    return true;
}
void CToJavaBridge::measureText(laya::JCFontInfo* pFontInfo, const char* p_sText, int& nResultWidth, int& nResultHeight)
{
    ThreadJNIData* pThreadData = checkThreadJNI();
    if (!pThreadData)return;
    JNIEnv* pJNI = pThreadData->pThreadJNI;

    jstring jstrText = pJNI->NewStringUTF(p_sText);
    jstring jstrFont = pJNI->NewStringUTF(pFontInfo->m_sFamily);
    jobjectArray objectArray = (jobjectArray)pJNI->CallStaticObjectMethod(m_jClass, m_jNativeMethodStrId2, jstrText, jstrFont, (int)pFontInfo->m_nFontSize);

    if (objectArray == NULL)
    {
        pJNI->DeleteLocalRef(jstrText);
        pJNI->DeleteLocalRef(jstrFont);
        return;
    }

    jobject objInteger = pJNI->GetObjectArrayElement(objectArray, 0);
    jmethodID intValue = pJNI->GetMethodID(m_jIntegerClass, "intValue", "()I");
    nResultWidth = (int)pJNI->CallIntMethod(objInteger, intValue);
    pJNI->DeleteLocalRef(objInteger);

    objInteger = pJNI->GetObjectArrayElement(objectArray, 1);
    intValue = pJNI->GetMethodID(m_jIntegerClass, "intValue", "()I");
    nResultHeight = (int)pJNI->CallIntMethod(objInteger, intValue);
    pJNI->DeleteLocalRef(objInteger);

    pJNI->DeleteLocalRef(objectArray);
    pJNI->DeleteLocalRef(jstrText);
    pJNI->DeleteLocalRef(jstrFont);
    return;
}

bool CToJavaBridge::getClassAndMethod(const char* className, const char* methodName, const char* methodSign, JNIEnv** ppEnv, jclass* pClazz, jmethodID* pMethodId)
{
	ThreadJNIData* pThreadData = checkThreadJNI();
	if (!pThreadData)return false;
	*ppEnv = pThreadData->pThreadJNI;

	JNIEnv* pEnv = *ppEnv;

	jclass thisClass = NULL;
	auto it = m_classMap.find(className);
	if (it != m_classMap.end())
		thisClass = it->second;

	if (thisClass == NULL)
	{
		LOGE("[Debug][???]Get class failed");
		return false;
	}

	//LOGI("[Debug][???]Get class OK %d", thisClass);
	*pClazz = thisClass;

	jmethodID methodID = pEnv->GetMethodID(thisClass, methodName, methodSign);
	if (NULL == methodID)
	{
		LOGE("[Debug][???]GetMethodID %s failed", methodName);
		return false;
	}

	//LOGI("[Debug][Video]GetMethodID %s OK  %d", methodName, methodID);

	*pMethodId = methodID;

	return true;
}

bool CToJavaBridge::newObject(jobject* obj, const char* className)
{
	static const char* s_methodSign = "()V";

	//jclass cls = pJNI->FindClass(className);

	JNIEnv* pJNI = nullptr;
	jclass cls = NULL;
	jmethodID methodID = NULL;

	bool ok = getClassAndMethod(className, "<init>", s_methodSign, &pJNI, &cls, &methodID);
	if (!ok)
		return false;

	jobject newObj = pJNI->NewObject(cls, methodID);
	*obj = pJNI->NewGlobalRef(newObj);

	if (*obj == NULL)
	{
		LOGI("[Debug][???]New object failed");
		return false;
	}

	if (pJNI->ExceptionOccurred()) {
		pJNI->ExceptionDescribe();
		pJNI->ExceptionClear();
	}

	return true;
}

bool CToJavaBridge::newObject(jobject* obj, const char* className, intptr_t thisObj)
{
	static const char* s_methodSign = "(J)V";

	//jclass cls = pJNI->FindClass(className);

	JNIEnv* pJNI = nullptr;
	jclass cls = NULL;
	jmethodID methodID = NULL;

	bool ok = getClassAndMethod(className, "<init>", s_methodSign, &pJNI, &cls, &methodID);
	if (!ok)
		return false;

	jobject newObj = pJNI->NewObject(cls, methodID, (int64_t)thisObj);
	*obj = pJNI->NewGlobalRef(newObj);


	if (*obj == NULL)
	{
		LOGI("[Debug][???]New object failed");
		return false;
	}

	if (pJNI->ExceptionOccurred()) {
		pJNI->ExceptionDescribe();
		pJNI->ExceptionClear();
	}

	return true;
}

bool CToJavaBridge::disposeObject(jobject& obj, const char* className, const char* methodName)
{
	static const char* s_methodSign = "()V";

	JNIEnv* pJNI = nullptr;
	jclass thisClass = NULL;
	jmethodID methodID = NULL;

	bool ok = getClassAndMethod(className, methodName, s_methodSign, &pJNI, &thisClass, &methodID);
	if (!ok)
		return false;

	if (!obj)
	{
		LOGE("[CToJavaBridge::disposeObject] obj is null");
		return false;
	}

	pJNI->CallVoidMethod(obj, methodID);

	pJNI->DeleteGlobalRef(obj);

	if (pJNI->ExceptionOccurred()) {
		pJNI->ExceptionDescribe();
		pJNI->ExceptionClear();
	}
	return true;
}


bool CToJavaBridge::callObjVoidMethod(jobject& obj, const char* className, const char* methodName)
{
	if (obj == NULL)
		return false;
	
	static const char* s_methodSign = "()V";

	JNIEnv* pJNI = nullptr;
	jclass thisClass = NULL;
	jmethodID methodID = NULL;

	bool ok = getClassAndMethod(className, methodName, s_methodSign, &pJNI, &thisClass, &methodID);
	if (!ok)
		return false;

	pJNI->CallVoidMethod(obj, methodID);

	if (pJNI->ExceptionOccurred()) {
		pJNI->ExceptionDescribe();
		pJNI->ExceptionClear();
	}

	return true;
}

bool CToJavaBridge::callObjVoidMethod(jobject& obj, const char* className, const char* methodName, const char* arg)
{
	if (obj == NULL)
		return false;
	
	static const char* s_methodSign = "(Ljava/lang/String;)V";

	//jclass thisClass = m_pDefJNIEnv->GetObjectClass(obj);

	JNIEnv* pJNI = nullptr;
	jclass thisClass = NULL;
	jmethodID methodID = NULL;

	bool ok = getClassAndMethod(className, methodName, s_methodSign, &pJNI, &thisClass, &methodID);
	if (!ok)
		return false;

	jstring jArg = pJNI->NewStringUTF(arg);
	pJNI->CallVoidMethod(obj, methodID, jArg);

	pJNI->DeleteLocalRef(jArg);
	
	if (pJNI->ExceptionOccurred()) {
		pJNI->ExceptionDescribe();
		pJNI->ExceptionClear();
	}

	return true;
}

bool CToJavaBridge::callObjVoidMethod(jobject& obj, const char* className, const char* methodName, int32_t arg)
{
	if (obj == NULL)
		return false;
	
	static const char* s_methodSign = "(I)V";

	JNIEnv* pJNI = nullptr;
	jclass thisClass = NULL;
	jmethodID methodID = NULL;

	bool ok = getClassAndMethod(className, methodName, s_methodSign, &pJNI, &thisClass, &methodID);
	if (!ok)
		return false;

	pJNI->CallVoidMethod(obj, methodID, arg);

	if (pJNI->ExceptionOccurred()) {
		pJNI->ExceptionDescribe();
		pJNI->ExceptionClear();
	}
	return true;
}

bool CToJavaBridge::callObjVoidMethod(jobject& obj, const char* className, const char* methodName, int64_t arg)
{
	if (obj == NULL)
		return false;
	
	static const char* s_methodSign = "(J)V";

	JNIEnv* pJNI = nullptr;
	jclass thisClass = NULL;
	jmethodID methodID = NULL;

	bool ok = getClassAndMethod(className, methodName, s_methodSign, &pJNI, &thisClass, &methodID);
	if (!ok)
		return false;

	pJNI->CallVoidMethod(obj, methodID, arg);

	if (pJNI->ExceptionOccurred()) {
		pJNI->ExceptionDescribe();
		pJNI->ExceptionClear();
	}
	return true;
}

bool CToJavaBridge::callObjVoidMethod(jobject& obj, const char* className, const char* methodName, bool arg)
{
	if (obj == NULL)
		return false;
	
	static const char* s_methodSign = "(Z)V";

	JNIEnv* pJNI = nullptr;
	jclass thisClass = NULL;
	jmethodID methodID = NULL;

	bool ok = getClassAndMethod(className, methodName, s_methodSign, &pJNI, &thisClass, &methodID);
	if (!ok)
		return false;

	pJNI->CallVoidMethod(obj, methodID, arg);

	if (pJNI->ExceptionOccurred()) {
		pJNI->ExceptionDescribe();
		pJNI->ExceptionClear();
	}
	return true;
}


bool CToJavaBridge::callObjVoidMethod(jobject& obj, const char* className, const char* methodName, double arg)
{
	if (obj == NULL)
		return false;
	
	static const char* s_methodSign = "(D)V";

	JNIEnv* pJNI = nullptr;
	jclass thisClass = NULL;
	jmethodID methodID = NULL;

	bool ok = getClassAndMethod(className, methodName, s_methodSign, &pJNI, &thisClass, &methodID);
	if (!ok)
		return false;

	pJNI->CallVoidMethod(obj, methodID, arg);

	if (pJNI->ExceptionOccurred()) {
		pJNI->ExceptionDescribe();
		pJNI->ExceptionClear();
	}
	return true;
}

bool CToJavaBridge::callObjRetMethod(jobject& obj, const char* className, const char* methodName, int* ret)
{
	if (obj == NULL)
		return false;
	
	static const char* s_methodSign = "()I";

	JNIEnv* pJNI = nullptr;
	jclass thisClass = NULL;
	jmethodID methodID = NULL;

	bool ok = getClassAndMethod(className, methodName, s_methodSign, &pJNI, &thisClass, &methodID);
	if (!ok)
		return false;

	*ret = (int) pJNI->CallIntMethod(obj, methodID);

	if (pJNI->ExceptionOccurred()) {
		pJNI->ExceptionDescribe();
		pJNI->ExceptionClear();
	}
	return true;
}

bool CToJavaBridge::callObjRetMethod(jobject& obj, const char* className, const char* methodName, double* ret)
{
	if (obj == NULL)
		return false;
	
	static const char* s_methodSign = "()D";

	JNIEnv* pJNI = nullptr;
	jclass thisClass = NULL;
	jmethodID methodID = NULL;

	bool ok = getClassAndMethod(className, methodName, s_methodSign, &pJNI, &thisClass, &methodID);
	if (!ok)
		return false;

	*ret = pJNI->CallDoubleMethod(obj, methodID);

	if (pJNI->ExceptionOccurred()) {
		pJNI->ExceptionDescribe();
		pJNI->ExceptionClear();
	}

	return true;
}

bool CToJavaBridge::callObjRetMethod(jobject& obj, const char* className, const char* methodName, bool* ret)
{
	if (obj == NULL)
		return false;
	
	static const char* s_methodSign = "()Z";

	JNIEnv* pJNI = nullptr;
	jclass thisClass = NULL;
	jmethodID methodID = NULL;

	bool ok = getClassAndMethod(className, methodName, s_methodSign, &pJNI, &thisClass, &methodID);
	if (!ok)
		return false;

	*ret = (bool)pJNI->CallBooleanMethod(obj, methodID);

	if (pJNI->ExceptionOccurred()) {
		pJNI->ExceptionDescribe();
		pJNI->ExceptionClear();
	}
	return true;

}

//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
