#include "stdafx.h"
#include "CBasicGame.h"


CBasicGame::CBasicGame()
{
}


CBasicGame::~CBasicGame()
{
}

void CBasicGame::StartGame()
{
	gameLogic.InitMap(m_graph);
}

