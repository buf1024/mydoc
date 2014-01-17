#ifndef X_TEST_H_
#define X_TEST_H_

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <stdlib.h>
#include <iostream>
#include <time.h>

///////////////////////////////////////
// Util
class TheUtilThatAssummingUsingOnlyHereSoThisClassNameIsVeryVeryVeryBoringLongLongLongLong
{
public:
    static char GetHex(char ch)
    {
        ch = (0x0F & ch);
        if(ch >= 10)
        {
            return 'A' + ch - 10;
        }
        return '0' + ch;
    }
    static std::string HexDump(void* pBuf, int nSize)
    {
        std::string strRet;
        if(pBuf)
        {
            const char* szTmpBuf = (const char*)pBuf;
            int nTmpSize = 0;
            while(nTmpSize < nSize)
            {
                if(nTmpSize == 0)
                {
                    strRet = "0x";
                }
                char ch = *(szTmpBuf + nTmpSize);
                strRet.push_back(GetHex((char)((0xF0 & ch) >> 4)));
                strRet.push_back(GetHex((char)(0x0F & ch)));
                nTmpSize++;
            }
        }
        return strRet;
    }
    static std::string ToLower(const char* szStr)
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
    /*
    * Routine to see if a text string is matched by a wildcard pattern.
    * Returns true if the text is matched, or false if it is not matched
    * or if the pattern is invalid.
    *  *		matches zero or more characters
    *  ?		matches a single character
    *  [abc]	matches 'a', 'b' or 'c'
    *  \c		quotes character c
    *  Adapted from code written by Ingo Wilken.
    */
    static bool Match(const char * text, const char * pattern)
    {
        const char *	retryPat;
        const char *	retryText;
        int		ch;
        bool		found;

        retryPat = NULL;
        retryText = NULL;

        while (*text || *pattern)
        {
            ch = *pattern++;

            switch (ch)
            {
            case '*':
                retryPat = pattern;
                retryText = text;
                break;

            case '[':
                found = false;

                while ((ch = *pattern++) != ']')
                {
                    if (ch == '\\')
                        ch = *pattern++;

                    if (ch == '\0')
                        return false;

                    if (*text == ch)
                        found = true;
                }

                if (!found)
                {
                    pattern = retryPat;
                    text = ++retryText;
                }

                if (*text++ == '\0')
                    return false;

                break;

            case '?':
                if (*text++ == '\0')
                    return false;

                break;

            case '\\':
                ch = *pattern++;

                if (ch == '\0')
                    return false;

                if (*text == ch) {
                    if (*text)
                        text++;
                    break;
                }

                if (*text) {
                    pattern = retryPat;
                    text = ++retryText;
                    break;
                }

                return false;

            default:
                if (*text == ch)
                {
                    if (*text)
                        text++;
                    break;
                }

                if (*text)
                {
                    pattern = retryPat;
                    text = ++retryText;
                    break;
                }

                return false;
            }

            if (pattern == NULL)
                return false;
        }

        return true;
    }
};

#define THEUTILCLASSNAME                                                                 \
    TheUtilThatAssummingUsingOnlyHereSoThisClassNameIsVeryVeryVeryBoringLongLongLongLong


class Environment
{
public:
    Environment(){}
    virtual ~Environment(){}
public:
    virtual void SetUp(){}
    virtual void TearDown(){}
};


class TestFixtrue;
class Test
{
protected:
    Test()
        : m_pTF((TestFixtrue*)0)
    {
    }
public:
    Test(TestFixtrue* pTF, std::string strName)
        : m_pTF(pTF)
        , m_strName(strName)
    {
    }

    virtual ~Test()
    {
    }

public:
    virtual void Run()
    {

    }
    static void SetUpTestCase()
    {
    }
    static void TearDownTestCase()
    {

    }
    virtual void SetUp()
    {

    }
    virtual void TearDown()
    {

    }
    std::string GetName() const
    {
        return m_strName;
    }
    void SetName(std::string strName)
    {
        m_strName = strName;
    }
    TestFixtrue* GetFixture() const
    {
        return m_pTF;
    }

protected:
    TestFixtrue* m_pTF;
    std::string m_strName;
};

typedef void (*FixturePrepareFunc)();

