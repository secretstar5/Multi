// DlgLog.cpp: 구현 파일
//

#include "pch.h"
#include "Multi.h"
#include "DlgLog.h"
#include "afxdialogex.h"


// CDlgLog 대화 상자

IMPLEMENT_DYNAMIC(CDlgLog, CDialogEx)

CDlgLog::CDlgLog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_LOG, pParent)
{

}

CDlgLog::~CDlgLog()
{
}

void CDlgLog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgLog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgLog::OnBnClickedButton2)
END_MESSAGE_MAP()


// CDlgLog 메시지 처리기


void CDlgLog::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


BOOL CDlgLog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetBackgroundColor(RGB(150, 100, 30));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
