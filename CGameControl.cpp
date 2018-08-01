#include "stdafx.h"
#include "CGameControl.h"
#include "CGameLogic.h"

CGameControl::CGameControl()
{
	m_nGrade = 0;
}

CGameControl::~CGameControl()
{
}

void CGameControl::SetGameFlag(FLAG flag)
{
	m_flag.bProp = flag.bProp;
	m_flag.bScore = flag.bScore;
	m_flag.bTimer = flag.bTimer;
	m_flag.szTitle = flag.szTitle;
}

void CGameControl::GetGameFlag(FLAG &flag)
{
	flag.bProp = m_flag.bProp;
	flag.bScore = m_flag.bScore;
	flag.bTimer = m_flag.bTimer;
	flag.szTitle = m_flag.szTitle;
}

int CGameControl::GetElement(int nRow, int nCol)
{
	return m_graph.GetVertexInfo(nRow * COL_SIZE + nCol);
}

void CGameControl::SetFirstPoint(int nRow, int nCol)
{
	m_ptSelFirst.col = nCol;
	m_ptSelFirst.row = nRow;
}

void CGameControl::SetSecPoint(int nRow, int nCol)
{
	m_ptSelSec.col = nCol;
	m_ptSelSec.row = nRow;
}

bool CGameControl::Link(Vertex avPath[MAX_PATH_VEX], int &nVexNum)
{
	//已被消子判断
	if (GetElement(m_ptSelFirst.row, m_ptSelFirst.col) == -1 || GetElement(m_ptSelSec.row, m_ptSelSec.col) == -1)
		return false;

	//判断消子
	if (gameLogic.IsLink(m_graph, m_ptSelFirst, m_ptSelSec)) {
		//消子
		gameLogic.Clear(m_graph, m_ptSelFirst, m_ptSelSec);
		//计分计算
		m_nGrade += 10;
		//获取路径信息
		nVexNum = gameLogic.GetVexPath(avPath);
		//重置路径信息
		gameLogic.Reset();
		return true;
	}
	//重置路径信息
	gameLogic.Reset();
	return false;

	/*
	
	//假设可消子
	//保存消子info
	int tempInfo = m_anMap[m_ptSelFirst.row][m_ptSelFirst.col];
	//消子
	gameLogic.Clear(m_anMap, m_ptSelFirst, m_ptSelSec);
	//两图是否能够连通
	if (gameLogic.IsLink(m_graph, m_ptSelFirst, m_ptSelSec)) {
		//获取路径信息
		nVexNum = gameLogic.GetVexPath(avPath);
		return true;
	}
	//不能消子则恢复info
	m_anMap[m_ptSelFirst.row][m_ptSelFirst.col] = tempInfo;
	m_anMap[m_ptSelSec.row][m_ptSelSec.col] = tempInfo;
	return false;
	
	*/
}

int CGameControl::IsWin(int nTime)
{
	if (nTime <= 0) {
		//游戏失败
		//清空地图
		m_graph.InitGraph();
		return GAME_LOSE;
	}
	//判断图中元素是否被全部消除
	if (gameLogic.IsBlank(m_graph)) {
		//清空分数
		m_nGrade = 0;
		return GAME_SUCCESS;
	}

	return GAME_PLAY;
}

bool CGameControl::Help(Vertex &v1, Vertex &v2)
{
	if (gameLogic.SearchValidPath(m_graph, v1, v2)) {
		//找到可消除路径
		//计分扣除
		m_nGrade -= 20;
		//重置路径信息
		gameLogic.Reset();
		return true;
	}
	return false;
}

void CGameControl::Reset()
{
	gameLogic.ResetGraph(m_graph);
	//计分扣除
	m_nGrade -= 30;

}

int CGameControl::GetGrade()
{
	return 0;
}

void CGameControl::UseProp()
{
}

bool CGameControl::PorpLink(Vertex avPath[MAX_PATH_VEX], int &nVexNum)
{
	return false;
}


