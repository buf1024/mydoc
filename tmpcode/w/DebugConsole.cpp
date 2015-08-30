////////////////////////////////////////////////////////////////////////////////////////
// 
// LGCUI -- Personal Windowless UI Library Project
// 
// FileName    : DebugConsole.cpp
// Purpose     : 
// Version     : 2011-05-08 (14:08) 1.0 Created
// Author      : heidong
// Contact     : buf1024@gmail.com
// Copyright(c): HEIDONG
////////////////////////////////////////////////////////////////////////////////////////
#include "DebugConsole.h"
#include "StdString.h"

DebugConsole::DebugConsole(LPCTSTR lpTitle, SHORT sHeight, SHORT sWidth, BOOL bClosable)
{
    AllocateConsole(lpTitle, sHeight, sWidth, bClosable);
}

DebugConsole::~DebugConsole(void)
{
    if (sm_bExist)
    {
        ::FreeConsole();
        sm_bExist = FALSE;
    }
}

void DebugConsole::SetAttribute(BOOL bRed, BOOL bGreen, BOOL bBlue, BOOL bIntensity)
{
    ::SetConsoleTextAttribute(
        ::GetStdHandle(STD_OUTPUT_HANDLE), 
        (bRed       ? FOREGROUND_RED        : 0) | 
        (bGreen     ? FOREGROUND_GREEN      : 0) | 
        (bBlue      ? FOREGROUND_BLUE       : 0) | 
        (bIntensity ? FOREGROUND_INTENSITY  : 0)
        );

}
void __cdecl DebugConsole::Printf(const TCHAR* szFormat, ...)
{
    const size_t SIZE = 1024;
    TCHAR szBuf[SIZE] = {0};
    va_list args;
    va_start(args, szFormat);
    _vsntprintf_s(szBuf, SIZE, SIZE, szFormat, args);
    va_end(args);

    DWORD dwWritten = 0L;
   
    if (sm_bExist)
    {
        
        ::WriteConsole(
            ::GetStdHandle(STD_OUTPUT_HANDLE),
            szBuf, lstrlen(szBuf), &dwWritten, NULL);
    }
    else
    {
        ::OutputDebugString(szBuf);
    }

}
void DebugConsole::EnableCloseButton(BOOL bClosable)
{
    if (FALSE == bClosable)
    {
        HMENU hMenu = ::GetSystemMenu(::GetConsoleWindow(), FALSE);
        ::EnableMenuItem(hMenu, SC_CLOSE, MF_BYCOMMAND | MF_GRAYED);
        ::DestroyMenu(hMenu);
    }
}

void DebugConsole::AllocateConsole(LPCTSTR lpTitle, SHORT sHeight, SHORT sWidth, BOOL bClosable)
{
    if (FALSE == sm_bExist)
    {
        ::AllocConsole();

        if (NULL != lpTitle)
            ::SetConsoleTitle(lpTitle);

        COORD size;
        size.X = sWidth;
        size.Y = sHeight;
        ::SetConsoleScreenBufferSize(
            ::GetStdHandle(STD_OUTPUT_HANDLE), size);

        EnableCloseButton(bClosable);

        sm_bExist = TRUE;
    }
}

BOOL DebugConsole::sm_bExist = FALSE;

#ifdef _DEBUG
void __DEBUG_MSG__(const WCHAR* szFormat, va_list args)
{
    const size_t SIZE = 4096;   
    WCHAR szBuf[SIZE] = {0}; 
    _vsnwprintf_s(szBuf, SIZE, SIZE, szFormat, args);
    lstrcatW(szBuf, L"\n");
    DebugConsole::Printf(GetStdString(szBuf).c_str());
}
void __DEBUG_MSG__(const CHAR* szFormat, va_list args)
{
    const size_t SIZE = 4096;   
    CHAR szBuf[SIZE] = {0}; 
    _vsnprintf_s(szBuf, SIZE, SIZE, szFormat, args);
    strcat_s(szBuf, SIZE, "\n");
    DebugConsole::Printf(GetStdString(szBuf).c_str());
}
void __cdecl DEBUG_INFO(const WCHAR* szFormat, ...)
{
    DebugConsole::SetAttribute(FALSE, FALSE, FALSE, TRUE);
    va_list args;
    va_start(args, szFormat); 
    __DEBUG_MSG__(szFormat, args);
    va_end(args);
}


void __cdecl DEBUG_WARNING(const WCHAR* szFormat, ...)
{
    DebugConsole::SetAttribute(FALSE, FALSE, TRUE, TRUE);
    va_list args;
    va_start(args, szFormat); 
    __DEBUG_MSG__(szFormat, args);
    va_end(args);
}  

void __cdecl DEBUG_ERROR(const WCHAR* szFormat, ...)
{ 
    DebugConsole::SetAttribute(TRUE, FALSE, FALSE, TRUE);
    va_list args;
    va_start(args, szFormat); 
    __DEBUG_MSG__(szFormat, args);
    va_end(args); 
} 

void __cdecl DEBUG_INFO(const CHAR* szFormat, ...)
{
    DebugConsole::SetAttribute(FALSE, FALSE, FALSE, TRUE);
    va_list args;
    va_start(args, szFormat); 
    __DEBUG_MSG__(szFormat, args);
    va_end(args);
}
void __cdecl DEBUG_WARNING(const CHAR* szFormat, ...)
{
    DebugConsole::SetAttribute(FALSE, FALSE, TRUE, TRUE);
    va_list args;
    va_start(args, szFormat); 
    __DEBUG_MSG__(szFormat, args);
    va_end(args);
}
void __cdecl DEBUG_ERROR(const CHAR* szFormat, ...)
{
    DebugConsole::SetAttribute(TRUE, FALSE, FALSE, TRUE);
    va_list args;
    va_start(args, szFormat); 
    __DEBUG_MSG__(szFormat, args);
    va_end(args); 
}

#endif
