#include "pch.h"
#include "CBattPartView.h"
#include <algorithm>
#define NOTUSED_RATIO 0.15
#define CELL_HEIGHT 50

BEGIN_MESSAGE_MAP(CBattPartView, CWnd)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

CBattPartView::CBattPartView(int nId)
{
	if (g_gps.m_bSuccess == FALSE) {
		return;
	}
	else {
	}
	m_Bkg = nullptr;
	m_nId = nId;
}

CBattPartView::~CBattPartView()
{
}

void CBattPartView::initViewer(CWnd * pWnd)
{
	//m_Bkg = Image::FromFile(L"res\\smallBatt.bmp");
	m_Bkg = Image::FromFile(L"smallBatt.bmp");
	if ((m_Bkg->GetLastStatus() != Gdiplus::Ok))
	{
		return;
	}
	m_thisWnd = pWnd;
	pWnd->GetClientRect(&m_rcView);
	m_parent = pWnd->GetParent()->m_hWnd;
}

void CBattPartView::setPartEnable(bool bEnable[PARTNUM])
{
	for (int i = 0; i < PARTNUM; i++)
	{
		m_bPartEnale[i] = bEnable[i];
	}
}

void CBattPartView::getPartEnable(bool bEnable[PARTNUM])
{
	for (int i = 0; i < PARTNUM; i++)
	{
		bEnable[i] = m_bPartEnale[i];
	}
}

void CBattPartView::Invalidate()
{
	draw();
}

bool CBattPartView::setPartNum(int nParts)
{
	if (nullptr == m_Bkg) {
		m_vWidths.clear();
		return false;
	}
	if (nParts < 3) {
		setPartNum(3);
		return false;
	}
	if (nParts > 10){
		setPartNum(10);
		return false;
	}
	int nPartNum = nParts;
	m_vWidths.clear();
	int nWidth = m_Bkg->GetWidth();
	int nHeight = m_Bkg->GetHeight();
	int nNotUsedWidth = NOTUSED_RATIO * nWidth;
	float fValidWidth = nWidth - nNotUsedWidth * 2;
	float fSteps = fValidWidth / nPartNum;
	for (int i = 0; i <= nPartNum; i++)
	{
		float fVal = nNotUsedWidth + fSteps * i;
		m_vWidths.push_back(fVal);
	}
	return true;
}

void CBattPartView::setStartPos(double dStPos)
{
	if (nullptr == m_Bkg) {
		dStPos = -1.0;
		return;
	}
	m_dStPos = dStPos;
}

int CBattPartView::getPartNum()
{
	return (m_vWidths.size() < 1) ? 0: (int)m_vWidths.size() -1;
}

bool CBattPartView::getPartEnable(int nIdx)
{
	if (0 <= nIdx && nIdx < PARTNUM)
		return m_bPartEnale[nIdx];
	else
		return false;
}

void CBattPartView::setPartEnable(int nIdx, bool bEnable)
{
	if (0 <= nIdx && nIdx < PARTNUM)
	{
		m_bPartEnale[nIdx] = bEnable;
	}
}

void CBattPartView::draw()
{
 	CClientDC dc(m_thisWnd);
	CRect rect = m_rcView;

	CDC memDC; // 메모리 DC
	CBitmap *pOldBitmap, bitmap;
	memDC.CreateCompatibleDC(&dc);		// Picture Control DC에 호환되는 새로운 CDC를 생성. (임시 버퍼)

	bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());	// Picture Control의 크기와 동일한 비트맵을 생성.
	pOldBitmap = memDC.SelectObject(&bitmap);		// 임시 버퍼에서 방금 생성한 비트맵을 선택하면서, 이전 비트맵을 보존.
	memDC.PatBlt(0, 0, rect.Width(), rect.Height(), WHITENESS);	// 임시 버퍼에 흰색으로 채움.

	// 임시 버퍼(memDC)에 그리는 동작을 수행.///////////////////////////////////////////////////////////////
	Gdiplus::Graphics G(memDC.m_hDC);
	int nWidth = m_Bkg->GetWidth();
	int nHeight = m_Bkg->GetHeight();
	G.DrawImage(m_Bkg, 0, 0, nWidth, nHeight);
	drawParts(G);

	///////////임시 버퍼(memDC)에 그리는 동작을 수행.//////////////////////////////////////////////////// 
	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);	// 임시 버퍼를 Picture Control에 그린다.
	memDC.SelectObject(pOldBitmap);// 이전 비트맵으로 재설정.
	memDC.DeleteDC();			// 생성한 리소스 해제.
	bitmap.DeleteObject();
}

