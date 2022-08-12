// Wizard.h : main header file for the WIZARD application
//

#if !defined(AFX_WIZARD_H__6FA89852_1850_4F2D_BC35_14F6E7A78E62__INCLUDED_)
#define AFX_WIZARD_H__6FA89852_1850_4F2D_BC35_14F6E7A78E62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CWizardApp:
// See Wizard.cpp for the implementation of this class
//

class CWizardApp : public CWinApp
{
public:
	CWizardApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWizardApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CWizardApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZARD_H__6FA89852_1850_4F2D_BC35_14F6E7A78E62__INCLUDED_)
