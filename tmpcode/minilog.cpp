/*
 * File       : minilog.h
 * Description: 
 * Version    : 2011-11-12 Created
 * Author     : buf1024@gmail.com
 */
#include "minilog.h"
#include <time.h>
#include <assert.h>
#include <string.h>
#ifdef _MSC_VER
#include <io.h>
#include <direct.h>
#else
#include <unistd.h>
#include <sys/time.h>
#endif

#define WRITE_BUF        2048
#define DEFAULT_BUF_SIZE 4096

//Minilog* Minilog::sm_pInst = 0;

Minilog::Minilog()
: m_eFileLvl(ALL)
, m_eConsoleLvl(ALL)
, m_pFile(NULL)
, m_pMsgBuf(NULL)
{
    memset(m_szPath, 0, sizeof(m_szPath));
    memset(m_szFile, 0, sizeof(m_szFile));


    SetLogBufSize(DEFAULT_BUF_SIZE);

#ifdef _MSC_VER
	m_hStdOut = ::GetStdHandle(STD_OUTPUT_HANDLE);
#endif

}

Minilog::~Minilog()
{
	if (m_pFile != 0)
	{
		FlushLog();
		fclose(m_pFile);
		m_pFile = 0;
	}
	if(m_pMsgBuf)
	{
	    delete[] m_pMsgBuf;
	    m_pMsgBuf = 0;
	}
#ifdef _MSC_VER
	CloseHandle(m_hStdOut);
#endif
}
void Minilog::Info(const char* szFormat, ...)
{
    va_list va;
    va_start(va, szFormat);
    LogMessageV(INFO, szFormat, va);
    va_end(va);
}
void Minilog::Debug(const char* szFormat, ...)
{
    va_list va;
    va_start(va, szFormat);
    LogMessageV(DEBUG, szFormat, va);
    va_end(va);
}
void Minilog::Warn(const char* szFormat, ...)
{
    va_list va;
    va_start(va, szFormat);
    LogMessageV(WARN, szFormat, va);
    va_end(va);
}
void Minilog::Error(const char* szFormat, ...)
{
    va_list va;
    va_start(va, szFormat);
    LogMessageV(ERR, szFormat, va);
    va_end(va);
}
void Minilog::Fatal(const char* szFormat, ...)
{
    va_list va;
    va_start(va, szFormat);
    LogMessageV(FATAL, szFormat, va);
    va_end(va);
}

void Minilog::SetFileLogLevel(LogLevel eLvl)
{
    m_eFileLvl = GetMapLevel(eLvl);
}
LogLevel Minilog::GetFileLogLevel() const
{
    return m_eFileLvl;
}
void Minilog::SetConsoleLogLevel(LogLevel eLvl)
{
    m_eConsoleLvl = GetMapLevel(eLvl);
}
LogLevel Minilog::GetConsoleLogLevel() const
{
    return m_eConsoleLvl;
}

void Minilog::SetLogPath(const char* szPath)
{
#ifdef _MSC_VER
    _getcwd(m_szPath, Minilog::LOG_MAX_PATH);
#else
    getcwd(m_szPath, Minilog::LOG_MAX_PATH);
#endif

	if (szPath != 0)
	{
	    int nRet = -1;
#ifdef _MSC_VER
		nRet = _access(szPath, 04); //Read
#else
		nRet = access(szPath, 04); //Read
#endif
		if (nRet != -1)
		{
			strcpy(m_szPath, szPath);
		}
	}
	int nLen = strlen(m_szPath);
	if (m_szPath[nLen - 1] == '/' || m_szPath[nLen - 1] == '\\')
	{
	    m_szPath[nLen - 1] = '\0';
	}
}
const char* Minilog::GetLogPath() const
{
    return m_szPath;
}

void Minilog::SetLogFile(const char* szFile, bool bSuffix)
{
	if (szFile != 0 && szFile[0] != '\0')
	{
		strcpy(m_szFile, szFile);
	}
	
	time_t t;
	time(&t);
	struct tm * pTm = localtime(&t);
	char szBuf[BUFSIZ] = "";
#ifdef _MSC_VER
	_snprintf(szBuf, BUFSIZ, "_%04d%02d%02d%02d%02d%02d_%ld.log",
	        pTm->tm_year + 1900, pTm->tm_mon + 1, pTm->tm_mday,
	        pTm->tm_hour, pTm->tm_min, pTm->tm_sec,
	        (long)GetCurrentProcessId());
#else
	snprintf(szBuf, BUFSIZ, "_%04d%02d%02d%02d%02d%02d_%ld.log",
	        pTm->tm_year + 1900, pTm->tm_mon + 1, pTm->tm_mday,
	        pTm->tm_hour, pTm->tm_min, pTm->tm_sec,
	        (long)getpid());
#endif
	if (strlen(m_szFile) == 0)
	{
	    strcpy(m_szFile, "Minilog");
		strcat(m_szFile, szBuf);
	}
	else
	{
		if (bSuffix)
		{
		    strcat(m_szFile, szBuf);
		}		
	}
	if (m_pFile != 0)
	{
		fflush(m_pFile);
		fclose(m_pFile);
	}
	strcpy(szBuf, m_szPath);
#ifdef _MSC_VER
	strcat(szBuf, "\\");
#else
	strcat(szBuf, "/");
#endif

	strcat(szBuf, m_szFile);

	m_pFile = fopen(szBuf, "w");
	assert(m_pFile != 0);
}
const char* Minilog::GetLogFile() const
{
	return m_szFile;
}

