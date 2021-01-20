/*===========================================================================
====                                                                     ====
====    File name           :  CxStatic.cpp                              ====
====    Creation date       :  09/2004                                   ====
====    Author(s)           :  Vincent Richomme                          ====
====    Thanks to norm.net and Chen-Cha Hsu                              ====
===========================================================================*/

#include "pch.h"
#include "CxStatic.h"
#include <string>
namespace CxSt {

	class CMemDCEx : public CDC
	{
	public:

		// constructor sets up the memory DC
		CMemDCEx(CDC* pDC, const CRect* pRect = NULL, BOOL transparent = FALSE) : CDC()
		{
			ASSERT(pDC != NULL);

			m_pDC = pDC;
			m_pOldBitmap = NULL;
			m_bMemDC = !pDC->IsPrinting();

			// Get the rectangle to draw
			if (pRect == NULL)
			{
				pDC->GetClipBox(&m_rect);
			}
			else
			{
				m_rect = *pRect;
			}

			if (m_bMemDC)    // Create a Memory DC
			{
				// Create a Memory DC
				CreateCompatibleDC(pDC);
				pDC->LPtoDP(&m_rect);

				m_bitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());
				m_pOldBitmap = SelectObject(&m_bitmap);

				SetMapMode(pDC->GetMapMode());
				pDC->DPtoLP(&m_rect);
				SetWindowOrg(m_rect.left, m_rect.top);
			}
			else			// Make a copy of the relevent parts of the current DC for printing
			{
				m_bPrinting = pDC->m_bPrinting;
				m_hDC = pDC->m_hDC;
				m_hAttribDC = pDC->m_hAttribDC;
			}

			SetBkMode(TRANSPARENT);

			// Fill background 
			if (transparent)
			{
				BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
					m_pDC, 0, 0, SRCCOPY);
			}
			else
			{
				//SetBkMode(TRANSPARENT);
				FillSolidRect(m_rect, pDC->GetBkColor()); // Not needed in OPAQUE mode???  See documentation
			}
		}

		// Destructor copies the contents of the mem DC to the original DC
		~CMemDCEx()
		{
			if (m_bMemDC)
			{
				// Copy the offscreen bitmap onto the screen.
				m_pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
					this, m_rect.left, m_rect.top, SRCCOPY);

				//Swap back the original bitmap.
				SelectObject(m_pOldBitmap);
			}
			else
			{
				// All we need to do is replace the DC with an illegal value,
				// this keeps us from accidently deleting the handles associated with
				// the CDC that was passed to the constructor.			
				m_hDC = m_hAttribDC = NULL;
			}
		}

		// Allow usage as a pointer
		CMemDCEx* operator->() { return this; }

		// Allow usage as a pointer
		operator CMemDCEx*() { return this; }

	private:
		CBitmap  m_bitmap;      // Offscreen bitmap
		CBitmap* m_pOldBitmap;  // bitmap originally found in CMemDCEx
		CDC*     m_pDC;         // Saves CDC passed in constructor
		CRect    m_rect;        // Rectangle of drawing area.
		BOOL     m_bMemDC;      // TRUE if CDC really is a Memory DC.
	};
}
using namespace CxSt;



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CxStatic
BEGIN_MESSAGE_MAP(CxStatic, CStatic)
	//ON_WM_PAINT()
	ON_WM_DRAWITEM_REFLECT()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_SETTEXT, OnSetText)
	ON_MESSAGE(WM_SETFONT, OnSetFont)
	ON_WM_MOUSEMOVE()
	//ON_MESSAGE(WM_DROPFILES)
	ON_WM_DROPFILES()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_WM_RBUTTONDOWN()
	ON_WM_SIZE()
	
	//ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// ctor
CxStatic::CxStatic()
{
	m_bBorder         = FALSE;
	m_ptRound         = CPoint(10, 10);

	hinst_msimg32	  = NULL;
	m_strText		  = _T("");
	m_bTransparentBk  = FALSE;
	m_bAutoWrapping	  = TRUE;
	m_bAutoAdjustFont = FALSE;
	m_bNotifyParent   =	FALSE;
	m_bRounded		  = FALSE;
	m_rgbText		  = ::GetSysColor(COLOR_WINDOWTEXT);
	m_rgbBkgnd		  = ::GetSysColor(COLOR_BTNFACE);
	m_cr3DHiliteColor =	RGB(0,0,255);
	m_rgbTransparent  = 0xFF000000;
	m_EDispMode		  = 0;
	m_fillmode		  = Normal;//HGradient
	m_crHiColor		  =	m_rgbBkgnd;
	m_crLoColor		  =	m_rgbBkgnd;
	m_nFontSize		  = m_nFontSizeVar = 8;
	m_csFontFamilly	  = _T("");
	m_bFont3d		  = FALSE;
	m_hBitmap		  = NULL;
	m_bBitmap		  = FALSE;
	m_nResourceID	  = -1;
	m_bHover		  = FALSE;
	m_bTracking		  = FALSE;
	m_bAllowMove	  = FALSE;
	m_dwTxtFlags	  = 0;
	m_hIcon			  = NULL;
	m_byAlign		  = ALIGN_HORIZ; 

	m_ptImageOrg.x = 3;
	m_ptImageOrg.y = 3;

	m_bFont = 0;
	m_nIcon = -1;

	m_strResourceName.Empty();

	// LIBRARY TO DRAW COLOR GRADIENTS
	hinst_msimg32 = LoadLibrary( _T("msimg32.dll") );
	if ( hinst_msimg32 ){
		dllfunc_GradientFill = ((LPFNDLLFUNC) GetProcAddress( hinst_msimg32, "GradientFill" ));
	}
}

