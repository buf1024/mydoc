////////////////////////////////////////////////////////////////////////////////////////
// 
// LGCUI -- Personal Windowless UI Library Project
// 
// FileName    : DebugConsole.h
// Purpose     : Pop up a debug window for better debugging
// Version     : 2011-05-08 (14:08) 1.0 Created
// Author      : heidong
// Contact     : buf1024@gmail.com
// Copyright(c): HEIDONG
////////////////////////////////////////////////////////////////////////////////////////
/** @file  DebugConsole.h */
#pragma once

#include "../CmmHdr.h"

/*! @brief A Debug Helper
 *
 *  Pop up a debug console for better debugging
 */
class DebugConsole
{
public:
    /** Allocation a new console, if the console already exist, doing nothing */
    DebugConsole(LPCTSTR lpTitle = NULL, SHORT sHeight = 400, SHORT sWidth = 80, BOOL bClosable = FALSE);
    ~DebugConsole(void);

public:
    /** Set console attribute */
    static void SetAttribute(BOOL bRed, BOOL bGreen, BOOL bBlue, BOOL bIntensity);
    /** Out put msg on the console */
    static void __cdecl Printf(const TCHAR* szFormat, ...);
    /** Enable/Disable close button */
    static void EnableCloseButton(BOOL bClosable);
    /** Allocation a new console, if the console already exist, doing nothing */
    static void AllocateConsole(LPCTSTR lpTitle = NULL, SHORT sHeight = 400, SHORT sWidth = 80, BOOL bClosable = FALSE);
private:
    static BOOL sm_bExist;

};

//The following function is use for output debug message
//By default info message is gray, waring message is blue, error message is red.
#ifdef _DEBUG
void __DEBUG_MSG__(const WCHAR* szFormat, va_list args);
void __DEBUG_MSG__(const CHAR* szFormat, va_list args);

/** Debug Info message color gray */
void __cdecl DEBUG_INFO(const WCHAR* szFormat, ...);
/** Debug Info message color blue */
void __cdecl DEBUG_WARNING(const WCHAR* szFormat, ...);
/** Debug Info message color red */
void __cdecl DEBUG_ERROR(const WCHAR* szFormat, ...);

void __cdecl DEBUG_INFO(const CHAR* szFormat, ...);
void __cdecl DEBUG_WARNING(const CHAR* szFormat, ...);
void __cdecl DEBUG_ERROR(const CHAR* szFormat, ...);
#else
#define DEBUG_INFO      (void)
#define DEBUG_WARNING   (void)
#define DEBUG_ERROR     (void)
#endif
