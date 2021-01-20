#if !defined(AFX_COLORSTATIC_H__6BD5BEDA_E04E_4D49_A8F1_D081504ADD5A__INCLUDED_)
#define AFX_COLORSTATIC_H__6BD5BEDA_E04E_4D49_A8F1_D081504ADD5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorStatic.h : header file
//

#define BLACK_COLOR			RGB(0, 0, 0)
#define WHITE_COLOR			RGB(255, 255,255)
#define TITLE_FORE_COLOR	RGB(0, 0, 0)
#define TITLE_BACK_COLOR	RGB(224,255,255)
#define VALUE_FORE_COLOR	RGB(0,0,0)
#define VALUE_BACK_COLOR	RGB(255,255,198)
#define LIGHT_BLUE_COLOR	RGB(224,255,255)
#define LIGHT_YELLOW_COLOR	RGB(255,255,198)

/////////////////////////////////////////////////////////////////////////////
// CColorStatic window

class CColorStatic : public CStatic
{
// Construction
public:
	CColorStatic();

// Attributes
public:
	CBrush m_brush;
	COLORREF		m_clrFore;
	COLORREF		m_clrBack;
// Operations
public:
	void SetBackColor( COLORREF clrBack ) ;
	void SetForeColor( COLORREF clrFore ) ;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CColorStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColorStatic)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORSTATIC_H__6BD5BEDA_E04E_4D49_A8F1_D081504ADD5A__INCLUDED_)
