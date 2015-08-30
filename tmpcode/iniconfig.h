/*
 * File       : iniconfig.h
 * Description: Ini配置文件
 * Version    : 2010-10-16 Created
 *              2011-09-24 1.1 多操作系统支持
 *              2011-12-16 1.2 增加环境变量解析
 * Author     : buf1024@gmail.com
 */

#ifndef XBASIC_INICONFIG_H_
#define XBASIC_INICONFIG_H_

#include <list>
#include <map>
#include <string>

/**
* Represent a single Section of an ini configuration file
*/
class Section
{
    typedef std::map< std::string, std::string >::iterator KVIterator;
public:
    /**
    * Construct a section with an specific section name
    * @param strSectionName section name, default to empty
    */
    Section(std::string strSectionName = "");
    ~Section();
public:
    /**
    * Set a section name
    * @param strSectionName the section name
    */
    void SetSectionName(const std::string strSectionName);
    /**
    * Get a section name
    * @return the section name
    */
    std::string GetSectionName() const;

    /** @name Get and Set function*/
    /** @{*/
    /**
    * Insert/update a specific key with a specific value
    * @param strKey Key
    * @param strValue the string value
    */
    void Insert(const std::string strKey, std::string strValue);
    /**
    * Insert/update a specific key with a specific value
    * @param strKey Key
    * @param chValue the character value
    */
    void Insert(const std::string strKey, char chValue);
    /**
    * Insert/update a specific key with a specific value
    * @param strKey Key
    * @param nValue the int value
    */
    void Insert(const std::string strKey, int nValue);
    /**
    * Insert/update a specific key with a specific value
    * @param strKey Key
    * @param lValue the long value
    */
    void Insert(const std::string strKey, long lValue);
    /**
    * Insert/update a specific key with a specific value
    * @param strKey Key
    * @param lValue the double dValue
    */
    void Insert(const std::string strKey, double dValue);
    /**
    * Get a specific key value, if the key not found, return the default one
    * @param strKey Key
    * @param strValue The return value
    * @param strDefault The default value
    * @return true if find the specific key, otherwise false
    */
    bool GetValue(const std::string strKey, std::string& strValue, std::string strDefault = "");
    bool GetValue(const std::string strKey, bool& bValue, bool bDefault = false);
    bool GetValue(const std::string strKey, int& nValue, int nDefault = 0);
    bool GetValue(const std::string strKey, long& lValue, long lDefault = 0L);
    bool GetValue(const std::string strKey, double& dValue, double fDefault = 0.0f);
    /**
    * Delete the specific key/value pair
    * @param strKey Key
    */
    void Delete(const std::string strKey);
    /**Empty the section*/
    void Empty();
    /**@}*/
    /**
    * Save the section to the specific file
    * @param strFilePath the file path
    * @return true if save successfully, otherwise false
    */
    bool Save(std::string strFilePath);

    
    bool Save(FILE* pFile);

private:
    std::string m_strSectionName;
    std::map< std::string, std::string > m_mapKeyValue;
    
};

/**Represent Ini Configuration File*/
class IniConfig
{
    typedef std::list<Section*>::iterator ICIterator;
public:
    typedef std::list<Section*>::const_iterator Iterator;
public:
    /**
    * Init IniConfig class with the specific file.
    * @param strFilePath the file path
    */
    IniConfig(std::string strFilePath = "");
    ~IniConfig();

    /**
    * Set the file path
    * @param strFilePath the file path
    */
    void SetFilePath(const std::string strFilePath);
    /**
    * Get the file path
    * @return the file path
    */
    std::string GetFilePath() const;

    /**
    * Insert the specific section
    * @param pSec The specific section
    * @return true if successfully inserted, otherwise is the section equals false.
    */
    bool Insert(Section* pSec);
    /**
    * Delete the specific section.*
    * @param strSection the specific secion name
    */
    void Delete(const std::string strSection);
    /**
    * Get the specific section
    * @param strSecName The specific section name
    * @return The specific section name, or NULL if not found.
    */
    Section* GetSection(const std::string strSecName);


    /**
    * Load the specific IniConfig from the file.
    * @param strFilePath the file path
    * @return true if load successfully, otherwise false
    */
    bool Load(const std::string strFilePath = "");
    /**
    * Save the IniConfig to the specific file
    * @param strFilePath the file path
    * @return true if save successfully, otherwise false
    */
    bool Save(const std::string strFilePath = "");

    Iterator begin() const
    {
        return m_lstSections.begin();
    }
    Iterator end() const
    {
        return  m_lstSections.end();
    }
private:
    void ClearUp();
private:
    std::string m_strFilePath;
    std::list<Section*> m_lstSections;
    
};


#endif /* XBASIC_INICONFIG_H_ */
