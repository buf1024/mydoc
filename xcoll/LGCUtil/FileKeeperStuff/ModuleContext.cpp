////////////////////////////////////////////////////////////////////////////////////////
// 
// Filekeeper Project
//  
//  
// FileName    : ModuleContext.cpp
// Purpose     : 
// Version     : 2010-12-30 (22:11) 1.0 Created
// Author      : heidong
// Contact     : buf1024@gmail.com
// Copyright(c): HEIDONG
////////////////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "ModuleContext.h"
#include <shellapi.h>

ModuleContext::ModuleContext(void)
: m_hModule(NULL)
, m_nLockNum(0L)
, m_nUseNum(0L)
, m_pPidlFolder(NULL)
, m_pDtObj(NULL)
, m_hkeyProgID(NULL)
{
	
}

ModuleContext::~ModuleContext(void)
{
	if (m_pPidlFolder)
	{
		ILFree((LPITEMIDLIST)m_pPidlFolder);
	}
	if (m_pDtObj)
	{
		m_pDtObj->Release();
	}
	if (m_hkeyProgID)
	{
		RegCloseKey(m_hkeyProgID);
	}
			
}

HMODULE ModuleContext::GetModuleHandle()
{
	_ASSERT(m_hModule != NULL);
	return m_hModule;
}
VOID ModuleContext::SetModuleHandle(HMODULE hModule)
{
	m_hModule = hModule;
}
ULONG ModuleContext::GetLockNum()
{
	return m_nLockNum;
}
ULONG ModuleContext::GetUsageNum()
{
	return m_nUseNum;
}

ULONG ModuleContext::AddRefLockNum()
{
	//m_nLockNum++;
    InterlockedIncrement(&m_nLockNum);
	return m_nLockNum;
}
ULONG ModuleContext::ReleaseLockNum()
{
	if (m_nLockNum ==0)
	{
		return m_nLockNum;
	}
	//m_nLockNum--;
    InterlockedDecrement(&m_nLockNum);
	return m_nLockNum;
}

ULONG ModuleContext::AddRefUsageNum()
{
	//m_nUseNum++;
    InterlockedIncrement(&m_nUseNum);
	return m_nUseNum;
}
ULONG ModuleContext::ReleaseUsageNum()
{
	if (m_nUseNum == 0)
	{
		return m_nUseNum;
	}
	//m_nUseNum--;
    InterlockedDecrement(&m_nUseNum);
	return m_nUseNum;
}

PIDLIST_ABSOLUTE ModuleContext::GetPIDLFolder()
{
	return m_pPidlFolder;
}
Std_String ModuleContext::GetContextPath() const
{
    return m_strPath;
}
void ModuleContext::SetPIDLFolder(PIDLIST_ABSOLUTE pidlFoder)
{
	m_pPidlFolder = pidlFoder;

    TCHAR szPath[MAX_PATH] = {0};
    if (m_pPidlFolder != NULL)
    {
        
        SHGetPathFromIDList(pidlFoder, szPath);        
    }
    m_strPath = szPath;
}

IDataObject* ModuleContext::GetDataObject()
{
	return m_pDtObj;
}
void ModuleContext::SetDataObject(IDataObject* pObj)
{
	m_pDtObj = pObj;
}

HKEY ModuleContext::GetFolderHEY()
{
	return m_hkeyProgID;
}
void ModuleContext::SetFolderHEY(HKEY hKey)
{
	m_hkeyProgID = hKey;
}

bool ModuleContext::AddCommandHandler(UINT nCmdID, CommandHandler pfnHandler)
{
	bool bRet = false;
	_ASSERT(nCmdID >= 0 && pfnHandler != NULL);
	CmdHandlerIterator iter = m_mapCommands.find(nCmdID);
	if (iter == m_mapCommands.end())
	{
		m_mapCommands.insert(make_pair(nCmdID, pfnHandler));
		bRet = true;
	}
	return bRet;
}
bool ModuleContext::RemoveCommandHandler(UINT nCmdID)
{
	CmdHandlerIterator iter = m_mapCommands.find(nCmdID);
	if (iter != m_mapCommands.end())
	{
		m_mapCommands.erase(iter);
	}
	return true;
}
ModuleContext::CommandHandler ModuleContext::GetCommandHandler(UINT nCmdID)
{
	CommandHandler pfnHandler = NULL;
	CmdHandlerIterator iter = m_mapCommands.find(nCmdID);
	if (iter != m_mapCommands.end())
	{
		pfnHandler = m_mapCommands[nCmdID];
	}
	return pfnHandler;
}

UINT ModuleContext::GetCommandHandlerSize() const
{
	return m_mapCommands.size();
}

bool ModuleContext::AddCommandString(UINT nCmdID, Std_String strVal)
{
	bool bRet = false;
	_ASSERT(nCmdID >= 0 && strVal != _T(""));
	CmdIDStringIterator iter = m_mapCmdString.find(nCmdID);
	if (iter == m_mapCmdString.end())
	{
		m_mapCmdString.insert(make_pair(nCmdID, strVal));
		bRet = true;
	}
	return bRet;
}
bool ModuleContext::RemoveCommandString(UINT nCmdID)
{
	CmdIDStringIterator iter = m_mapCmdString.find(nCmdID);
	if (iter != m_mapCmdString.end())
	{
		m_mapCmdString.erase(iter);
	}
	return true;
}
Std_String ModuleContext::GetCommandString(UINT nCmdID)
{
	Std_String strRet;
	CmdIDStringIterator iter = m_mapCmdString.find(nCmdID);
	if (iter != m_mapCmdString.end())
	{
		strRet = m_mapCmdString[nCmdID];
	}
	return strRet;
}

UINT ModuleContext::GetCommandStringSize() const
{
	return m_mapCmdString.size();
}

int ModuleContext::GetSelectedFiles(list<Std_String>& rgpFiles)
{
    rgpFiles.clear();

    if(!m_pDtObj) return -1;

    STGMEDIUM medium;
    FORMATETC fmte = {CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};
    HRESULT hres = m_pDtObj->GetData(&fmte, &medium);
    if(FAILED(hres))
        return -1;
    
    int numFiles = DragQueryFile((HDROP)medium.hGlobal, (UINT)-1, NULL, 0);
    TCHAR szPath[MAX_PATH] = _T("");
    for(int i = 0; i < numFiles; i++)
    {
        DragQueryFile((HDROP)medium.hGlobal, i, szPath, sizeof(szPath));
        rgpFiles.push_back(szPath);
    }
    ::ReleaseStgMedium(&medium);

    return rgpFiles.size();
}