class TestFixtrue
{
public:
    typedef std::map<std::string, Test*> TestCaseMap;
    typedef std::map<std::string, Test*>::iterator TestCaseMapIterator;
public:
    TestFixtrue(std::string strName)
        : m_strName(strName)
        , m_pFunSetup(0)
        , m_pFunTearDown(0)
    {
    }
    virtual ~TestFixtrue()
    {
    }

public:

    std::string GetName() const
    {
        return m_strName;
    }
    void SetName(std::string strName)
    {
        m_strName = strName;
    }
    Test* Get(std::string strName)
    {
        for (TestCaseMapIterator iter = m_mapTestCase.begin();
            iter != m_mapTestCase.end(); ++iter)
        {
            if (iter->first == strName)
            {
                return iter->second;
            }            
        }
        return (Test*)0;
    }
    Test* Add(Test* pTC)
    {
        if (pTC)
        {
            m_mapTestCase.insert(std::make_pair(pTC->GetName(), pTC));
        }
        return pTC;
    }
    void Remove(Test* pTC)
    {
        if (pTC)
        {
            TestCaseMapIterator iter =
                m_mapTestCase.find(pTC->GetName());
            if (iter != m_mapTestCase.end())
            {
                m_mapTestCase.erase(iter);
            }          
        }
        
    }
    TestCaseMapIterator begin()
    {
        return m_mapTestCase.begin();
    }
    TestCaseMapIterator end()
    {
        return m_mapTestCase.end();
    }
    void SetSetUp(FixturePrepareFunc pFun)
    {
        m_pFunSetup = pFun;
    }
    void SetTearDown(FixturePrepareFunc pFun)
    {
        m_pFunTearDown = pFun;
    }
    void SetUp()
    {
        if (m_pFunSetup)
        {
            (*m_pFunSetup)();
        }        
    }
    void TearDown()
    {
        if (m_pFunTearDown)
        {
            (*m_pFunTearDown)();
        }
        
    }
    int GetCaseCount() const
    {
        return m_mapTestCase.size();
    }
private:
    std::string m_strName;
    TestCaseMap m_mapTestCase;


    FixturePrepareFunc m_pFunSetup;
    FixturePrepareFunc m_pFunTearDown;
};

class TestManager
{
    enum
    {
        HelpOpt,
        ListTestOpt,
        RunTestOpt,
        InvalidOpt
    };
    struct Pattern
    {
        bool m_bNeg;
        std::string m_strPat;
    };
public:
    typedef std::map<std::string, TestFixtrue*> FixtureMap;
    typedef std::map<std::string, TestFixtrue*>::iterator FixtureIterator; 
    typedef std::vector<Pattern> PatternVec;
    typedef std::vector<Pattern>::iterator PatternIterator;
private:
    TestManager()
        : m_pGlobalEnv(0)
        , m_nOpt(RunTestOpt)
        , m_pLastFailedTC(0)
    {

    }
public:
    ~TestManager()
    {
        CleanUp();
    }

public:
    void Help()
    {
        std::cout <<
            "Very Very Simple Test Framework Help Message\n\n"
            "--help\n"
            "   Print this help messages\n\n"
            "--list_tests\n"
            "   List the names of all tests instead of running them. The name of\n"
            "   TEST(Foo, Bar) is \"Foo.Bar\".\n\n"
            "--filter=POSTIVE_PATTERNS[-NEGATIVE_PATTERNS]\n"
            "   Run only the tests whose name matches one of the positive patterns but\n"
            "   none of the negative patterns. '?' matches any single character; '*'\n"
            "   matches any substring; ':' separates two patterns.\n";
    }
    void ListTests()
    {
        if (m_mapTestFixture.size() > 0)
        {
            std::cout << "Following is all the tests\n";
            std::cout << "====================================\n\n";
        }
        else
        {
            std::cout << "There is no test!!\n";
        }
        
        for (FixtureIterator iter = m_mapTestFixture.begin();
            iter != m_mapTestFixture.end(); ++iter)
        {
            TestFixtrue* pTF = iter->second;
            std::cout << pTF->GetName() << ".\n"; 
            for (TestFixtrue::TestCaseMapIterator iter = pTF->begin();
                iter != pTF->end(); ++iter)
            {
                Test* pTC = iter->second;
                std::cout << "  " << pTC->GetName() << "\n";
            }
            std::cout << std::endl;
        }
    }
    static TestManager* GetInst()
    {
        static TestManager* spMnger = (TestManager*)0;
        if (spMnger == (TestManager*)0)
        {
            spMnger = new TestManager;
        }
        return spMnger;
    }
    void SetGlobalEnvironment(Environment* pEnv)
    {
        m_pGlobalEnv = pEnv;
    }
    TestFixtrue* Get(std::string strFixtureName)
    {
        for (FixtureIterator iter = m_mapTestFixture.begin();
            iter != m_mapTestFixture.end(); ++iter)
        {
            if (iter->first == strFixtureName)
            {
                return iter->second;
            }            
        }
        return (TestFixtrue*)0;        
    }
    TestFixtrue* Add(TestFixtrue* pTF)
    {
        if (pTF)
        {
            m_mapTestFixture.insert(std::make_pair(pTF->GetName(), pTF));
        }       
        return pTF;        
    }
    TestFixtrue* EnsureGet(std::string strFixtureName)
    {
        TestFixtrue* pTF = Get(strFixtureName);
        if (pTF == (TestFixtrue*)0)
        {
            pTF = new TestFixtrue(strFixtureName);
            Add(pTF);
        }        
        return pTF;        
    }
    FixtureIterator begin()
    {
        return m_mapTestFixture.begin();
    }
    FixtureIterator end()
    {
        return m_mapTestFixture.end();
    }

