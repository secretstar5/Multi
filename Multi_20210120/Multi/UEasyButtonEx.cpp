// UEasyButtonEx.cpp : implementation file
//

#include "pch.h"
#include "UEasyButtonEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// UEasyButtonEx

UEasyButtonEx::UEasyButtonEx()
{
	m_bClick			= 0;
	m_bIsFlat			= 1;	
	m_bIsCheckBox		= 0;
	m_bIsFocused		= 0;
	m_bIsPressed		= 0;
	m_bIsDisabled		= 0;
	m_bIsDefault		= 0;
	m_bMouseOnButton	= 0;
	m_bDrawBorder		= 1;
	m_bAlwaysTrack		= 1;
	m_hCursor			= 0;
	m_ToolTip.m_hWnd	= 0;
	m_dwToolTipStyle	= 0;
	m_nAlignMode		= 0;
	m_bDrawFlatFocus	= 0;
	m_bShowDisabledBitmap = 1;

	memset( &m_nPressedOffset, 0, sizeof(m_nPressedOffset) );
	m_nImageOrg.x		= 3;
	m_nImageOrg.y		= 3;

	FreeResource(FALSE);
	SetDefaultColor(FALSE);
	SetPressedStyle(PRESSED_LEFTRIGHT, FALSE);
	OffsetColor(COLOR_BG_IN, 50, FALSE);

	m_clrSelectColor	= RGB(127,255,0);
	m_nSelection		= 0;
}

UEasyButtonEx::~UEasyButtonEx()
{
	FreeResource();

	if( m_hCursor )
		::DestroyCursor( m_hCursor );
}


BEGIN_MESSAGE_MAP(UEasyButtonEx, CButton)
	//{{AFX_MSG_MAP(UEasyButtonEx)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_ACTIVATE()
	ON_WM_CANCELMODE()
	ON_CONTROL_REFLECT_EX(BN_CLICKED, OnClicked)
	ON_WM_ENABLE()
	ON_WM_GETDLGCODE()
	ON_WM_KILLFOCUS()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_SYSCOLORCHANGE()
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_MOUSELEAVE, OnMouseLeave )
	ON_MESSAGE( BM_SETSTYLE, OnSetStyle )
	ON_MESSAGE( BM_SETCHECK, OnSetCheck )
	ON_MESSAGE( BM_GETCHECK, OnGetCheck )
	ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// UEasyButtonEx message handlers

void UEasyButtonEx::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	CDC* pDC = CDC::FromHandle( lpDIS->hDC );

	// Is button pressed
	if( m_bIsCheckBox )
		m_bIsPressed = ( lpDIS->itemState & ODS_SELECTED ) || ( 0 != m_bClick );
	else
		m_bIsPressed = ( lpDIS->itemState & ODS_SELECTED );

	// Is button focused
	m_bIsFocused	= ( lpDIS->itemState & ODS_FOCUS );

	// Is button disabled
	m_bIsDisabled	= ( lpDIS->itemState & ODS_DISABLED );

	CRect rcItem	= lpDIS->rcItem;

	pDC->SetBkMode( TRANSPARENT );

	// Draw Button Background
	OnDrawBackground(pDC, &rcItem);

	// Draw Border
	OnDrawBorder(pDC, &rcItem);

	// Get Button's String
	CString strTitle;
	GetWindowText( strTitle );

	CRect rcCaption = lpDIS->rcItem;

	// Draw Icon
	if( m_sIcons[0].hIcon )
		OnDrawIcon( pDC, !strTitle.IsEmpty(), &rcItem, &rcCaption );

	// Draw Bitmap
	if( m_sBitmaps[0].hBitmap )
		OnDrawBitmap( pDC, !strTitle.IsEmpty(), &rcItem, &rcCaption );

	// Draw Text
	if( FALSE == strTitle.IsEmpty() )
		OnDrawText(pDC, (LPCTSTR)strTitle, &rcItem, &rcCaption);

	if( FALSE == m_bIsFlat || ( m_bIsFlat && m_bDrawFlatFocus ) )
	{
		if( m_bIsFocused )
		{
			CRect rcFocus(rcItem);

			rcFocus.DeflateRect(3,3);
			pDC->DrawFocusRect( &rcFocus );
		}
	}
}

void UEasyButtonEx::SetClick(BOOL bClick)
{
	if (m_bClick == bClick) return;

	m_bClick = bClick;
	this->Invalidate();
}

void UEasyButtonEx::TimerControl(BOOL bStart)
{
	if (FALSE == bStart && -1 != m_nTimerID)
	{
		KillTimer(m_nTimerID);
		m_nTimerID = -1;
		SetSelection(false);
	}
	else if (TRUE == bStart && -1 == m_nTimerID)
	{
		m_nTimerID = SetTimer(321, 700, NULL);
	}
	else
	{
		TRACE("NOTHING\n");
	}
}

void UEasyButtonEx::OnDrawBackground(CDC* pDC, CRect* pRect)
{
	COLORREF clrColor;

	if( FALSE == m_bIsFlat )
	{
		if( m_bIsFocused || m_bIsDefault )
		{
			CBrush br(RGB(0,0,0));
			pDC->FrameRect( pRect, &br );
			pRect->DeflateRect( 1, 1 );
		}
	}

	if( m_bMouseOnButton || m_bIsPressed )
		clrColor = m_clrColor[COLOR_BG_IN];
	else
	{
		if( m_bIsFocused )
			clrColor = m_clrColor[COLOR_BG_FOCUS];
		else
			clrColor = m_clrColor[COLOR_BG_OUT];
	}

	if( 1 == m_nSelection )
		clrColor = m_clrSelectColor;

	CBrush bg(clrColor);

	pDC->FillRect( pRect, &bg );
}

