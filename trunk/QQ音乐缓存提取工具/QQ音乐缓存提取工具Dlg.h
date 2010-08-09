// QQ音乐缓存提取工具Dlg.h : header file
//

#if !defined(AFX_QQDLG_H__47940EAC_4C87_4945_BF46_594B9B928EAA__INCLUDED_)
#define AFX_QQDLG_H__47940EAC_4C87_4945_BF46_594B9B928EAA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CQQDlg dialog

class CQQDlg : public CDialog
{
// Construction
public:
	CQQDlg(CWnd* pParent = NULL);	// standard constructor
	static UINT ThreadProc(LPVOID pParam);
// Dialog Data
	//{{AFX_DATA(CQQDlg)
	enum { IDD = IDD_QQ_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQQDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CStringList list1; 
	CStringList list2; 
	char QQPlayer[1024];
	char QZonePlayer[1024];
	float size1;
	float size2;
	void ChangeCheck();
	bool isrun;
	bool isok;
	CWinThread *pThread;
	// Generated message map functions
	//{{AFX_MSG(CQQDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnCheck2();
	afx_msg void OnCheck3();
	virtual void OnOK();
	afx_msg void OnCheck4();
	afx_msg void OnButton1();
	afx_msg void OnCheck6();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QQDLG_H__47940EAC_4C87_4945_BF46_594B9B928EAA__INCLUDED_)
