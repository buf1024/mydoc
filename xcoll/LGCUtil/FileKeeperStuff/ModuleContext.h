////////////////////////////////////////////////////////////////////////////////////////
// 
// Filekeeper Project
//  
//  
// FileName    : ModuleContext.h
// Purpose     : 
// Version     : 2010-12-30 (22:08) 1.0 Created
// Author      : heidong
// Contact     : buf1024@gmail.com
// Copyright(c): HEIDONG
////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Singleton.h"
#include <ShTypes.h>
#include <ObjIdl.h>
#include <ShlObj.h>
#include "stl.h"

#include "FileKeeperStuff.h"

using namespace lgc;

class ModuleContext : public Singleton<ModuleContext>
{
public:
	typedef void (FileKeeperStuff::* CommandHandler)();
	typedef map<UINT, Std_String>::iterator CmdIDStringIterator;	
	typedef map<UINT, CommandHandler>::iterator CmdHandlerIterator;

public:
	ModuleContext(void);
	~ModuleContext(void);

public:
	HMODULE STDMETHODCALLTYPE GetModuleHandle();
	VOID STDMETHODCALLTYPE SetModuleHandle(HMODULE hModule);
	ULONG STDMETHODCALLTYPE GetLockNum();
	ULONG STDMETHODCALLTYPE GetUsageNum();

	ULONG STDMETHODCALLTYPE AddRefLockNum();
	ULONG STDMETHODCALLTYPE ReleaseLockNum();

	ULONG STDMETHODCALLTYPE AddRefUsageNum();
	ULONG STDMETHODCALLTYPE ReleaseUsageNum();


	PIDLIST_ABSOLUTE GetPIDLFolder();
    Std_String GetContextPath() const;
	void SetPIDLFolder(PIDLIST_ABSOLUTE pidlFoder);

	IDataObject* GetDataObject();
	void SetDataObject(IDataObject* pObj);

	HKEY GetFolderHEY();
	void SetFolderHEY(HKEY hKey);

	bool AddCommandHandler(UINT nCmdID, CommandHandler pfnHandler);
	bool RemoveCommandHandler(UINT nCmdID);
	ModuleContext::CommandHandler GetCommandHandler(UINT nCmdID);
	UINT GetCommandHandlerSize() const;

	bool AddCommandString(UINT nCmdID, Std_String strVal);
	bool RemoveCommandString(UINT nCmdID);
	Std_String GetCommandString(UINT nCmdID);
	UINT GetCommandStringSize() const;


    int GetSelectedFiles(list<Std_String>& rgpFiles);

private:

    Std_String m_strPath;

	HMODULE m_hModule;
	LONG m_nLockNum;
	LONG m_nUseNum;

	//Context Initialize Data
	PIDLIST_ABSOLUTE m_pPidlFolder;
	IDataObject *m_pDtObj;
	HKEY m_hkeyProgID;

	//Menu Context
	map<UINT, Std_String> m_mapCmdString;
	map<UINT, CommandHandler> m_mapCommands;
};
