#include "stdafx.h"
#include "CNewButton.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
CNewButton::CNewButton() //构造函数
{
	m_BoundryPen.CreatePen(PS_INSIDEFRAME | PS_SOLID, 1, RGB(0, 0, 0));//按钮的外边框画笔
	m_InsideBoundryPenLeft.CreatePen(PS_INSIDEFRAME | PS_SOLID, 3, RGB(61, 97, 88));     m_InsideBoundryPenRight.CreatePen(PS_INSIDEFRAME | PS_SOLID, 3, RGB(61, 137, 106));
	m_InsideBoundryPenTop.CreatePen(PS_INSIDEFRAME | PS_SOLID, 2, RGB(102, 167, 114));     m_InsideBoundryPenBottom.CreatePen(PS_INSIDEFRAME | PS_SOLID, 2, RGB(102, 167, 114));
	//按钮的背景色
	m_FillActive.CreateSolidBrush(RGB(1, 76, 177));
	m_FillInactive.CreateSolidBrush(RGB(1, 76, 177));
	m_InsideBoundryPenLeftSel.CreatePen(PS_INSIDEFRAME | PS_SOLID, 3, RGB(1, 76, 177));
	m_InsideBoundryPenTopSel.CreatePen(PS_INSIDEFRAME | PS_SOLID, 2, RGB(1, 76, 177));
	m_InsideBoundryPenRightSel.CreatePen(PS_INSIDEFRAME | PS_SOLID, 3, RGB(1, 76, 177));
	m_InsideBoundryPenBottomSel.CreatePen(PS_INSIDEFRAME | PS_SOLID, 2, RGB(1, 76, 177));
	m_bOver = m_bSelected = m_bTracking = m_bFocus = FALSE;
}
CNewButton::~CNewButton()
{
	m_BoundryPen.DeleteObject();
	m_InsideBoundryPenLeft.DeleteObject();
	m_InsideBoundryPenRight.DeleteObject();
	m_InsideBoundryPenTop.DeleteObject();
	m_InsideBoundryPenBottom.DeleteObject();
	m_FillActive.DeleteObject();
	m_FillInactive.DeleteObject();
	m_InsideBoundryPenLeftSel.DeleteObject();
	m_InsideBoundryPenTopSel.DeleteObject();
	m_InsideBoundryPenRightSel.DeleteObject();
	m_InsideBoundryPenBottomSel.DeleteObject();
}
BEGIN_MESSAGE_MAP(CNewButton, CButton)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
END_MESSAGE_MAP()