int Minilog::SetLogBufSize(unsigned int nSize)
{
    if(m_pMsgBuf != 0)
    {
        FlushLog();
        delete m_pMsgBuf;
        m_pMsgBuf = 0;
    }
    m_nBufSize = nSize;
    m_pMsgBuf = new char[nSize];
    m_nIdx = 0;

    return m_pMsgBuf != 0;
}
unsigned int Minilog::GetBufSize() const
{
    return m_nBufSize;
}

void Minilog::FlushLog()
{
	if(m_pFile)
	{
	    FlushMsg(m_pMsgBuf, m_nIdx);
	    m_nIdx = 0;
	}
}

void Minilog::FlushMsg(const char* szBuf, int nLen)
{
    int nTotal = nLen;
    int nCount = nLen;
    size_t uIndex = 0;
    while(nTotal > 0){
        if (nTotal > WRITE_BUF)
        {
            nCount = WRITE_BUF;
        }
        fwrite(szBuf + uIndex, sizeof(char), nCount, m_pFile);
        uIndex += nCount;
        nTotal -= nCount;
        nCount = nTotal;
    }
    fflush(m_pFile);
}

void Minilog::LogConsole(LogLevel eLvl, const char* szMsg, int nLen)
{
	if (eLvl >= m_eConsoleLvl)
	{
		if (szMsg != 0 && szMsg[0] != '\0')
		{
		    const char* szHeader = GetLogHeader(eLvl);
			if(nLen < 0) nLen = strlen(szMsg);
#ifdef _MSC_VER
			CONSOLE_SCREEN_BUFFER_INFO csbi = {sizeof(csbi)};
			GetConsoleScreenBufferInfo(
				m_hStdOut, &csbi);
			WORD wAtt = csbi.wAttributes;
			SetConsoleColor(eLvl);        
			DWORD dwReal;
			WriteConsoleA(m_hStdOut, szHeader, strlen(szHeader), &dwReal, NULL);
			WriteConsoleA(m_hStdOut, szMsg, nLen, &dwReal, NULL);
			SetConsoleTextAttribute(m_hStdOut, wAtt);
#else
			SetConsoleColor(eLvl);
			printf("%s", szHeader);
			printf("%s", szMsg);
			printf("\033[0m");
#endif
		}
	}
}
void Minilog::LogFile(LogLevel eLvl, const char* szMsg, int nLen)
{
	if (eLvl >= m_eFileLvl)
	{
		if (m_pFile != 0 && szMsg != 0)
		{
		    const char* szHeader = GetLogHeader(eLvl);
		    unsigned int nHLen = strlen(szHeader);
		    if(nLen == -1) nLen = strlen(szMsg);
			if(m_nBufSize - m_nIdx >= nHLen + nLen)
			{
			    memcpy(m_pMsgBuf + m_nIdx, szHeader, nHLen);
			    m_nIdx += nHLen;
			    memcpy(m_pMsgBuf + m_nIdx, szMsg, nLen);
			    m_nIdx += nLen;
			}
			else
			{
			    FlushLog();
			    if(m_nBufSize - m_nIdx >= nHLen + nLen)
			    {
			        memcpy(m_pMsgBuf + m_nIdx, szHeader, nHLen);
			        m_nIdx += nHLen;
			        memcpy(m_pMsgBuf + m_nIdx, szMsg, nLen);
			        m_nIdx += nLen;
			    }
			    else
			    {
			        FlushMsg(szHeader, nHLen);
			        FlushMsg(szMsg, nLen);
			    }
			}
		}
		else
		{
			assert(false);
		}
	}
}
void Minilog::LogMessageV(LogLevel eLvl, const char* szFormat, va_list va)
{
    static char szMsgBuf[WRITE_BUF] = "";
	if (eLvl >= m_eConsoleLvl || eLvl >= m_eFileLvl)
	{
		if (vsnprintf(szMsgBuf, WRITE_BUF - 1, szFormat, va) < 0)
		{
			Fatal("Message too long\n");
			return;
		}
	}
	if (eLvl >= m_eConsoleLvl)
	{
		LogConsole(eLvl, szMsgBuf);
	}
	if (eLvl >= m_eFileLvl)
	{
		LogFile(eLvl, szMsgBuf);
	}
}
const char* Minilog::GetLogHeader(LogLevel eLvl)
{
    const int SIZE = 64;
    static char szHeader[SIZE - 1] = "";
    switch (eLvl) {
    case DEBUG:
        strcpy(szHeader, "[D]");
        break;
    case INFO:
        strcpy(szHeader, "[I]");
        break;
    case WARN:
        strcpy(szHeader, "[W]");
        break;
    case ERR:
        strcpy(szHeader, "[E]");
        break;
    case FATAL:
        strcpy(szHeader, "[F]");
        break;
    default:
        assert(false);
        strcpy(szHeader, "[X]");
        break;
    }

#ifdef _MSC_VER
    SYSTEMTIME st = {0};
    GetLocalTime(&st);
    _snprintf(szHeader + 3, SIZE - 4, " [%04d%02d%02d%02d%02d%02d.%ld] ",
            st.wYear, st.wMonth, st.wDay,
            st.wHour, st.wMinute, st.wSecond,
            st.wMilliseconds);
#else
    time_t t;
    time(&t);
    struct tm * pTm = localtime(&t);
    struct timeval usec;
    gettimeofday(&usec, 0);
    snprintf(szHeader + 3, SIZE - 4, " [%04d%02d%02d%02d%02d%02d.%ld] ",
            pTm->tm_year + 1900, pTm->tm_mon + 1, pTm->tm_mday,
            pTm->tm_hour, pTm->tm_min, pTm->tm_sec,
            usec.tv_usec);
#endif

    return szHeader;
}


