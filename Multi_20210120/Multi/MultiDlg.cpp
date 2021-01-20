#include "pch.h"
#include "framework.h"
#include "Multi.h"
#include "MultiDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMultiDlg::CMultiDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MULTI_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMultiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ST_SW_NAME, m_stSwName);
	DDX_Control(pDX, IDC_ST_SW_VERINFO, m_stVerInfo);
	DDX_Control(pDX, IDC_ST_RCP_NAME, m_stCurRcpName);
}

BEGIN_MESSAGE_MAP(CMultiDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_EXIT, &CMultiDlg::OnBnClickedBtnExit)
	ON_BN_CLICKED(IDC_BTN_MINIMIZE, &CMultiDlg::OnBnClickedBtnMinimize)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BTN_INSP, &CMultiDlg::OnBnClickedBtnInsp)
	ON_BN_CLICKED(IDC_BTN_RCP, &CMultiDlg::OnBnClickedBtnRcp)
	ON_BN_CLICKED(IDC_BTN_LOG, &CMultiDlg::OnBnClickedBtnLog)
	ON_BN_CLICKED(IDC_BTN_RECT, &CMultiDlg::OnBnClickedBtnRect)
END_MESSAGE_MAP()

#include <vector>
BOOL CMultiDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetIcon(m_hIcon, TRUE);	
	SetIcon(m_hIcon, FALSE);

	//SetBackgroundColor(RGB(244,114,0));
	SetBackgroundImage(IDB_MAINBK);

	// Calculate Rect
	CRect rtArea;
	GetDlgItem(IDC_AREA_BODY)->GetWindowRect(rtArea);
	ScreenToClient(rtArea);

	std::vector<CDialogEx*> vtDlgs;
	vtDlgs.push_back(&m_DlgInsp);
	vtDlgs.push_back(&m_DlgLog);
	vtDlgs.push_back(&m_DlgRcp);
	vtDlgs.push_back(&m_DlgRect);
	for (int i = 0; i < vtDlgs.size(); i++)
	{
		int nId = -1;
		if (i == 0) {
			nId = IDD_DLG_INSP;
		}
		if (i == 1) {
			nId = IDD_DLG_LOG;
		}
		if (i == 2) {
			nId = IDD_DLG_RCP;
		}
		if (i == 3) {
			nId = IDD_DLG_RECT;
		}
		vtDlgs[i]->Create(nId, this);
		vtDlgs[i]->MoveWindow(&rtArea);
		vtDlgs[i]->ShowWindow(SW_HIDE);
	}

	CDialogEx* arrDlg[4];
	arrDlg[0] = &m_DlgInsp;
	arrDlg[1] = &m_DlgLog;
	arrDlg[2] = &m_DlgRcp;
	arrDlg[3] = &m_DlgRect;

	//auto func1 = [&](int nId, CDialogEx*dlg) {
	//	dlg->Create(nId, this);
	//	dlg->MoveWindow(&rtArea);
	//	dlg->ShowWindow(SW_HIDE);
	//};	
	//func1(IDD_DLG_INSP, &m_DlgInsp);
	//func1(IDD_DLG_LOG, arrDlg[1]);
	//func1(IDD_DLG_RCP, arrDlg[2]);

	//for (int i = 0; i < 3; i++)
	//{
	//	int nId = -1;
	//	if (i == 0) {
	//		nId = IDD_DLG_INSP;
	//	}
	//	if (i == 1) {
	//		nId = IDD_DLG_LOG;
	//	}
	//	if (i == 2) {
	//		nId = IDD_DLG_RCP;
	//	}
	//	arrDlg[i]->Create(nId, this);
	//	arrDlg[i]->MoveWindow(&rtArea);
	//	arrDlg[i]->ShowWindow(SW_HIDE);
	//}
	arrDlg[0]->ShowWindow(SW_SHOW);

	//CDlgInsp* arrDlg2[2];
	//arrDlg2[0] = &m_DlgLog;
	//arrDlg2[1] = &m_DlgInsp;

	//m_DlgInsp.Create(IDD_DLG_INSP, this);
	//m_DlgInsp.MoveWindow(&rtArea);
	//m_DlgInsp.ShowWindow(SW_SHOW);

	//m_DlgLog.Create(IDD_DLG_LOG, this);
	//m_DlgLog.MoveWindow(&rtArea);
	//m_DlgLog.ShowWindow(SW_HIDE);

	//CDialogEx *rcpDlg = &m_DlgRcp;
	////CDlgRcp *rcpDlg = &m_DlgRcp;
	//rcpDlg->Create(IDD_DLG_RCP, this);
	//rcpDlg->MoveWindow(&rtArea);
	//rcpDlg->ShowWindow(SW_HIDE);
	//rcpDlg->message();


	//m_stSwName.SubclassDlgItem(IDC_ST_SW_NAME, this);
	m_stSwName.SetTextColor(RGB(255, 255, 255));
	m_stSwName.SetBkColor(RGB(36, 36, 38));
	m_stSwName.SetFont(_T("Arial"), 16, FW_BOLD);
	m_stSwName.SetWindowText(L"Battery W Inspection");


	m_stVerInfo.SetTextColor(RGB(255, 255, 255));
	m_stVerInfo.SetBkColor(RGB(36, 36, 38));
	m_stVerInfo.SetFont(_T("Arial"), 16, FW_BOLD);
	m_stVerInfo.SetWindowText(L"v1.0.0.1");


	m_stCurRcpName.SetTextColor(RGB(255, 255,0));
	m_stCurRcpName.SetBkColor(RGB(36, 36, 38));
	m_stCurRcpName.SetFont(_T("Arial"), 16, FW_BOLD);
	m_stCurRcpName.SetWindowText(L"Default");




	return TRUE; 
}

void CMultiDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialogEx::OnSysCommand(nID, lParam);
}

void CMultiDlg::OnPaint()
{
	CDialogEx::OnPaint();
	/*CClientDC dc(this);
	Graphics g(dc);
	Pen Redpen(Color(100, 255, 0, 0), 3);
	SolidBrush Redbrs(Color(100, 255, 0, 0));
	Pen Greenpen(Color(100, 0, 255, 0), 3);
	SolidBrush Greenbrs(Color(100, 0, 255, 0));


	int xPos = 10;

	
	for (int i = 0; i < 12; i++)
	{
		if (i == 0 || i == 11)
		{
			g.DrawRectangle(&Redpen, xPos, 700, 50, 80);
			g.FillRectangle(&Redbrs, xPos, 700, 50, 80);
		}
		else
		{
			g.DrawRectangle(&Greenpen, xPos, 700, 50, 80);
			g.FillRectangle(&Greenbrs, xPos, 700, 50, 80);
		}
		xPos += 55;
	}*/
}

HCURSOR CMultiDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMultiDlg::OnBnClickedBtnExit()
{
	ShowWindow(SW_MINIMIZE);
	PostMessage(WM_CLOSE);
}


void CMultiDlg::OnBnClickedBtnMinimize()
{
	ShowWindow(SW_MINIMIZE);
}


void CMultiDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, 0);
}


void CMultiDlg::OnBnClickedBtnInsp()
{
	m_DlgLog.ShowWindow(SW_HIDE);
	m_DlgRcp.ShowWindow(SW_HIDE);
	m_DlgInsp.ShowWindow(SW_SHOW);
	m_DlgRect.ShowWindow(SW_HIDE);
}


void CMultiDlg::OnBnClickedBtnRcp()
{
	m_DlgLog.ShowWindow(SW_HIDE);
	m_DlgInsp.ShowWindow(SW_HIDE);
	m_DlgRcp.ShowWindow(SW_SHOW);
	m_DlgRect.ShowWindow(SW_HIDE);
}


void CMultiDlg::OnBnClickedBtnLog()
{
	m_DlgInsp.ShowWindow(SW_HIDE);
	m_DlgRcp.ShowWindow(SW_HIDE);
	m_DlgLog.ShowWindow(SW_SHOW);
	m_DlgRect.ShowWindow(SW_HIDE);
}


BOOL CMultiDlg::DestroyWindow()
{
	m_DlgLog.DestroyWindow();
	m_DlgInsp.DestroyWindow();
	m_DlgRcp.DestroyWindow();
	m_DlgRect.DestroyWindow();
	return CDialogEx::DestroyWindow();
}


void CMultiDlg::OnBnClickedBtnRect()
{
	m_DlgInsp.ShowWindow(SW_HIDE);
	m_DlgRcp.ShowWindow(SW_HIDE);
	m_DlgLog.ShowWindow(SW_HIDE);
	m_DlgRect.ShowWindow(SW_SHOW);
}
