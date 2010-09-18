// Picasa3Dlg.h : header file
//

#if !defined(AFX_PICASA3DLG_H__5243BD71_0905_47CC_902D_BA9E6C5E5528__INCLUDED_)
#define AFX_PICASA3DLG_H__5243BD71_0905_47CC_902D_BA9E6C5E5528__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPicasa3Dlg dialog

class CPicasa3Dlg : public CDialog
{
// Construction
public:
	CPicasa3Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CPicasa3Dlg)
	enum { IDD = IDD_PICASA3_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPicasa3Dlg)
	protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	bool isAero;

	// Generated message map functions
	//{{AFX_MSG(CPicasa3Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg bool Win7Install(wchar_t *temppath);
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg UINT isRect(CPoint pt,CPoint Button);
	afx_msg BOOL ImageFromIDResource(UINT nID, LPCTSTR sTR,Image * &pImg);
	afx_msg BOOL ReleaseRes(CString strFileName,WORD wResID,CString strFileType);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PICASA3DLG_H__5243BD71_0905_47CC_902D_BA9E6C5E5528__INCLUDED_)