//添加Owner Draw属性
void CNewButton::PreSubclassWindow()
{
	CButton::PreSubclassWindow();
	ModifyStyle(0, BS_OWNERDRAW);
}
void CNewButton::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE | TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
	}
	CButton::OnMouseMove(nFlags, point);
}
LRESULT CNewButton::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_bOver = FALSE;
	m_bTracking = FALSE;
	InvalidateRect(NULL, FALSE);
	return 0;
}
LRESULT CNewButton::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	m_bOver = TRUE;
	InvalidateRect(NULL);
	return 0;
}
void CNewButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	//从lpDrawItemStruct获取控件的相关信息
	CRect rect = lpDrawItemStruct->rcItem;
	CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	int nSaveDC = pDC->SaveDC();
	UINT state = lpDrawItemStruct->itemState;
	POINT pt;
	TCHAR strText[MAX_PATH + 1];
	::GetWindowText(m_hWnd, strText, MAX_PATH);

	//画按钮的外边框，它是一个半径为5的圆角矩形
	pt.x = 5;
	pt.y = 5;
	CPen* hOldPen = pDC->SelectObject(&m_BoundryPen);
	pDC->RoundRect(&rect, pt);
	//获取按钮的状态
	if (state & ODS_FOCUS)
	{
		m_bFocus = TRUE;       m_bSelected = TRUE;
	}
	else
	{
		m_bFocus = FALSE;      m_bSelected = FALSE;
	}
	if (state & ODS_SELECTED || state & ODS_DEFAULT)
	{
		m_bFocus = TRUE;
	}
	pDC->SelectObject(hOldPen);
	rect.DeflateRect(CSize(GetSystemMetrics(SM_CXEDGE), GetSystemMetrics(SM_CYEDGE)));
	//根据按钮的状态填充按钮的底色
	CBrush* pOldBrush;
	if (m_bOver)
	{
		pOldBrush = pDC->SelectObject(&m_FillActive);
		DoGradientFill(pDC, &rect);
	}
	else
	{
		pOldBrush = pDC->SelectObject(&m_FillInactive);
		DoGradientFill(pDC, &rect);
	}
	//根据按钮的状态绘制内边框
	if (m_bOver || m_bSelected)
		DrawInsideBorder(pDC, &rect);
	pDC->SelectObject(pOldBrush);
	//显示按钮的文本
	if (strText != NULL)
	{
		CFont* hFont = GetFont();
		CFont* hOldFont = pDC->SelectObject(hFont);
		CSize szExtent = pDC->GetTextExtent(strText, lstrlen(strText));
		CPoint pt(rect.CenterPoint().x - szExtent.cx / 2, rect.CenterPoint().y - szExtent.cy / 2);
		if (state & ODS_SELECTED)
			pt.Offset(1, 1);
		int nMode = pDC->SetBkMode(TRANSPARENT);
		if (state & ODS_DISABLED)
			pDC->DrawState(pt, szExtent, strText, DSS_DISABLED, TRUE, 0, (HBRUSH)NULL);
		else
			pDC->DrawState(pt, szExtent, strText, DSS_NORMAL, TRUE, 0, (HBRUSH)NULL);
		pDC->SelectObject(hOldFont);
		pDC->SetBkMode(nMode);
	}
	pDC->RestoreDC(nSaveDC);
}
//绘制按钮的底色
void CNewButton::DoGradientFill(CDC *pDC, CRect* rect)
{
	CBrush brBk[64];
	int nWidth = rect->Width();
	int nHeight = rect->Height();
	CRect rct;
	for (int i = 0; i < 64; i++)
	{
		if (m_bOver)
		{
			if (m_bFocus)
				brBk[i].CreateSolidBrush(RGB(1, 76, 177));
			else
				brBk[i].CreateSolidBrush(RGB(1, 76, 177));
		}
		else
		{
			if (m_bFocus)
				brBk[i].CreateSolidBrush(RGB(1, 76, 177));
			else
				brBk[i].CreateSolidBrush(RGB(1, 76, 177));
		}
	}
	for (int i = rect->top; i <= nHeight + 2; i++)
	{
		rct.SetRect(rect->left, i, nWidth + 2, i + 1);
		pDC->FillRect(&rct, &brBk[((i * 63) / nHeight)]);
	}
	for (int i = 0; i < 64; i++)
		brBk[i].DeleteObject();
}
//绘制按钮的内边框
void CNewButton::DrawInsideBorder(CDC *pDC, CRect* rect)
{
	CPen *pLeft, *pRight, *pTop, *pBottom;
	if (m_bSelected && !m_bOver)
	{
		pLeft = &m_InsideBoundryPenLeftSel;
		pRight = &m_InsideBoundryPenRightSel;
		pTop = &m_InsideBoundryPenTopSel;
		pBottom = &m_InsideBoundryPenBottomSel;
	}
	else
	{
		pLeft = &m_InsideBoundryPenLeft;
		pRight = &m_InsideBoundryPenRight;
		pTop = &m_InsideBoundryPenTop;
		pBottom = &m_InsideBoundryPenBottom;
	}
	CPoint oldPoint = pDC->MoveTo(rect->left, rect->bottom - 1);
	CPen* pOldPen = pDC->SelectObject(pLeft);
	pDC->LineTo(rect->left, rect->top + 1);
	pDC->SelectObject(pRight);
	pDC->MoveTo(rect->right - 1, rect->bottom - 1);
	pDC->LineTo(rect->right - 1, rect->top);
	pDC->SelectObject(pTop);
	pDC->MoveTo(rect->left - 1, rect->top);
	pDC->LineTo(rect->right - 1, rect->top);
	pDC->SelectObject(pBottom);
	pDC->MoveTo(rect->left, rect->bottom);
	pDC->LineTo(rect->right - 1, rect->bottom);
	pDC->SelectObject(pOldPen);
	pDC->MoveTo(oldPoint);
	if (m_bSelected && !m_bOver)
		DrawFocusRect(pDC->m_hDC, rect);
}