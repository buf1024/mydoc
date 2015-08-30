/*
 * File       : minilog.h
 * Description: 
 * Version    : 2011-11-12 Created
 * Author     : buf1024@gmail.com
 */
 
#ifndef XBASIC_MINILOG_H_
#define XBASIC_MINILOG_H_

#include <stdarg.h>
#include <stdio.h>
#ifdef _MSC_VER
#include <windows.h>
#endif

#define LOG_DEBUG(...)                                               \
do{                                                                  \
	Minilog& sLog = Minilog::GetInst();                              \
	sLog.Debug(__VA_ARGS__);                                         \
}while(0)                                                            \

#define LOG_INFO(...)                                                \
do{                                                                  \
	Minilog& sLog = Minilog::GetInst();                              \
	sLog.Info(__VA_ARGS__);                                          \
}while(0)                                                            \

#define LOG_WARN(...)                                                \
do{                                                                  \
	Minilog& sLog = Minilog::GetInst();                              \
	sLog.Warn(__VA_ARGS__);                                          \
}while(0)                                                            \

#define LOG_ERROR(...)                                               \
do{                                                                  \
	Minilog& sLog = Minilog::GetInst();                              \
	sLog.Error(__VA_ARGS__);                                         \
}while(0)                                                            \

#define LOG_FATAL(...)                                               \
do{                                                                  \
	Minilog& sLog = Minilog::GetInst();                              \
	sLog.Fatal(__VA_ARGS__);                                         \
}while(0)                                                            \

#define SET_LOG_FILE_LEVEL(lvl)                                      \
do{                                                                  \
	Minilog& sLog = Minilog::GetInst();                              \
    sLog.SetFileLogLevel(Minilog::GetMapLevel(lvl));                 \
}while(0)                                                            \


#define SET_LOG_CONSOLE_LEVEL(lvl)                                   \
do{                                                                  \
	Minilog& sLog = Minilog::GetInst();                              \
	sLog.SetConsoleLogLevel(Minilog::GetMapLevel(lvl));              \
}while(0)                                                            \

#define SET_LOG_LEVEL(lvl)                                           \
do{                                                                  \
	Minilog& sLog = Minilog::GetInst();                              \
    sLog.SetConsoleLogLevel(Minilog::GetMapLevel(lvl));              \
	sLog.SetFileLogLevel(Minilog::GetMapLevel(lvl));                 \
}while(0)                                                            \


#define SET_LOG_PATH(szPath)                                         \
do{                                                                  \
	Minilog& sLog = Minilog::GetInst();                              \
	sLog.SetLogPath(szPath);                                         \
}while(0)                                                            \

#define SET_LOG_FILE_FIXED(szFile)                                   \
do{                                                                  \
	Minilog& sLog = Minilog::GetInst();                              \
	sLog.SetLogFile(szFile, false);                                  \
}while(0)                                                            \

#define SET_LOG_FILE(szFile)                                         \
do{                                                                  \
	Minilog& sLog = Minilog::GetInst();                              \
	sLog.SetLogFile(szFile);                                         \
}while(0)                                                            \

#define SET_LOG_BUF_SIZE(size)                                       \
do{                                                                  \
    Minilog& sLog = Minilog::GetInst();                              \
    sLog.SetLogBufSize(size);                                        \
}while(0)                                                            \

#define LOG_FLUSH()                                                  \
do{                                                                  \
	Minilog& sLog = Minilog::GetInst();                              \
	sLog.FlushLog();                                                 \
}while(0)                                                            \

#define LOG_RELEASE()                                                \
do{                                                                  \
	Minilog::ReleaseRC();                                            \
}while(0)                                                            \

#define LOG_IS_READY() Minilog::GetInst().IsLogReady()               \

enum LogLevel
{
    ALL     = 0,
    DEBUG   = 100,
    INFO    = 200,
    WARN    = 300,
    ERR     = 400,
    FATAL   = 500,
    OFF     = 600
};

class Minilog
{
private:
    Minilog();
public:
    ~Minilog();
public:
    void Info(const char* szFormat, ...);
    void Debug(const char* szFormat, ...);
    void Warn(const char* szFormat, ...);
    void Error(const char* szFormat, ...);
    void Fatal(const char* szFormat, ...);
    
    void SetFileLogLevel(LogLevel eLvl);
    LogLevel GetFileLogLevel() const;
    void SetConsoleLogLevel(LogLevel eLvl);
    LogLevel GetConsoleLogLevel() const;

    void SetLogPath(const char* szPath);
    const char* GetLogPath() const;

	void SetLogFile(const char* szFile, bool bSuffix = true);
	const char* GetLogFile() const;

	int SetLogBufSize(unsigned int nSize);
	unsigned int GetBufSize() const;

    void FlushLog();

	void LogConsole(LogLevel eLvl, const char* szMsg, int nLen = -1);
	void LogFile(LogLevel eLvl, const char* szMsg, int nLen = -1);

	int IsLogReady() const { return m_pFile != 0; }

	static LogLevel GetMapLevel(int nLvl);
	static LogLevel GetMapLevel(const char* szLvl);

    static Minilog& GetInst();
    static void ReleaseRC();

protected:	
    void LogMessageV(LogLevel eLvl, const char* szFormat, va_list va); 
    const char* GetLogHeader(LogLevel eLvl);

	void SetConsoleColor(LogLevel eLogLevel);

	void FlushMsg(const char* szBuf, int nLen);

private:
    LogLevel m_eFileLvl;
    LogLevel m_eConsoleLvl;
    
    FILE* m_pFile;

    static const int LOG_MAX_PATH = 255;
    char m_szPath[LOG_MAX_PATH + 1];
	char m_szFile[LOG_MAX_PATH + 1];

    unsigned int m_nBufSize;
	char* m_pMsgBuf;
	unsigned int m_nIdx;

	//static Minilog* sm_pInst;

#ifdef _MSC_VER
	HANDLE m_hStdOut;
#endif
};

 
#endif /* XBASIC_MINILOG_H_ */


