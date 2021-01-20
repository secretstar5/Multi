#include "pch.h"
#include "DialogTenkey.h"
#include <regex>
IMPLEMENT_DYNAMIC(CDialogTenkey, CDialogEx)

CDialogTenkey::CDialogTenkey(CWnd * pParent)
	: CDialogEx(CDialogTenkey::IDD, pParent)
{
	m_dSetValue = 0.0;
	m_dEditValue = 0.0;
	m_strEditValue = _T("");
	m_strName = _T("");
	m_kptInt = dtDOUBLE;
	m_dMin = -3000.;
	m_dMax = 3000.;
	m_nDlgX = 300;
	m_nDlgY = 300;
}

CDialogTenkey::~CDialogTenkey()
{
}


void CDialogTenkey::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_EDIT_VALUE, m_edtValue);
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	DDX_Control(pDX, IDC_STATIC_MIN, m_staticMin);
	DDX_Control(pDX, IDC_STATIC_MAX, m_staticMax);
}
BEGIN_MESSAGE_MAP(CDialogTenkey, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_NUM_0, &CDialogTenkey::OnBnClickedButtonNum0)
	ON_BN_CLICKED(IDC_BUTTON_DOT, &CDialogTenkey::OnBnClickedButtonDot)
	ON_BN_CLICKED(IDC_BUTTON_DASH, &CDialogTenkey::OnBnClickedButtonDash)
	ON_BN_CLICKED(IDC_BUTTON_NUM_1, &CDialogTenkey::OnBnClickedButtonNum1)
	ON_BN_CLICKED(IDC_BUTTON_NUM_2, &CDialogTenkey::OnBnClickedButtonNum2)
	ON_BN_CLICKED(IDC_BUTTON_NUM_3, &CDialogTenkey::OnBnClickedButtonNum3)
	ON_BN_CLICKED(IDC_BUTTON_NUM_6, &CDialogTenkey::OnBnClickedButtonNum6)
	ON_BN_CLICKED(IDC_BUTTON_NUM_5, &CDialogTenkey::OnBnClickedButtonNum5)
	ON_BN_CLICKED(IDC_BUTTON_NUM_4, &CDialogTenkey::OnBnClickedButtonNum4)
	ON_BN_CLICKED(IDC_BUTTON_NUM_7, &CDialogTenkey::OnBnClickedButtonNum7)
	ON_BN_CLICKED(IDC_BUTTON_NUM_8, &CDialogTenkey::OnBnClickedButtonNum8)
	ON_BN_CLICKED(IDC_BUTTON_NUM_9, &CDialogTenkey::OnBnClickedButtonNum9)
	ON_BN_CLICKED(IDC_BUTTON_BACKSPACE, &CDialogTenkey::OnBnClickedButtonBackspace)
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CDialogTenkey::OnBnClickedBtnClear)
	ON_BN_CLICKED(IDOK, &CDialogTenkey::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDialogTenkey::OnBnClickedCancel)
END_MESSAGE_MAP()


void CDialogTenkey::OnBnClickedButtonNum0()
{
	AddEditData(LOBYTE(::VkKeyScan(_T('0'))));
}


void CDialogTenkey::OnBnClickedButtonDot()
{
	AddEditData(LOBYTE(::VkKeyScan(_T('.'))));
}


void CDialogTenkey::OnBnClickedButtonDash()
{
	AddEditData(LOBYTE(::VkKeyScan(_T('-'))));
}


void CDialogTenkey::OnBnClickedButtonNum1()
{
	AddEditData(LOBYTE(::VkKeyScan(_T('1'))));
}


void CDialogTenkey::OnBnClickedButtonNum2()
{
	AddEditData(LOBYTE(::VkKeyScan(_T('2'))));
}


void CDialogTenkey::OnBnClickedButtonNum3()
{
	AddEditData(LOBYTE(::VkKeyScan(_T('3'))));
}


void CDialogTenkey::OnBnClickedButtonNum6()
{
	AddEditData(LOBYTE(::VkKeyScan(_T('6'))));
}


