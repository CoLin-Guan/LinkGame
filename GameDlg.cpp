// GameDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LLK.h"
#include "GameDlg.h"
#include "CGameLogic.h"
#include "afxdialogex.h"
#include "resource.h"


// CGameDlg 对话框

IMPLEMENT_DYNAMIC(CGameDlg, CDialog)

CGameDlg::CGameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_GAME_DIALOG, pParent)
{
	m_ptGameTop.col = GRAPH_TOP;
	m_ptGameTop.row = GRAPH_LEFT;
	m_sizeElem.cx = PIC_WIDTH;
	m_sizeElem.cy = PIC_HIGH;
	m_bFirstPoint = true;
	m_bPlaying = false;
	m_bPause = false;
	m_bProp = false;
}

CGameDlg::~CGameDlg()
{
}

void CGameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_TIME, m_GameProgress);
}

BEGIN_MESSAGE_MAP(CGameDlg, CDialog)
	ON_WM_PAINT()
	
	ON_BN_CLICKED(IDC_BUTTON_START, &CGameDlg::OnBnClickedButtonStart)
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_BUTTON_NOTE, &CGameDlg::OnClickedButtonNote)
	ON_BN_CLICKED(IDC_BUTTON_REMIX, &CGameDlg::OnClickedButtonRemix)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_PAUSE, &CGameDlg::OnClickedButtonPause)
	ON_BN_CLICKED(IDC_BUTTON_HELP, &CGameDlg::OnClickedButtonHelp)
	ON_BN_CLICKED(IDC_BUTTON_PROP, &CGameDlg::OnClickedButtonProp)
END_MESSAGE_MAP()


// CGameDlg 消息处理程序

