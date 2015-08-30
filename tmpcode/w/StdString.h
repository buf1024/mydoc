////////////////////////////////////////////////////////////////////////////////////////
// 
// LGCUI -- Personal Windowless UI Library Project
// 
// FileName    : StdString.h
// Purpose     : Extend string functionality
//               The reason why some paramters don't use reference is simple becase we
//               Sometimes need convenience to convert when pass pointer string.
// Version     : 2011-05-09 (21:16) 1.0 Created
//               2011-05-29 (17:47) 1.1 Privode two versions, char and wchat_t
// Author      : heidong
// Contact     : buf1024@gmail.com
// Copyright(c): HEIDONG
////////////////////////////////////////////////////////////////////////////////////////
/** @file StdString.h */
#pragma once

#include <stdlib.h>
#include <string>
#include <list>

#include <tchar.h>
#include <Windows.h>

#if _UNICODE
#define StdString   std::wstring
#define StdChar     wchar_t
#else
#define StdString   std::string
#define StdChar     char
#endif


//extend string service
/**
 * Convert an ANSI string to ANSI string
 * @return the ANSI string
 */
inline std::string GetAnsiString(const std::string strValue)
{
    return strValue;
}
/**
 * Convert a wide string to ANSI string
 * @return the ANSI string
 */
inline std::string GetAnsiString(const std::wstring strValue)
{
    if (strValue.empty())
    {
        return "";
    }

    int nLen = (strValue.length() + 1)*2;
    //int nLen = strValue.length() + 1;
    char* pszBuf = new char[nLen];
    //We don't use C Runtime convert function here
    ::WideCharToMultiByte(CP_ACP, 0L, 
        strValue.c_str(), -1, 
        pszBuf, nLen,
        NULL, NULL);

    //wcstombs(pszBuf, strValue.c_str(), nLen);

    std::string strRet = pszBuf;
    delete[] pszBuf;

    return strRet;
}
/**
 * Convert a wide string to wide string
 * @return the wide string
 */
inline std::wstring GetWideString(const std::wstring strValue)
{
    return strValue;
}
/**
 * Convert a ANSI string to wide string
 * @return the wide string
 */

inline std::wstring GetWideString(const std::string strValue)
{
    if (strValue.empty())
    {
        return L"";
    }

    int nLen = strValue.length() + 1;
    wchar_t* pszBuf = new wchar_t[nLen];
    //We don't use C Runtime convert function here
    ::MultiByteToWideChar(CP_ACP, 0L,
        strValue.c_str(), -1,       
        pszBuf, nLen);

    //mbstowcs(pszBuf, strValue.c_str(), nLen);

    std::wstring strRet = pszBuf;
    delete[] pszBuf;

    return strRet;
}

/**
 * Convert a ANSI string to std string
 * @return the std string
 */
inline StdString GetStdString(std::string strValue)
{
#if _UNICODE
    return GetWideString(strValue);
#else    
    return strValue;
#endif    
}
/**
 * Convert a wide string to std string
 * @return the std string
 */
inline StdString GetStdString(const std::wstring strValue)
{
#if _UNICODE
    return strValue;
#else
    return GetAnsiString(strValue);
#endif    
}

/**
 * @see GetCStyleStdString
 */
inline const char* GetCStyleAnsiString(const std::string strValue, char* pBuf)
{
    //std::string strVal = GetAnsiString(strValue);
    std::string strVal = strValue;
    int nLen = strVal.length() + 1;
    memcpy(pBuf, strVal.c_str(), nLen);
    return pBuf;
}
/**
 * @see GetCStyleStdString
 */
inline const char* GetCStyleAnsiString(const std::wstring strValue, char* pBuf)
{
    return GetCStyleAnsiString(
        GetAnsiString(strValue), pBuf);
}
/**
 * @see GetCStyleStdString
 */
inline const wchar_t* GetCStyleWideString(const std::wstring strValue, wchar_t* pBuf)
{
    std::wstring strVal = strValue;
    int nLen = (strVal.length() + 1)*sizeof(wchar_t);
    memcpy(pBuf, strVal.c_str(), nLen);
    return pBuf;
}

/**
 * @see GetCStyleStdString
 */
inline const wchar_t* GetCStyleWideString(const std::string strValue, wchar_t* pBuf)
{
    return GetCStyleWideString(
        GetWideString(strValue), pBuf);
}

/**
 * @see GetCStyleStdString
 */
