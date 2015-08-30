////////////////////////////////////////////////////////////////////////////////////////
// 
// Filekeeper Project
//  
//  
// FileName    : FileKeeperExport.cpp
// Purpose     : 
// Version     : 2010-12-30 (22:11) 1.0 Created
// Author      : heidong
// Contact     : buf1024@gmail.com
// Copyright(c): HEIDONG
////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "clsid.h"
#include "ComRegSrv.h"
#include "RegShExtension.h"
#include "ModuleContext.h"
#include "FileKeeperFactory.h"

using namespace lgc;


STDAPI DllGetClassObject(
						 REFCLSID rclsid,
						 REFIID riid,
						 LPVOID * ppv
						 )
{

	if (rclsid == CLSID_FileKeeperStuff)
	{		
		FileKeeperFactory* pFactory = new FileKeeperFactory;
		return pFactory->QueryInterface(riid, ppv);
	}
	return CLASS_E_CLASSNOTAVAILABLE;
	
}

STDAPI DllCanUnloadNow(void)
{
	ModuleContext* pCtx = ModuleContext::GetInstPtr();
	if(pCtx->GetLockNum() == 0 && pCtx->GetUsageNum() == 0) return S_OK;
	return S_FALSE;
}

STDAPI DllRegisterServer(void)
{
	HRESULT hRet = S_FALSE;
	TCHAR szFileName[MAX_PATH] = _T("");
	ModuleContext* pCtx = ModuleContext::GetInstPtr();
	if(GetModuleFileName(pCtx->GetModuleHandle(), szFileName, MAX_PATH))
	{
		ComRegSrv* pCom = ComRegSrv::GetInstPtr();
		RegShExtension* pEx = RegShExtension::GetInstPtr();

		hRet = pCom->RegisterServer(CLSID_FileKeeperStuff, szFileName, _T("Apartment"),
			_T("LGCUtility.ProgID"), _T("LGCUtility Shell Extension"));
		if (hRet == S_OK)
		{
			hRet = pEx->RegCtxMenuHandler(_T("LGCUtility"), CLSID_FileKeeperStuff);
		}

		RegShExtension::ReleaseRC();
		ComRegSrv::ReleaseRC();
	}
	return hRet;
}

STDAPI DllUnregisterServer(void)
{
	HRESULT hRet = S_FALSE;

	ComRegSrv* pCom = ComRegSrv::GetInstPtr();
	RegShExtension* pEx = RegShExtension::GetInstPtr();

	hRet = pCom->UnregisterServer(CLSID_FileKeeperStuff, _T("LGCUtility.ProgID"));
	if (hRet == S_OK)
	{
		hRet = pEx->UnRegCtxMenuHandler(_T("LGCUtility"));
	}

	RegShExtension::ReleaseRC();
	ComRegSrv::ReleaseRC();

	return hRet;
}