void UEasyButtonEx::OnDrawBorder(CDC* pDC, CRect* pRect)
{
	if( m_bIsPressed )
	{
		if( m_bIsFlat )
		{
			if( m_bDrawBorder )
				pDC->Draw3dRect( pRect, ::GetSysColor(COLOR_BTNSHADOW), ::GetSysColor(COLOR_BTNHILIGHT) );
		}
		else
		{
			CBrush br( ::GetSysColor(COLOR_BTNSHADOW) );
			pDC->FrameRect( pRect, &br );
		}
	}
	else
	{
		CPen penHiLight( PS_SOLID, 0, ::GetSysColor(COLOR_BTNHILIGHT) );//White
		CPen pen3DLight( PS_SOLID, 0, ::GetSysColor(COLOR_3DLIGHT) );//Light gray
		CPen penShadow( PS_SOLID, 0, ::GetSysColor(COLOR_BTNSHADOW) );//Dark gray
		CPen pen3DDKShadow( PS_SOLID, 0, ::GetSysColor(COLOR_3DDKSHADOW) );//Black

		if( m_bIsFlat )
		{
			if( m_bMouseOnButton && m_bDrawBorder )
				pDC->Draw3dRect( pRect, ::GetSysColor(COLOR_BTNHILIGHT), ::GetSysColor(COLOR_BTNSHADOW) );
		}
		else
		{
			CPen* pOldPen = (CPen*)pDC->SelectObject( &penHiLight );

			// Draw top-left border
			// White line
			pDC->MoveTo( pRect->left, pRect->bottom - 1 );
			pDC->LineTo( pRect->left, pRect->top );
			pDC->LineTo( pRect->right, pRect->top );

			// Light gray line
			pDC->SelectObject( &pen3DLight );
			pDC->MoveTo( pRect->left + 1, pRect->bottom - 1 );
			pDC->LineTo( pRect->left + 1, pRect->top + 1 );
			pDC->LineTo( pRect->right, pRect->top+1 );
			///////////////////////////////////////////////////////////////

			// Draw bottom-right border
			// Black line
			pDC->SelectObject( &pen3DDKShadow );
			pDC->MoveTo( pRect->left, pRect->bottom - 1 );
			pDC->LineTo( pRect->right - 1, pRect->bottom - 1 );
			pDC->LineTo( pRect->right - 1, pRect->top - 1 );
			// Dark gray line
			pDC->SelectObject( &penShadow );
			pDC->MoveTo( pRect->left + 1, pRect->bottom - 2 );
			pDC->MoveTo( pRect->right - 2, pRect->bottom - 2 );
			pDC->MoveTo( pRect->right - 2, pRect->top );

			pDC->SelectObject( pOldPen );
		}
	}
}

void UEasyButtonEx::OnDrawText(CDC* pDC, LPCTSTR lpszText, CRect* /*prcItem*/, CRect* prcCaption)
{
	if( m_bIsPressed && FALSE == m_bIsCheckBox )
		prcCaption->OffsetRect( m_nPressedOffset.x, m_nPressedOffset.y );

	CRect rcCenter(prcCaption);

	pDC->DrawText( lpszText, -1, prcCaption, DT_WORDBREAK | DT_CENTER | DT_CALCRECT );

	int nOffsetW = ( rcCenter.Width() - prcCaption->Width() ) / 2;
	int nOffsetH = ( rcCenter.Height() - prcCaption->Height() ) / 2;
	prcCaption->OffsetRect( nOffsetW, nOffsetH );

	pDC->SetBkMode( TRANSPARENT );

	if( m_bIsDisabled )
	{
		prcCaption->OffsetRect( 1, 1 );
		pDC->SetTextColor( ::GetSysColor(COLOR_3DHILIGHT) );
		pDC->DrawText( lpszText, -1, prcCaption, DT_CENTER | DT_WORDBREAK );

		prcCaption->OffsetRect( -1, -1 );
		pDC->SetTextColor( ::GetSysColor(COLOR_3DSHADOW) );
		pDC->DrawText( lpszText, -1, prcCaption, DT_CENTER | DT_WORDBREAK );
	}
	else
	{
		if( m_bIsPressed || m_bMouseOnButton )
		{ 
			pDC->SetTextColor( m_clrColor[COLOR_FG_IN] );
			pDC->SetBkColor( m_clrColor[COLOR_BG_IN] );
		}
		else
		{
			if( m_bIsFocused )
			{
				pDC->SetTextColor( m_clrColor[COLOR_FG_FOCUS] );
				pDC->SetBkColor( m_clrColor[COLOR_BG_FOCUS] );
			}
			else
			{
				pDC->SetTextColor( m_clrColor[COLOR_FG_OUT] );
				pDC->SetBkColor( m_clrColor[COLOR_BG_OUT] );
			}
		}

		pDC->DrawText( lpszText, -1, prcCaption, DT_CENTER | DT_WORDBREAK );
	}
}

void UEasyButtonEx::OnDrawIcon(CDC* pDC, BOOL bHasTitle, CRect* prcItem, CRect* prcCaption)
{
	int nIndex = 0;

	if( ( m_bIsCheckBox && m_bIsPressed ) || ( !m_bIsCheckBox && ( m_bIsPressed || m_bMouseOnButton ) ) )
		nIndex = 0;
	else
		nIndex = ( NULL == m_sIcons[1].hIcon ) ? 0 : 1;

	CRect rcImg;
	PrepareImageRect( bHasTitle, prcItem, prcCaption, m_sIcons[nIndex].dwWidth, m_sIcons[nIndex].dwHeight, &rcImg );

	pDC->DrawState( rcImg.TopLeft(), rcImg.Size(), m_sIcons[nIndex].hIcon,
					( m_bIsDisabled ? DSS_DISABLED : DSS_NORMAL ), (CBrush*)NULL );
}

