////////////////////////////////////////////////////////////////////////////////////////
// 
// Filekeeper Project
//  
//  
// FileName    : FileKeeperFactory.h
// Purpose     : Com factory
// Version     : 2010-12-30 (22:03) 1.0 Created
// Author      : heidong
// Contact     : buf1024@gmail.com
// Copyright(c): HEIDONG
////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "unknwn.h"

class FileKeeperFactory :
	public IClassFactory
{
public:
	FileKeeperFactory(void);
	~FileKeeperFactory(void);

	
public:
	//IClassFactory
	virtual /* [local] */ HRESULT STDMETHODCALLTYPE CreateInstance( 
		/* [unique][in] */ IUnknown *pUnkOuter,
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ void **ppvObject);

	virtual /* [local] */ HRESULT STDMETHODCALLTYPE LockServer( 
		/* [in] */ BOOL fLock);

	//IUnkown
	virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
		/* [in] */
		REFIID riid,
		/* [iid_is][out] */
		void **ppvObject);

	virtual ULONG STDMETHODCALLTYPE AddRef( void);

	virtual ULONG STDMETHODCALLTYPE Release( void);

private:
	LONG m_nRef;
};
