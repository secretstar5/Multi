#pragma once
#include "DlgInsp.h"
#include "DlgLog.h"
#include "DlgRcp.h"
#include "DlgRect.h"
#include "CxStatic.h"

class CMultiDlg : public CDialogEx
{
public:
	CMultiDlg(CWnd* pParent = nullptr);
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MULTI_DIALOG };
#endif
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);

private:
	CDlgInsp m_DlgInsp;
	CDlgLog m_DlgLog;
	CDlgRcp m_DlgRcp; 
	CDlgRect m_DlgRect;


	CxStatic m_stSwName;
	CxStatic m_stVerInfo;
	CxStatic m_stCurRcpName;


protected:
	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedBtnExit();
	afx_msg void OnBnClickedBtnMinimize();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedBtnInsp();
	afx_msg void OnBnClickedBtnRcp();
	afx_msg void OnBnClickedBtnLog();
	virtual BOOL DestroyWindow();
	afx_msg void OnBnClickedBtnRect();
};
