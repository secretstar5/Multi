// DlgInsp.cpp: 구현 파일
//

#include "pch.h"
#include "Multi.h"
#include "DlgInsp.h"
#include "afxdialogex.h"


// CDlgInsp 대화 상자

IMPLEMENT_DYNAMIC(CDlgInsp, CDialogEx)

CDlgInsp::CDlgInsp(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_INSP, pParent)
{

}

CDlgInsp::~CDlgInsp()
{
}

void CDlgInsp::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_test111);
}


BEGIN_MESSAGE_MAP(CDlgInsp, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgInsp::OnBnClickedButton1)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDlgInsp 메시지 처리기

#include "DialogTenkey.h"
void CDlgInsp::OnBnClickedButton1()
{
	//{
	//	CDialogTenkey dlg;
	//	dlg.DoModal();
	//}


	//{
	//	CDialogTenkey	dlg;
	//	dlg.CommentSet(L"Key Pad Test", dtDOUBLE, 0, -100, 1000, 200, 300);
	//	if (dlg.DoModal() == IDOK)
	//	{
	//		CString szData;
	//		szData.Format(L"%.3f", dlg.m_dEditValue);
	//		//SetDlgItemText(nUiId, szData);
	//		//GetDlgItem(nUiId)->Invalidate();
	//		double keyValue = _ttof(szData);
	//		int a = 1;
	//		//if (dMin < keyValue && keyValue < dMax) {
	//		//	dVal = keyValue;
	//		//	return true;
	//		//}
	//	};
	//}
	m_BattPartView->Invalidate();

}


BOOL CDlgInsp::DestroyWindow()
{
	
	if (m_BattPartView)
	{
		m_BattPartView->DestroyWindow();
		delete m_BattPartView;
		m_BattPartView = nullptr;
	}
	return CDialogEx::DestroyWindow();
}


BOOL CDlgInsp::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	{
		m_BattPartView = new CBattPartView(3);
		UINT uID = IDC_ST_RCP_BATTPART1;
		CRect rcView;
		GetDlgItem(uID)->GetWindowRect(rcView);
		ScreenToClient(rcView);
		m_BattPartView->Create(AfxRegisterWndClass(NULL), L"BatteryPartView", WS_CHILD | WS_VISIBLE, rcView, this, uID, NULL);
		m_BattPartView->ModifyStyle(WS_EX_CLIENTEDGE, NULL);
		m_BattPartView->MoveWindow(rcView);
		m_BattPartView->initViewer(GetDlgItem(uID));


		//m_BattPartView->Invalidate();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgInsp::OnPaint()
{
	CPaintDC dc(this); 
	//m_BattPartView->Invalidate();
}
