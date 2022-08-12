// WizardDoc.cpp : implementation of the CWizardDoc class
//

#include "stdafx.h"
#include "Wizard.h"

#include "WizardDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWizardDoc

IMPLEMENT_DYNCREATE(CWizardDoc, CDocument)

BEGIN_MESSAGE_MAP(CWizardDoc, CDocument)
	//{{AFX_MSG_MAP(CWizardDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizardDoc construction/destruction

CWizardDoc::CWizardDoc()
{
	// TODO: add one-time construction code here

}

CWizardDoc::~CWizardDoc()
{
}

BOOL CWizardDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CWizardDoc serialization

void CWizardDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CWizardDoc diagnostics

#ifdef _DEBUG
void CWizardDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWizardDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWizardDoc commands
