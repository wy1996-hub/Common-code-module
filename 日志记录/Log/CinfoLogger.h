#ifndef _INFO_LOGGER_H_
#define _INFO_LOGGER_H_

/********************************************************************
*	created:	8/7/2012   13:41
*	filename: 	CinfoLogger.h
*	author:		ZouYuhua @ BUAA
*
*	purpose:	这是一个简易的程序运行日志记录类
*				主要功能是
*				1. 将带参数的格式化字符串写入日志缓存字符串
*				2. 当缓存字符串长度大于阈值 或者 定时时间到 时，将缓存写入到本地log文件
*				3. 将带参数的格式化字符串转换为 std::string 字符串
*				4. 将当前系统时间转换为指定格式的字符串
*				5. 获取高精度（微秒级）的系统时间
*
*	sample:		使用示例
*
*	 // 包含头文件，可以放在 stdafx.h 中
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
*	 //具体调用方式
*
*	 //1.写入日志
*	 WRITE_LOG(LOG_LEVEL_MAIN, "正在获取数据。");
*	 WRITE_LOG(LOG_LEVEL_SUB_1, "The value is %d", value);
*	 cout << WRITE_LOG(LOG_LEVEL_MAIN, "写入日志缓存，并在控制台窗口显示信息。") << endl;
*
*	 //2.格式化字符串转换
*	 std::string info = "The ";
*	 info += STRING_FORMAT(LOG_LEVEL_FORMAT, "value is %d", value);
*
*	 //3.将当前时间转换为字符串，作为文件名的一部分
*	 std::string imgFileName = "img_" + GET_TIME_STRING() + ".jpg";
*
*	 //4.程序运行时间计时
*	 LONGLONG t_start = GET_TIME_NOW;
*	 // do something ...
*	 double t_elapsed = GET_TIME_ELAPSED( t_start );
*	 WRITE_LOG(LOG_LEVEL_MAIN, "[Main Process] 用时: %.3f ms", t_elapsed);
*
*	 //5.记录函数进出事件
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
* 在 stdafx.h 还包含了其它的头文件，也可能在本类中用到
*
#include "targetver.h"
#include <stdio.h>
#include <tchar.h>
// TODO: 在此处引用程序需要的其他头文件
#include <Windows.h>
#include <iostream>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <conio.h>
*/

#include <fstream>
//-------------------------------------------------------------------------
// 本类使用 boost 库的 thread 类进行线程管理
// 其在 VC6 中的应用和设置请参考以下资料：
//    《Boost 1_37_0 的安装以及在VC6.0中的使用》http://www.cnblogs.com/cy163/archive/2010/03/27/1698156.html
// 另外一种C++多线程方法，是利用 _beginthreadex，参考以下资料：
//	  《C/C++ Runtime 多线程函数》http://www.cppblog.com/mzty/archive/2007/07/25/28756.html
//-------------------------------------------------------------------------
#include <boost/thread.hpp>

//-------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------
#define		MAX_STR_BUF_LEN		30000	//日志缓存最大长度，超过则将日志缓存写入到本地文件
#define		TIME_TO_WRITE_LOG	5000	//定时时间（毫秒），超过则将日志缓存写入到本地文件

using namespace std;

//-------------------------------------------------------------------------
// Class
//-------------------------------------------------------------------------
class CinfoLogger
{
public:

	/*----------------------------
	* 功能 : 获取类实例静态指针
	*----------------------------
	* 函数 : CinfoLogger::instance
	* 访问 : public static
	* 返回 : CinfoLogger*
	*/
	static CinfoLogger* instance();

	/*----------------------------
	* 功能 : 将 带参数的日志信息 输出到 IDE 输出窗口，并保存到 日志文件缓存 中
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
	string LogInfo(int level, char *fmt, ...);

	/***
	*	LogInfo 重载，日志信息 为 字符串常量
	*
	* 参数 : int		level	[in]	日志级别
	* 参数 : string		info	[in]	字符串常量
	*/
	string LogInfo(int level, string info);

	/*----------------------------
	* 功能 : 记录进入某一功能函数的事件
	*----------------------------
	* 函数 : CinfoLogger::EnterFunction
	* 访问 : public
	* 返回 : void
	*
	* 参数 : string			funcName	[in]	函数名称
	* 或者
	* 参数 : const char*	funcName	[in]	函数名称
	*/
	void EnterFunction(string funcName)
	{
		LogInfo(1, "Enter function: " + funcName);
	}
	//重载1
	void EnterFunction(const char* funcName)
	{
		LogInfo(1, "Enter function: %s", funcName);
	}