CxStatic::~CxStatic()
{
	if (m_hIcon)		::DestroyIcon(m_hIcon);
	if (m_bFont)		m_Font.DeleteObject();
	if (m_hBitmap)		::DeleteObject(m_hBitmap);
	if (hinst_msimg32)	::FreeLibrary( hinst_msimg32 );
}

/////////////////////////////////////////////////////////////////////////////////
//// PreSubclassWindow
void CxStatic::PreSubclassWindow()
{
	//TRACE(_T("in CxStatic::PreSubclassWindow\n"));
	
	m_dwTxtFlags = GetStyle();
	ModifyStyle(SS_TYPEMASK, SS_OWNERDRAW);

	if (m_bFont) m_Font.DeleteObject();

	// get current font
	CFont* pFont = GetFont();
	if (!pFont)
	{
		HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		if (hFont == NULL)
			hFont = (HFONT) GetStockObject(ANSI_VAR_FONT);
		if (hFont)
			pFont = CFont::FromHandle(hFont);
	}
	ASSERT(pFont);
	ASSERT(pFont->GetSafeHandle());

	// create the font for this control
	LOGFONT lf;
	pFont->GetLogFont(&lf);

	m_Font.CreateFontIndirect(&lf);
	m_bFont = 1;

	this->GetWindowText(m_strText);

	DragAcceptFiles(TRUE);
	Invalidate();
}

void CxStatic::SetMoveable(BOOL moveAble)
{
	ModifyStyle(0, SS_NOTIFY);
	m_bAllowMove = moveAble;
}

///////////////////////////////////////////////////////////////////////////////
// SetBackgroundColor
void CxStatic::SetBkColor(COLORREF rgbBkgnd, COLORREF rgbBkgndHigh, BackFillMode mode, float fSplitGraident )
{
	
	if ((m_crLoColor == rgbBkgnd) && (m_crHiColor == rgbBkgndHigh) 
		&& (m_fillmode == mode))
	{
		return;
	}

	if (m_bTransparentBk)
	{
		m_bTransparentBk = FALSE;
		ModifyStyleEx(0,WS_EX_TRANSPARENT);
		RedrawWindow();
	}

	m_fLevelSplitGradient = fSplitGraident;
	m_crLoColor = rgbBkgnd;
	m_crHiColor = rgbBkgndHigh;
	m_fillmode = mode;

	m_rgbBkgnd = rgbBkgnd;
	RedrawWindow();
}

///////////////////////////////////////////////////////////////////////////////
// GlassColor
void CxStatic::SetGlassColor(COLORREF rgbBkgnd,float fSplitGraident )
{
	if (m_bTransparentBk)
	{
		m_bTransparentBk = FALSE;
		ModifyStyleEx(0,WS_EX_TRANSPARENT);
		RedrawWindow();
	}
	m_fLevelSplitGradient = fSplitGraident;
	m_crLoColor = rgbBkgnd;
	m_fillmode  = VGradient;
	m_rgbBkgnd  = rgbBkgnd;
	
	RedrawWindow();
}



/////////////////////////////////////////////////////////////////////////////////
//// SetTransparent
void CxStatic::SetTransparent(BOOL bTranspMode)
{
	if (m_bTransparentBk == bTranspMode) return;
	
	m_bTransparentBk = bTranspMode;
	ModifyStyleEx(0,WS_EX_TRANSPARENT);
	RedrawWindow();
}

void CxStatic::SetTextColor(COLORREF col)
{
	if (m_rgbText == col) return;

	m_rgbText = col;
	RedrawWindow();
}

void CxStatic::SetColor(COLORREF fcol, COLORREF bcol)
{
	SetTextColor(fcol);
	SetBkColor(bcol);
}

void CxStatic::SetAutoAdjustFont(BOOL bAutoAdjustFont)
{
	m_bAutoAdjustFont = bAutoAdjustFont;
}

void CxStatic::SetRounded(BOOL bRounded, CPoint ptRound)
{
	m_bRounded = bRounded;
	m_ptRound  = ptRound;
}

/////////////////////////////////////////////////////////////////////////////////
//// FONT

//void CxStatic::ReconstructFont()
//{
//	m_Font.DeleteObject();
//	BOOL bCreated = m_Font.CreateFontIndirect(&m_lf);
//	ASSERT(bCreated);
//
//	RedrawWindow();
//}

void CxStatic::SetFont(const CString& strFont,int nPointSize, int nWeight,
					   BOOL bRedraw /*= TRUE*/)
{
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));       // Zero out the structure.

	wcscpy_s(lf.lfFaceName, (LPCTSTR)strFont );			// Set Font Familly
	//strcpy_s(lf.lfFaceName, (LPCTSTR)strFont );			// Set Font Familly
	lf.lfHeight = nPointSize;// Set Height
	lf.lfWeight = nWeight;					// Set Weight

	SetFont(&lf, bRedraw);

	//m_csFontFamilly = strFont;
	//m_nFontSizeVar = m_nFontSize = nPointSize;
}

void CxStatic::SetFont(LOGFONT *pLogFont, BOOL bRedraw /*= TRUE*/)
{
	ASSERT(pLogFont);
	if (!pLogFont)
		return;

	if (m_bFont) m_Font.DeleteObject();

	LOGFONT lf = *pLogFont;

	m_Font.CreateFontIndirect(&lf);
	m_bFont = 1;

	if (bRedraw) RedrawWindow();
}

void CxStatic::SetFont(CFont *pFont, BOOL bRedraw /*= TRUE*/)
{
	ASSERT(pFont);
	if (!pFont)
		return;

	LOGFONT lf;
	memset(&lf, 0, sizeof(lf));

	pFont->GetLogFont(&lf);

	SetFont(&lf, bRedraw);
}

