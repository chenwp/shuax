// MyEclipseKeyGenDlg.h : header file
//

#if !defined(AFX_MYECLIPSEKEYGENDLG_H__288C6BB5_BF5C_4C63_8EA1_3133B2BC1A48__INCLUDED_)
#define AFX_MYECLIPSEKEYGENDLG_H__288C6BB5_BF5C_4C63_8EA1_3133B2BC1A48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMyEclipseKeyGenDlg dialog

class CMyEclipseKeyGenDlg : public CDialog
{
// Construction
public:
	CMyEclipseKeyGenDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMyEclipseKeyGenDlg)
	enum { IDD = IDD_MYECLIPSEKEYGEN_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyEclipseKeyGenDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMyEclipseKeyGenDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnChangeEdit1();
	afx_msg char* MyEclipseKeyGen(char* userId);
	afx_msg char* change(char *s);
	afx_msg long decode(char *s);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYECLIPSEKEYGENDLG_H__288C6BB5_BF5C_4C63_8EA1_3133B2BC1A48__INCLUDED_)
