#pragma once
#include "CBattPartView.h"

class CDlgInsp : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgInsp)
public:
	CDlgInsp(CWnd* pParent = nullptr);
	virtual ~CDlgInsp();
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_INSP };
#endif

private:
	CButton m_test111;
	//CBattPartView m_BattView;
	CBattPartView *m_BattPartView;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	virtual BOOL DestroyWindow();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
};
