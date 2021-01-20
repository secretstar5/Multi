#pragma once
class CDlgRcp : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgRcp)
public:
	CDlgRcp(CWnd* pParent = nullptr);  
	virtual ~CDlgRcp();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_RCP };
#endif
public:
	void message() { ::AfxMessageBox(L"Im Korean."); }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   

	DECLARE_MESSAGE_MAP()
};
