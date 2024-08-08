
// WFONTX64.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// メイン シンボル


// CWFONTX64App:
// このクラスの実装については、WFONTX64.cpp を参照してください
//

class CWFONTX64App : public CWinApp
{
public:
	CWFONTX64App();
	~CWFONTX64App();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CWFONTX64App theApp;