BOOL CGameDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//设置图标
	m_hIcon = AfxGetApp()->LoadIconW(IDR_MODEL1);
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	//获取控件显示FLAG并设置空间显示状态
	FLAG flag;
	m_GameControl->GetGameFlag(flag);
	if (!flag.bProp) {
		GetDlgItem(IDC_BUTTON_PROP)->ShowWindow(FALSE);
	}
	if (!flag.bTimer) {
		GetDlgItem(IDC_PROGRESS_TIME)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_TIME)->ShowWindow(FALSE);
	}
	if (!flag.bScore) {
		GetDlgItem(IDC_STATIC_SCORE)->ShowWindow(FALSE);
	}
	
	//设置窗口标题
	SetWindowText(flag.szTitle);
	
	//设置背景
	UpdateWindow();
	InitBackground();

	//初始化连连看元素
	InitElement();

	//设置按钮状态
	GetDlgItem(IDC_BUTTON_NOTE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_REMIX)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_PAUSE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_PROP)->EnableWindow(FALSE);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CGameDlg::InitBackground()
{
	HANDLE hBmpBG = ::LoadImage(NULL, _T("theme\\picture\\game_bg_big.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	CClientDC dc(this);
	m_dcBG.CreateCompatibleDC(&dc);
	m_dcBG.SelectObject(hBmpBG);

	m_dcMem.CreateCompatibleDC(&dc);
	CBitmap bmpMem;
	bmpMem.CreateCompatibleBitmap(&dc, GAME_DLG_WIDTH, GAME_DLG_HIGH);
	m_dcMem.SelectObject(&bmpMem);

	//绘制背景到内存DC
	m_dcMem.BitBlt(0, 0, GAME_DLG_WIDTH, GAME_DLG_HIGH, &m_dcBG, 0, 0, SRCCOPY);
}

void CGameDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CDialog::OnPaint()

	dc.BitBlt(0, 0, GAME_DLG_WIDTH, GAME_DLG_HIGH, &m_dcMem, 0, 0, SRCCOPY);
}

void CGameDlg::InitElement()
{
	//获得当前对话框的视频内存
	CClientDC dc(this);

	//加载连连看元素图片
	HANDLE hBmp = ::LoadImageW(NULL, _T("theme\\picture\\elements_big.bmp"),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	//创建与视频内存兼容的内存DC
	m_dcElement.CreateCompatibleDC(&dc);
	//将位图资源选入DC
	m_dcElement.SelectObject(hBmp);

	//加载mask图片
	HANDLE hMask = ::LoadImageW(NULL, _T("theme\\picture\\mask_big.bmp"),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	//创建与视频内存兼容的内存DC
	m_dcMask.CreateCompatibleDC(&dc);
	//将位图资源选入DC
	m_dcMask.SelectObject(hMask);

	//加载暂停图片
	HANDLE hPause = ::LoadImageW(NULL, _T("theme\\picture\\pause.bmp"),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	//创建与视频内存兼容的内存DC
	m_dcPause.CreateCompatibleDC(&dc);
	//将位图资源选入DC
	m_dcPause.SelectObject(hPause);

	
	/*
	CBitmap bmpCache;
	bmpCache.CreateCompatibleBitmap(&dc, 800, 600);
	m_dcCache.SelectObject(&bmpCache);
	*/
}

void CGameDlg::OnBnClickedButtonStart()
{
	//地图元素
	m_GameControl->StartGame();

	//绘制元素
	UpdateMap();
	Invalidate();
	//设置游戏中状态
	m_bPlaying = true;

	//基本模式下操作
	FLAG flag;
	m_GameControl->GetGameFlag(flag);
	if (flag.bTimer) {
		//启动定时器
		SetTimer(PLAY_TIMER_ID, 1000, NULL);
		//初始化进度条
		m_GameProgress.SetRange(0, 300);
		m_GameProgress.SetStep(-1);
		m_GameProgress.SetPos(300);
		//按钮控制
		GetDlgItem(IDC_BUTTON_NOTE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_REMIX)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_PAUSE)->EnableWindow(TRUE);
	}

	//开始游戏按钮关闭
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);

}

void CGameDlg::UpdateMap()
{
	//绘制地图
	int nTop = m_ptGameTop.row;
	int nLeft = m_ptGameTop.col;
	int nElemW = m_sizeElem.cx;
	int nElemH = m_sizeElem.cy;
	m_dcMem.BitBlt(m_rtGameRect.left, m_rtGameRect.top,
		m_rtGameRect.Width(), m_rtGameRect.Height(), &m_dcBG, m_rtGameRect.left, m_rtGameRect.top, SRCCOPY);

	for (int i = 0; i < ROW_SIZE; i++) {
		for (int j = 0; j < COL_SIZE; j++) {
			//绘制一个坐标上的图片
			int nInfo = m_GameControl->GetElement(i, j);

			m_dcMem.BitBlt(nLeft + j * nElemW, nTop + i * nElemH,
				nElemW, nElemH, &m_dcMask, 0, nInfo * nElemH, SRCPAINT);
			m_dcMem.BitBlt(nLeft + j * nElemW, nTop + i * nElemH,
				nElemW, nElemH, &m_dcElement, 0, nInfo * nElemH, SRCAND);
		}
	}
}

void CGameDlg::UpdateWindow()
{
	m_rtGameRect = new CRect(0, 0, GAME_DLG_WIDTH, GAME_DLG_HIGH);
	AdjustWindowRectEx(m_rtGameRect, GetStyle(), FALSE, GetExStyle());
	SetWindowPos(NULL, 0, 0, m_rtGameRect.Width(), m_rtGameRect.Height(), SWP_NOMOVE | SWP_NOZORDER);
	CenterWindow();
}

void CGameDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	//游戏状态判断
	if (!m_bPlaying||m_bPause)
		return;

	int nRow = (point.y - m_ptGameTop.row) / m_sizeElem.cy;
	int nCol = (point.x - m_ptGameTop.col) / m_sizeElem.cx;
	//判断是否在游戏区
	if (point.y < m_ptGameTop.row || point.x < m_ptGameTop.col)
		return CDialog::OnLButtonUp(nFlags, point);
	if (nRow >= ROW_SIZE || nCol >= COL_SIZE ) 
		return CDialog::OnLButtonUp(nFlags, point);

	//合法位置
	if (m_bFirstPoint) {
		DrawTipFrame(nRow, nCol);
		m_GameControl->SetFirstPoint(nRow, nCol);
	}
	else {
		DrawTipFrame(nRow, nCol);
		m_GameControl->SetSecPoint(nRow, nCol);

		if (m_bProp) {
			Vertex avPath[MAX_PATH_VEX];
			int nVexNum = 0;
			//使用道具
			if (m_GameControl->PorpLink(avPath, nVexNum)) {
				//绘制提示线
				DrawTipLine(avPath, nVexNum);
				//休闲模式下计分绘制
				FLAG flag;
				m_GameControl->GetGameFlag(flag);
				if (flag.bScore) {
					wchar_t grade[10];
					wint_t nPercent = m_GameControl->GetGrade();
					wsprintf(grade, L"%d", nPercent);
					GetDlgItem(IDC_STATIC_SCORE)->SetWindowText(grade);
					//判断按钮状态
					CalculateGameGrade();
				}
			}
			//更新地图
			UpdateMap();
			Sleep(200);
			InvalidateRect(&m_rtGameRect, FALSE);
			//道具使用关闭
			m_bProp = !m_bProp;
		}
		else {
			//未使用道具
			//获得路径
			Vertex avPath[MAX_PATH_VEX];
			int nVexNum = 0;
			//连接判断
			if (m_GameControl->Link(avPath, nVexNum)) {
				//绘制提示线
				DrawTipLine(avPath, nVexNum);
				//休闲模式下计分绘制
				FLAG flag;
				m_GameControl->GetGameFlag(flag);
				if (flag.bScore) {
					wchar_t grade[10];
					wint_t nPercent = m_GameControl->GetGrade();
					wsprintf(grade, L"%d", nPercent);
					GetDlgItem(IDC_STATIC_SCORE)->SetWindowText(grade);
					//判断按钮状态
					CalculateGameGrade();
				}
				//更新地图
				UpdateMap();
			}
			Sleep(200);
			InvalidateRect(&m_rtGameRect, FALSE);
		}
	}
	//判断胜负
	JudgeWin();

	m_bFirstPoint = !m_bFirstPoint;
	CDialog::OnLButtonUp(nFlags, point);
}

void CGameDlg::DrawTipFrame(int nRow, int nCol)
{
	CClientDC dc(this);
	CBrush brush(RGB(0, 255, 255));
	CRect rtTipFrame;
	rtTipFrame.left = m_ptGameTop.col + nCol * m_sizeElem.cx;
	rtTipFrame.top = m_ptGameTop.row + nRow * m_sizeElem.cy;
	rtTipFrame.right = rtTipFrame.left + m_sizeElem.cx;
	rtTipFrame.bottom = rtTipFrame.top + m_sizeElem.cy;
	dc.FrameRect(rtTipFrame, &brush);
}

void CGameDlg::DrawTipLine(Vertex asvPath[MAX_PATH_VEX], int nVexnum)
{
	//获取DC
	CClientDC dc(this);
	//设置画笔
	CPen penLine(PS_SOLID, 2, RGB(0, 255, 0));
	//将画笔选入DC
	CPen* pOldPen = dc.SelectObject(&penLine);
	
	//绘制连线
	for (int i = 0; i < nVexnum - 1; i++) {
		//移动到起点
		dc.MoveTo(m_ptGameTop.col + asvPath[i].col*m_sizeElem.cx + m_sizeElem.cx / 2,
			m_ptGameTop.row + asvPath[i].row *m_sizeElem.cy + m_sizeElem.cy / 2);
		//从起点到下一点...直到终点
		dc.LineTo(m_ptGameTop.col + asvPath[i+1].col*m_sizeElem.cx + m_sizeElem.cx / 2,
			m_ptGameTop.row + asvPath[i+1].row *m_sizeElem.cy + m_sizeElem.cy / 2);
	}

	dc.SelectObject(pOldPen);
}

void CGameDlg::OnClickedButtonNote()
{
	//可连接判断
	Vertex v1, v2;
	if (m_GameControl->Help(v1, v2)) {
		//找到可连接两顶点
		DrawTipFrame(v1.row, v1.col);
		DrawTipFrame(v2.row, v2.col);
		Sleep(400);
		InvalidateRect(&m_rtGameRect, FALSE);
	}
	//休闲模式下修改计分并判断按钮状态
	FLAG flag;
	m_GameControl->GetGameFlag(flag);
	if (flag.bScore) {
		wchar_t grade[10];
		wint_t nPercent = m_GameControl->GetGrade();
		wsprintf(grade, L"%d", nPercent);
		GetDlgItem(IDC_STATIC_SCORE)->SetWindowText(grade);
		CalculateGameGrade();
	}
}

void CGameDlg::OnClickedButtonRemix()
{
	if (m_bPlaying == false) return;
	//地图重排
	m_GameControl->Reset();
	//更新地图
	UpdateMap();
	//重绘界面
	Invalidate();
	//休闲模式下修改计分并判断按钮状态
	FLAG flag;
	m_GameControl->GetGameFlag(flag);
	if (flag.bScore) {
		wchar_t grade[10];
		wint_t nPercent = m_GameControl->GetGrade();
		wsprintf(grade, L"%d", nPercent);
		GetDlgItem(IDC_STATIC_SCORE)->SetWindowText(grade);
		CalculateGameGrade();
	}
}

void CGameDlg::JudgeWin()
{
	//游戏胜负判断
	int state = m_GameControl->IsWin(m_GameProgress.GetPos());
	//判断是否继续游戏
	if (state == GAME_PLAY) {
		return;
	}
	else {
		//设置游戏进行中标志
		m_bPlaying = false;
		//关闭计时器
		KillTimer(PLAY_TIMER_ID);
		//提示获胜
		CString strTitle;
		this->GetWindowTextW(strTitle);
		if (state == GAME_SUCCESS) {
			MessageBox(_T("WIN!\n"), strTitle);
		}
		else {
			MessageBox(_T("TIME OUT!\n"), strTitle);
		}
		//还原开始游戏按钮
		this->GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		//关闭其他按钮
		GetDlgItem(IDC_BUTTON_NOTE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_REMIX)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_PAUSE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_PROP)->EnableWindow(FALSE);
		//休闲模式下修改计分并加入排行榜
		FLAG flag;
		m_GameControl->GetGameFlag(flag);
		if (flag.bScore) {
			wchar_t grade[10];
			wint_t nPercent = m_GameControl->GetGrade();
			wsprintf(grade, L"%d", nPercent);
			GetDlgItem(IDC_STATIC_SCORE)->SetWindowText(grade);
			CalculateGameGrade();
			//加入排行榜
			if (AfxMessageBox(_T("DO U WANT TO SAVE THE RECORD?"), MB_OKCANCEL) == IDOK)
			{
				AddToRank();
			}
		}
	}
}

void CGameDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == PLAY_TIMER_ID && m_bPlaying && m_bPause == false) {
		//游戏时间-1
		m_GameProgress.StepIt();
		//DrawGameTime();
		wchar_t time[10];
		wint_t nPercent = m_GameProgress.GetPos();
		wsprintf(time, L"%d", nPercent);
		GetDlgItem(IDC_STATIC_TIME)->SetWindowText(time);
	}
	//判断游戏胜负
	JudgeWin();
	CDialog::OnTimer(nIDEvent);
}

void CGameDlg::OnClickedButtonPause()
{
	//游戏没有在进行
	//直接返回
	if (!m_bPlaying)
		return;

	if (m_bPause) {
		//暂停中
		//还原游戏地图
		SetTimer(PLAY_TIMER_ID, 1000, NULL);
		UpdateMap();
		//m_dcMem.BitBlt()
		//修改按钮文本为“暂停游戏”
		SetDlgItemText(IDC_BUTTON_PAUSE, _T("PAUSE"));


		//打开其他按钮
		GetDlgItem(IDC_BUTTON_NOTE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_REMIX)->EnableWindow(TRUE);
	}
	else {
		//游戏中
		//暂停游戏
		KillTimer(PLAY_TIMER_ID);
		//遮盖地图
		m_dcMem.BitBlt(40, 20, GAME_DLG_WIDTH, GAME_DLG_HIGH, &m_dcPause, 0, 0, SRCCOPY);
		//修改按钮文本为“继续游戏”
		SetDlgItemText(IDC_BUTTON_PAUSE, _T("RESTART"));

		//关闭其他按钮
		GetDlgItem(IDC_BUTTON_NOTE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_REMIX)->EnableWindow(FALSE);
	}
	//刷新界面
	InvalidateRect(&m_rtGameRect, FALSE);
	m_bPause = !m_bPause;
}

