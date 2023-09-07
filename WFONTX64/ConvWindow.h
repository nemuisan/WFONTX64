#pragma once


// CConvWindow ダイアログ

class CConvWindow : public CModalDialog
{
	DECLARE_DYNAMIC(CConvWindow)
protected:
	int nonstopflag;
	int type;
	LPLOGFONT lfp;
	CString filename;
	CString fontxname;

	CFile cfile;
	CFont cfont;

	UINT upk, lwk;

	BOOL paintflag;

public:
	CConvWindow(int f, int t, LPSTR fp, LPSTR xp, LPLOGFONT lp, CWnd *pP);

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONVERT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnStop();
	DECLARE_MESSAGE_MAP()
};
