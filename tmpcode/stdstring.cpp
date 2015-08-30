/*
 * File       : stdstring.cpp
 * Description: 字符串功能扩展
 * Version    : 2011-05-09 Created
 *              2011-09-24 1.1 多操作系统支持
 *              2011-10-20 1.2 改成仅支持char，保留某些wchar_t，
 *                             因为蛋痛，去掉std::string，char
 *                             不要装B，将英文改成中文注释
 * Author     : buf1024@gmail.com
 */

#include "stdstring.h"
#include <algorithm>
#include <stdlib.h>


namespace StdString {

#define STR_MIN(a, b) (((a) > (b)) ? (b) : (a))

std::string GetAnsiString(const std::string& strValue)
{
    return strValue;
}

std::string GetAnsiString(const std::wstring& strValue)
{
    if (strValue.empty())
    {
        return "";
    }

    int nLen = (strValue.length() + 1)*sizeof(wchar_t);
    char* pszBuf = new char[nLen];
#ifdef _MSC_VER
    //We don't use C Runtime convert function here
    ::WideCharToMultiByte(CP_ACP, 0L, 
        strValue.c_str(), -1, 
        pszBuf, nLen,
        NULL, NULL);
#else
    wcstombs(pszBuf, strValue.c_str(), strValue.length() + 1);
#endif
    std::string strRet = pszBuf;
    delete[] pszBuf;

    return strRet;
}

std::wstring GetWideString(const std::wstring& strValue)
{
    return strValue;
}

std::wstring GetWideString(const std::string& strValue)
{
    if (strValue.empty())
    {
        return L"";
    }

    int nLen = strValue.length() + 1;
    wchar_t* pszBuf = new wchar_t[nLen];
#ifdef _MSC_VER
    //We don't use C Runtime convert function here
    ::MultiByteToWideChar(CP_ACP, 0L,
        strValue.c_str(), -1,       
        pszBuf, nLen);
#else
    mbstowcs(pszBuf, strValue.c_str(), strValue.length() + 1);
#endif
    std::wstring strRet = pszBuf;
    delete[] pszBuf;

    return strRet;
}


const char* GetCStyleAnsiString(const std::string& strValue, char* pBuf, int& nBufLen)
{
    if (nBufLen > 0)
    {
        nBufLen = STR_MIN((int)strValue.length() + 1, nBufLen);
        memcpy(pBuf, strValue.c_str(), nBufLen);
        return pBuf;
    }
    return 0;
}

const char* GetCStyleAnsiString(const std::wstring& strValue, char* pBuf, int& nBufLen)
{
    return GetCStyleAnsiString(
        GetAnsiString(strValue), pBuf, nBufLen);
}

const wchar_t* GetCStyleWideString(const std::wstring& strValue, wchar_t* pBuf, int& nBufLen)
{
    if (nBufLen > 0)
    {
        nBufLen = STR_MIN((int)(strValue.length()*sizeof(wchar_t)), nBufLen);
        memcpy(pBuf, strValue.c_str(), nBufLen);
        return pBuf;
    }
    return 0;
}

const wchar_t* GetCStyleWideString(const std::string& strValue, wchar_t* pBuf, int& nBufLen)
{
    return GetCStyleWideString(
        GetWideString(strValue), pBuf, nBufLen);
}


int StringLenth(const char* szStrVal)
{
    if (szStrVal == 0)
    {
        return -1;
    }
    const char* pChar = szStrVal;
    while(*pChar != '\0')
    {
        pChar++;
    }

    return (int)(pChar - szStrVal);
    
}
int Split(const char* szStrValue, const char* szStrDelim, std::vector<std::string>& rgpRet)
{
    
    std::list<std::string> lstRet;
    int nRet = Split(szStrValue, szStrDelim, lstRet);
    
    rgpRet.clear();
    
    if (nRet > 0)
    { 
        rgpRet.resize(lstRet.size());
        std::copy(rgpRet.begin(), rgpRet.end(), lstRet.begin());
    }

    return nRet;
    
}
int Split(const std::string& strValue, const std::string& strDelim, std::vector<std::string>& rgpRet)
{
    return Split(strValue.c_str(), strDelim.c_str(), rgpRet);
}

int Split(const char* szStrValue, const char* szStrDelim, std::list<std::string>& rgpRet)
{
    if(szStrValue == 0 ||
            szStrDelim == 0 || *szStrDelim == '\0')
        return -1;
    rgpRet.clear();

    const char* pStrFirst = 0;
    const char* pStrValTmp = szStrValue;
    int nCount = 0;
    
    while((pStrFirst = FirstPosition(pStrValTmp, szStrDelim)))
    {
        nCount = pStrFirst - pStrValTmp;
        rgpRet.push_back(std::string(pStrValTmp, nCount));
        pStrValTmp = pStrValTmp + nCount + StringLenth(szStrDelim);
        if (*pStrValTmp == '\0')
        {
            rgpRet.push_back("");
        }     
    }
    if (*pStrValTmp != '\0')
    {
        rgpRet.push_back(pStrValTmp);
    }
    

    return rgpRet.size();
}

int Split(const std::string& strValue, const std::string& strDelim, std::list<std::string>& rgpRet)
{
    return Split(strValue.c_str(), strDelim.c_str(), rgpRet);
}


std::string TrimLeft(const char* szStrValue, const char* szStrDelim)
{
    if (szStrValue == 0 ||
        szStrDelim == 0)
    {
        return std::string();
    }
    const char* pStr = szStrValue;
    while(*pStr != '\0')
    {
        if (Contains(szStrDelim, *pStr))
        {
            pStr++;
        }
        else
        {
            break;
        }
        
    }
        
    return std::string(pStr);
}


std::string TrimLeft(const std::string& strValue, const std::string& strDelim)
{
    return TrimLeft(strValue.c_str(), strDelim.c_str());
}

std::string TrimRight(const char* szStrValue, const char* szStrDelim)
{
    if (szStrValue == 0 ||
        szStrDelim == 0)
    {
        return std::string();
    }
    int nLen = StringLenth(szStrValue);
    const char* pStr = szStrValue + nLen;
    while(pStr != szStrValue)
    {
        pStr--;
        if (!Contains(szStrDelim, *pStr))
        {
            pStr++;
            break;
        }
        
    }
    return std::string(szStrValue, pStr - szStrValue);
}

std::string TrimRight(const std::string& strValue, const std::string& strDelim)
{
    return TrimRight(strValue.c_str(), strDelim.c_str());
}

std::string Trim(const char* szStrValue, const char* szStrDelim)
{
    std::string strLeft = TrimLeft(szStrValue, szStrDelim);
    return TrimRight(strLeft.c_str(), szStrDelim);
}

std::string Trim(const std::string& strValue, const std::string& strDelim)
{
    return Trim(strValue.c_str(), strDelim.c_str());
}


bool StartsWith(const char* szStrValue, const char*szStrSubStr)
{
    if (szStrValue == 0 || szStrSubStr == 0)
    {
        return false;
    }

    const char* pStr = szStrValue;
    const char* pSub = szStrSubStr;

    while(*pSub != '\0' && *pStr != '\0')
    {
        if (*pStr != *pSub)
        {
            break;
        }   
        pStr++;
        pSub++;
    }
    if (*pSub == '\0')
    {
        return true;
    }
    return false;
    
}

bool StartsWith(const std::string& strValue, const std::string& strSubStr)
{
    return StartsWith(strValue.c_str(), strSubStr.c_str());
}

bool EndsWith(const char* szStrValue, const char*szStrSubStr)
{

    if (szStrValue == 0 || szStrSubStr == 0)
    {
        return false;
    }

    int nStrLen = StringLenth(szStrValue);
    int nSubLen = StringLenth(szStrSubStr);

    if (nStrLen < nSubLen)
    {
        return false;
    }    

    const char* pSub = szStrSubStr + nSubLen;
    const char* pStr = szStrValue + nStrLen;

    while(pSub != szStrSubStr)
    {
        if (*(--pSub) != *(--pStr))
        {
            return false;
        }
        
    }    
    
    return true;
}

bool EndsWith(const std::string& strValue, const std::string& strSubStr)
{
    return EndsWith(strValue.c_str(), strSubStr.c_str());
}

bool Contains(const char* szStrValue, const char ch)
{
    if (szStrValue == 0)
    {
        return false;
    }
    return strchr(szStrValue, ch)  != 0;
    
}
bool Contains(const std::string& strValue, const char ch)
{
    return Contains(strValue.c_str(), ch);
}


bool Contains(const char* szStrValue, const char* szStrSubStr)
{
    return FirstPosition(szStrValue, szStrSubStr) != 0;
}


bool Contains(const std::string& strValue, const std::string& strSubStr)
{
    return Contains(strValue.c_str(), strSubStr.c_str());
}

const char* FirstPosition(const char* szStrValue, const char* szStrSubStr)
{
    if(szStrValue == 0 ||
        szStrSubStr == 0)
    {
        return false;
    }

    const char* pSub = szStrValue;

    //int nSub = StringLenth(szStrSubStr);
    while((pSub = strchr(pSub, *szStrSubStr)))
    {
        const char* pSubTmp = szStrSubStr;
        const char* pStrTmp = pSub;
        while(*pSubTmp != '\0' && *pStrTmp != '\0')
        {
            if (*pSubTmp != *pStrTmp)
            {
                break;
            }    
            pSubTmp++;
            pStrTmp++;
        }
        if (*pSubTmp == '\0')
        {
            return pSub;
        }
        pSub++;
    }
    return 0;

}
const char* FirstPosition(const char* szStrVal, const char ch)
{
    if (szStrVal == 0)
    {
        return 0;
    }
    return strchr(szStrVal, ch);
}


std::string ToUpper(const char* szStr)
{
    if (szStr == 0)
    {
        return "";
    }
    const char* pStr = szStr;
    const int nDiff = 'z' - 'Z';
    std::string strUpper;
    char ch;
    while(*pStr != '\0')
    {
        ch = *pStr;
        if (ch >= 'a' && ch <= 'z')
        {
            ch -= nDiff;
        }
        strUpper += ch;    
        pStr++;
    }
    return strUpper;  
}
std::string ToUpper(const std::string& strStr)
{
    return ToUpper(strStr.c_str());
}

std::string ToLower(const char* szStr)
{
    if (szStr == 0)
    {
        return "";
    }
    const char* pStr = szStr;
    const int nDiff = 'z' - 'Z';
    std::string strLower;
    char ch;
    while(*pStr != '\0')
    {
        ch = *pStr;
        if (ch >= 'A' && ch <= 'Z')
        {
           ch += nDiff;
        }
        strLower += ch;
        pStr++;
    }
    return strLower;
}
std::string ToLower(const std::string& strStr)
{
    return ToLower(strStr.c_str());
}

bool IsDigit(const char* szStr)
{
    if (szStr == 0)
    {
        return false;
    }
    const char* pStr = szStr;
    while(*pStr != '\0')
    {
        if (*pStr < '0' || *pStr > '9')
        {
            return false;
        }
        pStr++;
    }
    return true; 
}
bool IsDigit(const std::string& strStr)
{
    return IsDigit(strStr.c_str());
}

bool IsAlpha(const char* szStr)
{
    if (szStr == 0)
    {
        return "";
    }
    const char* pStr = szStr;
    while(*pStr != '\0')
    {
        if (!(*pStr >= 'a' && *pStr <= 'z') ||
            !(*pStr >= 'A' && *pStr <= 'Z'))
        {
            return false;
        }
        pStr++;
    }
    return true; 
}
bool IsAlpha(const std::string& strStr)
{
    return IsAlpha(strStr.c_str());
}

std::string Replace(const char* szStrVal, const char* szStrOld, const char* szStrNew)
{
    if (szStrVal == 0 || *szStrVal == '\0')
    {
        return "";
    }
    if (szStrOld == 0 || szStrNew == 0)
    {
        return szStrVal;
    }
    std::string strRet;
    const char* pTmpStr = szStrVal;
    int nLenOld = StringLenth(szStrOld);
    while(true)
    {
        const char* pPosStr = FirstPosition(pTmpStr, szStrOld);
        if (pPosStr == 0)
        {
            break;
        }
        strRet.append(std::string(pTmpStr, pPosStr - pTmpStr));
        strRet.append(szStrNew);
        pTmpStr = pPosStr + nLenOld;
        
    }
    if (*pTmpStr != '\0')
    {
        strRet.append(pTmpStr);
    }
    return strRet;
    
}
std::string Replace(const std::string& strVal, const std::string strOld, const std::string strNew)
{
    return Replace(strVal.c_str(), strOld.c_str(), strNew.c_str());
}

std::string FromNumber(long lVal)
{
    char szTmp[32] = "";

#ifdef _MSC_VER
    if(lVal < 0){
        _snprintf(szTmp, sizeof(szTmp), "-%ld", -lVal);
    }else{
        _snprintf(szTmp, sizeof(szTmp), "%ld", lVal);
    }
#else
    if(lVal < 0){
        snprintf(szTmp, sizeof(szTmp), "-%ld", -lVal);
    }else{
        snprintf(szTmp, sizeof(szTmp), "%ld", lVal);
    }
#endif
    return szTmp;
}

std::string FromNumber(int nVal)
{
    return FromNumber((long)nVal);
}

std::string FromNumber(double fVal)
{
    char szTmp[32] = "";

#ifdef _MSC_VER
    if(fVal < 0){
        _snprintf(szTmp, sizeof(szTmp), "-%lf", -fVal);
    }else{
        _snprintf(szTmp, sizeof(szTmp), "%lf", fVal);
    }
#else
    if(fVal < 0){
        snprintf(szTmp, sizeof(szTmp), "-%lf", -fVal);
    }else{
        snprintf(szTmp, sizeof(szTmp), "%lf", fVal);
    }
#endif
    return szTmp;
}

long ToLong(const char* szStrVal, bool& bStat)
{
	long lVal = 0L;
	bStat = true;
	if (szStrVal == 0)
	{
		bStat = false;
	}
	else
	{
		int nRet = -1;
		nRet = sscanf(szStrVal, "%ld", &lVal);
		if (nRet == -1)
		{
			bStat = false;
		}
	}
    return lVal;
}
long ToLong(const std::string& strVal, bool& bStat)
{
    return ToLong(strVal.c_str(), bStat);
}
long ToInt(const char* szStrVal, bool& bStat)
{
	int nVal = 0;
	bStat = true;
	if (szStrVal == 0)
	{
		bStat = false;
	}
	else
	{
		int nRet = -1;
		nRet = sscanf(szStrVal, "%i", &nVal);
		if (nRet == -1)
		{
			bStat = false;
		}
	}
	return nVal;
}
long ToInt(const std::string& strVal, bool& bStat)
{
	return ToInt(strVal.c_str(), bStat);
}
double ToDouble(const char* szStrVal, bool& bStat)
{
	double dVal = 0.0;
	bStat = true;
	if (szStrVal == 0)
	{
		bStat = false;
	}
	else
	{
		int nRet = -1;
		nRet = sscanf(szStrVal, "%lf", &dVal);
		if (nRet == -1)
		{
			bStat = false;
		}
	}
	return dVal;
}
double ToDouble(const std::string& strVal, bool& bStat)
{
    return ToDouble(strVal.c_str(), bStat);
}

}