void CDialogTenkey::OnBnClickedButtonNum5()
{
	AddEditData(LOBYTE(::VkKeyScan(_T('5'))));
}


void CDialogTenkey::OnBnClickedButtonNum4()
{
	AddEditData(LOBYTE(::VkKeyScan(_T('4'))));
}


void CDialogTenkey::OnBnClickedButtonNum7()
{
	AddEditData(LOBYTE(::VkKeyScan(_T('7'))));
}


void CDialogTenkey::OnBnClickedButtonNum8()
{
	AddEditData(LOBYTE(::VkKeyScan(_T('8'))));
}


void CDialogTenkey::OnBnClickedButtonNum9()
{
	AddEditData(LOBYTE(::VkKeyScan(_T('9'))));
}


void CDialogTenkey::OnBnClickedButtonBackspace()
{
	AddEditData(VK_BACK);
}


void CDialogTenkey::OnBnClickedBtnClear()
{
	SetDlgItemText(IDC_EDIT_VALUE, _T(""));
	GetDlgItem(IDC_EDIT_VALUE)->SetFocus();
}


void CDialogTenkey::OnBnClickedOk()
{
	GetDlgItemText(IDC_EDIT_VALUE, m_strEditValue);
	std::regex pattern(R"(^([+-]?(?:[[:d:]]+\.?|[[:d:]]*\.[[:d:]]+))(?:[Ee][+-]?[[:d:]]+)?$)");
	std::string strNewRcpName = (CT2CA)m_strEditValue;
	if (false == std::regex_match(strNewRcpName, pattern))
	{
		::AfxMessageBox(_T("Actual Number Only."));
		return;
	}
	m_dEditValue = _ttof(m_strEditValue);
	if (m_dMin != m_dMax && (m_dEditValue < m_dMin || m_dEditValue > m_dMax))		// Min & Max가 같을 경우 확인 하지 말자.
	{
		m_dEditValue = m_dSetValue;													// 입력한 값이 범위를 넘어간 경우 Setting 값으로 초기화 해주자.
		m_strEditValue.Format(_T("%.3f"), m_dEditValue);

		//CDlgMsgView DlgMsgView;
		//DlgMsgView.m_bCheckMsg = true;
		//DlgMsgView.m_strMsg = _T("The Value check input range.");
		//DlgMsgView.DoModal();
		CDialogEx::OnCancel();
		return;
	}

	CDialogEx::OnOK();
}


void CDialogTenkey::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}

void CDialogTenkey::InitBtnControl()
{	
	// Set Button Font
	m_fntBtn.CreatePointFont(180, _T("Arial Bold"));

	GetDlgItem(IDC_BUTTON_NUM_1)->SetFont(&m_fntBtn);
	GetDlgItem(IDC_BUTTON_NUM_2)->SetFont(&m_fntBtn);
	GetDlgItem(IDC_BUTTON_NUM_3)->SetFont(&m_fntBtn);
	GetDlgItem(IDC_BUTTON_NUM_4)->SetFont(&m_fntBtn);
	GetDlgItem(IDC_BUTTON_NUM_5)->SetFont(&m_fntBtn);
	GetDlgItem(IDC_BUTTON_NUM_6)->SetFont(&m_fntBtn);
	GetDlgItem(IDC_BUTTON_NUM_7)->SetFont(&m_fntBtn);
	GetDlgItem(IDC_BUTTON_NUM_8)->SetFont(&m_fntBtn);
	GetDlgItem(IDC_BUTTON_NUM_9)->SetFont(&m_fntBtn);
	GetDlgItem(IDC_BUTTON_NUM_0)->SetFont(&m_fntBtn);
	GetDlgItem(IDC_BUTTON_DASH)->SetFont(&m_fntBtn);
	GetDlgItem(IDC_BUTTON_DOT)->SetFont(&m_fntBtn);
	GetDlgItem(IDC_BUTTON_BACKSPACE)->SetFont(&m_fntBtn);
	GetDlgItem(IDC_BTN_CLEAR)->SetFont(&m_fntBtn);
	m_btnOk.SetFont(&m_fntBtn);
	m_btnOk.SetFlat(FALSE);
	m_btnOk.SetImageOrg(12, 3);
	m_btnOk.SetIcon(IDI_OK);
	m_btnOk.SetColor(UEasyButtonEx::COLOR_FG_IN, RGB(0, 128, 0));
	m_btnOk.EnableBallonToolTip();
	m_btnOk.SetToolTipText(_T("Enter value"));
	m_btnOk.SetBtnCursor(IDC_MY_HAND);
	m_btnCancel.SetFont(&m_fntBtn);
	m_btnCancel.SetFlat(FALSE);
	m_btnCancel.SetImageOrg(12, 3);
	m_btnCancel.SetIcon(IDI_CANCEL);
	m_btnCancel.SetColor(UEasyButtonEx::COLOR_FG_IN, RGB(0, 128, 0));
	m_btnCancel.EnableBallonToolTip();
	m_btnCancel.SetToolTipText(_T("Cancel"));
	m_btnCancel.SetBtnCursor(IDC_MY_HAND);
}

