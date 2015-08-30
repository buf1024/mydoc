/*
 * File       : iniconfig.cpp
 * Description: Ini配置文件
 * Version    : 2010-10-16 Created
 *              2011-09-24 1.1 多操作系统支持
 *              2011-12-16 1.2 增加环境变量解析
 * Author     : buf1024@gmail.com
 */
#include "iniconfig.h"
#include "stdstring.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>

#define INI_BUFSIZ 512
#define INI_MAXLINE 1024

Section::Section(std::string strSectionName /* = "" */)
:m_strSectionName(strSectionName)
{
    m_mapKeyValue.clear();
    
}
Section::~Section()
{
}

void Section::SetSectionName(const std::string strSectionName)
{
    m_strSectionName = strSectionName;
}
std::string Section::GetSectionName() const
{
    return m_strSectionName;
}

void Section::Insert(const std::string strKey, std::string strValue)
{
    if (!strKey.empty())
    {
        if (!strValue.empty())
        {
            const char* szPos = strValue.c_str();
            while((szPos = StdString::FirstPosition(szPos, '$')) != 0)
            {
                if (szPos != strValue.c_str())
                {
                    // 转义
                    if (*(szPos-1) == '\\')
                    {
                        szPos++;
                        continue;
                    }
                    
                }
                
                const char* pStart = szPos + 1;
                const char* pEnd = pStart;
                while (*pEnd)
                {
                    if ((*pEnd >= 'a' && *pEnd <= 'z') ||
                        (*pEnd >= 'A' && *pEnd <= 'Z') ||
                        (*pEnd >= '0' && *pEnd <= '9') ||
                        *pEnd == '_')
                    {
                        pEnd++;
                    }
                    else
                    {
                        break;
                    }
                }
                if (pEnd != pStart)
                {
                    char szVal[INI_BUFSIZ] = "";
                    const char* pEnv = 0;
                    strncpy(szVal, pStart, pEnd - pStart);
                    pEnv = getenv(szVal);
                    if (pEnv)
                    {
                        szVal[0] ='$';
                        strncpy(szVal+1, pStart, pEnd - pStart);
                        int nPos = pStart - strValue.c_str() - 1;
                        strValue = StdString::Replace(strValue.c_str(), szVal, pEnv);
                        szPos = strValue.c_str() + nPos + StdString::StringLenth(pEnv);
                    }
                    else
                    {
                        szPos++;
                    }
                }
            }
            
            m_mapKeyValue[strKey] = strValue;
        }
        
    }

}
void Section::Insert(const std::string strKey, char chValue)
{
    if (!strKey.empty())
    {
        char szBuf[32] = {0};
#ifdef _MSC_VER
        _snprintf(szBuf, 32, "%c", chValue);
#else
        snprintf(szBuf, 32, "%c", chValue);
#endif
        m_mapKeyValue[strKey] = szBuf;
    }
}
void Section::Insert(const std::string strKey, int nValue)
{
    if (!strKey.empty())
    {
        char szBuf[32] = {0};
#if _MSC_VER
        _snprintf(szBuf, 32, "%d", nValue);
#else
        snprintf(szBuf, 32, "%d", nValue);
#endif
        m_mapKeyValue[strKey] = szBuf;
    }
}
void Section::Insert(const std::string strKey, long lValue)
{
    if (!strKey.empty())
    {
        char szBuf[32] = {0};
#ifdef _MSC_VER
        _snprintf(szBuf, 32, "%ld", lValue);
#else
        snprintf(szBuf, 32, "%ld", lValue);
#endif
        m_mapKeyValue[strKey] = szBuf;
    }

}
void Section::Insert(const std::string strKey, double dValue)
{
    if (!strKey.empty())
    {
        char szBuf[32] = {0};
#if _MSC_VER
        _snprintf(szBuf, 32, "%lf", dValue);
#else
        snprintf(szBuf, 32, "%lf", dValue);
#endif
        m_mapKeyValue[strKey] = szBuf;
    }
}
bool Section::GetValue(const std::string strKey, std::string& strValue, std::string strDefault)
{
    bool bRet = false;
    if (!strKey.empty())
    {
        KVIterator iter = m_mapKeyValue.find(strKey);
        if (iter != m_mapKeyValue.end())
        {
            strValue = iter->second;
            bRet = true;
        }
        else
        {
            strValue = strDefault;
        }

    }

    return bRet;
}

bool Section::GetValue(const std::string strKey, bool& bValue, bool bDefault)
{
    std::string strVal;
    if (GetValue(strKey, strVal))
    {
        strVal = StdString::ToLower(strVal);
        if (strVal == "1" || strVal == "true")
        {
            bValue = true;
            return true;
        }
        if (strVal == "0" || strVal == "false")
        {
            bValue = false;
            return true;
        }
        
    }
    bValue = bDefault;
    return false;    
}
bool Section::GetValue(const std::string strKey, int& nValue, int nDefault)
{
    std::string strVal;
    if (GetValue(strKey, strVal))
    {
        int nRet = 0;
        nRet = sscanf(strVal.c_str(), "%d", &nValue);
        if (nRet <= 0)
        {
            nValue = nDefault;
            return false;
        }
        return true;
    }
    nValue = nDefault;
    return false;   
}
bool Section::GetValue(const std::string strKey, long& lValue, long lDefault)
{
    std::string strVal;
    if (GetValue(strKey, strVal))
    {
        int nRet = 0;
        nRet = sscanf(strVal.c_str(), "%ld", &lValue);
        if (nRet <= 0)
        {
            lValue = lDefault;
            return false;
        }
        return true;
    }
    lValue = lDefault;
    return false;   
}
bool Section::GetValue(const std::string strKey, double& fValue, double fDefault)
{
    std::string strVal;
    if (GetValue(strKey, strVal))
    {
        int nRet = 0;
        nRet = sscanf(strVal.c_str(), "%lf", &fValue);
        if (nRet <= 0)
        {
            fValue = fDefault;
            return false;
        }
        return true;
    }
    fValue = fDefault;
    return false;
}

