// ResolutionDlg.h : header file
//

#pragma once
#include "afxwin.h"

// CResolutionDlg dialog
class CResolutionDlg : public CDialog
{
// Construction
public:
	CResolutionDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_RESOLUTION_DIALOG };
    CString editinput;
	bool stepsshown;
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSubmit();
	afx_msg void OnBnClickedReset();
	CListBox Ulistbox;
	CListBox Plistbox;
	afx_msg void OnBnClickedSelect();
	afx_msg void OnBnClickedCreset();
	afx_msg void OnBnClickedRemlast();
	afx_msg void OnBnClickedHelp();
	afx_msg void OnBnClickedOuthlp();
	afx_msg void OnBnClickedInHelp();
	afx_msg void OnBnClickedClauhlp();
	afx_msg void OnBnClickedConcheck();
	afx_msg void OnBnClickedShowsteps();
	afx_msg void OnBnClickedPreset();
	afx_msg void OnBnClickedAutphlp();
	afx_msg void OnBnClickedPremlast();
	afx_msg void OnBnClickedPfile();
	afx_msg void OnBnClickedUfile();
	afx_msg void OnBnClickedOk2();
};
