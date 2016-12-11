/*
	@author: Zhou Bowei
	@date: 2016/9/29
*/

// Polygon.h : main header file for the Polygon application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols



// CPolygonApp:
// See Polygon.cpp for the implementation of this class
//

class CPolygonApp : public CWinAppEx
{
public:
	CPolygonApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CPolygonApp theApp;