void Section::Delete(const std::string strKey)
{
    if (!strKey.empty())
    {
        KVIterator iter = m_mapKeyValue.find(strKey);
        if (iter != m_mapKeyValue.end())
        {
            m_mapKeyValue.erase(iter);
        }
    }

}
void Section::Empty()
{
    m_strSectionName = "";
    m_mapKeyValue.clear();
}

bool Section::Save(std::string strFilePath)
{
    FILE* pFile = fopen(strFilePath.c_str(), "w+");

    return Save(pFile);
}

bool Section::Save(FILE* pFile)
{
    if (pFile != 0 && !m_strSectionName.empty())
    {
        std::string strLine = "[";
        strLine.append(m_strSectionName);
        strLine.append("]");
        strLine.append("\n");

        std::string strBuf = strLine;

        fwrite(strBuf.c_str(), sizeof(char), strBuf.length(), pFile);
        
        for (KVIterator iter = m_mapKeyValue.begin(); iter != m_mapKeyValue.end(); ++iter)
        {
            strLine = iter->first;
            strLine.append("=");
            strLine.append(iter->second);
            strLine.append("\n");

            strBuf = strLine;

            fwrite(strBuf.c_str(), sizeof(char), strBuf.length(), pFile);
        }
        return true;
    }
    return false;
}

// IniConfig

IniConfig::IniConfig(std::string strFilePath)
{
    
    m_strFilePath = strFilePath;
    if (!m_strFilePath.empty())
    {
        Load(m_strFilePath);
    }    
    
}
IniConfig::~IniConfig()
{
    ClearUp();
}

void IniConfig::SetFilePath(const std::string strFilePath)
{
    m_strFilePath = strFilePath;
}
std::string IniConfig::GetFilePath() const
{
    return m_strFilePath;
}

bool IniConfig::Insert(Section* pSec)
{
    assert(pSec != 0);

    bool bRet = false;
    if (pSec)
    {
        if (GetSection(pSec->GetSectionName()) == NULL)
        {
            m_lstSections.push_back(pSec);
            bRet = true;
        }
    }
    return bRet;

}
void IniConfig::Delete(const std::string strSection)
{
    for(ICIterator iter = m_lstSections.begin();
        iter != m_lstSections.end();
        ++iter)
    {
        if ((*iter)->GetSectionName() == strSection)
        {
            delete *iter;
            m_lstSections.erase(iter);
            break;
        }
    }

}
Section* IniConfig::GetSection(const std::string strSecName)
{
    Section* pSec = 0;

    for(ICIterator iter = m_lstSections.begin();
        iter != m_lstSections.end();
        ++iter)
    {
        if ((*iter)->GetSectionName() == strSecName)
        {
            pSec = *iter;
            break;
        }
    }

    return pSec;
}

bool IniConfig::Load(const std::string strFilePath)
{
    ClearUp();
    if (!strFilePath.empty())
    {
        m_strFilePath = strFilePath;
    }
    if (m_strFilePath.empty())
    {
        return false;
    }
    

    FILE* pFile  = fopen(strFilePath.c_str(), "r");

    if (pFile == 0)
    {
        return false;
    }

    char szLine[INI_MAXLINE] = "";

    const char* pTmp = 0;

    Section* pCurSec = 0;

    while(feof(pFile) != EOF)
    {
        szLine[0] = '\0';

        if (fgets(szLine, INI_MAXLINE - 1, pFile) == 0)
        {
            break;
        }
        std::string strTrim = StdString::Trim(szLine, " \r\n");
        pTmp = strTrim.c_str();

//        int n = StringLenth(pTmp);
        if (StdString::StringLenth(pTmp) <= 0){
            continue;
        }
        if (*pTmp == '#' || *pTmp == ';')
        {
            continue;
        }
        // Start section
        if (*pTmp == '[')
        {
            const char* pNameDel = ++pTmp;
            while(*pTmp != '\0' && *pTmp != ']')
            {
                pTmp++;
            }
            if (*pTmp == '\0')
            {
                fclose(pFile);
                return false;
            }
            pCurSec = new Section(std::string(pNameDel, pTmp - pNameDel));
            m_lstSections.push_back(pCurSec);
            continue;
        }
        if (pCurSec != 0)
        {
            std::list<std::string> lstKV;
            int nCount = StdString::Split(pTmp, "=", lstKV);
            if (nCount != 2)
            {
                fclose(pFile);
                return false;
            }
            std::string strKey = lstKV.front();
            std::string strVal = lstKV.back();
            pCurSec->Insert(strKey, strVal);
            
        }
    }

    fclose(pFile);

    return true;
}
bool IniConfig::Save(const std::string strFilePath)
{
    std::string strFileSaved;
    if (strFilePath.empty())
    {
        strFileSaved = m_strFilePath;
    }
    else
    {
        strFileSaved = strFilePath;
    }

    FILE* pFile  = fopen(strFileSaved.c_str(), "w+");

    if (pFile == 0)
    {
        return false;
    }

    for(ICIterator iter = m_lstSections.begin();
        iter != m_lstSections.end();
        ++iter)
    {
        (*iter)->Save(pFile);
    }

    fclose(pFile);

    return true;

}
void IniConfig::ClearUp()
{
    //m_strFilePath = "";
    for(ICIterator iter = m_lstSections.begin();
        iter != m_lstSections.end();
        ++iter)
    {
        delete *iter;
    }
}

