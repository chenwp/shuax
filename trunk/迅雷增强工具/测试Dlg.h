// ²âÊÔDlg.h : header file
//


#if !defined(AFX_DLG_H__06BB8ABF_C400_4120_B397_E7AC7C3E3769__INCLUDED_)
#define AFX_DLG_H__06BB8ABF_C400_4120_B397_E7AC7C3E3769__INCLUDED_

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
	BOOL ImageFromIDResource(UINT nID, LPCTSTR sTR,Image * &pImg);
// Dialog Data
	//{{AFX_DATA(CMyDlg)
	enum { IDD = IDD_MY_DIALOG };
		// NOTE: the ClassWizard will add data members here
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
	//HICON m_hIcon;
	// Generated message map functions
	//{{AFX_MSG(CMyDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void getPath();
	afx_msg void LinkSimulate2();
	afx_msg void crack();
	afx_msg void restore();
	afx_msg UINT isRect(CPoint pt,CPoint Button);
	afx_msg BOOL ReleaseRes(CString strFileName,WORD wResID,CString strFileType);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	char path[128];
	char filepath[64];
	char bakpath[128];
	char regkey[128];
	char insdir[64];
	int version;
	BOOL bIsAero;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_H__06BB8ABF_C400_4120_B397_E7AC7C3E3769__INCLUDED_)
