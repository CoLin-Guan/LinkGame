#pragma once
#include "global.h"
#include "CGameControl.h"
#include "CGraph.h"
#include "afxcmn.h"
#include "CBasicGame.h"
#include "resource.h"

// CGameDlg 对话框

class CGameDlg : public CDialog
{
	DECLARE_DYNAMIC(CGameDlg)

public:
	CGameDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGameDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GAME_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	HICON m_hIcon;		//系统图标
	CDC m_dcMem;		//内存DC
	CDC m_dcElement;	//元素内存DC
	CDC m_dcMask;		//掩码内存DC
	CDC m_dcBG;			//背景内存DC
	CDC m_dcCache;		//缓存内存DC
	CDC m_dcPause;		//暂停图片DC

	bool m_bFirstPoint;	//判断是否被选中的第一个图标
	Vertex m_ptGameTop;	//游戏区域起点
	CSize m_sizeElem;	//元素图片的大小
	CRect m_rtGameRect;	//游戏区域的大小

	CGameControl * m_GameControl;	//游戏控制

	//游戏过程控制
	bool m_bPlaying;		//游戏中
	bool m_bPause;			//游戏暂停 false:游戏中 ture:游戏暂停
	bool m_bProp;			//使用道具状态

	//functions
	void InitBackground();
	void InitElement();
	void DrawTipFrame(int nRow, int nCol);							//根据所选行号和列号绘制提示框
	void DrawTipLine(Vertex asvPath[MAX_PATH_VEX], int nVexnum);	//绘制连线
	void JudgeWin();												//判断游戏胜负
	void AddToRank();												//加入排行榜记录

	CProgressCtrl m_GameProgress;	//游戏进度条

	DECLARE_MESSAGE_MAP()
public:
	void UpdateWindow();
	void UpdateMap();
	void SetGameModel(CGameControl *pGameControl);	//设置游戏控制类的值
	void SetGameFlag(FLAG flag);		//设置m_flag
	void CalculateGameGrade();			//获取游戏计分设置按钮状态

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnClickedButtonNote();
	afx_msg void OnClickedButtonRemix();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClickedButtonPause();
	afx_msg void OnClickedButtonHelp();
	afx_msg void OnClickedButtonProp();
};
