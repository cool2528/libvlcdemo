
// mfcvlcDlg.h : 头文件
//

#pragma once
#include "LibvlcEx.h"
#include "afxcmn.h"

// CmfcvlcDlg 对话框
class CmfcvlcDlg : public CDialogEx
{
// 构造
public:
	CmfcvlcDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CmfcvlcDlg();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCVLC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	void checkout();
private:
	int m_cx;
	int m_cy;
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton1();
	CLibvlcEx* libvlctemp;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	float m_rate;
	afx_msg void OnBnClickedButton5();
	int m_valome;
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	CSliderCtrl m_slider;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButton8();
};
