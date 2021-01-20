// DlgRect.cpp: 구현 파일
//

#include "pch.h"
#include "Multi.h"
#include "DlgRect.h"
#include "afxdialogex.h"


// CDlgRect 대화 상자

IMPLEMENT_DYNAMIC(CDlgRect, CDialogEx)

void CDlgRect::OnDrawImage()
{
	// Picture Control DC를 생성
	CClientDC dc(GetDlgItem(IDC_PC_IMAGE));

	// Picture Control 크기를 얻는다.
	CRect rect;
	GetDlgItem(IDC_PC_IMAGE)->GetClientRect(&rect);

	//// Picture Control에 호환되는 Graphics 생성
	Graphics mainG(dc.GetSafeHdc());

	//// Picture Control 크기로 비트맵(임시 버퍼) 생성
	Bitmap memBmp(rect.Width(), rect.Height());

	//// 임시 버퍼에 호환되는 Graphics 생성
	Graphics memG(&memBmp);

	//// 임시 버퍼에 검은색으로 채우기
	SolidBrush brs(Color(255, 255, 255, 255));
	memG.FillRectangle(&brs, 0, 0, rect.Width(), rect.Height());

	//CDC memDC; // 메모리 DC
	//memDC.CreateCompatibleDC(&dc);
	//
	//memDC.PatBlt(0, 0, rect.Width(), rect.Height(), WHITENESS);	// 임시 버퍼에 흰색으로 채움.

	Pen Redpen(Color(100, 255, 0, 0), 3);
	SolidBrush Redbrs(Color(100, 255, 0, 0));
	Pen Greenpen(Color(100, 0, 255, 0), 3);
	SolidBrush Greenbrs(Color(100, 0, 255, 0));
	
	SolidBrush textBR(Color(0, 0, 0));
	Gdiplus::Font font(_T("맑은 고딕"), 10, FontStyleBold, UnitPixel);
	memG.SetTextRenderingHint(TextRenderingHintAntiAlias);




	for (int i = 0; i < 12; i++)
	{
		int nXPos = 200 + 55*i;
		if (i == 0 || i == 11)
		{
			CString strDraw = _T("Not Used");
			memG.DrawString(strDraw, -1, &font, PointF(nXPos, 135), &textBR);
			memG.DrawRectangle(&Redpen, nXPos, 100, 50, 80);
			memG.FillRectangle(&Redbrs, nXPos, 100, 50, 80);
		}
		else
		{
			CString strDraw;
			strDraw.Format(L"Part %d", i);
			memG.DrawString(strDraw, -1, &font, PointF(nXPos +8, 135), &textBR);
			memG.DrawRectangle(&Greenpen, nXPos, 100, 50, 80);
			memG.FillRectangle(&Greenbrs, nXPos, 100, 50, 80);
		}
	}

	mainG.DrawImage(&memBmp, 0, 0);
}

CDlgRect::CDlgRect(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_RECT, pParent)
{

}

CDlgRect::~CDlgRect()
{
}

void CDlgRect::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgRect, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgRect::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDlgRect 메시지 처리기




void CDlgRect::OnBnClickedButton1()
{
	OnDrawImage();
}