void UEasyButtonEx::OnDrawBitmap(CDC* pDC, BOOL bHasTitle, CRect* prcItem, CRect* prcCaption)
{
	HDC			hdcBmpMem	= NULL;
	HBITMAP		hbmOldBmp	= NULL;
	HDC			hdcMem		= NULL;
	HBITMAP		hbmT		= NULL;

	int nIndex = 0;

	// Select the bitmap to use
	if (( m_bIsCheckBox && m_bIsPressed ) || ( !m_bIsCheckBox && ( m_bIsPressed || m_bMouseOnButton ) ) )
		nIndex = 0;
	else
		nIndex = (m_sBitmaps[1].hBitmap == NULL ? 0 : 1);

	CRect	rcImage;
	PrepareImageRect(bHasTitle, prcItem, prcCaption, m_sBitmaps[nIndex].dwWidth, m_sBitmaps[nIndex].dwHeight, &rcImage);

	hdcBmpMem = ::CreateCompatibleDC(pDC->m_hDC);

	hbmOldBmp = (HBITMAP)::SelectObject(hdcBmpMem, m_sBitmaps[nIndex].hBitmap);

	hdcMem = ::CreateCompatibleDC(NULL);

	hbmT = (HBITMAP)::SelectObject(hdcMem, m_sBitmaps[nIndex].hMask);

	if( m_bIsDisabled && m_bShowDisabledBitmap )
	{
		HDC		hDC = NULL;
		HBITMAP	hBitmap = NULL;

		hDC = ::CreateCompatibleDC(pDC->m_hDC);
		hBitmap = ::CreateCompatibleBitmap(pDC->m_hDC, m_sBitmaps[nIndex].dwWidth, m_sBitmaps[nIndex].dwHeight);
		HBITMAP	hOldBmp2 = (HBITMAP)::SelectObject(hDC, hBitmap);

		RECT	rRect;
		rRect.left		= 0;
		rRect.top		= 0;
		rRect.right		= rcImage.right + 1;
		rRect.bottom	= rcImage.bottom + 1;
		::FillRect(hDC, &rRect, (HBRUSH)RGB(255, 255, 255));

		COLORREF crOldColor = ::SetBkColor(hDC, RGB(255,255,255));

		::BitBlt(hDC, 0, 0, m_sBitmaps[nIndex].dwWidth, m_sBitmaps[nIndex].dwHeight, hdcMem, 0, 0, SRCAND);
		::BitBlt(hDC, 0, 0, m_sBitmaps[nIndex].dwWidth, m_sBitmaps[nIndex].dwHeight, hdcBmpMem, 0, 0, SRCPAINT);

		::SetBkColor(hDC, crOldColor);
		::SelectObject(hDC, hOldBmp2);
		::DeleteDC(hDC);

		pDC->DrawState(	CPoint(rcImage.left/*+1*/, rcImage.top), 
						CSize(m_sBitmaps[nIndex].dwWidth, m_sBitmaps[nIndex].dwHeight), 
						hBitmap, DST_BITMAP | DSS_DISABLED);

		::DeleteObject(hBitmap);
	} // if
	else
	{
		::BitBlt(pDC->m_hDC, rcImage.left, rcImage.top, m_sBitmaps[nIndex].dwWidth, m_sBitmaps[nIndex].dwHeight, hdcMem, 0, 0, SRCAND);

		::BitBlt(pDC->m_hDC, rcImage.left, rcImage.top, m_sBitmaps[nIndex].dwWidth, m_sBitmaps[nIndex].dwHeight, hdcBmpMem, 0, 0, SRCPAINT);
	} // else

	::SelectObject(hdcMem, hbmT);
	::DeleteDC(hdcMem);

	::SelectObject(hdcBmpMem, hbmOldBmp);
	::DeleteDC(hdcBmpMem);
}

void UEasyButtonEx::PrepareImageRect(BOOL bHasTitle, CRect* prcItem, CRect* prcTitle, DWORD dwWidth, DWORD dwHeight, CRect* prcImage)
{
	CRect rcBtn;

	prcImage->CopyRect(prcItem);

	switch (m_nAlignMode)
	{
		case ALIGN_HOR:
			if (bHasTitle == FALSE)
			{
				// Center image horizontally
				prcImage->left += ((prcImage->Width() - (long)dwWidth)/2);
			}
			else
			{
				// Image must be placed just inside the focus rect
				prcImage->left += m_nImageOrg.x;  
				prcTitle->left += dwWidth + m_nImageOrg.x;
			}
			// Center image vertically
			prcImage->top += ((prcImage->Height() - (long)dwHeight)/2);
			break;

		case ALIGN_HOR_RIGHT:
			GetClientRect(&rcBtn);
			if (bHasTitle == FALSE)
			{
				// Center image horizontally
				prcImage->left += ((prcImage->Width() - (long)dwWidth)/2);
			}
			else
			{
				// Image must be placed just inside the focus rect
				prcTitle->right = prcTitle->Width() - dwWidth - m_nImageOrg.x;
				prcTitle->left = m_nImageOrg.x;
				prcImage->left = rcBtn.right - dwWidth - m_nImageOrg.x;
				// Center image vertically
				prcImage->top += ((prcImage->Height() - (long)dwHeight)/2);
			}
			break;
		
		case ALIGN_VER:
			// Center image horizontally
			prcImage->left += ((prcImage->Width() - (long)dwWidth)/2);
			if (bHasTitle == FALSE)
			{
				// Center image vertically
				prcImage->top += ((prcImage->Height() - (long)dwHeight)/2);           
			}
			else
			{
				prcImage->top = m_nImageOrg.y;
				prcTitle->top += dwHeight;
			}
			break;

		case ALIGN_OVERLAP:
			break;
	} // switch
    
	// If button is pressed then press image also
	if( m_bIsPressed && FALSE == m_bIsCheckBox )
		prcImage->OffsetRect(m_nPressedOffset.x, m_nPressedOffset.y);
}

void UEasyButtonEx::SetDefaultColor(BOOL bRepaint)
{
	m_clrColor[COLOR_BG_IN]		= ::GetSysColor(COLOR_BTNFACE);
	m_clrColor[COLOR_FG_IN]		= ::GetSysColor(COLOR_BTNTEXT);
	m_clrColor[COLOR_BG_OUT]	= ::GetSysColor(COLOR_BTNFACE);
	m_clrColor[COLOR_FG_OUT]	= ::GetSysColor(COLOR_BTNTEXT);
	m_clrColor[COLOR_BG_FOCUS]	= ::GetSysColor(COLOR_BTNFACE);
	m_clrColor[COLOR_FG_FOCUS]	= ::GetSysColor(COLOR_BTNTEXT);

	if( bRepaint) 
		Invalidate();
}