// void CxStatic::setFont(LONG fontHeight, LONG fontWeight, UCHAR pitchAndFamily, LPCSTR faceName)
// {
// 	if (m_bFont) m_Font.DeleteObject();
// 
// 	//const CLogFont lf(fontHeight, fontWeight, pitchAndFamily, faceName);
// // 	
// // 	m_Font.CreateFontIndirect(&lf);
// // 	m_bFont = 1;
// 
// 	RedrawWindow();
// }

LRESULT CxStatic::OnSetFont(WPARAM wParam, LPARAM lParam)
{
	LRESULT lrReturn(Default());

	SetFont(CFont::FromHandle((HFONT)wParam) );

	RedrawWindow();

	return lrReturn;
}


void CxStatic::SetFont3D(BOOL bFont3D, Type3D type)
{
	m_bFont3d = bFont3D;
	m_3dType = type;

	RedrawWindow();
}


LRESULT CxStatic::OnSetText(WPARAM wParam, LPARAM lParam)
{
	LRESULT lrReturn(Default());

	this->GetWindowText(m_strText);

	return lrReturn;
}

BOOL CxStatic::SetBitmap(HBITMAP hBitmap, ImageSize Emode, COLORREF rgbTransparent)
{
	m_bBitmap = TRUE;
	::DeleteObject(m_hBitmap);
	m_hBitmap = hBitmap;

	if (Emode == OriginalSize)
	{
		CRect Rect;
		GetWindowRect(&Rect); // x,y -> screen
		ScreenToClient(&Rect); // screen -> to client ( view ou dialog)
	
		Rect.InflateRect(Rect.Width(),Rect.Height());
		SetWindowPos( NULL,0,0,Rect.Width(),Rect.Height(),SWP_NOMOVE | SWP_NOZORDER); //
	}

	Invalidate();

	return ::GetObject(m_hBitmap, sizeof(BITMAP), &m_bmInfo);
}

BOOL CxStatic::ResetBitmap()
{
	m_bBitmap = TRUE;
	::DeleteObject(m_hBitmap);

	Invalidate();
	return ::GetObject(m_hBitmap, sizeof(BITMAP), &m_bmInfo);
}

BOOL CxStatic::SetBitmap(UINT nIDResource, ImageSize Emode, COLORREF rgbTransparent)
{
	m_nResourceID = nIDResource;
	m_strResourceName.Empty();
	m_bTransparentBk = FALSE;

	HBITMAP hBmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(nIDResource),
		IMAGE_BITMAP,
		0,0,
		LR_LOADMAP3DCOLORS);
	if (!hBmp) return FALSE;
	return CxStatic::SetBitmap(hBmp, Emode, rgbTransparent);
}

BOOL CxStatic::SetBitmap(LPCTSTR lpszResourceName, ImageSize Emode, COLORREF rgbTransparent)
{
	m_nResourceID = -1;
	m_strResourceName = lpszResourceName;
	
	HBITMAP hBmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),
		lpszResourceName,
		IMAGE_BITMAP,
		0,0,
		LR_LOADFROMFILE);
	if (!hBmp) return FALSE;
	
	return CxStatic::SetBitmap(hBmp, Emode, rgbTransparent);
}

///////////////////////////////////////////////////////////////////////////////
// GetFontPointSize()
int CxStatic::GetFontPointSize(int nHeight)
{
	HDC hdc = ::CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
	ASSERT(hdc);
	int cyPixelsPerInch = ::GetDeviceCaps(hdc, LOGPIXELSY);
	::DeleteDC(hdc);

	int nPointSize = MulDiv(nHeight, 72, cyPixelsPerInch);
	if (nPointSize < 0)
		nPointSize = -nPointSize;

	return nPointSize;
}

///////////////////////////////////////////////////////////////////////////////
// GetFontHeight()
int CxStatic::GetFontHeight(int nPointSize)
{
	HDC hdc = ::CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
	ASSERT(hdc);
	int cyPixelsPerInch = ::GetDeviceCaps(hdc, LOGPIXELSY);
	::DeleteDC(hdc);

	int nHeight = -MulDiv(nPointSize, cyPixelsPerInch, 72);

	return nHeight;
}

void CxStatic::GetText(CString &strText)
{
	strText = m_strText;
}

void CxStatic::SetWindowText( LPCTSTR strText)
{
	if (m_strText == strText) return;

	m_strText = strText;

	CRect Rect;
	GetClientRect(&Rect);

	if ( m_bTransparentBk )
	{
		ClientToScreen(&Rect);
		Rect.InflateRect(1,1,1,1);
		CWnd *Parent = GetParent();
		Parent->ScreenToClient(&Rect);
		Parent->InvalidateRect(&Rect);
		Parent->UpdateWindow();
		//SendMessage(WM_ERASEBKGND);
	}
	else
		RedrawWindow();
}

void CxStatic::Format(LPCTSTR szFmt, ...)
{
	ULONG	bytesWriten;
	TCHAR	szBuffer[2048];
	va_list args;

	va_start(args, szFmt);
	bytesWriten = _vstprintf_s(szBuffer, szFmt, args);

	m_strText = szBuffer;

	va_end(args);

	RedrawWindow();
}

void CxStatic::AppendText(LPCTSTR szFmt, ...)
{
	ULONG	bytesWriten;
	TCHAR	szBuffer[2048];
	va_list args;

	va_start(args, szFmt);
	bytesWriten = _vstprintf_s(szBuffer, szFmt, args);

	m_strText += szBuffer;

	va_end(args);

	RedrawWindow();

}

