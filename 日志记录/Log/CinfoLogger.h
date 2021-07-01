#ifndef _INFO_LOGGER_H_
#define _INFO_LOGGER_H_

/********************************************************************
*	created:	8/7/2012   13:41
*	filename: 	CinfoLogger.h
*	author:		ZouYuhua @ BUAA
*
*	purpose:	����һ�����׵ĳ���������־��¼��
*				��Ҫ������
*				1. ���������ĸ�ʽ���ַ���д����־�����ַ���
*				2. �������ַ������ȴ�����ֵ ���� ��ʱʱ�䵽 ʱ��������д�뵽����log�ļ�
*				3. ���������ĸ�ʽ���ַ���ת��Ϊ std::string �ַ���
*				4. ����ǰϵͳʱ��ת��Ϊָ����ʽ���ַ���
*				5. ��ȡ�߾��ȣ�΢�뼶����ϵͳʱ��
*
*	sample:		ʹ��ʾ��
*
*	 // ����ͷ�ļ������Է��� stdafx.h ��
*
*	 #include "CinfoLogger.h"
*	 #define	WRITE_LOG			CinfoLogger::instance()->LogInfo
*	 #define	STRING_FORMAT		CinfoLogger::instance()->LogInfo
*	 #define	ENTER_FUNC			CinfoLogger::instance()->EnterFunction
*	 #define	LEAVE_FUNC			CinfoLogger::instance()->LeaveFunction
*	 #define	GET_TIME_STRING		CinfoLogger::instance()->GetCurTimeToString
*	 #define	GET_TIME_ELAPSED	CinfoLogger::instance()->GetTimeElapsed
*	 #define	GET_TIME_NOW		CinfoLogger::instance()->GetSystemTimerCount()
*	 #define	LOG_LEVEL_FORMAT	-1
*	 #define	LOG_LEVEL_MAIN		0
*	 #define	LOG_LEVEL_SUB_1		1
*	 #define	LOG_LEVEL_SUB_2		2
*	 #define	LOG_LEVEL_SUB_3		3
*
*	 //������÷�ʽ
*
*	 //1.д����־
*	 WRITE_LOG(LOG_LEVEL_MAIN, "���ڻ�ȡ���ݡ�");
*	 WRITE_LOG(LOG_LEVEL_SUB_1, "The value is %d", value);
*	 cout << WRITE_LOG(LOG_LEVEL_MAIN, "д����־���棬���ڿ���̨������ʾ��Ϣ��") << endl;
*
*	 //2.��ʽ���ַ���ת��
*	 std::string info = "The ";
*	 info += STRING_FORMAT(LOG_LEVEL_FORMAT, "value is %d", value);
*
*	 //3.����ǰʱ��ת��Ϊ�ַ�������Ϊ�ļ�����һ����
*	 std::string imgFileName = "img_" + GET_TIME_STRING() + ".jpg";
*
*	 //4.��������ʱ���ʱ
*	 LONGLONG t_start = GET_TIME_NOW;
*	 // do something ...
*	 double t_elapsed = GET_TIME_ELAPSED( t_start );
*	 WRITE_LOG(LOG_LEVEL_MAIN, "[Main Process] ��ʱ: %.3f ms", t_elapsed);
*
*	 //5.��¼���������¼�
*	 function func
*	 {
*		ENTER_FUNC("func");
*		...
*		LEAVE_FUNC("func");
*	 }
********************************************************************/


#pragma once

//-------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------

/***
* �� stdafx.h ��������������ͷ�ļ���Ҳ�����ڱ������õ�
*
#include "targetver.h"
#include <stdio.h>
#include <tchar.h>
// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include <Windows.h>
#include <iostream>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <conio.h>
*/

#include <fstream>
//-------------------------------------------------------------------------
// ����ʹ�� boost ��� thread ������̹߳���
// ���� VC6 �е�Ӧ�ú�������ο��������ϣ�
//    ��Boost 1_37_0 �İ�װ�Լ���VC6.0�е�ʹ�á�http://www.cnblogs.com/cy163/archive/2010/03/27/1698156.html
// ����һ��C++���̷߳����������� _beginthreadex���ο��������ϣ�
//	  ��C/C++ Runtime ���̺߳�����http://www.cppblog.com/mzty/archive/2007/07/25/28756.html
//-------------------------------------------------------------------------
#include <boost/thread.hpp>

//-------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------
#define		MAX_STR_BUF_LEN		30000	//��־������󳤶ȣ���������־����д�뵽�����ļ�
#define		TIME_TO_WRITE_LOG	5000	//��ʱʱ�䣨���룩����������־����д�뵽�����ļ�

using namespace std;

//-------------------------------------------------------------------------
// Class
//-------------------------------------------------------------------------
class CinfoLogger
{
public:

	/*----------------------------
	* ���� : ��ȡ��ʵ����ָ̬��
	*----------------------------
	* ���� : CinfoLogger::instance
	* ���� : public static
	* ���� : CinfoLogger*
	*/
	static CinfoLogger* instance();

	/*----------------------------
	* ���� : �� ����������־��Ϣ ����� IDE ������ڣ������浽 ��־�ļ����� ��
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
	string LogInfo(int level, char *fmt, ...);

	/***
	*	LogInfo ���أ���־��Ϣ Ϊ �ַ�������
	*
	* ���� : int		level	[in]	��־����
	* ���� : string		info	[in]	�ַ�������
	*/
	string LogInfo(int level, string info);