void UEasyButtonEx::PreSubclassWindow() 
{
	int nStyle = this->GetButtonStyle();

	if( nStyle & BS_CHECKBOX )
		m_bIsCheckBox = TRUE;

	nStyle |= BS_OWNERDRAW;

	this->SetButtonStyle( nStyle );
	
	CButton::PreSubclassWindow();
}

void UEasyButtonEx::SetPressedStyle(BYTE bStyle, BOOL bRepaint)
{
	switch( bStyle )
	{
	case PRESSED_LEFTRIGHT :
		m_nPressedOffset.x = m_nPressedOffset.y	= 1;
		break;
	case PRESSED_TOPBOTTOM :
		m_nPressedOffset.x	= 0;
		m_nPressedOffset.y	= 2;
		break;
	default :
		m_nPressedOffset.x = m_nPressedOffset.y	= 1;
		break;
	}

	if( bRepaint )
		Invalidate();
}

void UEasyButtonEx::SetFlat(BOOL bFlat, BOOL bRepaint)
{
	m_bIsFlat = bFlat;

	if( bRepaint )
		Invalidate();
}

void UEasyButtonEx::SetCheck(BOOL bCheck, BOOL bRepaint)
{
	if( m_bIsCheckBox )
	{
		m_bClick = bCheck;

		if( bRepaint )
			Invalidate();
	}
}

HBRUSH UEasyButtonEx::CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/) 
{
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a non-NULL brush if the parent's handler should not be called
	return NULL;
}

void UEasyButtonEx::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CButton::OnActivate(nState, pWndOther, bMinimized);
	
	if( WA_INACTIVE == nState )
		CancelHover();
	
}

void UEasyButtonEx::CancelHover()
{
	if( m_bIsFlat )  // Only for flat button
	{
		if( m_bMouseOnButton )
		{
			m_bMouseOnButton = FALSE;
			Invalidate();
		}
	}
}

void UEasyButtonEx::OnCancelMode() 
{
	CButton::OnCancelMode();
	
	CancelHover();
}

BOOL UEasyButtonEx::OnClicked()
{
	SetFocus();

	if( m_bIsCheckBox )
	{
		m_bClick = !m_bClick;
		Invalidate();
	}

	return FALSE;
}

void UEasyButtonEx::OnEnable(BOOL bEnable) 
{
	CButton::OnEnable(bEnable);
	
	if( bEnable )
	{
		/*CWnd* pWnd = GetParent()->GetNextDlgTabItem(this);

		if( pWnd )
			SetFocus();
		else
			GetParent()->SetFocus();*/

		CancelHover();
	}
}

UINT UEasyButtonEx::OnGetDlgCode() 
{
	UINT nCode = CButton::OnGetDlgCode();

	nCode |= ( m_bIsDefault ? DLGC_DEFPUSHBUTTON : DLGC_UNDEFPUSHBUTTON );
	
	return nCode;
}

void UEasyButtonEx::OnKillFocus(CWnd* pNewWnd) 
{
	CButton::OnKillFocus(pNewWnd);
	
	CancelHover();
}

void UEasyButtonEx::OnMouseMove(UINT nFlags, CPoint point) 
{
	CWnd*				wndUnderMouse = 0;
	CWnd*				wndActive = this;
	TRACKMOUSEEVENT		sTME;
	
	CButton::OnMouseMove(nFlags, point);

	ClientToScreen( &point );
	wndUnderMouse = WindowFromPoint( point );

	// If the mouse enter the button with the left button pressed then do nothing
	if( nFlags & MK_LBUTTON && FALSE == m_bMouseOnButton )
		return;

	if( FALSE == m_bIsFlat )
		return;

	if( FALSE == m_bAlwaysTrack )
		wndActive	= GetActiveWindow();

	if( wndUnderMouse && m_hWnd == wndUnderMouse->m_hWnd && wndActive )
	{
		if( FALSE == m_bMouseOnButton )
		{
			m_bMouseOnButton = TRUE;

			Invalidate();

			sTME.cbSize		= sizeof(sTME);
			sTME.dwFlags	= TME_LEAVE;
			sTME.hwndTrack	= m_hWnd;

			::_TrackMouseEvent( &sTME );
		}
	}
	else
		CancelHover();
}

BOOL UEasyButtonEx::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if( NULL != m_hCursor )
	{
		::SetCursor( m_hCursor );

		return TRUE;
	}
	
	return CButton::OnSetCursor(pWnd, nHitTest, message);
}

void UEasyButtonEx::SetBtnCursor(int nCursorID, BOOL bRepaint)
{
	HINSTANCE hInstResource = NULL;

	if( m_hCursor )
	{
		::DestroyCursor( m_hCursor );
		m_hCursor = NULL;
	}

	if( nCursorID )
	{
		hInstResource = ::AfxFindResourceHandle( MAKEINTRESOURCE(nCursorID), RT_GROUP_CURSOR );

		m_hCursor	= (HCURSOR)::LoadImage( hInstResource, MAKEINTRESOURCE(nCursorID), IMAGE_CURSOR, 0, 0, 0 );

		if( bRepaint )
			Invalidate();
	}
}

void UEasyButtonEx::OnSysColorChange() 
{
	CButton::OnSysColorChange();
	
	SetDefaultColor();	
}

BOOL UEasyButtonEx::SetColor(int nIndex, COLORREF clrColor, BOOL bRepaint)
{
	if (nIndex >= MAX_COLORS)	
		return FALSE;

	// Set new color
	if (m_clrColor[nIndex] == clrColor) return FALSE;
	m_clrColor[nIndex] = clrColor;

	if( bRepaint )	
		Invalidate();

	return TRUE;
}