void Minilog::SetConsoleColor(LogLevel eLogLevel)
{
#ifdef _MSC_VER
	WORD dwTextAttr = FOREGROUND_BLUE | FOREGROUND_GREEN |
		FOREGROUND_RED | FOREGROUND_INTENSITY;
	switch(eLogLevel)
	{
	case FATAL:
		dwTextAttr = FOREGROUND_RED | FOREGROUND_INTENSITY;
		break;
	case ERR:
		dwTextAttr = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		break;
	case WARN:
		dwTextAttr = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		break;
	case INFO:
		dwTextAttr = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
		break;
	case DEBUG:
		break;
	default:
		break;
	}
	SetConsoleTextAttribute(m_hStdOut, dwTextAttr);
#else
	char szBuf[32] = "\033[0m";
	switch(eLogLevel)
	{
	case FATAL:
		strcpy(szBuf, "\033[1;31m");
		break;
	case ERR:
		strcpy(szBuf, "\033[1;35m");
		break;
	case WARN:
		strcpy(szBuf, "\033[1;34m");
		break;
	case INFO:
		strcpy(szBuf, "\033[1;33m");
		break;
	case DEBUG:
		break;
	default:
		break;
	}
	printf("%s", szBuf);
#endif

}


LogLevel Minilog::GetMapLevel(int nLvl)
{
	if (nLvl <100)
	{
		return ALL;
	}
	if (nLvl >= 100 && nLvl < 200)
	{
		return DEBUG;
	}
	if (nLvl >= 200 && nLvl < 300)
	{
		return INFO;
	}
	if (nLvl >= 300 && nLvl < 400)
	{
		return WARN;
	}
	if (nLvl >= 400 && nLvl < 500)
	{
		return ERR;
	}
	if (nLvl >= 500 && nLvl < 600)
	{
		return FATAL;
	}
	return OFF;
}
LogLevel Minilog::GetMapLevel(const char* szLvl)
{
	struct _StrLvl
	{
		const char* szStr;
		LogLevel eLvl;
	};

	static _StrLvl strLvlMap [] = {
		{"ALL", ALL},
		{"DEBUG", DEBUG},
		{"INFO", INFO},
		{"WARN", WARN},
		{"ERROR", ERR},
		{"FATAL", FATAL},
		{"OFF", OFF}
	};

	if (szLvl == 0)
	{
		return ALL;
	}

	for (unsigned i=0; i<sizeof(strLvlMap); i++)
	{
#ifdef _MSC_VER
		if (stricmp(szLvl, strLvlMap[i].szStr) == 0)
#else
		if (strcasecmp(szLvl, strLvlMap[i].szStr) == 0)
#endif
		{
			return strLvlMap[i].eLvl;
		}
	}
	assert(false);
	return ALL;
}

Minilog& Minilog::GetInst()
{
/*    if (sm_pInst == 0)
    {
        sm_pInst = new Minilog;
    }
    return *sm_pInst;*/
    static Minilog theLog;
    return theLog;
}
void Minilog::ReleaseRC()
{
/*    if (sm_pInst != 0)
    {
        delete sm_pInst;
        sm_pInst = 0;
    }*/
}
