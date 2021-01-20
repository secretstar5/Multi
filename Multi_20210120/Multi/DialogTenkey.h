#pragma once
#include "resource.h"
#include <afxdialogex.h>
#include "UEasyButtonEx.h"
#include "ColorEdit.h"
#include "ColorStatic.h"
enum TDataTypeParam
{
	dtNone,
	dtDOUBLE,
	dtINTEGER,
	dtSTRING,
	dtComment,
};
class CDialogTenkey :public CDialogEx
{
	DECLARE_DYNAMIC(CDialogTenkey)
public:
	CDialogTenkey(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogTenkey();
	enum { IDD = IDD_DLG_KEYPAD_NUMBER };
	virtual void DoDataExchange(CDataExchange* pDX);
	UEasyButtonEx m_btnOk;
	UEasyButtonEx m_btnCancel;
	CColorEdit m_edtValue;
	CColorStatic m_staticName;
	CColorStatic m_staticMin;
	CColorStatic m_staticMax;
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedButtonNum0();
	afx_msg void OnBnClickedButtonDash();
	afx_msg void OnBnClickedButtonNum1();
	afx_msg void OnBnClickedButtonNum2();
	afx_msg void OnBnClickedButtonNum3();
	afx_msg void OnBnClickedButtonNum6();
	afx_msg void OnBnClickedButtonNum5();
	afx_msg void OnBnClickedButtonNum4();
	afx_msg void OnBnClickedButtonNum7();
	afx_msg void OnBnClickedButtonNum8();
	afx_msg void OnBnClickedButtonNum9();
	afx_msg void OnBnClickedButtonDot();
	afx_msg void OnBnClickedButtonBackspace();
	afx_msg void OnBnClickedBtnClear();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

public:
	void		InitBtnControl();
	void		InitStaticControl();
	void		InitEditControl();
	void		InitSettingDlg();
	void		AddEditData(BYTE bKeyCode);
	double		m_dSetValue;				// set init value.
	double		m_dEditValue;				// ten key에서 입력한 값.
	CString		m_strEditValue;				// Edit Value (string)	
	CString		m_strName;
	TDataTypeParam		m_kptInt;			// data type
	double		m_dMin;						// Min & Max가 같을 경우 확인 하지 말자.
	double		m_dMax;						// Min & Max가 같을 경우 확인 하지 말자.
	int			m_nDlgX;					// dlg Show window position.
	int			m_nDlgY;
	double		GetEditValue() { return	m_dEditValue; }
	void		CommentSet(CString strName, TDataTypeParam nInt, double dSetValue, double dMin = -3000., double dMax = 3000., int nDlgX = 300, int nDlgY = 300)
	{
		m_strName = strName;	m_dSetValue = dSetValue;	m_kptInt = nInt;   m_dMin = dMin;	m_dMax = dMax;	m_nDlgX = nDlgX;	m_nDlgY = nDlgY;
	}

protected:
	CFont		m_fntBtn;
	CFont		m_fntStatic;
	CFont		m_fntEdit;

public:
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
};

