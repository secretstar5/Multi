#if !defined(AFX_UEASYBUTTONEX_H__180B6B76_3323_4CB7_8072_88EB7C86FFF9__INCLUDED_)
#define AFX_UEASYBUTTONEX_H__180B6B76_3323_4CB7_8072_88EB7C86FFF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UEasyButtonEx.h : header file
//

//	Class:		CButtonST
//
//	Compiler:	Visual C++
//	Tested on:	Visual C++ 5.0
//				Visual C++ 6.0
//
//	Version:	See GetVersionC() or GetVersionI()
//
//	Created:	xx/xxxx/1998
//	Updated:	03/March/2003
//
//	Author:		Davide Calabro'		davide_calabro@yahoo.com
//									http://www.softechsoftware.it
//
//	Note:		Code for the PreSubclassWindow and OnSetStyle functions
//				has been taken from the COddButton class
//				published by Paolo Messina and Jerzy Kaczorowski
//
//	Disclaimer
//	----------
//	THIS SOFTWARE AND THE ACCOMPANYING FILES ARE DISTRIBUTED "AS IS" AND WITHOUT
//	ANY WARRANTIES WHETHER EXPRESSED OR IMPLIED. NO REPONSIBILITIES FOR POSSIBLE
//	DAMAGES OR EVEN FUNCTIONALITY CAN BE TAKEN. THE USER MUST ASSUME THE ENTIRE
//	RISK OF USING THIS SOFTWARE.
//
//	Terms of use
//	------------
//	THIS SOFTWARE IS FREE FOR PERSONAL USE OR FREEWARE APPLICATIONS.
//	IF YOU USE THIS SOFTWARE IN COMMERCIAL OR SHAREWARE APPLICATIONS YOU
//	ARE GENTLY ASKED TO DONATE 5$ (FIVE U.S. DOLLARS) TO THE AUTHOR:
//
//		Davide Calabro'
//		P.O. Box 65
//		21019 Somma Lombardo (VA)
//		Italy
//
// 출처 : wwww.codeproject.com 에 있는 버튼 콘트롤에서 퍼왔습니다~
// 수정 : Source Code 분석하여 좀 더 간략하게 만들려고 했으나....OWNER DRAW가 생각보다 어렵네요 휴~~
//        하다보니 원래 소스를 거의 그대로 사용하게 됐습니다.. ㅋㅋ MENU POP UP BUTTON 기능만 삭제 했습니다.
//        인터넷에 돌아댕기는 버튼 소스 중에 이 소스가 그래도 젤 훌륭한 듯 해서 가져다 사용하게 됐습니다.
//        원 저자에게 감사하는 마음으로 사용합시다...(5달라 달라는데...우리는 걍 가져다 씁시다~~~^^)
//        2007.04.03 yrkwon.  이 아래부터는 수정된 사항이나 업그레이드 정보 적어주세요~~~

#define	BTNST_AUTO_GRAY					(HICON)(0xffffffff - 1L)
#define	BTNST_AUTO_DARKER				(HICON)(0xffffffff - 2L)
#define	TTS_BALLOON						0x40

typedef struct _tagICONS {
	HICON		hIcon;			// Handle to icon
	DWORD		dwWidth;		// Width of icon
	DWORD		dwHeight;		// Height of icon
} SICONS;

typedef struct _tagBITMAPS {
	HBITMAP		hBitmap;		// Handle to bitmap
	DWORD		dwWidth;		// Width of bitmap
	DWORD		dwHeight;		// Height of bitmap
	HBITMAP		hMask;			// Handle to mask bitmap
	COLORREF	clrTransparent;	// Transparent color
} SBITMAPS;

/////////////////////////////////////////////////////////////////////////////
// UEasyButtonEx window

class UEasyButtonEx : public CButton
{
// Construction
public:
	UEasyButtonEx();

// Attributes
public:
	enum {
			ALIGN_HOR = 0,		// Icon/Bitmap on the left, text on the right
			ALIGN_VER,			// Icon/Bitmap on the top, text on th bottom
			ALIGN_HOR_RIGHT,	// Icon/Bitmap on the right, text on the left
			ALIGN_OVERLAP		// Icon/Bitmap on the same space as text
		};
	enum {
			COLOR_BG_IN = 0,	// Background color when mouse is inside
			COLOR_FG_IN,		// Text color when mouse is inside
			COLOR_BG_OUT,		// Background color when mouse is outside
			COLOR_FG_OUT,		// Text color when mouse is outside
			COLOR_BG_FOCUS,		// Background color when button is focused
			COLOR_FG_FOCUS,		// Text color when button is focused
			MAX_COLORS
		};
	enum {
			PRESSED_LEFTRIGHT = 0,	// Pressed style from left to right (as usual)
			PRESSED_TOPBOTTOM		// Pressed style from top to bottom
		};

// Operations
public:
	BOOL	getTimerOn() {
		if (m_nTimerID == -1) 
			{ return FALSE; }
		else 
			{ return TRUE; }
	}
private:
	int		m_nTimerID = -1;
public:
	bool    getTimerWorking() { if (m_nTimerID == -1) return false; else return true; }
	void	SetClick(BOOL bClick);
	void	TimerControl(BOOL bStart);

	void	OnDrawBackground(CDC* pDC, CRect* pRect);
	void	OnDrawBorder(CDC* pDC, CRect* pRect);
	void	OnDrawText(CDC* pDC, LPCTSTR lpszText, CRect* prcItem, CRect* prcCaption);
	void	OnDrawIcon(CDC* pDC, BOOL bHasTitle, CRect* prcItem, CRect* prcCaption);
	void	OnDrawBitmap(CDC* pDC, BOOL bHasTitle, CRect* prcItem, CRect* prcCaption);

