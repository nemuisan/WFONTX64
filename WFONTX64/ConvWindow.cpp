///////////////////////////////////////////////////////////////
//オリジナル作者
//	wfontx.cpp
//	Windows font to FontX font program
//
//	Copyright (C) NERD Corp. 1992
//		by Yoshiki Oka
//  http://www.vector.co.jp/soft/dos/writing/se002881.html
//
///////////////////////////////////////////////////////////////
//ｖｃ6へ移植しました(WindowsXP SP3にて動作確認)
//
// 趣味の電子工作ｂｙすん
// http://ekousaku.web.fc2.com/
// 2010.1.13
///////////////////////////////////////////////////////////////
// Migrated Visual Studio 2017(Confirmed by Windows10 x64)
//
// ねむいさんのぶろぐ
// http://nemuisan.blog.bai.ne.jp
// 2023.08.30
///////////////////////////////////////////////////////////////

// ConvWindow.cpp : インプリメンテーション ファイル
//

#include "pch.h"
#include "WFONTX64.h"
#include "ConvWindow.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CConvWindow, CDialog)

UINT_PTR gIdTimer = 0;
#define TIMER_MS 10

/////////////////////////////////////////////////////////////////////////////
// CConvWindow ダイアログ
CConvWindow::CConvWindow(int f, int t, LPSTR fp, LPSTR xp, LPLOGFONT lp, CWnd *pP)
	: CModalDialog(IDD_CONVERT_DIALOG, pP)
{
	nonstopflag = f;
	type = t;
	lfp = lp;
	filename = fp;
	fontxname = xp;
	paintflag = FALSE;
}

void CConvWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CConvWindow, CDialog)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDD_STOP, OnStop)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConvWindow メッセージ ハンドラ

