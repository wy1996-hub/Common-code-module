#include "CinfoLogger.h"


//-------------------------------------------------------------------------
//
// ------ �ڲ������뺯�� ------
// 
//-------------------------------------------------------------------------


// ��ʵ����ָ̬�루��ʼΪ�գ�
CinfoLogger* CinfoLogger::_instance = NULL;


/*----------------------------
* ���� : ��ʵ����ʼ��
*----------------------------
* ���� : CinfoLogger::CinfoLogger
* ���� : private
* ���� :
*/
CinfoLogger::CinfoLogger(void)
{
	// ������Ϣ
	m_strWriteStrInfo = "";

	// ��־�ļ�����
	m_strLogFileName = "Run_" + GetCurTimeToString(FOR_FILE_NAME) + ".log";

	// ������־�߳�
	m_bRun = true;
	m_hThread = boost::thread(boost::bind(&CinfoLogger::LogProcStart, this));
}


/*----------------------------
* ���� : ��־ѭ���߳�
* ÿ�����붨ʱ������ ��־�����ַ������ȴ�����ֵʱ
* ���������־��Ϣǿ��д�뵽�����ļ�
*----------------------------
* ���� : CinfoLogger::LogProcStart
* ���� : private
* ���� : void
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
			WriteLogToFile();	// ÿ������дһ����־
		}

	} while (m_bRun);

	if (!m_strWriteStrInfo.empty())
		WriteLogToFile();	// �˳�ѭ�����������ǿգ��򽫻���д����־ ���˳��߳�

	cout << LogInfo(LOG_LEVEL_SUB_1, "[- Info Logger] �����˳���־�̣߳�") << endl;
}


/*----------------------------
* ���� : ������־����
*----------------------------
* ���� : CinfoLogger::WriteLog
* ���� : private
* ���� : void
*
* ���� : string		strLog		[in]	��־��Ϣ
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
		WriteLogToFile();	// �ﵽһ����ʱ��д�뵽�ļ���
	}
}


/*----------------------------
* ���� : ���������־��Ϣд�뵽������־�ļ�
*----------------------------
* ���� : CinfoLogger::WriteLogToFile
* ���� : private
* ���� : void
*/
void
CinfoLogger::WriteLogToFile()
{
	if (m_logMutex.try_lock())
	{
		try
		{
			// �� Append ��ʽ����־�ļ�
			ofstream osWriter(m_strLogFileName.c_str(), ios::out | ios_base::app);

			if (osWriter.is_open())
			{
				// д����־����
				osWriter << m_strWriteStrInfo.c_str();

				// �����־����
				m_strWriteStrInfo = "";

				// �ر��ļ�д����
				osWriter.close();
			}
		}
		catch (std::exception e)
		{
			cout << LogInfo(LOG_LEVEL_FORMAT, "д���ļ�ʧ�ܣ�%s", e.what()) << endl;
		}

		m_logMutex.unlock();
	}
}


//-------------------------------------------------------------------------
// 
// ------- ���ʽӿ� ------
// 
//-------------------------------------------------------------------------


/*----------------------------
* ���� : ������ʵ����ָ̬��
*----------------------------
* ���� : CinfoLogger::instance
* ���� : public static
* ���� : CinfoLogger*
*/
CinfoLogger*
CinfoLogger::instance()
{
	// ����ָ̬��Ϊ�գ��򴴽���ʵ��������ȡָ��
	if (_instance == NULL)
	{
		_instance = new CinfoLogger();
	}
	// ���ؾ�ָ̬��
	return _instance;
}


/*----------------------------
* ���� : �� ��־��Ϣ ����� IDE ������ڣ������浽 ��־�ļ����� ��
*----------------------------
* ���� : CinfoLogger::LogInfo
* ���� : public
* ���� : string				[ret]	��ʽ������ַ���
*
* ���� : int		level	[in]	��־����
*									 ���ճ����֧�����壬������ Kinect_NI �ļ���Ϊ 0
*									 �� level < 0 ʱ�������൱�� sprintf ����
*									 ������ string �ַ������������������ʾ��Ҳ��д�뻺��
* ���� : char *		fmt		[in]	��ʽ���ַ���
* ���� : ...				[in]	�������
*/
string
CinfoLogger::LogInfo(int level, char *fmt, ...)
{
	// ��ʽ��������ַ��� fmt ������ַ��� out
	char out[1024];
	va_list body;
	va_start(body, fmt);
	vsprintf_s(out, fmt, body);
	va_end(body);

	// ����־��Ϣת��Ϊ string �ַ��������뻺�������
	string strTemp(out);

	//��־������Ҫ�����趨���� ���� С�� 0����ֻ�����ַ�����������¼
	if (level > CparamServer::instance()->LogLevel() || level < 0)
	{
		return strTemp;
	}

	// �����������ʾ��־��Ϣ
	OutputDebugStringA(out);
	OutputDebugStringA("\n");

	// ����Ϣ������־������
	WriteLog(strTemp);

	return strTemp;
}


/*----------------------------
*	LogInfo ���أ���־��Ϣ Ϊ �ַ�������
*----------------------------
* ���� : int		level	[in]	��־���𣬰��ճ����֧�����壬������ Kinect_NI �ļ���Ϊ 0
* ���� : string		info	[in]	�ַ�������
*/
string
CinfoLogger::LogInfo(int level, string info)
{
	//��־������ҪС�ڻ�����趨���𣬷��򲻼�¼
	if (level > CparamServer::instance()->LogLevel())
	{
		return info;
	}

	if (level >= 0)
	{
		// �����������ʾ��־��Ϣ
		OutputDebugStringA(info.c_str());
		// ����Ϣ������־������
		WriteLog(info);
	}

	return info;
}


/*----------------------------
* ���� : ����ǰϵͳʱ��ת��Ϊ string �ַ���
*----------------------------
* ���� : CinfoLogger::GetCurTimeStr
* ���� : public
* ���� : std::string	[ret]	ϵͳʱ���ַ���
*
* ���� : TIME_STRING_MODE	mode	[in] ʱ���ַ����������ʽ�������֣��ļ���ģʽ �� �ı�ģʽ
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