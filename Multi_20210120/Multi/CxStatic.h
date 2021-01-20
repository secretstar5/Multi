/*===========================================================================
====    Project : CxStatic (www.codeproject.com)                         ====
====    File name           :  CxStatic.h                                ====
====    Creation date       :  09/2004                                   ====
====    Author(s)           :  Vincent Richomme                          ====
====    Thanks to norm.net and Chen-Cha Hsu                              ====
===========================================================================*/

#ifndef CSTATICTEST_H
#define CSTATICTEST_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "MemDC.h"

typedef UINT (CALLBACK* LPFNDLLFUNC)(HDC,CONST PTRIVERTEX,DWORD,CONST PVOID,DWORD,DWORD);

class CxStatic : public CStatic
{
public:
    enum {	ALIGN_HORIZ	= 0,			// Icon/bitmap on the left, text on the right
			ALIGN_VERT,					// Icon/bitmap on the top, text on the bottom
			ALIGN_HORIZ_RIGHT,			// Icon/bitmap on the right, text on the left
			ALIGN_OVERLAP				// Icon/bitmap on the same space as text
			};
public:

	enum ImageSize { OriginalSize, FitControl};
	enum LinkStyle { LinkNone, HyperLink, MailLink };
	enum Type3D { Raised, Sunken};
	enum BackFillMode { Normal, HGradient, VGradient, HVGradient, VHGradient};

	CxStatic();
	virtual ~CxStatic();
	
    void    SetGlassColor(COLORREF rgbBkgnd,float fSplitGraident  = float(0.5));//jeon

	BOOL	IsMoveable() { return m_bAllowMove; }
	void	SetMoveable(BOOL moveAble = TRUE); 
	void	SetBkColor(COLORREF rgb, COLORREF crBkgndHigh = 0, BackFillMode mode = Normal, float fSplitGraident = float(0.5)	);
	void	SetTransparent(BOOL bTranspMode);
	void	SetTextColor(COLORREF col);
	void	SetFont(const CString& strFont, int nPointSize,int nWeight, BOOL bRedraw = TRUE);
	void	SetFont(LOGFONT *pLogFont, BOOL bRedraw = TRUE);
	void	SetFont(CFont *pFont, BOOL bRedraw = TRUE);
	void	SetFont3D(BOOL bFont3D, Type3D type);
	
// 	void	setFont(LONG fontHeight		 = -8, 
// 					LONG fontWeight      = FW_NORMAL,
// 					UCHAR pitchAndFamily = DEFAULT_PITCH | FF_DONTCARE,
// 					LPCSTR faceName      = "Arial" );
	
	void	SetAutoAdjustFont(BOOL bAutoAdjustFont);
	
	BOOL	SetBitmap(HBITMAP hBitmap, ImageSize Emode = FitControl, 
				COLORREF rgbTransparent =  0xFF000000);

	BOOL	SetBitmap(LPCTSTR lpszResName, ImageSize Emode = FitControl,
				COLORREF rgbTransparent = 0xFF000000);	// Loads bitmap from resource name

	BOOL	SetBitmap(UINT nIDResource, ImageSize Emode = FitControl,
		COLORREF rgbTransparent = 0xFF000000);	// Loads bitmap from resource ID

	BOOL	ResetBitmap();	

	void	SetRounded(BOOL bRounded, CPoint ptRound = CPoint(10, 10));
	void	SetBorder(BOOL bBorder, COLORREF rgbBorder = RGB(0, 0, 0))
	{
		m_bBorder = bBorder;
		m_rgbBorder = rgbBorder;
	}
	void    SetAutoWrapping(bool bEnable = true) { m_bAutoWrapping = bEnable; }

	void	GetText(CString &strText);
	void	SetWindowText(LPCTSTR strText);

	void	AppendText(LPCTSTR szFmt, ...);
	void	Format(LPCTSTR szFmt, ...);
	
	void	SetAlign(BYTE byAlign, BOOL bRepaint);
	void	SetColor(COLORREF fcol, COLORREF bcol);

	HICON	CreateGrayscaleIcon(HICON hIcon);
	void	SetIcon(int nIcon);
	void	SetIcon(HICON hIcon);
	void	DeleteIcon();