void CGameDlg::OnClickedButtonHelp()
{
	FLAG flag;
	m_GameControl->GetGameFlag(flag);
	if (flag.bTimer) {
		MessageBox(_T("1.CLICK ON TWO SAME ICONS AND THE TWO CAN BE LINKED\n2.THEN BOTH OF THEM WILL BE REMOVE\n3.TRY TO REMOVE ALL OF THE ICONS\n4.NOTICE THE TIME\nHAVE FUN!\n"), _T("HELP"), MB_OK);
	}
	else {
		MessageBox(_T("1.CLICK ON TWO SAME ICONS AND THE TWO CAN BE LINKED\n2.THEN BOTH OF THEM WILL BE REMOVE\n3.EVERY TIME YOU REMOVE TWO ICONS YOU WILL GET 10 POINTS\n4.USE 20 POINTS TO GET NOTE\n5.USE 30 POINTS TO REMIX ICONS\n6.USE 50 POINTS TO GET PROP\n7.PROP CAN ASSURE THE TWO SMAE ICONS YOU CLICKED BE REMOVE\nHAVE FUN!\n"), _T("HELP"), MB_OK);

	}
}

void CGameDlg::SetGameModel(CGameControl *pGameControl)
{
	m_GameControl = pGameControl;
}

void CGameDlg::OnClickedButtonProp()
{
	m_bProp = true;
	//使用道具修改计分
	m_GameControl->UseProp();
	//休闲模式下修改计分并判断按钮状态
	FLAG flag;
	m_GameControl->GetGameFlag(flag);
	if (flag.bScore) {
		wchar_t grade[10];
		wint_t nPercent = m_GameControl->GetGrade();
		wsprintf(grade, L"%d", nPercent);
		GetDlgItem(IDC_STATIC_SCORE)->SetWindowText(grade);
		CalculateGameGrade();
	}
}