    void InitTest(int argc, char** argv)
    {
        for (int i=1; i<argc; i++)
        {
            std::string strOpt = argv[i];
            size_t idx = strOpt.find('=');
            if (idx != std::string::npos)
            {
                std::string strPat = strOpt.substr(idx + 1);
                std::string runOpt = strOpt.substr(0, idx);
                runOpt = THEUTILCLASSNAME::ToLower(runOpt.c_str());
                if (runOpt != "--filter")
                {
                    m_nOpt = InvalidOpt;
                }
                else
                {
                    if (strPat.empty() || strPat == "-")
                    {
                        m_nOpt = InvalidOpt;
                    }
                    else
                    {
                        std::cout << "XXXXXXXX!!!PLEASE NOTE!!!XXXXXXXX ==> "
                            << runOpt << "=" << strPat << std::endl; 
                        m_nOpt = RunTestOpt;
                        while((idx = strPat.find(':')) != std::string::npos)
                        {
                            strOpt = strPat.substr(0, idx);
                            strPat = strPat.substr(idx + 1);
                            
                            Pattern pat;
                            pat.m_bNeg = false;
                            if (strOpt.at(0) == '-')
                            {
                                pat.m_bNeg = true;
                                strOpt = strOpt.substr(1);
                            }
                            if (!strOpt.empty())
                            {
                                pat.m_strPat = strOpt;
                                m_vecPat.push_back(pat);
                            } 
                        }
                        if (!strPat.empty())
                        {
                            Pattern pat;
                            pat.m_bNeg = false;
                            if (strPat.at(0) == '-')
                            {
                                pat.m_bNeg = true;
                                strPat = strPat.substr(1);
                            }
                            if (!strPat.empty())
                            {
                                pat.m_strPat = strPat;
                                m_vecPat.push_back(pat);
                            }
                            
                        }                       
                                             
                    }
                }
            }
            else
            {
                strOpt = THEUTILCLASSNAME::ToLower(strOpt.c_str());
                if (strOpt == "--help")
                {
                    m_nOpt = HelpOpt;
                }
                else if (strOpt == "--list_tests")
                {
                    m_nOpt = ListTestOpt;
                }
                else
                {
                    m_nOpt = InvalidOpt;
                }
                
            }
            
        }
        
    }
    int Run()
    {
        int nRet = 0;
        switch(m_nOpt)
        {
        case HelpOpt:
            Help();
            break;
        case ListTestOpt:
            ListTests();
            break;
        case RunTestOpt:
            nRet = RunAllTest();
            break;
        default:
            std::cout << "Invalid Option! Please reference the following help message\n\n";
            Help();
            break;
        }
        return nRet;
    }
    int RunAllTest()
    {
        ApplyFilter();


        int nCaseCount = GetCaseCount();
        int nFixtureCount = GetFixtureCount();
        clock_t topb = clock();
        std::cout << "[==========] Running " << nCaseCount << " tests from "<<
            nFixtureCount<<" test cases\n";

        if (m_pGlobalEnv)
        {
            std::cout << "[----------] Custom global test environment set-up\n";
            m_pGlobalEnv->SetUp(); 
        }

        for (FixtureIterator iter = m_mapTestFixture.begin();
            iter != m_mapTestFixture.end(); ++iter)
        {
            TestFixtrue* pTF = iter->second;
            int nTFCount =  pTF->GetCaseCount();
            std::string strTFName = pTF->GetName();
            std::cout << "[----------] "<< nTFCount << " tests from " << strTFName << "\n";
            clock_t fb = clock();
           
            pTF->SetUp();          
            for (TestFixtrue::TestCaseMapIterator iter = pTF->begin();
                iter != pTF->end(); ++iter)
            {
                Test* pTC = iter->second;
                std::string strFullName = GetTestCaseFullName(pTC);
                std::cout << "[ RUN      ] "<< strFullName <<"\n";
                
                clock_t tb = clock();
                pTC->SetUp();
                pTC->Run();
                pTC->TearDown();
                clock_t te = clock();               
                if (IsLastTestFail(pTC))
                {
                    std::cout << "[   FAILED ] "<< strFullName << " (" << te - tb << " ms)\n";
                }
                else
                {
                    std::cout << "[       OK ] "<< strFullName << " (" << te - tb << " ms)\n";
                }
                
                  
            }
            pTF->TearDown();
            
            clock_t fe = clock();
            std::cout << "[----------] " << nTFCount << " tests from " << strTFName << 
                " (" << fe - fb << " ms total)\n\n";

            
            
            
        }
        if (m_pGlobalEnv)
        {
            std::cout << "[----------] Custom global test environment tear-down\n";
            m_pGlobalEnv->TearDown(); 
        }
        clock_t tope = clock();
        std::cout << "[==========] " <<  nCaseCount <<" tests from " << 
            nFixtureCount << " test case ran (" << tope - topb << " ms total)\n";
        if (nCaseCount > 0)
        {
            int nFailed = m_vecFailed.size();
            std::cout << "[  PASSED  ] " << nCaseCount - nFailed << " tests\n";
            if (nFailed > 0)
            {
                std::cout << "[  FAILED  ] " << nFailed << " tests\n";
                for (std::vector<Test*>::iterator iter = m_vecFailed.begin();
                    iter != m_vecFailed.end(); ++iter)
                {
                    std::cout << "[  FAILED  ] " << GetTestCaseFullName(*iter) << "\n";
                }
                
            }
            
        }
        
        return m_vecFailed.size();
        
    }
    void ApplyFilter()
    {
        std::vector<Test*> vecRemove;
        std::vector<TestFixtrue*> vecFixRemove;
        for (FixtureIterator iter = m_mapTestFixture.begin();
            iter != m_mapTestFixture.end(); ++iter)
        {
            TestFixtrue* pTF = iter->second;
            int nCountRm = 0;
            for (TestFixtrue::TestCaseMapIterator iter = pTF->begin();
                iter != pTF->end(); ++iter)
            {
                Test* pTC = iter->second;
                if (!MatchFilter(pTC))
                {
                    vecRemove.push_back(pTC);
                    nCountRm ++;
                }               

            }
            if (pTF->GetCaseCount() == nCountRm)
            {
                vecFixRemove.push_back(pTF);
            }
            
        }
        for (std::vector<Test*>::iterator iter = vecRemove.begin();
            iter != vecRemove.end(); ++iter)
        {
            Test* pTC = (*iter);
            TestFixtrue* pTF = pTC->GetFixture();
            pTF->Remove(pTC);
            delete pTC;
        }
        for (std::vector<TestFixtrue*>::iterator iter = vecFixRemove.begin();
            iter != vecFixRemove.end(); ++iter)
        {
            TestFixtrue* pTF = *iter;
            FixtureIterator fiter = m_mapTestFixture.find(pTF->GetName());
            if (fiter != m_mapTestFixture.end())
            {
                m_mapTestFixture.erase(fiter);
                delete pTF;
            }
            
        }
        
    }
    bool MatchFilter(Test* pTC)
    {
        bool bRet = false;
        if (pTC)
        {
            if (m_vecPat.size() == 0)
            {
                bRet = true;
            }
            else
            {
                std::string strName = GetTestCaseFullName(pTC);

                for (PatternIterator iter = m_vecPat.begin();
                    iter != m_vecPat.end(); ++iter)
                {
                    bRet = THEUTILCLASSNAME::Match(strName.c_str(), iter->m_strPat.c_str());
                    if (iter->m_bNeg)
                    {
                        bRet = !bRet;
                    }
                    if (bRet)
                    {
                        break;
                    }

                }
            }
        }
        
        return bRet;
    }
    void CleanUp()
    {
        for (FixtureIterator iter = m_mapTestFixture.begin();
            iter != m_mapTestFixture.end(); ++iter)
        {
            delete iter->second;
        }
        m_mapTestFixture.clear();
        m_vecFailed.clear();
        if (m_pGlobalEnv)
        {
            delete m_pGlobalEnv;
            m_pGlobalEnv = 0;
        }        
    }
    void AddFailedTest(Test* pTC)
    {
        m_pLastFailedTC = pTC;
        if (std::find(m_vecFailed.begin(), m_vecFailed.end(), pTC) == m_vecFailed.end())
        {
            m_vecFailed.push_back(pTC);
        }
        
    }
    int GetFixtureCount() const
    {
        return m_mapTestFixture.size();
    }
    int GetCaseCount() const
    {
        int nCount = 0;
        for (std::map<std::string, TestFixtrue*>::const_iterator iter = m_mapTestFixture.begin();
            iter != m_mapTestFixture.end(); ++iter)
        {
            TestFixtrue* pTF = iter->second;
            nCount += pTF->GetCaseCount();
        }
        return nCount;
    }
    bool IsLastTestFail(Test* pTC)
    {
        bool bRet = false;
        if (pTC)
        {
            bRet = (pTC == m_pLastFailedTC);
        }
        return bRet;
    }
    std::string GetTestCaseFullName(Test* pTC) const
    {
        std::string strName = pTC->GetFixture()->GetName();
        strName += ".";
        strName += pTC->GetName();
        return strName;
    }

private:
    //static TestManager* sm_pMnger;
    FixtureMap m_mapTestFixture;
    Environment* m_pGlobalEnv;
    std::vector<Test*> m_vecFailed;

