#pragma once
#include <afxwin.h>
#include <array>
#include <vector>
#define PARTNUM 10
#define STATUS_CHANGE	WM_USER + 102

class CBattPartView :public CWnd
{
public:
	CBattPartView(int nId);
	~CBattPartView();
	void initViewer(CWnd * pWnd);
public:
	void setPartEnable(bool bEnable[PARTNUM]);
	void getPartEnable(bool bEnable[PARTNUM]);
	bool getPartEnable(int nIdx);
	void setPartEnable(int nIdx, bool bEnable);

private:
	bool m_bPartEnale[PARTNUM];
	bool m_nId;

public:
	void Invalidate();
	int getPartNum();
	bool setPartNum(int nParts);
	void setStartPos(double dStPos);
	virtual BOOL DestroyWindow();

private:
	void draw();
	void drawParts(Gdiplus::Graphics &G);
	int getIdxOnMouse(CPoint point);

private:
	HWND m_parent;
	CWnd* m_thisWnd;
	CRect m_rcView;
	Gdiplus::Image *m_Bkg;
	CGdiPlusStarter g_gps;
	int m_nCurIdx = -1;
	double m_dStPos = -1.0;
	std::vector<float> m_vWidths;
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