void CBattPartView::drawParts(Gdiplus::Graphics & G)
{
	CStringW szCurInfo;
	Gdiplus::RectF rcBar;
	Gdiplus::PointF ptInfo;
	Gdiplus::StringFormat format;
	format.SetAlignment(Gdiplus::StringAlignmentCenter);
	int nWidth = m_Bkg->GetWidth();
	int nHeight = m_Bkg->GetHeight();
	int nNotUsedWidth = NOTUSED_RATIO * nWidth;
	Gdiplus::Color clrSel = Gdiplus::Color(180, 30, 180, 56);
	Gdiplus::Color clrUnSel = Gdiplus::Color(180, 250, 56, 30);
	Gdiplus::Color clrHoverOnSel = Gdiplus::Color(80, 30, 180, 56);
	Gdiplus::Color clrHoverOnUnSel = Gdiplus::Color(80, 250, 56, 30);
	Gdiplus::Color clrNotUsed = Gdiplus::Color(120, 250, 56, 30);
	Gdiplus::Font fntX(L"Century Gothic", 15, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
	/////////////////////////////////////////////////
	// 1. Not Used 영역
	rcBar = Gdiplus::RectF(0, nHeight - CELL_HEIGHT, nNotUsedWidth, CELL_HEIGHT);
	G.FillRectangle(&SolidBrush(clrNotUsed), rcBar);
	ptInfo = Gdiplus::PointF(rcBar.X + rcBar.Width / 2, rcBar.Y + 15);
	szCurInfo = L"Not Used.";
	//if (m_dStPos < 0) {
	//	szCurInfo = L"Not Used.";
	//}
	//else {
	//	szCurInfo.Format(L"Not Used.\n ~ %.2fmm", m_dStPos);
	//}
	G.DrawString(szCurInfo, -1, &fntX, ptInfo, &format, &SolidBrush(Gdiplus::Color(255, 255, 255)));

	rcBar = Gdiplus::RectF(nWidth - nNotUsedWidth, nHeight - CELL_HEIGHT, nNotUsedWidth, CELL_HEIGHT);
	G.FillRectangle(&SolidBrush(clrNotUsed), rcBar);
	ptInfo = Gdiplus::PointF(rcBar.X + rcBar.Width / 2, rcBar.Y + 15);
	format.SetAlignment(Gdiplus::StringAlignmentCenter);
	szCurInfo = L"Not Used.";
	G.DrawString(szCurInfo, -1, &fntX, ptInfo, &format, &SolidBrush(Gdiplus::Color(255, 255, 255)));
	/////////////////////////////////////////////////
	// 2. Cell 영역
	if (m_vWidths.size() > 0)
	{
		int nPartNum = m_vWidths.size()-1;
		int nNotUsedWidth = NOTUSED_RATIO * nWidth;
		float fSteps = (m_vWidths.back() - nNotUsedWidth) / (float)nPartNum;
		for (int i = 0; i < nPartNum; i++)
		{
			rcBar = Gdiplus::RectF(m_vWidths.at(i), nHeight - CELL_HEIGHT, fSteps, CELL_HEIGHT);
			Gdiplus::Color clr;
			if (m_nCurIdx == i){
				if (m_bPartEnale[i]) { clr = clrHoverOnSel; }
				else { clr = clrHoverOnUnSel; }
			}else {
				if (m_bPartEnale[i]) { clr = clrSel; }					
				else { clr = clrUnSel; }								
			}
			G.FillRectangle(&SolidBrush(clr), rcBar);
			rcBar.Offset(0, 1);
			G.DrawRectangle(&Pen(Gdiplus::Color(255, 255, 255), 2), rcBar);

			CString szIdx = L"";
			if (i == 0) { szIdx = L"A"; }
			else if (i == 1) { szIdx = L"B"; }
			else if (i == 2) { szIdx = L"C"; }
			else if (i == 3) { szIdx = L"D"; }
			else if (i == 4) { szIdx = L"E"; }
			else if (i == 5) { szIdx = L"F"; }
			else if (i == 6) { szIdx = L"G"; }
			else if (i == 7) { szIdx = L"H"; }
			else if (i == 8) { szIdx = L"I"; }
			else if (i == 9) { szIdx = L"J"; }

			if (nPartNum < 8) {
				ptInfo = Gdiplus::PointF(rcBar.X + rcBar.Width / 2, rcBar.Y + 15);
				//szCurInfo.Format(L"Part %d", i + 1);
				szCurInfo.Format(L"Part %ls", szIdx);
			}
			else {
				ptInfo = Gdiplus::PointF(rcBar.X + rcBar.Width / 2, rcBar.Y + 5);
				//szCurInfo.Format(L"Part \n%d", i + 1);
				szCurInfo.Format(L"Part \n%ls", szIdx);
			}
			G.DrawString(szCurInfo, -1, &fntX, ptInfo, &format, &SolidBrush(Gdiplus::Color(255, 255, 255)));
		}
	}
}

int CBattPartView::getIdxOnMouse(CPoint point)
{
	int nHeight = m_Bkg->GetHeight();
	int nWidth = m_Bkg->GetWidth();
	bool bFind = false;
	int i(0);
	if (nHeight - CELL_HEIGHT+5 < point.y && point.y < nHeight-5 && m_vWidths.size()>0)
	{
		int nPartNum = m_vWidths.size() - 1;
		int nNotUsedWidth = NOTUSED_RATIO * nWidth;
		float fSteps = (m_vWidths.back() - nNotUsedWidth) / (float)nPartNum;
		for (i = 0; i < m_vWidths.size()-1; i++)
		{
			Gdiplus::RectF rcBar = Gdiplus::RectF(m_vWidths.at(i), nHeight - CELL_HEIGHT, fSteps, CELL_HEIGHT);
			if (rcBar.Contains(Gdiplus::PointF(point.x, point.y))) {
				bFind = true;
				break;
			}
		}
	}
	return (bFind == true) ? i : -1;
}

void CBattPartView::OnLButtonUp(UINT nFlags, CPoint point)
{
	int ret = getIdxOnMouse(point);
	if (ret >= 0) {
		bool bEnable = getPartEnable(ret);
		setPartEnable(ret, !bEnable);
		Invalidate();
		m_thisWnd->GetParent()->SendMessage(STATUS_CHANGE, m_nId, 0);
	}
	CWnd::OnLButtonUp(nFlags, point);
}


void CBattPartView::OnMouseMove(UINT nFlags, CPoint point)
{
	int ret = getIdxOnMouse(point);
	if (ret != m_nCurIdx)
	{
		m_nCurIdx = ret;
		Invalidate();
	}
	CWnd::OnMouseMove(nFlags, point);
}


BOOL CBattPartView::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CWnd::DestroyWindow();
}