    int m_nOpt;
    PatternVec m_vecPat;

    Test* m_pLastFailedTC;
};
// TestManager* TestManager::sm_pMnger = 0;

/////////////////////////////////////////////////////////////////////////////////////////

#define TEST_F(TestFixtureName, TestCaseName)                                           \
    class TestFixtureName##_##TestCaseName##_Test : public TestFixtureName              \
    {                                                                                   \
    public:                                                                             \
        TestFixtureName##_##TestCaseName##_Test(std::string strName)                    \
        {                                                                               \
            TestFixtrue* pTF = TestManager::GetInst()->EnsureGet(#TestFixtureName);     \
            m_pTF = pTF;                                                                \
            m_strName = strName;                                                        \
            pTF->Add(this);                                                             \
            pTF->SetSetUp(&TestFixtureName::SetUpTestCase);                             \
            pTF->SetTearDown(&TestFixtureName::TearDownTestCase);                       \
        }                                                                               \
        virtual void Run();                                                             \
    };                                                                                  \
    TestFixtureName##_##TestCaseName##_Test*                                            \
        pDummy##TestFixtureName##_##TestCaseName##_Test  =                              \
                       new TestFixtureName##_##TestCaseName##_Test(#TestCaseName);      \
    void TestFixtureName##_##TestCaseName##_Test::Run()                                 \


#define TEST(TestFixtureName, TestCaseName)                                             \
    class TestFixtureName##_##TestCaseName##_Test : public Test                         \
    {                                                                                   \
    public:                                                                             \
        TestFixtureName##_##TestCaseName##_Test(std::string strName)                    \
        {                                                                               \
            TestFixtrue* pTF = TestManager::GetInst()->EnsureGet(#TestFixtureName);     \
            m_pTF = pTF;                                                                \
            m_strName = strName;                                                        \
            pTF->Add(this);                                                             \
        }                                                                               \
        virtual void Run();                                                             \
    };                                                                                  \
    TestFixtureName##_##TestCaseName##_Test*                                            \
        pDummy##TestFixtureName##_##TestCaseName##_Test  =                              \
                       new TestFixtureName##_##TestCaseName##_Test(#TestCaseName);      \
    void TestFixtureName##_##TestCaseName##_Test::Run()                                 \


#define SET_ENVIRONMENT(a) {                                                            \
    TestManager::GetInst()->SetGlobalEnvironment(a);                                    \
}   

#define INIT_TEST(a, b) {                                                               \
    TestManager::GetInst()->InitTest(a, b);                                             \
   }                                                                                    \


