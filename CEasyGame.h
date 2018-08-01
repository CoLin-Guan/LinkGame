#pragma once
#include "CGameControl.h"
class CEasyGame :
	public CGameControl
{
public:
	CEasyGame();
	~CEasyGame();
	void StartGame();
	int GetGrade();
	int IsWin(int nTime);
	void UseProp();
	bool PorpLink(Vertex avPath[MAX_PATH_VEX], int &nVexNum);
};

