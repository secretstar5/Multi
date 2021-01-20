#pragma once
#include "resource.h"
#include "Label.h"
enum { mvYESNO, mvOKOnly };
class CDlgBlockMsg : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgBlockMsg)
public:
	CDlgBlockMsg(CWnd* pParent = NULL);
	virtual ~CDlgBlockMsg();
	enum { IDD = IDD_DLG_MSG_VIEW };
	CString m_strMsg;
	int		m_nFontSize;
	CLabel	m_ctrlMsgTxt;
	bool	m_bCheckMsg;
	CFont	m_fntStatic;
	BOOL	ShowMsg(bool bCheckMsg, CString strMsg);
protected:
	DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
};	


class CDlgFloatMsg : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgFloatMsg)
public:
	CDlgFloatMsg(CWnd* pParent = NULL);
	virtual ~CDlgFloatMsg();
	enum { IDD = IDD_DLG_MSG_VIEW };
	int		m_nFontSize;
	CLabel	m_ctrlMsgTxt;
	CFont	m_fntStatic;
	void updateMsg(CString szMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	afx_msg void OnBnClickedOk();
};


class CDlgTimerMsg : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTimerMsg)
public:
	CDlgTimerMsg(int nTimerCnt = 10, CWnd* pParent = NULL);
	virtual ~CDlgTimerMsg();
	enum { IDD = IDD_DLG_MSG_VIEW };
	CString m_strMsg;
	BOOL	ShowMsg(CString strMsg);
private:
	int     m_nTimerTime = 0;
	int		m_nCurCnt = 0;
	int		m_nFontSize;
	CLabel	m_ctrlMsgTxt;
	CFont	m_fntStatic;
	DECLARE_MESSAGE_MAP()
	virtual BOOL DestroyWindow();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};