BOOL CConvWindow::OnInitDialog()
{
	CDialog::OnInitDialog();


	if (lfp->lfWidth == 0) {
		MessageBox("Set Width !", "Parameter Error", MB_OK | MB_ICONASTERISK);
		PostMessage(WM_CLOSE, 0, 0);
		return(FALSE);
	}

	if (lfp->lfHeight == 0) {
		MessageBox("Set Height !", "Parameter Error", MB_OK | MB_ICONASTERISK);
		PostMessage(WM_CLOSE, 0, 0);
		return(FALSE);
	}

	if (cfile.Open(filename, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary) == FALSE) {
		char buf[128];

		if(filename.GetLength()==0)
		{
			sprintf_s(buf, sizeof(buf), "Set Save Name!");
			MessageBox(buf, "File Error", MB_OK | MB_ICONASTERISK);
		}
		else 
		{
			sprintf_s(buf, sizeof(buf), "Cannot open %s.", (const char *)filename);
			MessageBox(buf, "File Error", MB_OK | MB_ICONASTERISK);
		}
		PostMessage(WM_CLOSE, 0, 0);
		return(FALSE);
	}

	if (gIdTimer != 0) {
		KillTimer(gIdTimer);
		gIdTimer = 0;
	}

	if (!(gIdTimer = SetTimer(1, TIMER_MS, NULL))) {
		MessageBox("Too many timer.", "Timer Error", MB_OK | MB_ICONASTERISK);
		cfile.Close();
		PostMessage(WM_CLOSE, 0, 0);
		return(FALSE);
	}

	cfile.Write("FONTX2", 6);
	cfile.Write(fontxname, 8);
	if (type) {	// KANJI
		unsigned char c;
		c = (unsigned char)lfp->lfWidth * 2;
		cfile.Write(&c, 1);
		c = (unsigned char)lfp->lfHeight;
		cfile.Write(&c, 1);
		c = 1;	// Kanji
		cfile.Write(&c, 1);

		c = ((0x9f - 0x81 + 1) + (0xef - 0xe0 + 1)) * 2;	//	81-9f,e0-ef
		cfile.Write(&c, 1);
		for (unsigned int u = 0x8100; u <= 0xef00; u += 0x0100) {
			if (u == 0xa000)	u = 0xe000;
			unsigned int cd = u + 0x0040;
			cfile.Write(&cd, 2);
			cd = u + 0x007e;
			cfile.Write(&cd, 2);
			cd = u + 0x0080;
			cfile.Write(&cd, 2);
			cd = u + 0x00fc;
			cfile.Write(&cd, 2);
		}

		upk = 0x81;
		lwk = 0x40;
	}
	else {
		unsigned char c;
		c = (unsigned char)lfp->lfWidth;
		cfile.Write(&c, 1);
		c = (unsigned char)lfp->lfHeight;
		cfile.Write(&c, 1);
		c = 0;	// ANK
		cfile.Write(&c, 1);

		upk = lwk = 0;
	}

	cfont.CreateFontIndirect(lfp);

	RECT	r;
	GetWindowRect(&r);
	int	w = lfp->lfWidth * 10 > 100 ? lfp->lfWidth * 10 : 100;
	int	h = lfp->lfHeight * 5 > 100 ? lfp->lfHeight * 5 : 100;
	r.right = r.left + w;
	r.bottom = r.top + h + GetSystemMetrics(SM_CYCAPTION) * 2;
	MoveWindow(&r);

	if (!nonstopflag) {
		GetClientRect(&r);
		RECT	rr;
		rr.top = lfp->lfHeight * 3;
		rr.left = (r.right - r.left - 80) / 2;
		rr.right = rr.left + 80;
		rr.bottom = rr.top + GetSystemMetrics(SM_CYCAPTION);
		/*
				CButton *cbp = new CButton();
				cbp->Create("&Stop", BS_PUSHBUTTON, rr, this, IDCANCEL);
				cbp->ShowWindow(SW_SHOWNOACTIVATE);
				cbp->UpdateWindow();
		*/
		GetDlgItem(IDD_STOP)->SetWindowPos(0, rr.left, rr.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

		int x = rr.left;
		int y = rr.top + 30;
		GetDlgItem(IDC_STATIC_NUMBER)->SetWindowPos(0, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	}

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
				  // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CConvWindow::OnPaint()
{
	CPaintDC dc(this); // 描画用のデバイス コンテキスト

	paintflag = TRUE;

	// 描画用メッセージとして CDialog::OnPaint() を呼び出してはいけません
}

void CConvWindow::OnTimer(UINT_PTR nIDEvent)
{
	if (paintflag == FALSE)	return;

	if (gIdTimer != 0) {
		KillTimer(gIdTimer);
		gIdTimer = 0;
	}

	int	slen, xbytes, allbytes /*, xeven*/;
	if (type) {
		slen = 2;
	}
	else {
		slen = 1;
	}

	xbytes = lfp->lfWidth * slen;
	xbytes = (xbytes - 1) / 8 + 1;

	CDC	*cdcp = GetDC();
	CDC	cdc;
	cdc.CreateCompatibleDC(cdcp);
	CBitmap cbm;
	cbm.CreateCompatibleBitmap(&cdc, lfp->lfWidth*slen, lfp->lfHeight);
	CBitmap *pOld = cdc.SelectObject(&cbm);
	BITMAP	bm;
	cbm.GetObject(sizeof(BITMAP), &bm);

	allbytes = bm.bmWidthBytes * bm.bmHeight;
	unsigned char	*bitmapp = new unsigned char[allbytes];

	cdcp->SelectObject(&cfont);
	cdc.SelectObject(&cfont);

	cdcp->SetBkColor(RGB(0, 0, 0));
	cdc.SetBkColor(RGB(0, 0, 0));

	CRect	cr(0, 0, lfp->lfWidth*slen, lfp->lfHeight);
	CBrush	cb(RGB(0, 0, 0));	// Black Background

	char str[10];
	sprintf_s(str, sizeof(str),"%02x%02x", upk, lwk);
	SetDlgItemText(IDC_STATIC_NUMBER, str);

top:

	char buf[128];
	if (type) {
		buf[0] = upk;
		buf[1] = lwk;
		buf[2] = 0;
	}
	else {
		buf[0] = lwk;
		buf[1] = 0;
	}

	cdcp->FillRect((LPRECT)cr, &cb);
	cdc.FillRect((LPRECT)cr, &cb);
	cdcp->SetTextColor(RGB(0xff, 0xff, 0xff));	// White Text
	cdc.SetTextColor(RGB(0xff, 0xff, 0xff));	// White Text
	CSize	cs = cdc.GetTextExtent(buf, slen);
	int	sx = (lfp->lfWidth*slen - cs.cx) / 2;
	int sy = (lfp->lfHeight - cs.cy) / 2;
	cdcp->TextOut(sx, sy, buf, slen);
	cdc.TextOut(sx, sy, buf, slen);

	cbm.GetBitmapBits(allbytes, bitmapp);
	for (int y = 0; y < lfp->lfHeight; y++) {
		cfile.Write(&bitmapp[y*bm.bmWidthBytes], xbytes);
	}

	if (type) {
		lwk++;
		if (lwk == 0x7f)		lwk = 0x80;
		else if (lwk > 0xfc) {
			lwk = 0x40;
			upk++;
			if (upk == 0xa0)	upk = 0xe0;
			else if (upk > 0xef) {	// Over JIS. SHIFT-JIS is continued.
				OnClose();
				nonstopflag = 0;
			}
		}
	}
	else {
		lwk++;
		if (lwk >= 0x100) {
			OnClose();
			nonstopflag = 0;
		}
	}

	if (nonstopflag) {
		goto	top;
	}

	delete[] bitmapp;
	cdc.SelectObject(pOld);
	cbm.DeleteObject();
	cdc.DeleteDC();
	ReleaseDC(cdcp);

	gIdTimer = SetTimer(1, TIMER_MS, NULL);

	CDialog::OnTimer(nIDEvent);
}

void CConvWindow::OnClose()
{
	if (gIdTimer != 0) {
		KillTimer(gIdTimer);
		gIdTimer = 0;
	}
	if (cfile.m_hFile != CFile::hFileNull) {
		cfile.Close();
	}
	EndDialog(0);

	CDialog::OnClose();
}

void CConvWindow::OnStop()
{
	PostMessage(WM_CLOSE, 0, 0);
}
