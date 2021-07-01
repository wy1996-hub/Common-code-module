#include "CinfoLogger.h"


//-------------------------------------------------------------------------
//
// ------ 内部变量与函数 ------
// 
//-------------------------------------------------------------------------


// 类实例静态指针（初始为空）
CinfoLogger* CinfoLogger::_instance = NULL;


/*----------------------------
* 功能 : 类实例初始化
*----------------------------
* 函数 : CinfoLogger::CinfoLogger
* 访问 : private
* 返回 :
*/
CinfoLogger::CinfoLogger(void)
{
	// 启动信息
	m_strWriteStrInfo = "";

	// 日志文件名称
	m_strLogFileName = "Run_" + GetCurTimeToString(FOR_FILE_NAME) + ".log";

	// 创建日志线程
	m_bRun = true;
	m_hThread = boost::thread(boost::bind(&CinfoLogger::LogProcStart, this));
}


/*----------------------------
* 功能 : 日志循环线程
* 每隔五秒定时、或者 日志缓存字符串长度大于阈值时
* 将缓存的日志信息强制写入到本地文件
*----------------------------
* 函数 : CinfoLogger::LogProcStart
* 访问 : private
* 返回 : void
*/
void
CinfoLogger::LogProcStart()
{
	int nCount = 1;
	clock_t preSaveTime = clock();
	do
	{
		Sleep(250);

		if ((clock() - preSaveTime) > TIME_TO_WRITE_LOG)
		{
			preSaveTime = clock();
			WriteLogToFile();	// 每隔五秒写一次日志
		}

	} while (m_bRun);

	if (!m_strWriteStrInfo.empty())
		WriteLogToFile();	// 退出循环后如果缓存非空，则将缓存写入日志 再退出线程

	cout << LogInfo(LOG_LEVEL_SUB_1, "[- Info Logger] 正在退出日志线程！") << endl;
}


/*----------------------------
* 功能 : 更新日志缓存
*----------------------------
* 函数 : CinfoLogger::WriteLog
* 访问 : private
* 返回 : void
*
* 参数 : string		strLog		[in]	日志信息
*/
void
CinfoLogger::WriteLog(string strLog)
{
	if (!strLog.empty())
	{
		m_strWriteStrInfo += "[" + GetCurTimeToString() + "] ";
		m_strWriteStrInfo += strLog;
		m_strWriteStrInfo += _T("\r\n");
	}

	if (m_strWriteStrInfo.length() > MAX_STR_BUF_LEN)
	{
		WriteLogToFile();	// 达到一定量时才写入到文件中
	}
}


/*----------------------------
* 功能 : 将缓存的日志信息写入到本地日志文件
*----------------------------
* 函数 : CinfoLogger::WriteLogToFile
* 访问 : private
* 返回 : void
*/
void
CinfoLogger::WriteLogToFile()
{
	if (m_logMutex.try_lock())
	{
		try
		{
			// 以 Append 方式打开日志文件
			ofstream osWriter(m_strLogFileName.c_str(), ios::out | ios_base::app);

			if (osWriter.is_open())
			{
				// 写入日志缓存
				osWriter << m_strWriteStrInfo.c_str();

				// 清空日志缓存
				m_strWriteStrInfo = "";

				// 关闭文件写入器
				osWriter.close();
			}
		}
		catch (std::exception e)
		{
			cout << LogInfo(LOG_LEVEL_FORMAT, "写入文件失败：%s", e.what()) << endl;
		}

		m_logMutex.unlock();
	}
}


//-------------------------------------------------------------------------
// 
// ------- 访问接口 ------
// 
//-------------------------------------------------------------------------


/*----------------------------
* 功能 : 创建类实例静态指针
*----------------------------
* 函数 : CinfoLogger::instance
* 访问 : public static
* 返回 : CinfoLogger*
*/
CinfoLogger*
CinfoLogger::instance()
{
	// 若静态指针为空，则创建类实例，并获取指针
	if (_instance == NULL)
	{
		_instance = new CinfoLogger();
	}
	// 返回静态指针
	return _instance;
}


/*----------------------------
* 功能 : 将 日志信息 输出到 IDE 输出窗口，并保存到 日志文件缓存 中
*----------------------------
* 函数 : CinfoLogger::LogInfo
* 访问 : public
* 返回 : string				[ret]	格式化后的字符串
*
* 参数 : int		level	[in]	日志级别
*									 按照程序分支级别定义，主程序 Kinect_NI 的级别为 0
*									 当 level < 0 时，功能相当于 sprintf 函数
*									 仅生成 string 字符串，不在输出窗口显示、也不写入缓存
* 参数 : char *		fmt		[in]	格式化字符串
* 参数 : ...				[in]	具体参数
*/
string
CinfoLogger::LogInfo(int level, char *fmt, ...)
{
	// 格式化输入的字符串 fmt 到输出字符串 out
	char out[1024];
	va_list body;
	va_start(body, fmt);
	vsprintf_s(out, fmt, body);
	va_end(body);

	// 将日志信息转换为 string 字符串，存入缓存变量中
	string strTemp(out);

	//日志级别需要大于设定级别 或者 小于 0，则只返回字符串，不作记录
	if (level > CparamServer::instance()->LogLevel() || level < 0)
	{
		return strTemp;
	}

	// 在输出窗口显示日志信息
	OutputDebugStringA(out);
	OutputDebugStringA("\n");

	// 将信息存入日志缓存中
	WriteLog(strTemp);

	return strTemp;
}


/*----------------------------
*	LogInfo 重载，日志信息 为 字符串常量
*----------------------------
* 参数 : int		level	[in]	日志级别，按照程序分支级别定义，主程序 Kinect_NI 的级别为 0
* 参数 : string		info	[in]	字符串常量
*/
string
CinfoLogger::LogInfo(int level, string info)
{
	//日志级别需要小于或等于设定级别，否则不记录
	if (level > CparamServer::instance()->LogLevel())
	{
		return info;
	}

	if (level >= 0)
	{
		// 在输出窗口显示日志信息
		OutputDebugStringA(info.c_str());
		// 将信息存入日志缓存中
		WriteLog(info);
	}

	return info;
}


/*----------------------------
* 功能 : 将当前系统时间转换为 string 字符串
*----------------------------
* 函数 : CinfoLogger::GetCurTimeStr
* 访问 : public
* 返回 : std::string	[ret]	系统时间字符串
*
* 参数 : TIME_STRING_MODE	mode	[in] 时间字符串的输出格式，分两种：文件名模式 和 文本模式
*/
string
CinfoLogger::GetCurTimeToString(TIME_STRING_MODE mode /* = FOR_TEXT */)
{
	int strLen = 20;
	string retStr = "0";

	if (mode == FOR_TEXT)
	{
		char *pszCurrTime1 = (char*)malloc(sizeof(char)*strLen);
		memset(pszCurrTime1, 0, sizeof(char)*strLen);
		time_t now;
		time(&now);
		strftime(pszCurrTime1, strLen, "%Y-%m-%d %H:%M:%S", localtime(&now));

		retStr = pszCurrTime1;
	}
	else if (mode == FOR_FILE_NAME)
	{
		char *pszCurrTime2 = (char*)malloc(sizeof(char)*strLen);
		memset(pszCurrTime2, 0, sizeof(char)*strLen);
		time_t now;
		time(&now);
		strftime(pszCurrTime2, strLen, "%Y_%m_%d_%H_%M_%S", localtime(&now));

		retStr = pszCurrTime2;
	}

	return retStr;
}