	/*----------------------------
	* ���� : ��¼����ĳһ���ܺ������¼�
	*----------------------------
	* ���� : CinfoLogger::EnterFunction
	* ���� : public
	* ���� : void
	*
	* ���� : string			funcName	[in]	��������
	* ����
	* ���� : const char*	funcName	[in]	��������
	*/
	void EnterFunction(string funcName)
	{
		LogInfo(1, "Enter function: " + funcName);
	}
	//����1
	void EnterFunction(const char* funcName)
	{
		LogInfo(1, "Enter function: %s", funcName);
	}


	/*----------------------------
	* ���� : ��¼�뿪ĳһ���ܺ������¼�
	*----------------------------
	* ���� : CinfoLogger::LeaveFunction
	* ���� : public
	* ���� : void
	*
	* ���� : string			funcName	[in]	��������
	* ����
	* ���� : const char*	funcName	[in]	��������
	*/
	void LeaveFunction(string funcName)
	{
		LogInfo(1, "Leave function: " + funcName);
	}
	//����1
	void LeaveFunction(const char* funcName)
	{
		LogInfo(1, "Leave function: %s", funcName);
	}

	/*----------------------------
	* ���� : ����ǰϵͳʱ�䣨������ʱ���룩ת��Ϊ string �ַ���
	*----------------------------
	* ���� : CinfoLogger::GetCurTimeToString
	* ���� : public
	* ���� : std::string	[ret]	ϵͳʱ���ַ���
	*
	* ���� : TIME_STRING_MODE	mode	[in] ʱ���ַ����������ʽ�������֣��ļ���ģʽ �� �ı�ģʽ
	*/
	enum TIME_STRING_MODE {
		FOR_FILE_NAME,	// �� �ļ�����ʽ ��ȡʱ���ַ���
		FOR_TEXT		// �� �ı���ʽ ��ȡʱ���ַ���
	};
	string GetCurTimeToString(TIME_STRING_MODE mode = FOR_TEXT);

	/*----------------------------
	* ���� : ��ȡ������ڲ���ʱ���ĵ�ǰֵ
	*----------------------------
	* ���� : CinfoLogger::GetSystemTimerCount
	* ���� : public
	* ���� : LONGLONG	[ret]	Ƶ�ʼ���
	*
	*/
	LONGLONG GetSystemTimerCount()
	{
		LARGE_INTEGER liTmp;
		QueryPerformanceCounter(&liTmp);

		return liTmp.QuadPart;
	}

	/*----------------------------
	* ���� : ��ȡ΢�뼶�ľ�ȷʱ��
	*----------------------------
	* ���� : CinfoLogger::GetTimeElapsed
	* ���� : public
	* ���� : double				[ret]	����λ��ʾ���롢С��λ��ʾ΢���ʱ��ֵ
	*
	* ���� : LONGLONG tStart	[in]	��ʱ������ 2
	*/
	double GetTimeElapsed(LONGLONG tStart)
	{
		double dfTime, dfFreq, dfDiff;
		LARGE_INTEGER liTmp;
		LONGLONG tNow;

		QueryPerformanceCounter(&liTmp);	//��õ�ǰ����ֵ
		tNow = liTmp.QuadPart;

		QueryPerformanceFrequency(&liTmp);	//���ʱ��Ƶ��
		dfFreq = (double)liTmp.QuadPart;

		dfDiff = (double)(tNow - tStart);
		dfTime = dfDiff / dfFreq;	//��ö�Ӧ��ʱ��ֵ���룩
		dfTime *= 1000;				//ת��Ϊ����ֵ

		return dfTime;
	}


	/*----------------------------
	* ���� : ���ڲ����� m_bRun ��Ϊ false�����˳���־ѭ���߳�
	*----------------------------
	* ���� : CinfoLogger::SetExit
	* ���� : public
	* ���� : void
	*/
	void SetExit() { m_bRun = false; }

private:

	//-------------------------------------------------------------------------
	// �ڲ�����
	//-------------------------------------------------------------------------
	static CinfoLogger* _instance;	// ��־��ʵ����ָ̬�룬ֻ����һ�Σ�������ͨ����ָ�����������ں���

	boost::thread m_hThread;		// �߳�ָ��
	boost::mutex m_logMutex;		// �̶߳���
	bool m_bRun;					// �߳�ѭ��ʹ�ܱ�־

	string m_strWriteStrInfo;		// ��־�ļ������ַ���
	string m_strLogFileName;		// ��־�ļ�����ÿ������ʱ����ϵͳʱ���Զ�����

									//-------------------------------------------------------------------------
									// �ڲ�����
									//-------------------------------------------------------------------------

									/*----------------------------
									* ���� : ��ʵ����ʼ��
									*----------------------------
									* ���� : CinfoLogger::CinfoLogger
									* ���� : private
									* ���� :
									*/
	CinfoLogger(void);

	/*----------------------------
	* ���� : ������־����
	*----------------------------
	* ���� : CinfoLogger::WriteLog
	* ���� : private
	* ���� : void
	*
	* ���� : string		strLog		[in]	��־��Ϣ
	*/
	void WriteLog(string strLog);

	/*----------------------------
	* ���� : ��־ѭ���߳�
	* ÿ�����붨ʱ������ ��־�����ַ������ȴ�����ֵʱ
	* ���������־��Ϣǿ��д�뵽�����ļ�
	*----------------------------
	* ���� : CinfoLogger::LogProcStart
	* ���� : private
	* ���� : void
	*/
	void LogProcStart();

	/*----------------------------
	* ���� : ���������־��Ϣд�뵽������־�ļ�
	*----------------------------
	* ���� : CinfoLogger::WriteLogToFile
	* ���� : private
	* ���� : void
	*/
	void WriteLogToFile();

};

#endif