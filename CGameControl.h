#pragma once
#include "global.h"
#include "CGameLogic.h"
#include "CGraph.h"

class CGameControl
{
public:
	CGameControl();
	~CGameControl();
	virtual void StartGame() = 0;		//初始化地图
	virtual int IsWin(int nTime);							//胜负判断
	virtual bool Help(Vertex &v1, Vertex &v2);				//帮助提示
	virtual void Reset();									//重排地图
	virtual int GetGrade();									//获得游戏计分
	virtual void UseProp();									//道具使用
	virtual bool PorpLink(Vertex avPath[MAX_PATH_VEX], int &nVexNum);			//使用道具连接

	void SetGameFlag(FLAG flag);		//设置m_flag
	void GetGameFlag(FLAG &flag);		//获得m_flag
	int GetElement(int nRow, int nCol);	//获取某行列图片编号
	void SetFirstPoint(int nRow, int nCol);	//设置第一个点
	void SetSecPoint(int nRow, int nCol);	//设置第二个点
	bool Link(Vertex avPath[MAX_PATH_VEX], int &nVexNum);	//判断连接函数

protected:
	CGameLogic gameLogic;
	CGraph m_graph;		//游戏地图
	Vertex m_ptSelFirst;//选中的第一个点
	Vertex m_ptSelSec;	//选中的第二个点
	FLAG m_flag;		//游戏模式标志
	int m_nGrade;		//休闲模式下游戏计分
};