BOOL UEasyButtonEx::GetColor(int nIndex, COLORREF* pclrColor)
{
	if (nIndex >= MAX_COLORS)	
		return FALSE;

	*pclrColor	= m_clrColor[nIndex];

	return TRUE;
}

void UEasyButtonEx::OffsetColor(BYTE bIndex, short sOffset, BOOL bRepaint)
{
	if( bIndex > MAX_COLORS )
		bIndex = COLOR_BG_IN;
	if( sOffset > 255 || sOffset < -255 )
		sOffset = 50;
	
	BYTE bRed = 0, bGreen = 0, bBlue = 0;
	short sOffsetRed = sOffset, sOffsetGreen = sOffset, sOffsetBlue = sOffset;
	short sOffsetMin = 0;

	bRed	= GetRValue( m_clrColor[bIndex] );
	bGreen	= GetGValue( m_clrColor[bIndex] );
	bBlue	= GetBValue( m_clrColor[bIndex] );

	if( sOffset > 0 )
	{
		if( bRed + sOffset > 255 )
			sOffsetRed = (BYTE)(255 - bRed);
		if( bGreen + sOffset > 255 )
			sOffsetGreen = (BYTE)(255 - bGreen);
		if( bBlue + sOffset > 255 )
			sOffsetBlue = (BYTE)(255 - bBlue);

		sOffsetMin = min( min( sOffsetRed, sOffsetGreen ), sOffsetBlue );
	}
	else
	{
		if( bRed + sOffset < 0 )
			sOffsetRed = (short)(-bRed);
		if( bGreen + sOffset < 0 )
			sOffsetGreen = (short)(-bGreen);
		if( bBlue + sOffset < 0 )
			sOffsetBlue = (short)(-bBlue);
		sOffsetMin = max( max( sOffsetRed, sOffsetGreen ), sOffsetBlue );
	}

	m_clrColor[bIndex] = RGB( bRed + sOffsetMin, bGreen + sOffsetMin, bBlue + sOffsetMin );

	if( bRepaint )
		Invalidate();
}

LRESULT UEasyButtonEx::OnMouseLeave( WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	CancelHover();

	return 0L;
}

LRESULT UEasyButtonEx::OnSetStyle( WPARAM wParam, LPARAM lParam )
{
	UINT nNewType = (wParam & SS_TYPEMASK);

	if( BS_DEFPUSHBUTTON == nNewType )
		m_bIsDefault = TRUE;
	else if( BS_PUSHBUTTON == nNewType )
		m_bIsDefault = FALSE;
	return DefWindowProc(BM_SETSTYLE, (wParam & ~SS_TYPEMASK) | BS_OWNERDRAW, lParam);
}

LRESULT UEasyButtonEx::OnSetCheck(WPARAM wParam, LPARAM /*lParam*/)
{
	switch( wParam )
	{
	case BST_CHECKED :
	case BST_INDETERMINATE :
		SetCheck(1);
		break;
	default :
		SetCheck(0);
		break;
	}

	return 0L;
}

LRESULT UEasyButtonEx::OnGetCheck(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	return GetCheck();
}

BOOL UEasyButtonEx::PreTranslateMessage(MSG* pMsg) 
{
	InitToolTip();
	m_ToolTip.RelayEvent(pMsg);

	if( WM_LBUTTONDBLCLK == pMsg->message )
		pMsg->message = WM_LBUTTONDOWN;
	
	return CButton::PreTranslateMessage(pMsg);
}

void UEasyButtonEx::InitToolTip()
{
	if( NULL == m_ToolTip.m_hWnd )
	{
		m_ToolTip.Create(this, m_dwToolTipStyle);
		m_ToolTip.Activate(FALSE);
		m_ToolTip.SendMessage(TTM_SETMAXTIPWIDTH, 0, 400);
	}
}

void UEasyButtonEx::SetToolTipText(int nIndex, BOOL bActivate)
{
	CString strText;

	strText.LoadString(nIndex);

	if( FALSE == strText.IsEmpty() )
		SetToolTipText( (LPCTSTR)strText, bActivate );
}

void UEasyButtonEx::SetToolTipText(LPCTSTR lpszText, BOOL bActivate)
{
	if( NULL == lpszText )
		return;

	InitToolTip();

	if( 0 == m_ToolTip.GetToolCount() )
	{
		CRect rc;
		GetClientRect( &rc );
		m_ToolTip.AddTool( this, lpszText, rc, 1 );
	}

	m_ToolTip.UpdateTipText( lpszText, this, 1 );
	m_ToolTip.Activate(bActivate);
}

void UEasyButtonEx::ActivateToolTip(BOOL bEnable)
{
	if( 0 == m_ToolTip.GetToolCount() )
		return;

	m_ToolTip.Activate( bEnable );
}

DWORD UEasyButtonEx::EnableBallonToolTip()
{
	m_dwToolTipStyle |= TTS_BALLOON;

	return m_dwToolTipStyle;
}

void UEasyButtonEx::SetAlign(BYTE bAlign, BOOL bRepaint)
{
	if( bAlign >= ALIGN_HOR && bAlign <= ALIGN_OVERLAP )
		m_nAlignMode = bAlign;

	if( bRepaint )
		Invalidate();
}

void UEasyButtonEx::DrawBorder(BOOL bDrawBorder, BOOL bRepaint)
{
	m_bDrawBorder	= bDrawBorder;

	if( bRepaint )
		Invalidate();
}

void UEasyButtonEx::DrawFlatFocus(BOOL bDrawFlatBorder, BOOL bRepaint)
{
	m_bDrawFlatFocus = bDrawFlatBorder;

	if( bRepaint )
		Invalidate();
}

