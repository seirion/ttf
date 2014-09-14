// B2TTFDlg.h : header file
//

#if !defined(AFX_B2TTFDLG_H__C089280F_F843_4688_99B7_486610871F12__INCLUDED_)
#define AFX_B2TTFDLG_H__C089280F_F843_4688_99B7_486610871F12__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////////
// include class
#include "BaboTTF.h"
#include "ControlPoint.h"
/////////////////////////////////////////////////////////////////////////////
// CB2TTFDlg dialog

class CB2TTFDlg : public CDialog
{
// Construction
public:
	void Init();
	CB2TTFDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CB2TTFDlg)
	enum { IDD = IDD_B2TTF_DIALOG };
	CEdit	m_CtrlIn;
	CStatic	m_CtrlPoints;
	CStatic	m_CtrlContours;
	CStatic	m_CtrlUnicode;
	CStatic	m_CtrlFontName;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CB2TTFDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CB2TTFDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonFontOpen();
	afx_msg void OnButtonIn();
	afx_msg void OnButtonExportText();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL LoadFont(LPCSTR path);
	//////////////////////////////////////////////////////////////////////////
	// System variables
	BOOL m_LoadFont;		// ttf files
	BOOL m_LoadGlyph;		// character glyph
	
	BaboTTF *m_TTF;
	CControlPoint m_ControlPoint;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_B2TTFDLG_H__C089280F_F843_4688_99B7_486610871F12__INCLUDED_)