#define RUN_ALL_TEST()                                                                  \
    int XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX = \
    TestManager::GetInst()->Run();                                                      \
    TestManager::GetInst()->CleanUp();                                                  \
    return                                                                              \
        XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX;  \


#define EXPECT_EQ(a, b)   {                                                             \
    if(a != b) {                                                                        \
        std::cout << "[W]File: " << __FILE__ << " Line:" << __LINE__ <<                 \
            " EXPECT: " << a << " ACTUAL: "<< b << "\n";                                \
        TestManager::GetInst()->AddFailedTest(this);                                    \
    }                                                                                   \
}                                                                                       \

#define EXPECT_NEQ(a, b) {                                                              \
    if(a == b) {                                                                        \
        std::cout << "[W]File: " << __FILE__ << " Line:" << __LINE__ <<                 \
            " EXPECT: " << a << " != " << b <<                                          \
            " ACTUAL: "<< a << " == " << b << "\n";                                     \
        TestManager::GetInst()->AddFailedTest(this);                                    \
    }                                                                                   \
}                                                                                       \

#define EXPECT_STREQ(a, b) {                                                            \
    if(strncmp(a, b, strlen(a)) != 0) {                                                 \
        std::cout << "[W]File: " << __FILE__ << " Line:" << __LINE__ <<                 \
            " EXPECT: " << a << " ACTUAL: "<< b << "\n";                                \
        TestManager::GetInst()->AddFailedTest(this);                                    \
    }                                                                                   \
}                                                                                       \