void UEasyButtonEx::FreeResource(BOOL bCheckForNULL)
{
	if( bCheckForNULL )
	{
		if( m_sIcons[0].hIcon )
			::DestroyIcon( m_sIcons[0].hIcon );
		if( m_sIcons[1].hIcon )
			::DestroyIcon( m_sIcons[1].hIcon );

		if( m_sBitmaps[0].hBitmap )
			::DeleteObject( m_sBitmaps[0].hBitmap );
		if( m_sBitmaps[1].hBitmap )
			::DeleteObject( m_sBitmaps[1].hBitmap );

		if( m_sBitmaps[0].hMask )
			::DeleteObject( m_sBitmaps[0].hMask );
		if( m_sBitmaps[1].hMask )
			::DeleteObject( m_sBitmaps[1].hMask );

	}

	ZeroMemory( &m_sIcons, sizeof(m_sIcons) );
	ZeroMemory( &m_sBitmaps, sizeof(m_sBitmaps) );
}

HICON UEasyButtonEx::CreateGrayscaleIcon(HICON hIcon)
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

					if (crPixel)	
						::SetPixel(hMemDC2, dwLoopX, dwLoopY, RGB(byNewPixel, byNewPixel, byNewPixel));
					else
						::SetPixel(hMemDC2, dwLoopX, dwLoopY, crPixel);
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
}

