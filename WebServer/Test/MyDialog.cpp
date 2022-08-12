// MyDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Test.h"
#include "MyDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyDialog dialog


CMyDialog::CMyDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMyDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyDialog)
	m_num1 = 0;
	m_num2 = 0;
	m_num3 = 0;
	//}}AFX_DATA_INIT
}


void CMyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyDialog)
	DDX_Control(pDX, IDC_EDIT3, m_edit3);
	DDX_Control(pDX, IDC_EDIT2, m_edit2);
	DDX_Control(pDX, IDC_EDIT1, m_edit1);
	DDX_Text(pDX, IDC_EDIT1, m_num1);
	DDX_Text(pDX, IDC_EDIT2, m_num2);
	DDX_Text(pDX, IDC_EDIT3, m_num3);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyDialog, CDialog)
	//{{AFX_MSG_MAP(CMyDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_NUMBER1, OnNumber1)
	ON_BN_CLICKED(IDC_NUMBER2, OnNumber2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyDialog message handlers

void CMyDialog::OnOK() 
{
	// TODO: Add extra validation here

	CDialog::OnOK();
}

void CMyDialog::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	EndDialog(0);
}

void CMyDialog::OnButton1() 
{
	// TODO: Add your control notification handler code here
	int num1, num2, num3;
	char c1[10], c2[10], c3[10];
	GetDlgItem(IDC_EDIT1)->GetWindowText(c1, 10);
	GetDlgItem(IDC_EDIT2)->GetWindowText(c2, 10);	
    num1 = atoi(c1);
	num2 = atoi(c2);
	num3 = num1 + num2;
	itoa(num3,c3,10);
	GetDlgItem(IDC_EDIT3)->SetWindowText(c3);


}

void CMyDialog::OnNumber1() 
{
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItem(IDC_NUMBER1)->GetWindowText(str);
	if (str == "Number1:")
		GetDlgItem(IDC_NUMBER1)->SetWindowText("ÊýÖµ1£º");
	else
		GetDlgItem(IDC_NUMBER1)->SetWindowText("Number1:");

}

void CMyDialog::OnNumber2() 
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_NUMBER2);
	CClientDC dc(pWnd);

	CRect rc;
	pWnd->GetClientRect(&rc);
	CBrush brush(RGB(255,0,0));

	// CDC* pDC = pWnd->GetDC();
	dc.FillRect(&rc,&brush);
	dc.SetBkMode(TRANSPARENT);
	dc.TextOut(13,5,"HELLO");	
// 	ReleaseDC(pDC);

}

void CMyDialog::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;
	CDialog::PostNcDestroy();
}