#define EXPECT_STRNEQ(a, b) {                                                           \
    if(strncmp(a, b, strlen(a)) == 0) {                                                 \
        std::cout << "[W]File: " << __FILE__ << " Line:" << __LINE__ <<                 \
            " EXPECT: " << a << " != " << b <<                                          \
            " ACTUAL: "<< a << " == " << b << "\n";                                     \
        TestManager::GetInst()->AddFailedTest(this);                                    \
    }                                                                                   \
}                                                                                       \

#define EXPECT_BINEQ(a, b, s) {                                                         \
    if(memcmp(a, b, s) != 0) {                                                          \
        std::cout << "[E]File: " << __FILE__ << " Line:" << __LINE__ <<                 \
            " EXPECT: " << THEUTILCLASSNAME::HexDump(a, s) << " == " <<                 \
            THEUTILCLASSNAME::HexDump(b, s) <<                                          \
            " ACTUAL: "<< THEUTILCLASSNAME::HexDump(a, s) << " != " <<                  \
            THEUTILCLASSNAME::HexDump(b, s) << "\n";                                    \
        TestManager::GetInst()->AddFailedTest(this);                                    \
    }                                                                                   \
}                                                                                       \

#define EXPECT_BINNEQ(a, b, s) {                                                        \
    if(memcmp(a, b, s) == 0) {                                                          \
        std::cout << "[E]File: " << __FILE__ << " Line:" << __LINE__ <<                 \
            " EXPECT: " << THEUTILCLASSNAME::HexDump(a, s) << " != " <<                 \
            THEUTILCLASSNAME::HexDump(b, s) <<                                          \
            " ACTUAL: "<< THEUTILCLASSNAME::HexDump(a, s) << " == " <<                  \
            THEUTILCLASSNAME::HexDump(b, s) << "\n";                                    \
        TestManager::GetInst()->AddFailedTest(this);                                    \
    }                                                                                   \
}                                                                                       \

#define EXPECT_TRUE(a) {                                                                \
    if(!((int)a)){                                                                      \
        std::cout << "[W]File: " << __FILE__ << " Line:" << __LINE__ <<                 \
            " EXPECT: TRUE ACTUAL: FALSE\n";                                            \
        TestManager::GetInst()->AddFailedTest(this);                                    \
    }                                                                                   \
}                                                                                       \

#define EXPECT_FALSE(a) {                                                               \
    if(!((int)a)){                                                                      \
        std::cout << "[W]File: " << __FILE__ << " Line:" << __LINE__ <<                 \
            " EXPECT: FALSE ACTUAL: TRUE\n";                                            \
        TestManager::GetInst()->AddFailedTest(this);                                    \
    }                                                                                   \
}                                                                                       \