BOOL CxStatic::RedrawWindow()
{
	/*CRect rect;

	GetClientRect( rect );
	InvalidateRect( rect );*/
	Invalidate();
	return TRUE;
}

void CxStatic::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	ASSERT(lpDrawItemStruct != NULL);

	CBitmap		bmp;
	CBitmap*	pOldBitmap = NULL;

	CDC*  pDC     = CDC::FromHandle(lpDrawItemStruct->hDC);
    CRect rect    = lpDrawItemStruct->rcItem;

	//Double Buffering - Modify MemDC for transparent background but doesn't work
	// because I have to invalidate parent when SetWindowText - Need to find a fix!!!
	pDC->SetBkColor(m_rgbBkgnd);

	CMemDCEx MemDC(pDC, &rect, m_bRounded?TRUE:m_bTransparentBk);
	CBrush fill_bursh(m_rgbBkgnd);

	// PAINT SOLID BACKGROUND IF NO BITMAP
	
	if ( !m_bTransparentBk )
	{
		if (m_fillmode == Normal)
		{
			CRect rectBuff(rect);
			if (m_bRounded)
			{
				CPen pen(PS_SOLID, 1, m_rgbBorder), *pOldPen;
				CBrush brush(m_rgbBorder), *pOldBrush;

				pOldBrush = MemDC.SelectObject(&brush);
				pOldPen = MemDC.SelectObject(&pen);

				MemDC.RoundRect(rectBuff, CPoint(10, 10));
				rectBuff.DeflateRect(1, 1, 1, 1);

				pen.DeleteObject();
				brush.DeleteObject();

				pen.CreatePen(PS_SOLID, 1, m_rgbBkgnd);
				brush.CreateSolidBrush(m_rgbBkgnd);
				MemDC.SelectObject(&brush);
				MemDC.RoundRect(rectBuff, CPoint(10, 10));
				MemDC.SelectObject(pOldBrush);
				MemDC.SelectObject(pOldPen);

				pen.DeleteObject();
				brush.DeleteObject();
			}
			else
			{
				if (m_bBorder)
				{
					CBrush brBroder(m_rgbBorder);

					MemDC.FillRect(&rectBuff, &brBroder);
					rect.DeflateRect(1, 1, 1, 1);
				}
				MemDC.FillRect(&rectBuff, &fill_bursh);
			}
		}
		else DrawGradientFill(&MemDC, &rect, m_fillmode);
	}

	if (m_bBitmap != NULL)
	{
		DrawTheBitmap(&MemDC, !m_strText.IsEmpty(), &rect, &rect);
	}

	// TEXT RENDERING
	CFont* pOldFont  = MemDC.SelectObject( &m_Font );
	COLORREF rgbText = MemDC.SetTextColor(m_rgbText);

	if (m_hIcon != NULL)
	{
		DrawTheIcon(&MemDC, !m_strText.IsEmpty(), &rect, &rect);
	} // if

	if (m_strText.IsEmpty() == FALSE)
	{
//		DrawTheText(pDC, (LPCTSTR)sTitle, &lpDIS->rcItem, &captionRect, m_bIsPressed, m_bIsDisabled);
	} // if

	DrawText(&MemDC, &rect, m_strText);

	// Restore DC's State
	MemDC.SelectObject(pOldFont);
	MemDC.SetTextColor(rgbText);

	pOldFont->DeleteObject();
}


