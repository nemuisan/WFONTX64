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

// WFONTX64Dlg.cpp : 実装ファイル
//

#include "pch.h"
#include "framework.h"
#include "WFONTX64.h"
#include "WFONTX64Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern CWFONTX64Dlg* pdlg;

BOOL FontFunc(LPLOGFONT lplf, LPNEWTEXTMETRIC lptm, DWORD nft, LPVOID lps);
BOOL FontNameFunc(LPLOGFONT lplf, LPNEWTEXTMETRIC lptm, DWORD nft, LPVOID lps);

// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CWFONTX64Dlg ダイアログ



CWFONTX64Dlg::CWFONTX64Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WFONTX64_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWFONTX64Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWFONTX64Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDD_EXIT, &CWFONTX64Dlg::OnBnClickedExit)
	ON_BN_CLICKED(IDD_ANK, &CWFONTX64Dlg::OnBnClickedAnk)
	ON_BN_CLICKED(IDD_KNJ, &CWFONTX64Dlg::OnBnClickedKnj)
	ON_BN_CLICKED(IDD_STRIKEOUT, &CWFONTX64Dlg::OnBnClickedStrikeout)
	ON_BN_CLICKED(IDD_UNDERLINE, &CWFONTX64Dlg::OnBnClickedUnderline)
	ON_BN_CLICKED(IDD_BOLD, &CWFONTX64Dlg::OnBnClickedBold)
	ON_BN_CLICKED(IDD_ITALIC, &CWFONTX64Dlg::OnBnClickedItalic)
	ON_BN_CLICKED(IDD_RUN, &CWFONTX64Dlg::OnBnClickedRun)
	ON_BN_CLICKED(IDD_NONSTOP, &CWFONTX64Dlg::OnBnClickedNonstop)
	ON_EN_CHANGE(IDD_HEIGHT, &CWFONTX64Dlg::OnEnChangeHeight)
	ON_EN_CHANGE(IDD_WIDTH, &CWFONTX64Dlg::OnEnChangeWidth)
	ON_EN_CHANGE(IDD_FONTXNAME, &CWFONTX64Dlg::OnEnChangeFontxname)
	ON_EN_CHANGE(IDD_FILENAME, &CWFONTX64Dlg::OnEnChangeFilename)
	ON_LBN_SELCHANGE(IDD_FONTLIST, &CWFONTX64Dlg::OnLbnSelchangeFontlist)
END_MESSAGE_MAP()


// CWFONTX64Dlg メッセージ ハンドラー

BOOL CWFONTX64Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// "バージョン情報..." メニューをシステム メニューに追加します。

	// IDM_ABOUTBOX は、システム コマンドの範囲内になければなりません。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。
	memset(&lf, 0, sizeof(LOGFONT));
	ccwp = NULL;
	fonttype = 0;

	ap = new CPtrArray;

	memset(&lf, 0, sizeof(lf));
	lf.lfCharSet = DEFAULT_CHARSET;

	CDC	*cdcp = GetDC();
	EnumFontFamiliesEx(cdcp->GetSafeHdc(), &lf, (FONTENUMPROC)FontNameFunc, NULL, 0);
	ReleaseDC(cdcp);

	CListBox *clbp = (CListBox*)GetDlgItem(IDD_FONTLIST);
	for (int i = 0; i < ap->GetSize(); i++) {
		clbp->AddString((LPCTSTR)(*ap)[i]);
		delete	(*ap)[i];
	}
	delete	ap;
	ap = NULL;

	CheckRadioButton(IDD_ANK, IDD_KNJ, IDD_ANK);

	CEdit *cep = (CEdit *)GetDlgItem(IDD_FONTXNAME);
	cep->LimitText(8);

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

void CWFONTX64Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CWFONTX64Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CWFONTX64Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CWFONTX64Dlg::OnClose()
{
	if (ccwp != NULL) {
		delete ccwp;
		ccwp = NULL;
	}

	PostQuitMessage(0);
	EndDialog(IDOK);	// EndDialogで"OK"として終了する.
	CDialog::OnClose();
}


void CWFONTX64Dlg::OnBnClickedExit()
{
	OnClose();
}

void CWFONTX64Dlg::Convert(int f)
{
	char buf[80], buf2[9], fontxname[9];
	GetDlgItemText(IDD_FILENAME, buf, 80);
	GetDlgItemText(IDD_FONTXNAME, buf2, 9);
	sprintf_s(fontxname,sizeof(fontxname),"%-8s", buf2);
	int	type = GetCheckedRadioButton(IDD_ANK, IDD_KNJ) - IDD_ANK;
	if (ccwp != NULL) delete ccwp;
	ccwp = new CConvWindow(f, type, buf, fontxname, &lf, this);
	ccwp->DoModal();
	delete	ccwp;
	ccwp = NULL;
}

const char *GetCharSet(BYTE cs)
{
	static char *charsetstr[] = {
		"ANSI", "Default", "Symbol", "ShiftJIS", "Hangeul", "ChineseBig5","OEM",
		"Unknown"
	};
	if (cs == ANSI_CHARSET)			return(charsetstr[0]);
	if (cs == DEFAULT_CHARSET)		return(charsetstr[1]);
	if (cs == SYMBOL_CHARSET)		return(charsetstr[2]);
	if (cs == SHIFTJIS_CHARSET)		return(charsetstr[3]);
	if (cs == HANGEUL_CHARSET)		return(charsetstr[4]);
	if (cs == CHINESEBIG5_CHARSET)	return(charsetstr[5]);
	if (cs == OEM_CHARSET)			return(charsetstr[6]);
	return(charsetstr[7]);
}