inline const StdChar* GetCStyleStdString(const std::string strValue, StdChar* pBuf)
{
#if _UNICODE
    return GetCStyleWideString(strValue, pBuf);
#else
    return GetCStyleAnsiString(strValue, pBuf);
#endif
}

/**
 * Get the C style string
 * @param strValue the string
 * @param pBuf the buffer to hold the value
 * @return the C style string
 */
inline const StdChar* GetCStyleStdString(const std::wstring strValue, StdChar* pBuf)
{
#if _UNICODE
    return GetCStyleWideString(strValue, pBuf);
#else
    return GetCStyleAnsiString(strValue, pBuf);
#endif
}


/**
 * Split a given string into group
 * @param strValue the string that will be handled
 * @param strDelim the delimiter
 * @param rgpRet the result that return
 * @return the count of the split result
 */
template<typename T>
int Split(T& strValue, const T& strDelim, std::list<T>& rgpRet)
{
    rgpRet.clear();

    if (strDelim.empty())
    {
        return -1;
    }

    size_t nStartPos = 0;
    size_t nIdx = strValue.find_first_of(strDelim);
    while(nIdx != T::npos)
    {
        T strSub = strValue.substr(nStartPos, nIdx - nStartPos);
        if (!strSub.empty())
        {
            rgpRet.push_back(strSub);
        }        
        nStartPos = nIdx + 1;
        nIdx = strValue.find_first_of(strDelim, nStartPos);
    }
    if (nStartPos < strValue.length())
    {
        rgpRet.push_back(strValue.substr(nStartPos));
    }
    return rgpRet.size();
}

/**
 * Trim the left of a given string
 * @param strValue the string that will be handled
 * @param strDelim the delimiter
 * @return the trimmed result
 * @see TrimRight
 * @see Trim
 */
template<typename T>
T TrimLeft(T& strValue, const T& strDelim)
{
    const T::traits_type::_Elem* pData = strValue.c_str();
    size_t nStart = 0;
    for(size_t i=0; i<strValue.length(); i++)
    {        
        if (strDelim.find(pData[i]) != T::npos)
        {
            nStart++;
        }
        else
        {
            break;
        }
    }
    return pData + nStart;
}


/**
 * Trim the right of a given string
 * @param strValue the string that will be handled
 * @param strDelim the delimiter
 * @return the trimmed result
 * @see TrimLeft
 * @see Trim
 */
template<typename T>
T TrimRight(T& strValue, const T& strDelim)
{
    const T::traits_type::_Elem* pData = strValue.c_str();
    size_t nEnd = strValue.length();
    for(size_t i=nEnd-1; i>=0; i--)
    {
        if (strDelim.find(pData[i]) != T::npos)
        {
            nEnd--;
        }
        else
        {
            break;
        }
    }
    return strValue.substr(0, nEnd);
}

/**
 * Trim the given string
 * @param strValue the string that will be handled
 * @param strDelim the delimiter
 * @return the trimmed result
 * @see TrimLeft
 * @see TrimRight
 */
template<typename T>
inline T Trim(T& strValue, const T& strDelim)
{
    return TrimLeft(
        TrimRight(strValue, strDelim), strDelim);

}

/**
 * Check whether a string is starts with another string
 * @param strValue the string that will be checked
 * @param strSubStr the started string
 * @return true is a string is starts with another string, false otherwise
 */
template<typename T>
bool StartsWith(const T& strValue, const T& strSubStr)
{
    T str = strValue.substr(0, strSubStr.length());
    return str == strSubStr;
}

/**
 * Check whether a string is ends with another string
 * @param strValue the string that will be checked
 * @param strSubStr the ended string
 * @return true is a string is ends with another string, false otherwise
 */
//template<typename T>
//bool EndsWith(T& strValue, T& strSubStr);
template<typename T>
bool EndsWith(const T& strValue, const T& strSubStr)
{
    int nPos = strValue.length() - strSubStr.length();
    if (nPos >= 0)
    {
        T str = strValue.substr(nPos);
        return str == strSubStr;
    }
    return false;
}
/**
 * Check whether a string contains another string
 * @param strValue the string that will be checked
 * @param strSubStr the ended string
 * @return true is a string is ends with another string, false otherwise
 */
template<typename T>
bool Contains(const T& strValue, const T& strSubStr)
{
    return strValue.find(strSubStr) != T::npos;
}