void CxStatic::DrawText(CDC* pDCMem, CRect* pRect, CString csText)
{
	DWORD dwStyle = m_dwTxtFlags;
	DWORD dwFlags = 0;

	// Map "Static Styles" to "Text Styles" - WARNING MACROS
#define MAP_STYLE(src, dest) if (dwStyle & (src)) dwFlags |= (dest)
#define NMAP_STYLE(src, dest) if (!(dwStyle & (src))) dwFlags |= (dest)

	MAP_STYLE(	SS_RIGHT,			DT_RIGHT   					);
	MAP_STYLE(	SS_CENTER,			DT_CENTER					);
	MAP_STYLE(	SS_LEFT,			DT_LEFT					    );
	//MAP_STYLE(	SS_CENTERIMAGE,		DT_VCENTER | DT_SINGLELINE	);
	MAP_STYLE(	SS_NOPREFIX,		DT_NOPREFIX					);
	MAP_STYLE(	SS_WORDELLIPSIS,	DT_WORD_ELLIPSIS			);
	MAP_STYLE(	SS_ENDELLIPSIS,		DT_END_ELLIPSIS				);
	MAP_STYLE(	SS_PATHELLIPSIS,	DT_PATH_ELLIPSIS			);

	// TAb expansion
	if (csText.Find( _T('\t') ) != -1)
		dwFlags |= DT_EXPANDTABS;

	//csText.Replace(
	// TEXT WRAPPING - Inspired by Chen-Cha Hsu and improved
	CRect		newRC;
	TEXTMETRIC	tag;
	CSize		sz;
	::GetTextExtentPoint32(pDCMem->GetSafeHdc(), csText,csText.GetLength(), &sz);
	CStringArray	asText;
	int				nLine = 0;
	CString			s2;
	int				nIdx = 0, nX = 0;
	int				nId = 0;
	char			nCR = 0;

	// Autowrapping mode enabled
	if ( m_bAutoWrapping )
	{
		std::string szText = (CT2CA(csText));
		std::string szC;
		for (auto it = szText.begin(); it != szText.end();)
		{
			CString cs(szC.c_str());
			if (pDCMem->GetTextExtent(cs, szC.length()).cx > pRect->Width())
			{
				std::string szBuff;
				std::string::size_type pos;
				pos = szC.rfind(" ");
				if (pos == std::string::npos)
				{
					pos  = szC.length() - 1;
					pos -= (unsigned char) szC.back() > 0x7F?2:1;
				}
				szBuff = szC.substr(pos, szC.length() - 1);
				szC.erase(pos);
				CString cs(szC.c_str());
				asText.Add(cs);
				szC = szBuff;
			}
			if (*it == '\n')
			{
				CString cs(szC.c_str());
				asText.Add(cs);
				szC.clear();
			}
			else
			{
				szC.append(1, *it);
				if ((unsigned char) szC.back() > 0x7F && (it + 1) != szText.end())
				{
					szC.append(1, *++it);
				}
			}
			++it;
		}
		if (szC.length()) {
			CString cs(szC.c_str());
			asText.Add(cs);
		}


		if (0)
		{
			for (int i = 1; i <= csText.GetLength(); i++)
			{
				s2 = csText.Left(i);
				//CARRIAGE RETURN not recognised with SS_CENTERIMAGE - manual handling
				if (csText.GetAt(i-1) == '\r' || csText.GetAt(i-1) == '\n')
				{
					if (nCR == 0)
					{
						nCR = (char)csText.GetAt(i-1);
					}
					else if (nCR != csText.GetAt(i-1)) // "\r\n" or "\n\r"
					{ 
						s2 = csText.Left(i-2);
						asText.Add(s2);
						csText = csText.Mid(i);
						i = nCR = 0;
					}
				}

				::GetTextExtentPoint32(pDCMem->GetSafeHdc(), s2, s2.GetLength(), &sz);

				if ( sz.cx > pRect->Width() ) // We found how many letters fits
				{
					s2 = csText.Left(i-1);
					if ( IsASingleWord(s2) )
					{
						asText.Add(s2);
						csText = csText.Mid(i-1);
						i = 0;
					}
					else // Do not break a word
					{ 
						nId = s2.ReverseFind(' ');
						s2 = s2.Left(nId);
						asText.Add(s2);
						csText = csText.Mid(nId + 1);
						i = 0;
					}
				}
			}
			if ( ! csText.IsEmpty() )
				asText.Add(csText);
		}
	}
	else // Standard CStatic behaviour
	{
		asText.SetAtGrow(0, csText);
	}

	nLine = asText.GetSize();
	pDCMem->GetTextMetrics(&tag);
	newRC = *pRect;

	LONG nDiff = pRect->bottom - pRect->top - tag.tmHeight * nLine;

	if (dwStyle & SS_CENTERIMAGE) pRect->top = nDiff/2;
	if (!(dwStyle & SS_CENTER) && !(dwStyle & SS_RIGHT)) newRC.left += 5;

	//TRACE( "The value of nDiff is %d\n", nDiff );
	if (m_bAutoAdjustFont)
	{
		if (nDiff < 0)
		{
			m_nFontSizeVar--;
			SetFont( m_lf.lfFaceName, m_nFontSizeVar, m_lf.lfWeight );
		}
		//pDCMem->SelectObject( m_pFont ); TODO CHECK WITH AUTOADJUST
		//RedrawWindow();
	}

	for (int j = 0; j < nLine; j++)
	{
		newRC.top = pRect->top + tag.tmHeight * j;

		pDCMem->DrawText(asText[j], &newRC,dwFlags);

		if (m_bFont3d)
		{
			if (m_3dType == Raised)	newRC.OffsetRect(-1,-1);
			else					newRC.OffsetRect(1,1);

			pDCMem->DrawText(asText[j], &newRC,dwFlags);
		}
	}
}

BOOL CxStatic::IsASingleWord(const CString & csText)
{
	char	cEnd = 0;

	cEnd = (char)csText.GetAt(csText.GetLength() - 1);
	if ( ( csText.Find(_T(" ")) == -1 ) || (cEnd == ' ') )
		return TRUE;
	else
		return FALSE;
}

void CxStatic::DrawTheBitmap(CDC* pDCMem, BOOL bHasTitle, RECT* rpItem, CRect* rpCaption)
{
	CDC dcMem;
	CRect	rImage;
	RECT r;

	if (!m_hBitmap)
		return;

	VERIFY( dcMem.CreateCompatibleDC(pDCMem) );

	r.left = 0;
	r.right = m_bmInfo.bmWidth;
	r.top = 0;
	r.bottom = m_bmInfo.bmHeight;

	PrepareImageRect(bHasTitle, rpItem, rpCaption, m_bmInfo.bmWidth, m_bmInfo.bmHeight, &rImage);

	// Select bitmap into memory DC.
	HBITMAP* pBmpOld = (HBITMAP*) ::SelectObject(dcMem.m_hDC, m_hBitmap);

	pDCMem->StretchBlt(rImage.left, rImage.top, rImage.Width(), rImage.Height(),
		&dcMem, 0, 0, m_bmInfo.bmWidth-1, m_bmInfo.bmHeight-1,
		SRCCOPY);
}


void CxStatic::DrawBitmap(CDC* pDCMem, CRect* pRect)
{
	CDC dcMem;
	CRect rect;
    GetClientRect( rect );

	CRect child_rect;
	GetWindowRect( child_rect );

	CRect parent_rect;
	GetParent()->GetWindowRect(parent_rect);

	if (!m_hBitmap)
		return;

	VERIFY( dcMem.CreateCompatibleDC(pDCMem) );
	m_bTransparentBk = FALSE;

	// Select bitmap into memory DC.
	HBITMAP* pBmpOld = (HBITMAP*) ::SelectObject(dcMem.m_hDC, m_hBitmap);

	pDCMem->StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(),
		&dcMem, 0, 0, m_bmInfo.bmWidth-1, m_bmInfo.bmHeight-1,
		SRCCOPY);
}