	BOOL	SetColor(int nIndex, COLORREF clrColor, BOOL bRepaint=TRUE);
	BOOL	GetColor(int nIndex, COLORREF* pclrColor);
	void	SetDefaultColor(BOOL bRepaint=TRUE);
	void	OffsetColor(BYTE bIndex, short sOffset, BOOL bRepaint=TRUE);

	void	DrawBorder(BOOL bDrawBorder, BOOL bRepaint=TRUE);
	void	DrawFlatFocus(BOOL bDrawFlatFocus, BOOL bRepaint=TRUE);

	void	SetPressedStyle(BYTE bStyle, BOOL bRepaint=TRUE);
	void	SetFlat(BOOL bFlat, BOOL bRepaint = TRUE);
	void	SetAlign(BYTE bAlign, BOOL bRepaint=TRUE);
	void	SetAlwaysTrack(BOOL bTrack)	{ m_bAlwaysTrack = bTrack; }
	
	BOOL	GetCheck()		{ return m_bClick; }
	void	SetCheck(BOOL bCheck, BOOL bRepaint=TRUE);
	
	void	SetBtnCursor(int nCursorID, BOOL bRepaint=TRUE);
	void	CancelHover();
	
	void	FreeResource(BOOL bCheckForNULL=TRUE);
	void	PrepareImageRect(BOOL bHasTitle, CRect* prcItem, CRect* prcTitle, DWORD dwWidth, DWORD dwHeight, CRect* prcImg);
	HICON	CreateGrayscaleIcon(HICON hIcon);
	HICON	CreateDarkerIcon(HICON hIcon);
	COLORREF DarkenColor(COLORREF clrColor, double dFactor);
	DWORD	SetIcon(HICON hIconIn, HICON hIconOut=NULL);
	DWORD	SetIcon(int nIconIn, int nIconOut=0);
	DWORD	SetIcon(int nIconIn, int nCxDesiredIn, int nCyDesiredIn, int nIconOut=0, int nCxDesiredOut=0, int nCyDesiredOut=0 );

	HBITMAP	CreateGrayscaleBitmap(HBITMAP hBitmap, DWORD dwWidth, DWORD dwHeight, COLORREF clrTrans);
	HBITMAP	CreateDarkerBitmap(HBITMAP hBitmap, DWORD dwWidth, DWORD dwHeight, COLORREF clrTrans);
	HBITMAP	CreateBitmapMask(HBITMAP hBitmap, DWORD dwWidht, DWORD dwHeight, COLORREF clrTrans);
	DWORD	SetBitmap(int nBitmapIn, COLORREF clrTransColorIn, int nBitmapOut=0, COLORREF clrTransColorOut=0);
	DWORD	SetBitmap(HBITMAP hBitmapIn, COLORREF clrTansColorIn, HBITMAP hBitmapOut=NULL, COLORREF clrTransColorOut=0);
	void	SizeToContent();

	void	InitToolTip();
	void	SetToolTipText(int nText, BOOL bActivate=TRUE);
	void	SetToolTipText(LPCTSTR lpszText, BOOL bActivate=TRUE);
	void	ActivateToolTip(BOOL bEnable=TRUE);
	DWORD	EnableBallonToolTip();

	void	SetImageOrg(int x, int y);

	void	SetSelection(int nSel)		
	{ 
		if (m_nSelection == nSel) return;
		m_nSelection = nSel; 
		Invalidate(); 
	}
	int 	GetSelection()				{ return m_nSelection; }
	void	SetSelection(int nSel, COLORREF sColor);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(UEasyButtonEx)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~UEasyButtonEx();

	// Generated message map functions
protected:
	BOOL			m_bClick;
	BOOL			m_bIsFlat;			// Is flat button
	BOOL			m_bIsCheckBox;		// Check box or not
	BOOL			m_bIsFocused;		// Is button focused
	BOOL			m_bIsPressed;		// Is button pressed
	BOOL			m_bIsDisabled;		// Is button disabled
	BOOL			m_bIsDefault;		// Is button default
	BOOL			m_bMouseOnButton;	// Is mouse in button
	BOOL			m_bDrawBorder;		// Is draw button'border
	BOOL			m_bDrawFlatFocus;
	BOOL			m_bAlwaysTrack;
	BOOL			m_bShowDisabledBitmap;
	COLORREF		m_clrColor[MAX_COLORS];
	COLORREF		m_clrSelectColor;
	int				m_nSelection;
	CPoint			m_nPressedOffset;
	CPoint			m_nImageOrg;
	HCURSOR			m_hCursor;
	CToolTipCtrl	m_ToolTip;
	DWORD			m_dwToolTipStyle;
	int				m_nAlignMode;

	SICONS			m_sIcons[2];
	SBITMAPS		m_sBitmaps[2];

	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	LRESULT OnSetStyle(WPARAM wParam, LPARAM lParam);
	LRESULT OnSetCheck(WPARAM wParam, LPARAM lParam);
	LRESULT OnGetCheck(WPARAM wParam, LPARAM lParam);

	//{{AFX_MSG(UEasyButtonEx)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnCancelMode();
	afx_msg BOOL OnClicked();
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSysColorChange();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UEASYBUTTONEX_H__180B6B76_3323_4CB7_8072_88EB7C86FFF9__INCLUDED_)
