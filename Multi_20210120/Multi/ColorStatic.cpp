// ColorStatic.cpp : implementation file
//

//#include "stdafx.h"
#include "pch.h"
#include "ColorStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorStatic

CColorStatic::CColorStatic()
{
	m_brush.CreateSolidBrush(RGB(255,255,255)) ;
//	m_clrFore	= RGB(255,255,255);
//	m_clrBack	= RGB(255,255,255);
}

CColorStatic::~CColorStatic()
{
	m_brush.DeleteObject() ;
}


BEGIN_MESSAGE_MAP(CColorStatic, CStatic)
	//{{AFX_MSG_MAP(CColorStatic)
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorStatic message handlers

HBRUSH CColorStatic::CtlColor(CDC* pDC, UINT /*nCtlColor*/) 
{
//	HBRUSH hbr = CStatic::OnCtlColor(pDC, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	pDC->SetTextColor(m_clrFore) ;
	pDC->SetBkMode(TRANSPARENT) ;
	return (HBRUSH) m_brush.m_hObject ;	
	// TODO: Return a different brush if the default is not desired
//	return hbr;
}

void CColorStatic::SetBackColor( COLORREF clrBack )
{
	m_clrBack = clrBack ;

	m_brush.DeleteObject() ;
	m_brush.CreateSolidBrush(m_clrBack) ;
}

void CColorStatic::SetForeColor( COLORREF clrFore ) 
{
	m_clrFore = clrFore ;
}