void CxStatic::DrawGradientFill(CDC* pDCMem, CRect* pRect, BackFillMode FillMode)
{
	TRIVERTEX rcVertex[2];
	TRIVERTEX rcVertex1[2];

	GRADIENT_RECT grect;
	grect.UpperLeft  = 0;
	grect.LowerRight = 1;
	
	UINT  GRADIENT_DARK_LEVEL0 = 40;
	UINT  GRADIENT_DARK_LEVEL1 = 20;
	UINT  GRADIENT_DARK_LEVEL2 = 20;

	if(FillMode == VGradient)
	{
		COLOR16 clRed, clGreen, clBlue;

		//--------------------------------------------------------------------
		clRed  = (GetRValue(m_crLoColor)+GRADIENT_DARK_LEVEL0); if(clRed  > 255) clRed   =255; 
		clGreen= (GetGValue(m_crLoColor)+GRADIENT_DARK_LEVEL0); if(clGreen> 255) clGreen =255; 
		clBlue = (GetBValue(m_crLoColor)+GRADIENT_DARK_LEVEL0); if(clBlue > 255) clBlue  =255; 

		rcVertex[0].x = pRect->left;
		rcVertex[0].y = pRect->top ;
		rcVertex[0].Red   = clRed  <<8;
		rcVertex[0].Green = clGreen<<8;
		rcVertex[0].Blue  = clBlue <<8;
		rcVertex[0].Alpha = 0x0000;

		clRed  = (GetRValue(m_crLoColor)); 
		clGreen= (GetGValue(m_crLoColor)); 
		clBlue = (GetBValue(m_crLoColor)); 

		rcVertex[1].x = pRect->right;
		rcVertex[1].y = UINT(pRect->bottom*m_fLevelSplitGradient);
		rcVertex[1].Red   = clRed  <<8;
		rcVertex[1].Green = clGreen<<8;
		rcVertex[1].Blue  = clBlue <<8;
		rcVertex[1].Alpha = 0;
		//--------------------------------------------------------------------------------
		clRed  = (GetRValue(m_crLoColor)-GRADIENT_DARK_LEVEL1); if(clRed  < 0) clRed   =0; 
		clGreen= (GetGValue(m_crLoColor)-GRADIENT_DARK_LEVEL1); if(clGreen< 0) clGreen =0; 
		clBlue = (GetBValue(m_crLoColor)-GRADIENT_DARK_LEVEL1); if(clBlue < 0) clBlue  =0; 

		rcVertex1[0].x = pRect->left;
		rcVertex1[0].y = UINT(pRect->top +pRect->bottom*m_fLevelSplitGradient);
		rcVertex1[0].Red   = clRed  <<8;
		rcVertex1[0].Green = clGreen<<8;
		rcVertex1[0].Blue  = clBlue <<8;
		rcVertex1[0].Alpha = 0x0000;
		
		clRed  = (GetRValue(m_crLoColor)+GRADIENT_DARK_LEVEL2); if(clRed  > 255) clRed   =255;
		clGreen= (GetGValue(m_crLoColor)+GRADIENT_DARK_LEVEL2); if(clGreen> 255) clGreen =255;
		clBlue = (GetBValue(m_crLoColor)+GRADIENT_DARK_LEVEL2); if(clBlue > 255) clBlue  =255;

		rcVertex1[1].x = pRect->right;
		rcVertex1[1].y = pRect->bottom;
		rcVertex1[1].Red   = clRed  <<8;
		rcVertex1[1].Green = clGreen<<8;
		rcVertex1[1].Blue  = clBlue <<8;
		rcVertex1[1].Alpha = 0;
		//-----------------------------------------------------------------------------------
		dllfunc_GradientFill( *pDCMem ,rcVertex1,2,&grect,1,GRADIENT_FILL_RECT_V);
	} else {
		rcVertex[0].x = pRect->left;
		rcVertex[0].y = pRect->top;
		rcVertex[0].Red   = GetRValue(m_crHiColor)<<8;
		rcVertex[0].Green = GetGValue(m_crHiColor)<<8;
		rcVertex[0].Blue  = GetBValue(m_crHiColor)<<8;
		rcVertex[0].Alpha = 0x0000;
		rcVertex[1].x = pRect->right;
		rcVertex[1].y = pRect->bottom;
		rcVertex[1].Red   = GetRValue(m_crLoColor)<<8;
		rcVertex[1].Green = GetGValue(m_crLoColor)<<8;
		rcVertex[1].Blue  = GetBValue(m_crLoColor)<<8;
		rcVertex[1].Alpha = 0;
	}

	dllfunc_GradientFill( *pDCMem ,rcVertex,2,&grect,1,
		FillMode == HGradient ? GRADIENT_FILL_RECT_H :  GRADIENT_FILL_RECT_V);
}


///////////////////////////////////////////////////////////////////////////////
// OnEraseBkgnd
BOOL CxStatic::OnEraseBkgnd(CDC* pDC)
{

	return FALSE; //CStatic::OnEraseBkgnd(pDC);
}

void CxStatic::OnSize(UINT nType, int cx, int cy)
{
	CStatic::OnSize(nType, cx, cy);
	Invalidate();
}


//HBRUSH CxStatic::CtlColor(CDC* pDC, UINT nCtlColor)
//{
//	pDC->SetBkMode(TRANSPARENT);
//	HBRUSH brush = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
//	return brush;
//}

//Credits goes to Aquiruse http://www.thecodeproject.com/buttonctrl/hoverbuttonex.asp
void CxStatic::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bTracking){
		/*TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
		m_point = point;*/
	}
	CStatic::OnMouseMove(nFlags, point);
}

