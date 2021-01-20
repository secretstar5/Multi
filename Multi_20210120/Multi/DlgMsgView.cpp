#include "pch.h"
#include "DlgMsgView.h"
#include "afxdialogex.h"
IMPLEMENT_DYNAMIC(CDlgFloatMsg, CDialogEx)
CDlgFloatMsg::CDlgFloatMsg(CWnd* pParent /*=NULL*/): CDialogEx(CDlgFloatMsg::IDD, pParent)
{
	m_nFontSize = 13;
}
CDlgFloatMsg::~CDlgFloatMsg()
{
}

void CDlgFloatMsg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STC_MSG_TXT, m_ctrlMsgTxt);
}
BEGIN_MESSAGE_MAP(CDlgFloatMsg, CDialogEx)
END_MESSAGE_MAP()

BOOL CDlgFloatMsg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetBackgroundColor(RGB(250, 250, 250));
	m_fntStatic.CreatePointFont(125, _T("Arial Bold"));
	GetDlgItem(IDOK)->SetFont(&m_fntStatic);
	GetDlgItem(IDCANCEL)->SetFont(&m_fntStatic);

	m_ctrlMsgTxt
		.SetFontName(_T("Arial Bold"))
		.SetFontSize(m_nFontSize)
		.SetTextColor(RGB(200,255,200))
		.SetFontBold(TRUE)
		.SetBorder(FALSE)
		.SetSunken(TRUE)
		.SetBkColor(RGB(128,128,128))
		.SetText(L"None Message");
	SetDlgItemText(IDOK, _T("close"));
	GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
	GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);
	return TRUE;  
}

BOOL CDlgFloatMsg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			pMsg->message = NULL;
	};
	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CDlgFloatMsg::DestroyWindow()
{
	m_fntStatic.DeleteObject();
	return CDialogEx::DestroyWindow();
}

void CDlgFloatMsg::updateMsg(CString szMsg)
{
	m_ctrlMsgTxt.SetText(szMsg);
}
//////////////////////////////////////////////////////////////////////////////////





IMPLEMENT_DYNAMIC(CDlgBlockMsg, CDialogEx)
CDlgBlockMsg::CDlgBlockMsg(CWnd * pParent /*=NULL*/) : CDialogEx(CDlgBlockMsg::IDD, pParent)
{
	m_strMsg = _T("None Message");
	m_nFontSize = 13;
	m_bCheckMsg = false;
}

CDlgBlockMsg::~CDlgBlockMsg()
{
}


void CDlgBlockMsg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_STC_MSG_TXT, m_ctrlMsgTxt);
	CDialogEx::DoDataExchange(pDX);
}
BEGIN_MESSAGE_MAP(CDlgBlockMsg, CDialogEx)
END_MESSAGE_MAP()


BOOL CDlgBlockMsg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetBackgroundColor(RGB(250, 250, 250));
	m_fntStatic.CreatePointFont(125, _T("Arial Bold"));
	GetDlgItem(IDOK)->SetFont(&m_fntStatic);
	GetDlgItem(IDCANCEL)->SetFont(&m_fntStatic);

	m_ctrlMsgTxt
		.SetFontName(_T("Arial Bold"))
		.SetFontSize(m_nFontSize)
		.SetTextColor(RGB(200, 255, 200))
		.SetFontBold(TRUE)
		.SetBorder(FALSE)
		.SetSunken(TRUE)
		.SetBkColor(RGB(128, 128, 128))
		.SetText(m_strMsg);

	// 단순 확인 버튼인 경우.
	if (m_bCheckMsg)
	{
		SetDlgItemText(IDOK, _T("OK"));
		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
		GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);
	}
	return TRUE;
}

BOOL CDlgBlockMsg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			pMsg->message = NULL;
	};
	return CDialogEx::PreTranslateMessage(pMsg);
}
BOOL CDlgBlockMsg::ShowMsg(bool bCheckMsg, CString strMsg)
{
	m_strMsg = strMsg;
	m_bCheckMsg = bCheckMsg;
	if (bCheckMsg == mvYESNO)			// Ok, NG 확인 할 경우.
	{
		if (DoModal() != IDOK)	return FALSE;
	}
	else
		DoModal();
	return TRUE;
}

BOOL CDlgBlockMsg::DestroyWindow()
{
	m_fntStatic.DeleteObject();
	return CDialogEx::DestroyWindow();
}

IMPLEMENT_DYNAMIC(CDlgTimerMsg, CDialogEx)
CDlgTimerMsg::CDlgTimerMsg(int nTimerCnt, CWnd * pParent /*=NULL*/) : m_nTimerTime(nTimerCnt), CDialogEx(CDlgTimerMsg::IDD, pParent)
{
	m_strMsg = _T("None Message");
	m_nFontSize = 13;
}

CDlgTimerMsg::~CDlgTimerMsg()
{
}

BOOL CDlgTimerMsg::ShowMsg(CString strMsg)
{
	m_strMsg = strMsg;
	DoModal();
	return true;
}

void CDlgTimerMsg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_STC_MSG_TXT, m_ctrlMsgTxt);
	CDialogEx::DoDataExchange(pDX);
}
BEGIN_MESSAGE_MAP(CDlgTimerMsg, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


BOOL CDlgTimerMsg::PreTranslateMessage(MSG* pMsg)
{
	if (WM_SYSKEYDOWN == pMsg->message && VK_F4 == pMsg->wParam) {
		return TRUE;
	}
	if (WM_KEYDOWN == pMsg->message)
	{
		if (VK_ESCAPE == pMsg->wParam || VK_EXECUTE == pMsg->wParam || VK_RETURN == pMsg->wParam)
			return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CDlgTimerMsg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetBackgroundColor(RGB(250, 250, 250));
	m_fntStatic.CreatePointFont(125, _T("Arial Bold"));
	GetDlgItem(IDOK)->SetFont(&m_fntStatic);
	GetDlgItem(IDCANCEL)->SetFont(&m_fntStatic);

	m_ctrlMsgTxt
		.SetFontName(_T("Arial Bold"))
		.SetFontSize(m_nFontSize)
		.SetTextColor(RGB(200, 255, 200))
		.SetFontBold(TRUE)
		.SetBorder(FALSE)
		.SetSunken(TRUE)
		.SetBkColor(RGB(128, 128, 128))
		.SetText(m_strMsg);

	GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
	GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	GetDlgItem(IDOK)->ShowWindow(SW_HIDE);

	SetTimer(100, 1000, NULL);
	return TRUE;
}


BOOL CDlgTimerMsg::DestroyWindow()
{
	m_fntStatic.DeleteObject();
	return CDialogEx::DestroyWindow();
}

void CDlgTimerMsg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 100)
	{
		m_nCurCnt++;
		if (m_nTimerTime == m_nCurCnt)
		{
			KillTimer(100);
			CDialogEx::OnCancel();
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}
