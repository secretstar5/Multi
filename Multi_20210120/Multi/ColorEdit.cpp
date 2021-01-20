// ColorEdit.cpp : implementation file
//

#include "pch.h"
#include "ColorEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorEdit

CColorEdit::CColorEdit()
{
	m_brush.CreateSolidBrush(RGB(255,255,255)) ;
	m_clrDisable	= ::GetSysColor( COLOR_BTNFACE );
	m_nReceivedFlag = 0 ;
	m_clrFore		= BLACK_COLOR;
	m_clrBack		= WHITE_COLOR;
}

CColorEdit::~CColorEdit()
{
}


BEGIN_MESSAGE_MAP(CColorEdit, CEdit)
	//{{AFX_MSG_MAP(CColorEdit)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorEdit message handlers

HBRUSH CColorEdit::CtlColor(CDC* pDC, UINT /*nCtlColor*/) 
{
//	HBRUSH hbr = CEdit::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	pDC->SetTextColor(m_clrFore) ;
	pDC->SetBkMode(TRANSPARENT) ;
	return (HBRUSH) m_brush.m_hObject ;
	
	// TODO: Return a different brush if the default is not desired
//	return hbr;
}

void CColorEdit::SetBackColor( COLORREF clrBack )
{
	m_clrBack = clrBack ;

	m_brush.DeleteObject() ;
	m_brush.CreateSolidBrush(m_clrBack) ;
}

void CColorEdit::SetForeColor( COLORREF clrFore ) 
{
	m_clrFore = clrFore ;
}

void CColorEdit::SetDisableBackColor()
{
	m_brush.DeleteObject();
	m_brush.CreateSolidBrush(m_clrDisable);
}

void CColorEdit::SetReceivedFlag(int flag)
{
	m_nReceivedFlag = flag ;
}

void CColorEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	switch(m_nReceivedFlag)
	{
	case -1 : // Read-Only
		break;
	case 1 :  // Unsigned Integer
		if( ( nChar >= _T('0') && nChar <= _T('9') ) || ( VK_BACK == nChar ) )
			CEdit::OnChar(nChar,nRepCnt, nFlags);
		else
			::MessageBeep(MB_ICONASTERISK);
		break;
	case 5 : // Signed Integer
		if( ( nChar >= _T('0') && nChar <= _T('9') ) || ( _T('-') == nChar ) || ( VK_BACK == nChar ) )
			CEdit::OnChar(nChar,nRepCnt, nFlags);
		else
			::MessageBeep(MB_ICONASTERISK);
		break;
	case 2 : // Alphabet
		if( ( nChar >= _T('A') && nChar <= _T('Z') ) || ( nChar >= _T('a') && nChar <= _T('z') ) || ( VK_BACK == nChar ) )
			CEdit::OnChar(nChar, nRepCnt, nFlags);
		else
			::MessageBeep(MB_ICONASTERISK);
		break;
	case 3 : // Floating-Point
		if( ( nChar >= _T('0') && nChar <= _T('9') ) || ( VK_BACK == nChar ) || ( _T('.') == nChar ) || ( _T('-') == nChar ) )
			CEdit::OnChar(nChar,nRepCnt, nFlags);
		else
			::MessageBeep(MB_ICONASTERISK);
		break;
	case 4 : // Alpha-Numeric
		if( ( nChar >= _T('0') && nChar <= _T('9') ) || ( VK_BACK == nChar ) ||
			( nChar >= _T('A') && nChar <= _T('Z') ) || ( nChar >= _T('a') && nChar <= _T('z') ) ||
			( _T('\\') == nChar ) || ( _T('-') == nChar ) || ( _T('_') == nChar ) || 
			( _T(' ') == nChar ) || ( _T('/') == nChar ) || ( _T('.') == nChar ) )
			CEdit::OnChar(nChar,nRepCnt, nFlags);
		else
			::MessageBeep(MB_ICONASTERISK);
		break;
	case 100 :
		::MessageBeep(MB_ICONASTERISK);
		break;
	default :
		CEdit::OnChar(nChar,nRepCnt, nFlags);
		break;
	}
}

void CColorEdit::EnableWindow(BOOL bEnable)
{
	if( bEnable )
		SetBackColor( m_clrBack );
	else
		SetDisableBackColor();

	CEdit::EnableWindow(bEnable);
}