LRESULT CxStatic::OnMouseHover(WPARAM wparam, LPARAM lparam)
{
	m_bHover=TRUE;
	::SetActiveWindow(GetParent()->GetSafeHwnd());
	Invalidate();

	return 0;
}

LRESULT CxStatic::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bTracking = FALSE;
	m_bHover=FALSE;
	Invalidate();

	return 0;
}

void CxStatic::OnDropFiles(HDROP hDropInfo)
{
	int		nFiles = 0;
	TCHAR	szFile[MAX_PATH];
	TCHAR*	ext;


	nFiles = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);

	if (nFiles != 1)
		return;

	DragQueryFile(hDropInfo, 0, szFile, sizeof(szFile));
	if (ext = _tcsrchr(szFile, '.'))	{
		if (_tcsicmp(ext, _T(".bmp")) == 0){
			SetBitmap(szFile);
		}
	}

	DragFinish(hDropInfo);

}

void CxStatic::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (IsMoveable() == TRUE)
	{
		m_point = point;
		CRect rect;
		GetWindowRect( rect );
		ScreenToClient( rect );
		m_pTrack = new CRectTracker(&rect, CRectTracker::dottedLine |
								CRectTracker::resizeInside |
								CRectTracker::hatchedBorder);
		// RIGHT-CLICK + CONTROL
		if (nFlags & MK_CONTROL){
			GetWindowRect(rect);
			GetParent()->ScreenToClient(rect);
			m_pTrack->TrackRubberBand(GetParent(), rect.TopLeft());
			m_pTrack->m_rect.NormalizeRect();
		}
		else {
			m_pTrack->Track(GetParent(), point);
		}
		rect = LPRECT(m_pTrack->m_rect);
		MoveWindow(&rect,TRUE);//Move Window to our new position		
		
		// Clean our mess
		if (m_pTrack)
			delete m_pTrack;

		rect = NULL;
	}

	CStatic::OnRButtonDown(nFlags, point);
}

HICON CxStatic::CreateGrayscaleIcon(HICON hIcon)
{
	HICON		hGrayIcon = NULL;
	HDC			hMainDC = NULL, hMemDC1 = NULL, hMemDC2 = NULL;
	BITMAP		bmp;
	HBITMAP		hOldBmp1 = NULL, hOldBmp2 = NULL;
	ICONINFO	csII, csGrayII;
	BOOL		bRetValue = FALSE;

	bRetValue = ::GetIconInfo(hIcon, &csII);
	if (bRetValue == FALSE)	return NULL;

	hMainDC = ::GetDC(NULL);
	hMemDC1 = ::CreateCompatibleDC(hMainDC);
	hMemDC2 = ::CreateCompatibleDC(hMainDC);
	if (hMainDC == NULL || hMemDC1 == NULL || hMemDC2 == NULL)	return NULL;
  
	if (::GetObject(csII.hbmColor, sizeof(BITMAP), &bmp))
	{
		DWORD	dwWidth = csII.xHotspot*2;
		DWORD	dwHeight = csII.yHotspot*2;

		csGrayII.hbmColor = ::CreateBitmap(dwWidth, dwHeight, bmp.bmPlanes, bmp.bmBitsPixel, NULL);
		if (csGrayII.hbmColor)
		{
			hOldBmp1 = (HBITMAP)::SelectObject(hMemDC1, csII.hbmColor);
			hOldBmp2 = (HBITMAP)::SelectObject(hMemDC2, csGrayII.hbmColor);

			//::BitBlt(hMemDC2, 0, 0, dwWidth, dwHeight, hMemDC1, 0, 0, SRCCOPY);

			DWORD		dwLoopY = 0, dwLoopX = 0;
			COLORREF	crPixel = 0;
			BYTE		byNewPixel = 0;

			for (dwLoopY = 0; dwLoopY < dwHeight; dwLoopY++)
			{
				for (dwLoopX = 0; dwLoopX < dwWidth; dwLoopX++)
				{
					crPixel = ::GetPixel(hMemDC1, dwLoopX, dwLoopY);

					byNewPixel = (BYTE)((GetRValue(crPixel) * 0.299) + (GetGValue(crPixel) * 0.587) + (GetBValue(crPixel) * 0.114));
					if (crPixel)	::SetPixel(hMemDC2, dwLoopX, dwLoopY, RGB(byNewPixel, byNewPixel, byNewPixel));
				} // for
			} // for

			::SelectObject(hMemDC1, hOldBmp1);
			::SelectObject(hMemDC2, hOldBmp2);

			csGrayII.hbmMask = csII.hbmMask;

			csGrayII.fIcon = TRUE;
			hGrayIcon = ::CreateIconIndirect(&csGrayII);
		} // if

		::DeleteObject(csGrayII.hbmColor);
		//::DeleteObject(csGrayII.hbmMask);
	} // if

	::DeleteObject(csII.hbmColor);
	::DeleteObject(csII.hbmMask);
	::DeleteDC(hMemDC1);
	::DeleteDC(hMemDC2);
	::ReleaseDC(NULL, hMainDC);

	return hGrayIcon;
} // End of CreateGrayscaleIcon

void CxStatic::SetIcon(int nIcon)
{
	HICON		hIcon			= NULL;
	HINSTANCE	hInstResource	= NULL;

	if (m_nIcon == nIcon) return;

	m_nIcon = nIcon;
	// Find correct resource handle
	hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(nIcon), RT_GROUP_ICON);

	// Set icon when the mouse is IN the button
	hIcon = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(nIcon), IMAGE_ICON, 0, 0, 0);

  	// Set icon when the mouse is OUT the button

	SetIcon(hIcon);
} // End of SetIcon

