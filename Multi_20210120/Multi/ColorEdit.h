#if !defined(AFX_COLOREDIT_H__947A0D31_9413_41C8_A2E1_4BC2C01606FF__INCLUDED_)
#define AFX_COLOREDIT_H__947A0D31_9413_41C8_A2E1_4BC2C01606FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

// CColorEdit window

#define BLACK_COLOR			RGB(0, 0, 0)
#define WHITE_COLOR			RGB(255, 255,255)
#define LIGHT_BLUE_COLOR	RGB(224,255,255)
#define LIGHT_YELLOW_COLOR	RGB(255,255,198)

class CColorEdit : public CEdit
{
// Construction
public:
	CColorEdit();

// Attributes
public:
	CBrush			m_brush;
	COLORREF		m_clrFore ;
	COLORREF		m_clrBack ;
	COLORREF		m_clrDisable;

	enum {ceNONE = 0, ceREADONLY = -1, ceUINT = 1, ceINT = 5, ceALPH = 2, ceFLOAT = 3, ceALPH_NUM = 4};
	int             m_nReceivedFlag ;

// Operations
public:
	void SetDisableBackColor();
	void SetBackColor( COLORREF clrBack ) ;
	void SetForeColor( COLORREF clrFore ) ;
	void SetReceivedFlag( int flag) ;
	void EnableWindow(BOOL bEnable=TRUE);
	CString GetText() 
	{
		CString str;
		GetWindowText(str);	

		return str;
	};

public:
	//property(CString, GetText,SetWindowText ) Value;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CColorEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColorEdit)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLOREDIT_H__947A0D31_9413_41C8_A2E1_4BC2C01606FF__INCLUDED_)