int CWFONTX64Dlg::ChangeFont()
{
	static char *pitchstr[] = { "Default","Fixed", "Variable","" };
	static char *familystr[] = {
		"Dontcare", "Roman", "Swiss", "Modern", "Script", "Decorative", "", ""
	};

	CListBox *clbp = (CListBox*)GetDlgItem(IDD_FONTLIST);
	clbp->GetText(clbp->GetCurSel(), lf.lfFaceName);
	if (!lf.lfFaceName[0]) return(0);

	lf.lfCharSet = DEFAULT_CHARSET;

	CDC	*cdcp = GetDC();
	EnumFontFamiliesEx(cdcp->GetSafeHdc(), &lf, (FONTENUMPROC)FontFunc, NULL, 0);
	ReleaseDC(cdcp);

	lf.lfWidth = GetDlgItemInt(IDD_WIDTH);
	lf.lfHeight = GetDlgItemInt(IDD_HEIGHT);

	char buf[256];
	sprintf_s(buf,
		sizeof(buf),
		"%s\n"
		"W:%d, O:%d, Q:%d\n"
		"P:%s, F:%s\n"
		"%c%c%c%c",
		GetCharSet(lf.lfCharSet),
		lf.lfWeight, lf.lfOutPrecision, lf.lfQuality,
		pitchstr[lf.lfPitchAndFamily & 3], familystr[(lf.lfPitchAndFamily) >> 4],
		fonttype & 0x08 ? 'W' : ' ',	// WIFE
		fonttype & 0x04 ? 'T' : ' ',	// TrueType
		fonttype & RASTER_FONTTYPE ? 'R' : 'V',
		fonttype & DEVICE_FONTTYPE ? 'D' : 'G'
	);
	SetDlgItemText(IDD_FONTTYPE, buf);

	LOGFONT	llf = lf;

	if (!llf.lfWidth || !llf.lfHeight) return(0);

	llf.lfEscapement = llf.lfOrientation = 0;
	CButton *cbp;
	cbp = (CButton *)GetDlgItem(IDD_STRIKEOUT);
	llf.lfStrikeOut = cbp->GetCheck() ? 1 : 0;
	cbp = (CButton *)GetDlgItem(IDD_UNDERLINE);
	llf.lfUnderline = cbp->GetCheck() ? 1 : 0;
	cbp = (CButton *)GetDlgItem(IDD_ITALIC);
	llf.lfItalic = cbp->GetCheck() ? 1 : 0;
	cbp = (CButton *)GetDlgItem(IDD_BOLD);
	llf.lfWeight = cbp->GetCheck() ? FW_BOLD : 0;
	cf.DeleteObject();
	cf.CreateFontIndirect(&llf);
	CStatic *csp = (CStatic *)GetDlgItem(IDD_STRING);
	csp->SetFont(&cf, TRUE);
	return(1);
}

void CWFONTX64Dlg::ChangeButton()
{
	if (ChangeFont()) {
		char buf[256];
		GetDlgItemText(IDD_FILENAME, buf, 255);
		if (buf[0]) {
			GetDlgItemText(IDD_FONTXNAME, buf, 255);
			if (buf[0]) {
				CButton *cbp;
				cbp = (CButton*)GetDlgItem(IDD_RUN);
				cbp->EnableWindow(TRUE);
				cbp = (CButton*)GetDlgItem(IDD_NONSTOP);
				cbp->EnableWindow(TRUE);
			}
		}
	}
}




BOOL FontFunc(LPLOGFONT lplf, LPNEWTEXTMETRIC lptm, DWORD nft, LPVOID lps)
{
	pdlg->lf = *lplf;
	pdlg->fonttype = nft;

	return FALSE;
}

BOOL FontNameFunc(LPLOGFONT lplf, LPNEWTEXTMETRIC lptm, DWORD nft, LPVOID lps)
{
	for (int i = 0; i < pdlg->ap->GetSize(); i++) {
		char *p = (char *)(*pdlg->ap)[i];
		if (!strcmp(p, lplf->lfFaceName))		return TRUE;
	}
	char *pp = new char[strlen(lplf->lfFaceName) + 1];
	strcpy_s(pp, strlen(lplf->lfFaceName) + 1, lplf->lfFaceName);
	pdlg->ap->Add(pp);

	return TRUE;
}

void CWFONTX64Dlg::OnBnClickedAnk()
{
	ChangeButton();
}

void CWFONTX64Dlg::OnBnClickedKnj()
{
	ChangeButton();
}

void CWFONTX64Dlg::OnEnChangeFontxname()
{
	ChangeButton();
}

void CWFONTX64Dlg::OnEnChangeFilename()
{
	ChangeButton();
}

void CWFONTX64Dlg::OnBnClickedStrikeout()
{
	ChangeFont();
}

void CWFONTX64Dlg::OnBnClickedUnderline()
{
	ChangeFont();
}

void CWFONTX64Dlg::OnBnClickedBold()
{
	ChangeFont();
}

void CWFONTX64Dlg::OnBnClickedItalic()
{
	ChangeFont();
}

void CWFONTX64Dlg::OnEnChangeHeight()
{
	ChangeFont();
}

void CWFONTX64Dlg::OnEnChangeWidth()
{
	ChangeFont();
}

void CWFONTX64Dlg::OnLbnSelchangeFontlist()
{
	ChangeFont();
}

void CWFONTX64Dlg::OnBnClickedRun()
{
	Convert(0);
}

void CWFONTX64Dlg::OnBnClickedNonstop()
{
	Convert(1);
}