void CDialogTenkey::InitStaticControl()
{	
	// Set Static Font
	m_fntStatic.CreatePointFont(125, _T("Arial Bold"));

	m_staticName.SetFont(&m_fntStatic);
	m_staticName.SetBackColor(RGB(220, 220, 220));
	m_staticName.SetForeColor(RGB(240, 60, 100));

	m_staticMin.SetForeColor(RGB(240, 60, 100));
	m_staticMin.SetBackColor(RGB(240, 240, 240));

	m_staticMax.SetForeColor(RGB(240, 60, 100));
	m_staticMax.SetBackColor(RGB(240, 240, 240));
}

void CDialogTenkey::InitEditControl()
{	
	// Set Edit Font
	m_fntEdit.CreatePointFont(125, _T("Arial Bold"));

	GetDlgItem(IDC_EDIT_VALUE)->SetFont(&m_fntStatic);

	m_edtValue.SetReceivedFlag(4); // Alpha-nemeric
	m_edtValue.SetForeColor(RGB(0, 0, 255)); 
}

void CDialogTenkey::InitSettingDlg()
{	
	// Base setting of UI
	m_staticName.SetWindowText(m_strName);
	CString strData;
	if (m_kptInt == dtINTEGER)
		strData.Format(_T("%d"), (int)m_dSetValue);
	else if (m_kptInt == dtDOUBLE)
		strData.Format(_T("%.3f"), m_dSetValue);
	if (strData == "0" || strData == "0.000")
		strData = "";
	SetDlgItemText(IDC_EDIT_VALUE, (LPCTSTR)strData);
	strData.Format(_T("Min : %.3f"), m_dMin);
	m_staticMin.SetWindowText(strData);
	strData.Format(_T("Max : %.3f"), m_dMax);
	m_staticMax.SetWindowText(strData);

	BOOL bTRUE = TRUE;
	if (m_kptInt == dtINTEGER)
		bTRUE = FALSE;
	GetDlgItem(IDC_BUTTON_DOT)->EnableWindow(bTRUE);
	SetWindowPos(NULL, m_nDlgX, m_nDlgY, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE);
}

void CDialogTenkey::AddEditData(BYTE bKeyCode)
{
	GetDlgItem(IDC_EDIT_VALUE)->SetFocus();
	::keybd_event(bKeyCode, 0, 0, 0);
	::keybd_event(bKeyCode, 0, KEYEVENTF_KEYUP, 0);
}

BOOL CDialogTenkey::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetBackgroundColor(RGB(255, 255, 255));

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	InitBtnControl();
	InitStaticControl();
	InitEditControl();

	InitSettingDlg();

	CEdit* pEdit;

	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_VALUE);
	//pEdit->SetSel(0, -1);
	pEdit->SetFocus();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CDialogTenkey::DestroyWindow()
{
	m_fntBtn.DeleteObject();
	m_fntStatic.DeleteObject();
	m_fntEdit.DeleteObject();

	return CDialogEx::DestroyWindow();
}
