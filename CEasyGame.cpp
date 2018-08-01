#include "stdafx.h"
#include "CEasyGame.h"


CEasyGame::CEasyGame()
{
}

CEasyGame::~CEasyGame()
{
}

void CEasyGame::StartGame()
{
	gameLogic.InitMap(m_graph);
}

int CEasyGame::GetGrade()
{
	return m_nGrade;
}

int CEasyGame::IsWin(int nTime)
{
	if (gameLogic.IsBlank(m_graph))
		return GAME_SUCCESS;
	return GAME_PLAY;
}

void CEasyGame::UseProp()
{
	m_nGrade -= 50;
}

bool CEasyGame::PorpLink(Vertex avPath[MAX_PATH_VEX], int &nVexNum)
{
	//已被消子判断
	if (GetElement(m_ptSelFirst.row, m_ptSelFirst.col) == -1 || GetElement(m_ptSelSec.row, m_ptSelSec.col) == -1)
		return false;
	if (m_graph.GetVertexInfo(m_ptSelFirst.row*COL_SIZE + m_ptSelFirst.col) == m_graph.GetVertexInfo(m_ptSelSec.row*COL_SIZE + m_ptSelSec.col)) {
		//加入直接连接路径
		avPath[0] = m_ptSelFirst;
		avPath[1] = m_ptSelSec;
		nVexNum = 2;
		//消子
		gameLogic.Clear(m_graph, m_ptSelFirst, m_ptSelSec);
		//计分更新
		m_nGrade += 10;
		return true;
	}
	return false;
}