#define ASSERT_EQ(a, b) {                                                               \
    if(a != b) {                                                                        \
        std::cout << "[E]File: " << __FILE__ << " Line:" << __LINE__ <<                 \
            " EXPECT: " << a << " ACTUAL: "<< b << "\n";                                \
        TestManager::GetInst()->AddFailedTest(this);                                    \
        return;                                                                         \
    }                                                                                   \
}                                                                                       \

#define ASSERT_NEQ(a, b) {                                                              \
    if(a == b) {                                                                        \
        std::cout << "[E]File: " << __FILE__ << " Line:" << __LINE__ <<                 \
            " EXPECT: " << a << " != " << b <<                                          \
            " ACTUAL: "<< a << " == " << b << "\n";                                     \
        TestManager::GetInst()->AddFailedTest(this);                                    \
        return;                                                                         \
    }                                                                                   \
}                                                                                       \

#define ASSERT_STREQ(a, b) {                                                            \
    if(strncmp(a, b, strlen(a)) != 0) {                                                 \
        std::cout << "[E]File: " << __FILE__ << " Line:" << __LINE__ <<                 \
            " EXPECT: " << a << " ACTUAL: "<< b << "\n";                                \
        TestManager::GetInst()->AddFailedTest(this);                                    \
        return;                                                                         \
    }                                                                                   \
}                                                                                       \

#define ASSERT_STRNEQ(a, b) {                                                           \
    if(strncmp(a, b, strlen(a)) == 0) {                                                 \
        std::cout << "[E]File: " << __FILE__ << " Line:" << __LINE__ <<                 \
            " EXPECT: " << a << " != " << b <<                                          \
            " ACTUAL: "<< a << " == " << b << "\n";                                     \
        TestManager::GetInst()->AddFailedTest(this);                                    \
        return;                                                                         \
    }                                                                                   \
}                                                                                       \

#define ASSERT_BINEQ(a, b, s) {                                                         \
    if(memcmp(a, b, s) != 0) {                                                          \
        std::cout << "[E]File: " << __FILE__ << " Line:" << __LINE__ <<                 \
            " EXPECT: " << THEUTILCLASSNAME::HexDump(a, s) << " == " <<                 \
            THEUTILCLASSNAME::HexDump(b, s) <<                                          \
            " ACTUAL: "<< THEUTILCLASSNAME::HexDump(a, s) << " != " <<                  \
            THEUTILCLASSNAME::HexDump(b, s) << "\n";                                    \
        TestManager::GetInst()->AddFailedTest(this);                                    \
        return;                                                                         \
    }                                                                                   \
}                                                                                       \

#define ASSERT_BINNEQ(a, b, s) {                                                        \
    if(memcmp(a, b, s) == 0) {                                                          \
        std::cout << "[E]File: " << __FILE__ << " Line:" << __LINE__ <<                 \
            " EXPECT: " << THEUTILCLASSNAME::HexDump(a, s) << " != " <<                 \
            THEUTILCLASSNAME::HexDump(b, s) <<                                          \
            " ACTUAL: "<< THEUTILCLASSNAME::HexDump(a, s) << " == " <<                  \
            THEUTILCLASSNAME::HexDump(b, s) << "\n";                                    \
        TestManager::GetInst()->AddFailedTest(this);                                    \
        return;                                                                         \
    }                                                                                   \
}                                                                                       \

#define ASSERT_TRUE(a) {                                                                \
    if(!((int)a)){                                                                      \
        std::cout << "[E]File: " << __FILE__ << " Line:" << __LINE__ <<                 \
            " EXPECT: TRUE ACTUAL: FALSE\n";                                            \
        TestManager::GetInst()->AddFailedTest(this);                                    \
        return;                                                                         \
    }                                                                                   \
}                                                                                       \

#define ASSERT_FALSE(a) {                                                               \
    if((int)a){                                                                         \
        std::cout << "[E]File: " << __FILE__ << " Line:" << __LINE__ <<                 \
            " EXPECT: FALSE ACTUAL: TRUE\n";                                            \
        TestManager::GetInst()->AddFailedTest(this);                                    \
        return;                                                                         \
    }                                                                                   \
}                                                                                       \


#endif /* X_TEST_H_ */