void CGameDlg::SetGameFlag(FLAG flag)
{
	m_GameControl->SetGameFlag(flag);
}

void CGameDlg::CalculateGameGrade()
{
	int grade = m_GameControl->GetGrade();
	if (grade >= 20) {
		//设置按钮状态
		GetDlgItem(IDC_BUTTON_NOTE)->EnableWindow(TRUE);
		if (grade >= 30) {
			GetDlgItem(IDC_BUTTON_REMIX)->EnableWindow(TRUE);
			if (grade >= 50) {
				GetDlgItem(IDC_BUTTON_PROP)->EnableWindow(TRUE);
			}
		}
	}
	else {
		GetDlgItem(IDC_BUTTON_NOTE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_REMIX)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_PROP)->EnableWindow(FALSE);
	}
}

void CGameDlg::AddToRank()
{
	int grade = m_GameControl->GetGrade();
	//获取系统时间 　　
	CString str; 
	CTime tm; 
	tm = CTime::GetCurrentTime();
	str = tm.Format("%Y-%m-%d %X\n");
	//写入文件
	CStdioFile file;
	if (file.Open(_T("record.txt"), CFile::typeText | CFile::modeCreate | CFile::modeReadWrite | CFile::modeNoTruncate)) {
		file.SeekToEnd();		//这里追加到文件的末尾
		file.WriteString(str);   
	}
	str.Format(_T("%d\n"), grade);
	file.WriteString(str);  
	file.Close();
}



/*

void CGameDlg::DrawGameTime()
{
//创建字体
CFont font;
font.CreatePointFont(200, _T("Courier New"));
//将字体选入DC
CFont *oldFont;
oldFont = m_dcMem.SelectObject(&font);
//设置字体颜色
m_dcMem.SetTextColor(RGB(0, 255, 255));
m_dcMem.SetBkColor(RGB(77, 129, 200));
//获取时间内容
int pos = m_GameProgress.GetPos();
CString strTime;
strTime.Format(_T("%d"), pos);
//获得文本输出区域
CRect rect;
m_GameProgress.GetWindowRect(rect);
ScreenToClient(rect);
//绘制时间
m_dcMem.TextOutW(rect.left + rect.Width() + 10, rect.top - 5, strTime);
//还原字体
m_dcMem.SelectObject(oldFont);
//更新界面
InvalidateRect(&m_rtGameRect, FALSE);
}


*/