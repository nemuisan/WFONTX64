
// WFONTX64Dlg.h : ヘッダー ファイル
//

#pragma once

#include "ConvWindow.h"

// CWFONTX64Dlg ダイアログ
class CWFONTX64Dlg : public CDialogEx
{

protected:
	CConvWindow	*ccwp;

// コンストラクション
public:
	CWFONTX64Dlg(CWnd* pParent = nullptr);	// 標準コンストラクター


	CFont cf;

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WFONTX64_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	// Tuika
	afx_msg void OnClose();
	void ChangeButton();
	int ChangeFont();
	void Convert(int f);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedExit();
	afx_msg void OnBnClickedAnk();
	afx_msg void OnBnClickedKnj();
	afx_msg void OnBnClickedStrikeout();
	afx_msg void OnBnClickedUnderline();
	afx_msg void OnBnClickedBold();
	afx_msg void OnBnClickedItalic();
	afx_msg void OnEnChangeHeight();
	afx_msg void OnEnChangeWidth();
	afx_msg void OnEnChangeFontxname();
	afx_msg void OnEnChangeFilename();
	afx_msg void OnLbnSelchangeFontlist();
	afx_msg void OnBnClickedRun();
	afx_msg void OnBnClickedNonstop();
public:
	LOGFONT	lf;
	DWORD fonttype = 0;
	CPtrArray *ap = NULL;
	bool IsProportioned = false;
};
