// B2TTF.h : main header file for the B2TTF application
//

#if !defined(AFX_B2TTF_H__8B9A8AD6_0681_4F0E_A8C1_A6451DF90A36__INCLUDED_)
#define AFX_B2TTF_H__8B9A8AD6_0681_4F0E_A8C1_A6451DF90A36__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CB2TTFApp:
// See B2TTF.cpp for the implementation of this class
//

class CB2TTFApp : public CWinApp
{
public:
	CB2TTFApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CB2TTFApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CB2TTFApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_B2TTF_H__8B9A8AD6_0681_4F0E_A8C1_A6451DF90A36__INCLUDED_)
