// DlgRcp.cpp: 구현 파일
//

#include "pch.h"
#include "Multi.h"
#include "DlgRcp.h"
#include "afxdialogex.h"


// CDlgRcp 대화 상자

IMPLEMENT_DYNAMIC(CDlgRcp, CDialogEx)

CDlgRcp::CDlgRcp(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_RCP, pParent)
{

}

CDlgRcp::~CDlgRcp()
{
}

void CDlgRcp::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgRcp, CDialogEx)
END_MESSAGE_MAP()


// CDlgRcp 메시지 처리기