	void	FontSet(int Size, int Bold, LPTSTR Styel, BYTE bItalic = FALSE);

protected:
	COLORREF	m_rgbBorder;
	BOOL		m_bBorder;
	CPoint		m_ptRound;

	float            m_fLevelSplitGradient ;//bugjeon 2012.05.8 
	CRgn*         m_pRgn; //bugjeon 2012.04.05 
	CString		m_strText;
	BOOL			m_bNotifyParent;
	BOOL			m_bTransparentBk;
	BOOL			m_bAutoAdjustFont;
	BOOL			m_bAutoWrapping;
	BOOL			m_bRounded;
	COLORREF		m_rgbText;
	BackFillMode	m_fillmode;
	COLORREF		m_rgbBkgnd;
	COLORREF		m_crHiColor;
	COLORREF		m_crLoColor;
	COLORREF		m_cr3DHiliteColor;
	CRect			m_rc;
	DWORD			m_dwTxtFlags;

	BYTE			m_byAlign;			// Align mode

	HINSTANCE		hinst_msimg32;
	LPFNDLLFUNC		dllfunc_GradientFill;

	HICON		m_hIcon;			// Handle to icon
	DWORD		m_dwIconWidth;		// Width of icon
	DWORD		m_dwIconHeight;		// Height of icon
	// BITMAP ATTRIBUTES

	COLORREF		m_rgbTransparent;
	int				m_EDispMode;
	BOOL			m_bBitmap;
	int				m_nResourceID;
	HBITMAP			m_hBitmap;
    BITMAP			m_bmInfo;
    CString			m_strResourceName;

	// FONT ATTRIBUTES
	Type3D			m_3dType;
	BOOL			m_bFont3d;
	LOGFONT			m_lf;
	CFont			m_Font;
	BOOL			m_bFont;
	int				m_nFontSize;
	int				m_nFontSizeVar;
	CString			m_csFontFamilly;
	
	// MODIFIABLE ATTRIBUTES
	BOOL			m_bAllowMove;
	BOOL			m_bHover;
	BOOL			m_bTracking;
	CPoint			m_point;
	CRectTracker*	m_pTrack;

	POINT			m_ptImageOrg;

	int				m_nIcon;


	
	// FONT METHODS
	void	ReconstructFont();
	int		GetFontPointSize(int nHeight);
	int		GetFontHeight(int nPointSize);
	
	// TEXT METHODS
	void	DrawText(CDC* pDC, CRect* pRect, CString csText);
	BOOL	IsASingleWord(const CString & csText);
	void	DrawGradientFill(CDC* pDC, CRect* pRect, BackFillMode enumFillMode);

	// IMAGE METHODS
	void	DrawBitmap(CDC* pDC, CRect* pRect);

	BOOL	RedrawWindow();

	void	DrawTheBitmap(CDC* pDC, BOOL bHasTitle, RECT* rpItem, CRect* rpCaption);
	void	DrawTheText(CDC* pDC, LPCTSTR lpszText, RECT* rpItem, CRect* rpCaption);
	void	DrawTheIcon(CDC* pDC, BOOL bHasTitle, RECT* rpItem, CRect* rpCaption);
	void	PrepareImageRect(BOOL bHasTitle, RECT* rpItem, CRect* rpTitle, DWORD dwWidth, DWORD dwHeight, CRect* rpImage);

	// MESSAGE HANDLER
	virtual void PreSubclassWindow();
	virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );

	//afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT	OnSetText(WPARAM wParam, LPARAM lParam); // WM_SETTEXT for UpdateData
	afx_msg LRESULT	OnSetFont(WPARAM wParam, LPARAM lParam); // WM_SETFONT
	afx_msg void	OnDropFiles(HDROP hDropInfo); // WM_DROPFILES
	afx_msg void	OnSize(UINT nType, int cx, int cy);
	//afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnMouseHover(WPARAM wparam, LPARAM lparam) ;
	afx_msg void OnRButtonDown( UINT nFlags, CPoint point );

	DECLARE_MESSAGE_MAP()
};

#endif // CXSTATIC_H