	/*----------------------------
	* 功能 : 记录离开某一功能函数的事件
	*----------------------------
	* 函数 : CinfoLogger::LeaveFunction
	* 访问 : public
	* 返回 : void
	*
	* 参数 : string			funcName	[in]	函数名称
	* 或者
	* 参数 : const char*	funcName	[in]	函数名称
	*/
	void LeaveFunction(string funcName)
	{
		LogInfo(1, "Leave function: " + funcName);
	}
	//重载1
	void LeaveFunction(const char* funcName)
	{
		LogInfo(1, "Leave function: %s", funcName);
	}

	/*----------------------------
	* 功能 : 将当前系统时间（年月日时分秒）转换为 string 字符串
	*----------------------------
	* 函数 : CinfoLogger::GetCurTimeToString
	* 访问 : public
	* 返回 : std::string	[ret]	系统时间字符串
	*
	* 参数 : TIME_STRING_MODE	mode	[in] 时间字符串的输出格式，分两种：文件名模式 和 文本模式
	*/
	enum TIME_STRING_MODE {
		FOR_FILE_NAME,	// 按 文件名格式 获取时间字符串
		FOR_TEXT		// 按 文本格式 获取时间字符串
	};
	string GetCurTimeToString(TIME_STRING_MODE mode = FOR_TEXT);

	/*----------------------------
	* 功能 : 获取计算机内部定时器的当前值
	*----------------------------
	* 函数 : CinfoLogger::GetSystemTimerCount
	* 访问 : public
	* 返回 : LONGLONG	[ret]	频率计数
	*
	*/
	LONGLONG GetSystemTimerCount()
	{
		LARGE_INTEGER liTmp;
		QueryPerformanceCounter(&liTmp);

		return liTmp.QuadPart;
	}

	/*----------------------------
	* 功能 : 获取微秒级的精确时间
	*----------------------------
	* 函数 : CinfoLogger::GetTimeElapsed
	* 访问 : public
	* 返回 : double				[ret]	整数位表示毫秒、小数位表示微秒的时间值
	*
	* 参数 : LONGLONG tStart	[in]	定时器计数 2
	*/
	double GetTimeElapsed(LONGLONG tStart)
	{
		double dfTime, dfFreq, dfDiff;
		LARGE_INTEGER liTmp;
		LONGLONG tNow;

		QueryPerformanceCounter(&liTmp);	//获得当前计数值
		tNow = liTmp.QuadPart;

		QueryPerformanceFrequency(&liTmp);	//获得时钟频率
		dfFreq = (double)liTmp.QuadPart;

		dfDiff = (double)(tNow - tStart);
		dfTime = dfDiff / dfFreq;	//获得对应的时间值（秒）
		dfTime *= 1000;				//转换为毫秒值

		return dfTime;
	}


	/*----------------------------
	* 功能 : 将内部变量 m_bRun 置为 false，以退出日志循环线程
	*----------------------------
	* 函数 : CinfoLogger::SetExit
	* 访问 : public
	* 返回 : void
	*/
	void SetExit() { m_bRun = false; }

private:

	//-------------------------------------------------------------------------
	// 内部变量
	//-------------------------------------------------------------------------
	static CinfoLogger* _instance;	// 日志类实例静态指针，只创建一次，其它类通过此指针来访问类内函数

	boost::thread m_hThread;		// 线程指针
	boost::mutex m_logMutex;		// 线程对象
	bool m_bRun;					// 线程循环使能标志

	string m_strWriteStrInfo;		// 日志文件缓存字符串
	string m_strLogFileName;		// 日志文件名，每次启动时按照系统时间自动生成

									//-------------------------------------------------------------------------
									// 内部函数
									//-------------------------------------------------------------------------

									/*----------------------------
									* 功能 : 类实例初始化
									*----------------------------
									* 函数 : CinfoLogger::CinfoLogger
									* 访问 : private
									* 返回 :
									*/
	CinfoLogger(void);

	/*----------------------------
	* 功能 : 更新日志缓存
	*----------------------------
	* 函数 : CinfoLogger::WriteLog
	* 访问 : private
	* 返回 : void
	*
	* 参数 : string		strLog		[in]	日志信息
	*/
	void WriteLog(string strLog);

	/*----------------------------
	* 功能 : 日志循环线程
	* 每隔五秒定时、或者 日志缓存字符串长度大于阈值时
	* 将缓存的日志信息强制写入到本地文件
	*----------------------------
	* 函数 : CinfoLogger::LogProcStart
	* 访问 : private
	* 返回 : void
	*/
	void LogProcStart();

	/*----------------------------
	* 功能 : 将缓存的日志信息写入到本地日志文件
	*----------------------------
	* 函数 : CinfoLogger::WriteLogToFile
	* 访问 : private
	* 返回 : void
	*/
	void WriteLogToFile();

};

#endif