void CxStatic::DeleteIcon()
{
	if (m_hIcon)	::DestroyIcon(m_hIcon);
	m_hIcon = NULL;
}

void CxStatic::SetIcon(HICON hIcon)
{
	BOOL		bRetValue;
	ICONINFO	ii;

	// Free any loaded resource
	if (m_hIcon)	::DestroyIcon(m_hIcon);

	m_hIcon = hIcon;
	// Get icon dimension
	::ZeroMemory(&ii, sizeof(ICONINFO));
	bRetValue = ::GetIconInfo(hIcon, &ii);

	if (bRetValue == FALSE)
	{
		if (m_hIcon) ::DestroyIcon(m_hIcon);
		return;
	} // if

	m_dwIconWidth	= (DWORD)(ii.xHotspot * 2);

	m_dwIconHeight	= (DWORD)(ii.yHotspot * 2);
	::DeleteObject(ii.hbmMask);
	::DeleteObject(ii.hbmColor);

	Invalidate();
} // End of SetIcon

void CxStatic::DrawTheIcon(CDC* pDC, BOOL bHasTitle, RECT* rpItem, CRect* rpCaption)
{
	CRect	rImage;
	PrepareImageRect(bHasTitle, rpItem, rpCaption, m_dwIconWidth, m_dwIconHeight, &rImage);

	// Ole'!
	pDC->DrawState(rImage.TopLeft(), rImage.Size(), m_hIcon, DSS_NORMAL, (CBrush*)NULL);
} // End of DrawTheIcon

void CxStatic::PrepareImageRect(BOOL bHasTitle, RECT* rpItem, CRect* rpTitle, DWORD dwWidth, DWORD dwHeight, CRect* rpImage)
{
	CRect rBtn;

	rpImage->CopyRect(rpItem);

	switch (m_byAlign)
	{
		case ALIGN_HORIZ:
			if (bHasTitle == FALSE)
			{
				// Center image horizontally
				rpImage->left += ((rpImage->Width() - (long)dwWidth)/2);
			}
			else
			{
				// Image must be placed just inside the focus rect
				rpImage->left += m_ptImageOrg.x;
				rpTitle->left += dwWidth + m_ptImageOrg.x;
			}
			// Center image vertically
			rpImage->top += ((rpImage->Height() - (long)dwHeight)/2);
			break;

		case ALIGN_HORIZ_RIGHT:
			GetClientRect(&rBtn);
			if (bHasTitle == FALSE)
			{
				// Center image horizontally
				rpImage->left += ((rpImage->Width() - (long)dwWidth)/2);
			}
			else
			{
				// Image must be placed just inside the focus rect
				rpTitle->right = rpTitle->Width() - dwWidth - m_ptImageOrg.x;
				rpTitle->left = m_ptImageOrg.x;
				rpImage->left = rBtn.right - dwWidth - m_ptImageOrg.x;
				// Center image vertically
				rpImage->top += ((rpImage->Height() - (long)dwHeight)/2);
			}
			break;
		
		case ALIGN_VERT:
			// Center image horizontally
			rpImage->left += ((rpImage->Width() - (long)dwWidth)/2);
			if (bHasTitle == FALSE)
			{
				// Center image vertically
				rpImage->top += ((rpImage->Height() - (long)dwHeight)/2);           
			}
			else
			{
				rpImage->top = m_ptImageOrg.y;
				rpTitle->top -= dwHeight;
			}
			break;
		case ALIGN_OVERLAP:
			rpImage->left += ((rpImage->Width() - (long)dwWidth)/2);
			rpImage->top += ((rpImage->Height() - (long)dwHeight)/2);
			break;
	} // switch
    
	rpImage->right = rpImage->left + dwWidth;
	rpImage->bottom = rpImage->top + dwHeight;
	
} // End of PrepareImageRect

void CxStatic::SetAlign(BYTE byAlign, BOOL bRepaint)
{
	switch (byAlign)
	{    
		case ALIGN_HORIZ:
		case ALIGN_HORIZ_RIGHT:
		case ALIGN_VERT:
		case ALIGN_OVERLAP:
			m_byAlign = byAlign;
			if (bRepaint)	Invalidate();
			break;
	} // switch
} // End of SetAlign

void CxStatic::DrawTheText(CDC* pDC, LPCTSTR lpszText, RECT* rpItem, CRect* rpCaption)
{
	// Center text
	CRect centerRect = rpCaption;

	pDC->DrawText(lpszText, -1, rpCaption, DT_WORDBREAK | DT_CENTER | DT_CALCRECT);
	rpCaption->OffsetRect((centerRect.Width() - rpCaption->Width())/2, (centerRect.Height() - rpCaption->Height())/2);

	pDC->SetBkMode(TRANSPARENT);

	pDC->SetTextColor(m_rgbText);
	pDC->SetBkColor(m_crLoColor);
	pDC->DrawText(lpszText, -1, rpCaption, DT_WORDBREAK | DT_CENTER);

} // End of DrawTheText

void CxStatic::FontSet(int Size, int Bold, LPTSTR Styel, BYTE bItalic)
{
	if (m_bFont) m_Font.DeleteObject();
	
	m_Font.CreateFont( Size, 0, 0, 0, Bold, 
					 bItalic, FALSE, FALSE, ANSI_CHARSET,
					 OUT_TT_PRECIS, CLIP_TT_ALWAYS, PROOF_QUALITY,
					 DEFAULT_PITCH, Styel);
	m_bFont = 1;
	SetFont(&m_Font);
}
