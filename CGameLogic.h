#pragma once
#include "global.h"
#include "CGraph.h"
class CGameLogic
{
protected:
#define MAX_CORNER 5		//最大允许拐点数
#define MAX_PATH_NUM 20		//DFS最大路径数
#define MAX_PATH_VEX 168	//最长连线32顶点
	Vertex m_anPath[MAX_PATH_NUM][MAX_PATH_VEX];//保存连通路径	
	int m_nVexNum[MAX_PATH_NUM];				//经过顶点数
	int m_nCorner[MAX_PATH_NUM];				//连通路径中的拐点数
	int m_PathNum;			//连通路径数量
	int m_nMinCorner;		//最小拐点数
	int m_nMinPath;			//最小拐点数对应路径

	//functions
	void PushVertex(Vertex v);				//路径顶点压入m_anPath，同时更新顶点数
	void PopVertex();						//路径顶点出m_anPath，取出前判断是否是拐点
	bool DFSPath(CGraph &g, Vertex v1, Vertex v2, int nStartInfo);	//DFS判断v1,v2能否连通，并记录最小拐点路径
	bool IsExsit(int nVi);					//判断nVi是否保存在当前连通路径m_anPath中
	bool IsCornor();						//判断三个顶点是否构成一个拐点
	bool IsAjacent(Vertex v1, Vertex v2);	//判断是否左右或者上下相邻

public:
	CGameLogic();
	~CGameLogic();
	void InitMap(CGraph &g);						//初始化游戏地图
	bool IsLink(CGraph &g, Vertex v1, Vertex v2);	//判断是否相同元素
	void Clear(CGraph &g, Vertex v1, Vertex v2);	//消子函数
	int GetVexPath(Vertex avPath[]);				//获得路径并返回顶点数
	void Reset();									//重置路径信息变量
	bool IsBlank(CGraph &g);						//判断地图是否全-1（全消除）
	bool SearchValidPath(CGraph &g, Vertex &v1, Vertex &v2);	//寻找消子路径（提示）
	void ResetGraph(CGraph &g);									//对地图进行随机重排
	
	


	/*


	bool LinkInRow(int anMap[][4], Vertex v1, Vertex v2);		//判断横向是否连通
	bool LinkInCol(int anMap[][4], Vertex v1, Vertex v2);		//判断纵向是否连通
	bool OneCornerLink(int anMap[][4], Vertex v1, Vertex v2);	//一个拐点连接判断
	bool TwoCornerLink(int anMap[][4], Vertex v1, Vertex v2);	//两个拐点连接判断
	bool LineY(int anMap[][4], int nRow1, int nRow2, int nCol);	//直线连通Y轴
	bool LineX(int anMap[][4], int nRow, int nCol1, int nCol2);	//直线连通X轴
	*/

};

