////////////////////////////////////////////////////////////////////////////////////////
// 
// Filekeeper Project
//  
//  
// FileName    : FileKeeperStuff.h
// Purpose     : Shell extension
// Version     : 2010-12-30 (22:08) 1.0 Created
// Author      : heidong
// Contact     : buf1024@gmail.com
// Copyright(c): HEIDONG
////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <shlobj.h>
#include <shobjidl.h>
#include "stl.h"

class FileKeeperStuff :
	public IShellExtInit, public IContextMenu
{
public:
	FileKeeperStuff(void);
	~FileKeeperStuff(void);

public:
	//IShellExtInit
	virtual HRESULT STDMETHODCALLTYPE Initialize( 
		/* [unique][in] */ 
		PCIDLIST_ABSOLUTE pidlFolder,
		/* [unique][in] */ 
		IDataObject *pdtobj,
		/* [unique][in] */ 
		HKEY hkeyProgID);

	//IContextMenu
	virtual HRESULT STDMETHODCALLTYPE QueryContextMenu( 
		/* [in] */ 
		HMENU hmenu,
		/* [in] */ 
		UINT indexMenu,
		/* [in] */ 
		UINT idCmdFirst,
		/* [in] */ 
		UINT idCmdLast,
		/* [in] */ 
		UINT uFlags);

	virtual HRESULT STDMETHODCALLTYPE InvokeCommand( 
		/* [in] */ 
		CMINVOKECOMMANDINFO *pici);

	virtual HRESULT STDMETHODCALLTYPE GetCommandString( 
		/* [in] */ 
		UINT_PTR idCmd,
		/* [in] */ 
		UINT uType,
		/* [in] */ 
		UINT *pReserved,
		/* [out] */ 
		LPSTR pszName,
		/* [in] */ 
		UINT cchMax);

	//IUnkown
	virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
		/* [in] */
		REFIID riid,
		/* [iid_is][out] */
		void **ppvObject);

	virtual ULONG STDMETHODCALLTYPE AddRef( void);

	virtual ULONG STDMETHODCALLTYPE Release( void);


public:
	//Menu Handler
	void OnAbout();
	void OnSetting();


private:
    void StartCmd(Std_String strPath);
private:
	LONG m_nRef;
};
