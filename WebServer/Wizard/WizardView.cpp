// WizardView.cpp : implementation of the CWizardView class
//

#include "stdafx.h"
#include "Wizard.h"

#include "WizardDoc.h"
#include "WizardView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWizardView

IMPLEMENT_DYNCREATE(CWizardView, CView)

BEGIN_MESSAGE_MAP(CWizardView, CView)
	//{{AFX_MSG_MAP(CWizardView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizardView construction/destruction

CWizardView::CWizardView()
{
	// TODO: add construction code here

}

CWizardView::~CWizardView()
{
}

BOOL CWizardView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}


/////////////////////////////////////////////////////////////////////////////
// CWizardView diagnostics

#ifdef _DEBUG
void CWizardView::AssertValid() const
{
	CView::AssertValid();
}

void CWizardView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWizardDoc* CWizardView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWizardDoc)));
	return (CWizardDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWizardView message handlers
CPoint m_ptOrigin,m_ptEnd;
void CWizardView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_ptOrigin = m_ptEnd = point;

	CView::OnLButtonDown(nFlags, point);
}

void CWizardView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default


	CView::OnLButtonUp(nFlags, point);
}

void CWizardView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	

	CView::OnMouseMove(nFlags, point);
}

BOOL CWizardView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

int CWizardView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	CClientDC dc(this);
	TEXTMETRIC tm;
	dc.GetTextMetrics(&tm);
	CreateSolidCaret(tm.tmAveCharWidth/8, tm.tmHeight);
	ShowCaret();

	// TODO: Add your specialized creation code here
	
	return 0;
}
