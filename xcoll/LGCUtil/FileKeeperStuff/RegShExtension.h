////////////////////////////////////////////////////////////////////////////////////////
// 
// Filekeeper Project
//  
//  
// FileName    : RegShExtension.h
// Purpose     : 
// Version     : 2010-12-30 (22:09) 1.0 Created
// Author      : heidong
// Contact     : buf1024@gmail.com
// Copyright(c): HEIDONG
////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "stl.h"
#include <Guiddef.h>
#include "Singleton.h"

namespace lgc
{
	class RegShExtension : public Singleton<RegShExtension>
	{
	public:
		//ContexMenu Handler
		HRESULT STDMETHODCALLTYPE RegCtxMenuHandler(Std_String strProgName, const CLSID& clsid);
		HRESULT STDMETHODCALLTYPE UnRegCtxMenuHandler(Std_String strProgName);

	};

}


