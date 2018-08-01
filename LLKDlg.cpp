
// LLKDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "LLK.h"
#include "LLKDlg.h"
#include "GameDlg.h"
#include "afxdialogex.h"
#include "CEasyGame.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CLLKDlg 对话框



CLLKDlg::CLLKDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_LLK_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLLKDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

}

BEGIN_MESSAGE_MAP(CLLKDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_MODEL1, &CLLKDlg::OnClickedButtonModel1)
	ON_BN_CLICKED(IDC_BUTTON_MODEL2, &CLLKDlg::OnBnClickedButtonModel2)
	ON_BN_CLICKED(IDC_BUTTON_RANK, &CLLKDlg::OnClickedButtonRank)
	ON_BN_CLICKED(IDC_BUTTON_HELP, &CLLKDlg::OnClickedButtonHelp)
END_MESSAGE_MAP()


// CLLKDlg 消息处理程序

BOOL CLLKDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	//设置按钮
	GetDlgItem(IDC_BUTTON_MODEL3)->EnableWindow(FALSE);

	//初始化背景
	UpdateWindow();
	InitBackground();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CLLKDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CLLKDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//创建CPaintDC对象
		CPaintDC dc(this);
		dc.BitBlt(0, 0, GAME_DLG_WIDTH, GAME_DLG_HIGH, &m_dcMem, 0, 0, SRCCOPY);
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CLLKDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//初始化背景
void CLLKDlg::InitBackground()
{
	//加载位图
	CBitmap bmpMain;
	bmpMain.LoadBitmapW(IDB_BITMAP_BG);

	//创建兼容DC
	CClientDC dc(this);
	m_dcMem.CreateCompatibleDC(&dc);

	//将为图选入DC
	m_dcMem.SelectObject(&bmpMain);
}

void CLLKDlg::UpdateWindow()
{
	CRect rcClient(0, 0, GAME_DLG_WIDTH, GAME_DLG_HIGH);
	AdjustWindowRectEx(rcClient, GetStyle(), FALSE, GetExStyle());
	SetWindowPos(NULL, 0, 0, rcClient.Width(), rcClient.Height(), SWP_NOMOVE | SWP_NOZORDER);
	CenterWindow();
}

void CLLKDlg::OnClickedButtonModel1()
{
	//基本模式
	this->ShowWindow(SW_HIDE);
	CGameDlg dig;
	//设置游戏模式
	CBasicGame basic;
	dig.SetGameModel(&basic);
	//组装flag
	FLAG flag;
	flag.bProp = false;
	flag.bScore = false;
	flag.bTimer = true;
	flag.szTitle = _T("Basic Model");
	dig.SetGameFlag(flag);
	dig.DoModal();
	this->ShowWindow(SW_SHOW);

}

void CLLKDlg::OnBnClickedButtonModel2()
{
	//休闲模式
	this->ShowWindow(SW_HIDE);
	CGameDlg dig;
	//设置游戏模式
	CEasyGame easy;
	dig.SetGameModel(&easy);
	//组装flag
	FLAG flag;
	flag.bProp = true;
	flag.bScore = true;
	flag.bTimer = false;
	flag.szTitle = _T("Relax Model");
	dig.SetGameFlag(flag);
	dig.DoModal();
	this->ShowWindow(SW_SHOW);

}


void CLLKDlg::OnClickedButtonRank()
{
	//MessageBox(_T("YOU ARE ALWAYS THE FIRST IN MY HEART"), MB_OK);
	this->ShowWindow(SW_HIDE);
	CRankDlg dlg;
	dlg.DoModal();
	this->ShowWindow(SW_SHOW);
}


void CLLKDlg::OnClickedButtonHelp()
{
	MessageBox(_T("1.CLICK BASIC MODEL TO START BASIC GAME\n2.CLICK RELAX MODEL TO START RELAX GAME\n3.CLICK LEVEL MODEL TO START LEVEL GAME\n"), _T("HELP"), MB_OK);
}
