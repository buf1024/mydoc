/*
 * File       : stdstring.h
 * Description: 字符串功能扩展
 * Version    : 2011-05-09 Created
 *              2011-09-24 1.1 多操作系统支持
 *              2011-10-20 1.2 改成仅支持char，保留某些wchar_t，
 *                             因为蛋痛，去掉std::string，char
 *                             不要装B，将英文改成中文注释
 * Author     : buf1024@gmail.com
 */

#ifndef XBASIC_STDSTRING_H_
#define XBASIC_STDSTRING_H_

#include <string>
#include <list>
#include <vector>

namespace StdString {

//字符串扩展功能
/**
 * 将ANSI/宽字符串转换为ANSI/宽字符串
 * @param strValue 要转换的ANSI/宽字符串
 * @return 已转换的ANSI/宽字符串
 */
std::string GetAnsiString(const std::string& strValue);
/**
 * @see GetAnsiString
 */
std::string GetAnsiString(const std::wstring& strValue);
/**
 * @see GetAnsiString
 */
std::wstring GetWideString(const std::string& strValue);
/**
 * @see GetAnsiString
 */
std::wstring GetWideString(const std::wstring& strValue);

/**
 * 获取C风格的字符串
 * @param szValue 要计算的字符串
 * @param pBuf 结果缓存
 * @param nBufLen 作为传入参数时，为缓存的大小，传出参数时，为实际的长度
 * @return 缓存的地址
 */
const char* GetCStyleAnsiString(const std::string& strValue, char* pBuf, int& nBufLen);
/**
 * @see GetCStyleAnsiString
 */
const char* GetCStyleAnsiString(const std::wstring& strValue, char* pBuf, int& nBufLen);
/**
 * @see GetCStyleAnsiString
 */
const wchar_t* GetCStyleWideString(const std::wstring& strValue, wchar_t* pBuf, int& nBufLen);
/**
 * @see GetCStyleAnsiString
 */
const wchar_t* GetCStyleWideString(const std::string& strValue, wchar_t* pBuf, int& nBufLen);

/**
 * 计算字符串的长度
 * @param szStrValue 要计算的字符串
 * @return 字符个数，如果出错则返回负数
 */
int StringLenth(const char* szStrVal);
/**
 * 将给出的字符串分割到组里
 * @param szStrValue 要分割的字符串
 * @param strDelim 分割符
 * @param szStrDelim 结果集合
 * @return 结果集的个数，如果出错则返回负数
 */
int Split(const char* szStrValue, const char* szStrDelim, std::vector<std::string>& rgpRet);
/**
 * @see Split
 */
int Split(const std::string& strValue, const std::string& strDelim, std::vector<std::string>& rgpRet);
/**
 * @see Split
 */
int Split(const char* szStrValue, const char* szStrDelim, std::list<std::string>& rgpRet);
/**
 * @see Split
 */
int Split(const std::string& strValue, const std::string& strDelim, std::list<std::string>& rgpRet);

/**
 * 去掉字符串A左边包含字符串B的部分
 * @param szStrValue 要处理的字符串
 * @param szStrDelim 包含的字符串
 * @return 已经去掉字符串A左边包含字符串B的部分
 * @see TrimRight
 * @see Trim
 */
std::string TrimLeft(const char* szStrValue, const char* szStrDelim);

/**
 * @see TrimLeft
 */
std::string TrimLeft(const std::string& strValue, const std::string& strDelim);


/**
 * 去掉字符串A右边包含字符串B的部分
 * @param szStrValue 要处理的字符串
 * @param szStrDelim 包含的字符串
 * @return 已经去掉字符串A右边包含字符串B的部分
 * @see TrimLeft
 * @see Trim
 */
std::string TrimRight(const char* szStrValue, const char* szStrDelim);
/**
 * @see TrimRight
 */
std::string TrimRight(const std::string& strValue, const std::string& strDelim);
/**
 * 去掉字符串A左边和右边包含字符串B的部分
 * @param szStrValue 要处理的字符串
 * @param szStrDelim 包含的字符串
 * @return 去掉字符串A左边和右边包含字符串B的部分
 * @see TrimLeft
 * @see TrimRight
 */
std::string Trim(const char* szStrValue, const char* szStrDelim);
/**
 * @see Trim
 */
std::string Trim(const std::string& strValue, const std::string& strDelim);

/**
 * 检查字符串A是否以字符串B开始
 * @param strValue 被检查的字符串A
 * @param strSubStr 开始的字符串B
 * @return false 字符串A不是以字符串B开始或者出借, true 查字符串是以字符串B开始
 */
bool StartsWith(const char* szStrValue, const char*szStrSubStr);
/**
 * @see StartsWith
 */
bool StartsWith(const std::string& strValue, const std::string& strSubStr);
/**
 * 测试A字符串是否以B字符串结束
 * @param strValue 被测试的字符串
 * @param strSubStr 结束的字符串
 * @return true 字符串A以B字符串结束, false 字符串A不以B字符串结束
 */
bool EndsWith(const char* szStrValue, const char*szStrSubStr);

/**
 * @see EndsWith
 */
bool EndsWith(const std::string& strValue, const std::string& strSubStr);

/**
 * 测试A字符串是否包含B字符
 * @param strValue 被测试的字符串
 * @param ch B字符
 * @return true 字符串A包含B字符串, false 字符串A不包含B字符
 */
bool Contains(const char* szStrValue, const char ch);
/**
 * @see EndsWith
 */
bool Contains(const std::string& strValue, const char ch);
/**
 * @see EndsWith
 */
bool Contains(const char* szStrValue, const char* szStrSubStr);
/**
 * @see EndsWith
 */
bool Contains(const std::string& strValue, const std::string& strSubStr);
/**
 * 找出A字符串里以B字符串开始的地址
 * @param szStrVal A字符串
 * @param szSubVal B字符串
 */
const char* FirstPosition(const char* szStrVal, const char* szSubVal);
/**
 * @see FirstPosition
 */
const char* FirstPosition(const char* szStrVal, const char ch);


std::string ToUpper(const char* szStr);
std::string ToUpper(const std::string& strStr);

std::string ToLower(const char* szStr);
std::string ToLower(const std::string& strStr);

bool IsDigit(const char* szStr);
bool IsDigit(const std::string& strStr);

bool IsAlpha(const char* szStr);
bool IsAlpha(const std::string& strStr);

std::string Replace(const char* szStrVal, const char* szStrOld, const char* szStrNew);
std::string Replace(const std::string& strVal, const std::string strOld, const std::string strNew);

std::string FromNumber(long lVal);
std::string FromNumber(int nVal);
std::string FromNumber(double fVal);

long ToLong(const char* szStrVal, bool& bStat);
long ToLong(const std::string& strVal, bool& bStat);

long ToInt(const char* szStrVal, bool& bStat);
long ToInt(const std::string& strVal, bool& bStat);

double ToDouble(const char* szStrVal, bool& bStat);
double ToDouble(const std::string& strVal, bool& bStat);

}


#endif /* XBASIC_STDSTRING_H_*/
