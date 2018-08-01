
// LLKDlg.h: 头文件
//

#pragma once
#include "afxwin.h"
#include "CNewButton.h"
#include "RankDlg.h"


// CLLKDlg 对话框
class CLLKDlg : public CDialog
{
// 构造
public:
	CLLKDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LLK_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	CDC m_dcMem;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	//初始化背景
	void InitBackground();

public:
	afx_msg void OnClickedButtonModel1();
	void UpdateWindow();

	afx_msg void OnBnClickedButtonModel2();
	afx_msg void OnClickedButtonRank();
	afx_msg void OnClickedButtonHelp();
};
