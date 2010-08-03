// ×Ö·ûÌæ»»×¨¼ÒDlg.h : header file
//
#if !defined(AFX_DLG_H__4F641D0B_2EC6_422B_BD22_687E46F6BF51__INCLUDED_)
#define AFX_DLG_H__4F641D0B_2EC6_422B_BD22_687E46F6BF51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMyDlg dialog

class CMyDlg : public CDialog
{
// Construction
public:
	CMyDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMyDlg)
	enum { IDD = IDD_MY_DIALOG };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	

	
	void FormatHex(char *src);
	void H2Hex(int nlDDlgltem);
	void A2Hex(int nlDDlgltem1, int nlDDlgltem2);
	void U2Hex(int nlDDlgltem1, int nlDDlgltem2);
	int GetHexValue(char *src);
	void H2UNICODE(int nlDDlgltem1, int nlDDlgltem2);
	void H2ASCII(int nlDDlgltem1, int nlDDlgltem2);

	bool is_edit;
	// Generated message map functions
	//{{AFX_MSG(CMyDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnChangeFindH();
	afx_msg void OnChangeReplaceH();
	afx_msg void OnChangeFindA();
	afx_msg void OnChangeFindU();
	afx_msg void OnChangeReplaceA();
	afx_msg void OnChangeReplaceU();
	afx_msg void Onexit();
	afx_msg void OnReplace();
	afx_msg void OnBrowse();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_H__4F641D0B_2EC6_422B_BD22_687E46F6BF51__INCLUDED_)
