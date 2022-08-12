// WizardDoc.h : interface of the CWizardDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WIZARDDOC_H__C7FE8717_997C_48A6_9C24_DFD83B7598FC__INCLUDED_)
#define AFX_WIZARDDOC_H__C7FE8717_997C_48A6_9C24_DFD83B7598FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CWizardDoc : public CDocument
{
protected: // create from serialization only
	CWizardDoc();
	DECLARE_DYNCREATE(CWizardDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWizardDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWizardDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CWizardDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZARDDOC_H__C7FE8717_997C_48A6_9C24_DFD83B7598FC__INCLUDED_)
