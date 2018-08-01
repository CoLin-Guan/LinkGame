#pragma once
class CNewButton : public CButton
{
public:
	CNewButton();
protected:
	CPen m_BoundryPen; //按钮的外边框
					   //鼠标指针置于按钮之上时按钮的内边框
	CPen m_InsideBoundryPenLeft;
	CPen m_InsideBoundryPenRight;
	CPen m_InsideBoundryPenTop;
	CPen m_InsideBoundryPenBottom;
	//按钮获得焦点时按钮的内边框
	CPen m_InsideBoundryPenLeftSel;
	CPen m_InsideBoundryPenRightSel;
	CPen m_InsideBoundryPenTopSel;
	CPen m_InsideBoundryPenBottomSel;
	//按钮的底色，包括有效和无效两种状态
	CBrush m_FillActive;
	CBrush m_FillInactive;
	//按钮的状态
	BOOL m_bOver; //鼠标位于按钮之上时该值为true，反之为flase
	BOOL m_bTracking; //在鼠标按下没有释放时该值为true
	BOOL m_bSelected; //按钮被按下是该值为true
	BOOL m_bFocus;     //按钮为当前焦点所在时该值为true
public:
protected:
	virtual void PreSubclassWindow();
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void DoGradientFill(CDC *pDC, CRect* rect);
	virtual void DrawInsideBorder(CDC *pDC, CRect* rect);
	virtual ~CNewButton();
protected:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};