HICON UEasyButtonEx::CreateDarkerIcon(HICON hIcon)
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

			for (dwLoopY = 0; dwLoopY < dwHeight; dwLoopY++)
			{
				for (dwLoopX = 0; dwLoopX < dwWidth; dwLoopX++)
				{
					crPixel = ::GetPixel(hMemDC1, dwLoopX, dwLoopY);

					if (crPixel)	
						::SetPixel(hMemDC2, dwLoopX, dwLoopY, DarkenColor(crPixel, 0.25));
					else
						::SetPixel(hMemDC2, dwLoopX, dwLoopY, crPixel);
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
}

COLORREF UEasyButtonEx::DarkenColor(COLORREF clrColor, double dFactor)
{
	if (dFactor > 0.0 && dFactor <= 1.0)
	{
		BYTE red,green,blue,lightred,lightgreen,lightblue;
		red = GetRValue(clrColor);
		green = GetGValue(clrColor);
		blue = GetBValue(clrColor);
		lightred = (BYTE)(red-(dFactor * red));
		lightgreen = (BYTE)(green-(dFactor * green));
		lightblue = (BYTE)(blue-(dFactor * blue));
		clrColor = RGB(lightred,lightgreen,lightblue);
	} // if

	return clrColor;
}

DWORD UEasyButtonEx::SetIcon(int nIconIn, int nCxDesiredIn, int nCyDesiredIn, int nIconOut, int nCxDesiredOut, int nCyDesiredOut )
{
	HICON		hIconIn			= 0;
	HICON		hIconOut		= 0;
	HINSTANCE	hInstResource	= 0;

	hInstResource = ::AfxFindResourceHandle( MAKEINTRESOURCE(nIconIn), RT_GROUP_ICON );

	hIconIn	= (HICON)::LoadImage( hInstResource, MAKEINTRESOURCE(nIconIn), IMAGE_ICON, nCxDesiredIn, nCyDesiredIn, 0 );

	switch( nIconOut )
	{
	case (int)BTNST_AUTO_GRAY :
		hIconOut = BTNST_AUTO_GRAY;
		break;
	case (int)BTNST_AUTO_DARKER :
		hIconOut = BTNST_AUTO_DARKER;
		break;
	default :
		hIconOut = (HICON)::LoadImage( hInstResource, MAKEINTRESOURCE(nIconOut), IMAGE_ICON, nCxDesiredOut, nCyDesiredOut, 0 );
		break;
	}

	return SetIcon( hIconIn, hIconOut );
}

DWORD UEasyButtonEx::SetIcon(int nIconIn, int nIconOut)
{
	return SetIcon( nIconIn, 0, 0, nIconOut,0, 0 );
}

DWORD UEasyButtonEx::SetIcon(HICON hIconIn, HICON hIconOut)
{
	BOOL bRet;
	ICONINFO iInfo;

	FreeResource();

	if( hIconIn )
	{
		m_sIcons[0].hIcon	= hIconIn;
		ZeroMemory( &iInfo, sizeof(iInfo) );

		bRet = ::GetIconInfo( hIconIn, &iInfo );

		if( FALSE == bRet )
		{
			FreeResource();
			return FALSE;
		}

		m_sIcons[0].dwWidth		= (DWORD)( iInfo.xHotspot * 2 );
		m_sIcons[0].dwHeight	= (DWORD)( iInfo.yHotspot * 2 );

		::DeleteObject( iInfo.hbmColor );
		::DeleteObject( iInfo.hbmMask );

		if( hIconOut )
		{
			switch( (int)hIconOut )
			{
			case (int)BTNST_AUTO_GRAY :
				hIconOut = CreateGrayscaleIcon(hIconIn);
				break;
			case (int)BTNST_AUTO_DARKER :
				break;
				hIconOut = CreateDarkerIcon(hIconIn);
			}
			m_sIcons[1].hIcon	= hIconOut;
			ZeroMemory( &iInfo, sizeof(iInfo) );

			bRet = ::GetIconInfo( hIconOut, &iInfo );

			if( FALSE == bRet )
			{
				FreeResource();
				return FALSE;
			}

			m_sIcons[1].dwWidth		= (DWORD)( iInfo.xHotspot * 2 );
			m_sIcons[1].dwHeight	= (DWORD)( iInfo.yHotspot * 2 );

			::DeleteObject( iInfo.hbmColor );
			::DeleteObject( iInfo.hbmMask );
		}
	}

	Invalidate();

	return TRUE;
}

HBITMAP UEasyButtonEx::CreateGrayscaleBitmap(HBITMAP hBitmap, DWORD dwWidth, DWORD dwHeight, COLORREF clrTrans)
{
	HBITMAP		hGrayBitmap = NULL;
	HDC			hMainDC = NULL, hMemDC1 = NULL, hMemDC2 = NULL;
	HBITMAP		hOldBmp1 = NULL, hOldBmp2 = NULL;

	hMainDC = ::GetDC(NULL);
	if (hMainDC == NULL)	return NULL;
	hMemDC1 = ::CreateCompatibleDC(hMainDC);
	if (hMemDC1 == NULL)
	{
		::ReleaseDC(NULL, hMainDC);
		return NULL;
	} // if
	hMemDC2 = ::CreateCompatibleDC(hMainDC);
	if (hMemDC2 == NULL)
	{
		::DeleteDC(hMemDC1);
		::ReleaseDC(NULL, hMainDC);
		return NULL;
	} // if

	hGrayBitmap = ::CreateCompatibleBitmap(hMainDC, dwWidth, dwHeight);
	if (hGrayBitmap)
	{
		hOldBmp1 = (HBITMAP)::SelectObject(hMemDC1, hGrayBitmap);
		hOldBmp2 = (HBITMAP)::SelectObject(hMemDC2, hBitmap);

		//::BitBlt(hMemDC1, 0, 0, dwWidth, dwHeight, hMemDC2, 0, 0, SRCCOPY);

		DWORD		dwLoopY = 0, dwLoopX = 0;
		COLORREF	crPixel = 0;
		BYTE		byNewPixel = 0;

		for (dwLoopY = 0; dwLoopY < dwHeight; dwLoopY++)
		{
			for (dwLoopX = 0; dwLoopX < dwWidth; dwLoopX++)
			{
				crPixel = ::GetPixel(hMemDC2, dwLoopX, dwLoopY);
				byNewPixel = (BYTE)((GetRValue(crPixel) * 0.299) + (GetGValue(crPixel) * 0.587) + (GetBValue(crPixel) * 0.114));

				if (crPixel != clrTrans)
					::SetPixel(hMemDC1, dwLoopX, dwLoopY, RGB(byNewPixel, byNewPixel, byNewPixel));
				else
					::SetPixel(hMemDC1, dwLoopX, dwLoopY, crPixel);
			} // for
		} // for

		::SelectObject(hMemDC1, hOldBmp1);
		::SelectObject(hMemDC2, hOldBmp2);
	} // if

	::DeleteDC(hMemDC1);
	::DeleteDC(hMemDC2);
	::ReleaseDC(NULL, hMainDC);

	return hGrayBitmap;
}

HBITMAP UEasyButtonEx::CreateDarkerBitmap(HBITMAP hBitmap, DWORD dwWidth, DWORD dwHeight, COLORREF clrTrans)
{
	HBITMAP		hGrayBitmap = NULL;
	HDC			hMainDC = NULL, hMemDC1 = NULL, hMemDC2 = NULL;
	HBITMAP		hOldBmp1 = NULL, hOldBmp2 = NULL;

	hMainDC = ::GetDC(NULL);
	if (hMainDC == NULL)	return NULL;
	hMemDC1 = ::CreateCompatibleDC(hMainDC);
	if (hMemDC1 == NULL)
	{
		::ReleaseDC(NULL, hMainDC);
		return NULL;
	} // if
	hMemDC2 = ::CreateCompatibleDC(hMainDC);
	if (hMemDC2 == NULL)
	{
		::DeleteDC(hMemDC1);
		::ReleaseDC(NULL, hMainDC);
		return NULL;
	} // if

	hGrayBitmap = ::CreateCompatibleBitmap(hMainDC, dwWidth, dwHeight);
	if (hGrayBitmap)
	{
		hOldBmp1 = (HBITMAP)::SelectObject(hMemDC1, hGrayBitmap);
		hOldBmp2 = (HBITMAP)::SelectObject(hMemDC2, hBitmap);

		//::BitBlt(hMemDC1, 0, 0, dwWidth, dwHeight, hMemDC2, 0, 0, SRCCOPY);

		DWORD		dwLoopY = 0, dwLoopX = 0;
		COLORREF	crPixel = 0;

		for (dwLoopY = 0; dwLoopY < dwHeight; dwLoopY++)
		{
			for (dwLoopX = 0; dwLoopX < dwWidth; dwLoopX++)
			{
				crPixel = ::GetPixel(hMemDC2, dwLoopX, dwLoopY);

				if (crPixel != clrTrans)
					::SetPixel(hMemDC1, dwLoopX, dwLoopY, DarkenColor(crPixel, 0.25));
				else
					::SetPixel(hMemDC1, dwLoopX, dwLoopY, crPixel);
			} // for
		} // for

		::SelectObject(hMemDC1, hOldBmp1);
		::SelectObject(hMemDC2, hOldBmp2);
	} // if

	::DeleteDC(hMemDC1);
	::DeleteDC(hMemDC2);
	::ReleaseDC(NULL, hMainDC);

	return hGrayBitmap;
}

HBITMAP UEasyButtonEx::CreateBitmapMask(HBITMAP hSourceBitmap, DWORD dwWidth, DWORD dwHeight, COLORREF clrTransColor)
{
	HBITMAP		hMask		= NULL;
	HDC			hdcSrc		= NULL;
	HDC			hdcDest		= NULL;
	HBITMAP		hbmSrcT		= NULL;
	HBITMAP		hbmDestT	= NULL;
	COLORREF	crSaveBk;
	COLORREF	crSaveDestText;

	hMask = ::CreateBitmap(dwWidth, dwHeight, 1, 1, NULL);
	if (hMask == NULL)	return NULL;

	hdcSrc	= ::CreateCompatibleDC(NULL);
	hdcDest	= ::CreateCompatibleDC(NULL);

	hbmSrcT = (HBITMAP)::SelectObject(hdcSrc, hSourceBitmap);
	hbmDestT = (HBITMAP)::SelectObject(hdcDest, hMask);

	crSaveBk = ::SetBkColor(hdcSrc, clrTransColor);

	::BitBlt(hdcDest, 0, 0, dwWidth, dwHeight, hdcSrc, 0, 0, SRCCOPY);

	crSaveDestText = ::SetTextColor(hdcSrc, RGB(255, 255, 255));
	::SetBkColor(hdcSrc,RGB(0, 0, 0));

	::BitBlt(hdcSrc, 0, 0, dwWidth, dwHeight, hdcDest, 0, 0, SRCAND);

	SetTextColor(hdcDest, crSaveDestText);

	::SetBkColor(hdcSrc, crSaveBk);
	::SelectObject(hdcSrc, hbmSrcT);
	::SelectObject(hdcDest, hbmDestT);

	::DeleteDC(hdcSrc);
	::DeleteDC(hdcDest);

	return hMask;
}

DWORD UEasyButtonEx::SetBitmap(int nBitmapIn, COLORREF clrTransColorIn, int nBitmapOut, COLORREF clrTransColorOut)
{
	HBITMAP			hBitmapIn = 0;
	HBITMAP			hBitmapOut = 0;
	HINSTANCE		hInstResource = 0;

	hInstResource = ::AfxFindResourceHandle( MAKEINTRESOURCE(nBitmapIn), RT_BITMAP );

	hBitmapIn = (HBITMAP)::LoadImage( hInstResource, MAKEINTRESOURCE(nBitmapIn), IMAGE_BITMAP, 0, 0, 0 );

	switch( nBitmapOut )
	{
	case (int)BTNST_AUTO_GRAY :
		hBitmapOut = (HBITMAP)BTNST_AUTO_GRAY;
		break;
	case (int)BTNST_AUTO_DARKER :
		hBitmapOut = (HBITMAP)BTNST_AUTO_DARKER;
		break;
	default :
		hBitmapOut = (HBITMAP)::LoadImage( hInstResource, MAKEINTRESOURCE(nBitmapOut), IMAGE_BITMAP, 0, 0, 0 );
		break;
	}

	return SetBitmap( hBitmapIn, clrTransColorIn, hBitmapOut, clrTransColorOut );
}

DWORD UEasyButtonEx::SetBitmap(HBITMAP hBitmapIn, COLORREF clrTransColorIn, HBITMAP hBitmapOut, COLORREF clrTransColorOut)
{
	int nRet = 0;
	BITMAP sBitmapSize;

	FreeResource();

	if( hBitmapIn )
	{
		m_sBitmaps[0].hBitmap = hBitmapIn;
		m_sBitmaps[0].clrTransparent = clrTransColorIn;

		nRet = ::GetObject( (HBITMAP)hBitmapIn, sizeof(sBitmapSize), &sBitmapSize );

		if( FALSE == nRet )
		{
			FreeResource();
			return FALSE;
		}

		m_sBitmaps[0].dwWidth	= (DWORD)sBitmapSize.bmWidth;
		m_sBitmaps[0].dwHeight	= (DWORD)sBitmapSize.bmHeight;

		switch( (int)hBitmapOut )
		{
		case (int)BTNST_AUTO_GRAY :
			hBitmapOut = CreateGrayscaleBitmap( hBitmapIn, m_sBitmaps[0].dwWidth, m_sBitmaps[0].dwHeight, clrTransColorIn );
			m_sBitmaps[1].hBitmap = hBitmapOut;
			clrTransColorOut = clrTransColorIn;
			break;
		case (int)BTNST_AUTO_DARKER :
			hBitmapOut = CreateDarkerBitmap( hBitmapIn, m_sBitmaps[0].dwWidth, m_sBitmaps[0].dwHeight, clrTransColorIn );
			m_sBitmaps[1].hBitmap = hBitmapOut;
			clrTransColorOut = clrTransColorIn;
			break;
		}

		m_sBitmaps[0].hMask = CreateBitmapMask( hBitmapIn, m_sBitmaps[0].dwWidth, m_sBitmaps[0].dwHeight, clrTransColorIn );

		if( NULL == m_sBitmaps[0].hMask )
		{
			FreeResource();
			return FALSE;
		}

		if( hBitmapOut )
		{
			m_sBitmaps[1].hBitmap = hBitmapOut;
			m_sBitmaps[1].clrTransparent = clrTransColorOut;

			nRet = ::GetObject( (HBITMAP)hBitmapOut, sizeof(sBitmapSize), &sBitmapSize );

			if( FALSE == nRet )
			{
				FreeResource();
				return FALSE;
			}

			m_sBitmaps[1].dwWidth	= (DWORD)sBitmapSize.bmWidth;
			m_sBitmaps[1].dwHeight	= (DWORD)sBitmapSize.bmHeight;

			m_sBitmaps[1].hMask = CreateBitmapMask( hBitmapOut, m_sBitmaps[1].dwWidth, m_sBitmaps[1].dwHeight, clrTransColorOut );

			if( NULL == m_sBitmaps[1].hMask )
			{
				FreeResource();
				return FALSE;
			}
		}
	}

	Invalidate();

	return TRUE;
}

void UEasyButtonEx::SizeToContent()
{
	if (m_sIcons[0].hIcon)
	{
		m_nImageOrg.x = 0;
		m_nImageOrg.y = 0;
	    SetWindowPos(	NULL, -1, -1, m_sIcons[0].dwWidth, m_sIcons[0].dwHeight,
						SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW | SWP_NOACTIVATE);
	} // if
	else
	if (m_sBitmaps[0].hBitmap)
	{
		m_nImageOrg.x = 0;
		m_nImageOrg.y = 0;
	    SetWindowPos(	NULL, -1, -1, m_sBitmaps[0].dwWidth, m_sBitmaps[0].dwHeight,
						SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW | SWP_NOACTIVATE);
	} // if
}

void UEasyButtonEx::SetImageOrg(int x, int y)
{
	m_nImageOrg.x	= x;
	m_nImageOrg.y	= y;
}

void UEasyButtonEx::SetSelection(int nSel, COLORREF sColor)
{
	if (m_clrSelectColor == sColor && m_nSelection == nSel) return;
	m_clrSelectColor = sColor;
	m_nSelection = nSel; 
	Invalidate();
}


void UEasyButtonEx::OnTimer(UINT_PTR nIDEvent)
{
	if (m_bClick)
	{
		SetSelection(true, RGB(0, 255, 255));
	}
	else
	{
		SetSelection(false);
	}
	m_bClick = !m_bClick;


	//SetClick(!m_bClick);
	CButton::OnTimer(nIDEvent);
}
