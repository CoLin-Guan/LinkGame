#include "stdafx.h"
#include "CGameLogic.h"
#include <algorithm>
#include <ctime>


CGameLogic::CGameLogic()
{
	memset(m_nVexNum, 0, sizeof(m_nVexNum));
	memset(m_nCorner, 0, sizeof(m_nCorner));
	m_PathNum = 0;
	m_nMinCorner = MAX_CORNER;	//最大拐点数
	m_nMinPath = -1;
}

CGameLogic::~CGameLogic()
{
}


void CGameLogic::InitMap(CGraph &g)
{
	//清除原数据
	g.InitGraph();
	
	//地图元素
	int nVexNum = MAX_VERTEX_NUM;
	//地图合法判断变量
	int nCntPics[PICS_NUM] = { 0 };
	int nIndex = -1;
	//生成地图
	srand((unsigned)(time(NULL)));
	while (nVexNum != 0) {
		//生成随机数
		nIndex = rand() % PICS_NUM;
		//判断该图片是否已满
		if (nCntPics[nIndex] >= REPEAT_NUM)
			continue;
		//合法加入地图
		nCntPics[nIndex]++;
		g.AddVertex(nIndex);
		nVexNum--;
	}
	return;
}

void CGameLogic::Clear(CGraph &g, Vertex v1, Vertex v2)
{
	g.UpdateVertex(v1.row * COL_SIZE + v1.col, -1);
	g.UpdateVertex(v2.row * COL_SIZE + v2.col, -1);
}

bool CGameLogic::IsLink(CGraph &g, Vertex v1, Vertex v2)
{
	int nIndex = v1.row * COL_SIZE + v1.col;
	PushVertex(v1);
	int nStartInfo = g.GetVertexInfo(nIndex);

	if (DFSPath(g, v1, v2, nStartInfo)) {
		//找到路径
		return true;
	}
	else
		//没有找到路径
		return false;

	/*
	//是否为同一个图片
	if (v1.row == v2.row && v1.col == v2.col)
		return false;
	//两图是否同类
	if (anMap[v1.row][v2.col] != m_anMap[v1.row][v2.col])
		return false;

	//添加第一个点
	PushVertex(v1);
	//一条直线连接
	if (v1.row == v2.row) {
		//同一行
		if (LinkInRow(anMap, v1, v2)) {
			PushVertex(v2);
			return true;
		}
	}
	if(v1.col == v2.col) {
		//同一列
		if (LinkInCol(anMap, v1, v2)) {
			PushVertex(v2);
			return true;
		}
	}
	//两条直线连接
	if (OneCornerLink(anMap, v1, v2)) {
		PushVertex(v2);
		return true;
	}
	//三条直线连接
	if (TwoCornerLink(anMap, v1, v2)) {
		PushVertex(v2);
		return true;
	}
	//不能连接则将v1出栈
	PopVertex();
	return false;

	*/	
}

bool CGameLogic::DFSPath(CGraph &g, Vertex v1, Vertex v2, int nStartInfo)
{	
	//超出最大路径数返回
	if (m_PathNum >= MAX_PATH_NUM)
		return true;
	//初始化遍历参数
	int nIndex1 = v1.row * COL_SIZE + v1.col;
	int nIndex2 = v2.row * COL_SIZE + v2.col;
	int index[4] = { nIndex1 - 1,nIndex1 + 1,nIndex1 - COL_SIZE,nIndex1 + COL_SIZE };
	int directionRow[4] = { 0,0,-1,1 };
	int directionCol[4] = { -1,1,0,0 };

	//从v1向四个方向搜索
	for (int i = 0; i < 4; i++) {
		if (v1.col <= 0 && i == 0) continue;
		if (v1.col >= COL_SIZE - 1 && i == 1) continue;
		if (v1.row <= 0 && i == 2) continue;
		if (v1.row >= ROW_SIZE - 1 && i == 3) continue;
		if (IsExsit(index[i])) continue;
		if (index[i] == nIndex2 && g.GetVertexInfo(nIndex2) == nStartInfo) {
			//DFS到终点
			//加入路径
			PushVertex(v2);
			if (m_nCorner[m_PathNum] < m_nMinCorner) {
				m_nMinCorner = m_nCorner[m_PathNum];
				m_nMinPath = m_PathNum;
				//生成一条新的路径
				m_PathNum++;
				if (m_PathNum >= MAX_PATH_NUM)
					//超出最大路径数
					return true;
				//拷贝路径
				int nLastVexNum = m_nVexNum[m_PathNum - 1];
				for (int i = 0; i < nLastVexNum - 1; i++)
					PushVertex(m_anPath[m_PathNum - 1][i]);
				return true;
			}
			PopVertex();
		}
		else if (g.GetVertexInfo(index[i]) == -1) {
			//构造顶点
			Vertex v;
			v.row = v1.row + directionRow[i];
			v.col = v1.col + directionCol[i];
			v.info = 0;
			//加入路径
			PushVertex(v);
			if (m_nCorner[m_PathNum] < m_nMinCorner)
				DFSPath(g, v, v2, nStartInfo);
			PopVertex();
		}
	}
	if (m_PathNum > 0) return true;
	return false;
}

bool CGameLogic::IsAjacent(Vertex v1, Vertex v2)
{
	if (abs(v1.col - v2.col) == 1 && abs(v1.row - v2.row) == 0)
		return true;
	if (abs(v1.col - v2.col) == 0 && abs(v1.row - v2.row) == 1)
		return true;
	return false;
}

void CGameLogic::PushVertex(Vertex v)
{
	//将索引号为nV的顶点压入m_anPath
	//并判断是否为拐点
	//若是拐点则拐点数加1
	m_anPath[m_PathNum][m_nVexNum[m_PathNum]++] = v;

	if (IsCornor()) {
		m_nCorner[m_PathNum]++;
	}

}

void CGameLogic::PopVertex()
{
	if (IsCornor()) {
		m_nCorner[m_PathNum]--;
	}
	m_nVexNum[m_PathNum]--;
}

int CGameLogic::GetVexPath(Vertex avPath[MAX_PATH_VEX])
{
	//返回拐点最少路径实际顶点个数
	for (int i = 0; i < m_nVexNum[m_nMinPath]; i++)
		avPath[i] = m_anPath[m_nMinPath][i];
	return m_nVexNum[m_nMinPath];
}

void CGameLogic::Reset()
{
	//每次使用DFS后需要重置路径
	//清空路径栈
	while (m_PathNum != -1) {
		while (m_nVexNum[m_PathNum])
			PopVertex();
		m_PathNum--;
	}
	m_PathNum = 0;
	m_nMinCorner = MAX_CORNER;
	m_nMinPath = -1;
}

bool CGameLogic::IsExsit(int nVi)
{
	for (int i = 0; i < m_nVexNum[m_PathNum]; i++) {
		int nIndex = m_anPath[m_PathNum][i].row * COL_SIZE + m_anPath[m_PathNum][i].col;
		if (nIndex == nVi)
			return true;
	}
	return false;
}

bool CGameLogic::IsCornor()
{
	if (m_nVexNum[m_PathNum] < 3) return false;
	//判断路径栈顶点是否为拐点
	int nIndex[3] = { 0 };
	nIndex[0] = m_anPath[m_PathNum][m_nVexNum[m_PathNum] - 1].row * COL_SIZE + m_anPath[m_PathNum][m_nVexNum[m_PathNum] - 1].col;
	nIndex[1] = m_anPath[m_PathNum][m_nVexNum[m_PathNum] - 2].row * COL_SIZE + m_anPath[m_PathNum][m_nVexNum[m_PathNum] - 2].col;
	nIndex[2] = m_anPath[m_PathNum][m_nVexNum[m_PathNum] - 3].row * COL_SIZE + m_anPath[m_PathNum][m_nVexNum[m_PathNum] - 3].col;
	
	std::sort(nIndex, nIndex + 3);
	if (nIndex[0] - nIndex[1] == nIndex[1] - nIndex[2])
		return false;
	return true;
}

bool CGameLogic::IsBlank(CGraph &g)
{
	int nVexNum = g.GetVexNum();
	for (int i = 0; i < nVexNum; i++) {
		if (g.GetVertexInfo(i) != -1)
			return false;
	}
	
	return true;
}

bool CGameLogic::SearchValidPath(CGraph &g, Vertex &v1, Vertex &v2)
{
	//寻找消子路径（提示）
	for (int i = 0; i < MAX_VERTEX_NUM; i++) {
		if (g.GetVertexInfo(i) == -1) continue;
		for (int j = i + 1; j < MAX_VERTEX_NUM; j++) {
			if (g.GetVertexInfo(j) != g.GetVertexInfo(i)) continue;
			//组装顶点
			g.GetVertex(i, v1);
			g.GetVertex(j, v2);
			int nStartInfo = g.GetVertexInfo(i);
			//寻找路径
			if (DFSPath(g, v1, v2, nStartInfo)) {
				//成功找到可连接路径
				return true;
			}
		}
	}
	return false;

}

void CGameLogic::ResetGraph(CGraph &g)
{
	//随机交换任意现有地图g中的两张图片100次
	int cnt = 100;
	int nIndex1 = 0, nIndex2 = 0;
	int nInfo1 = 0, nInfo2 = 0;
	srand((unsigned)(time(NULL)));
	for (int i = 0; i < cnt; i++) {
		do {
			//随机得到两个图片
			nIndex1 = rand() % (MAX_VERTEX_NUM);
			nIndex2 = rand() % (MAX_VERTEX_NUM);
			//若所选两张图都是空
			//则重选
		} while (g.GetVertexInfo(nIndex1) == -1 && g.GetVertexInfo(nIndex2) == -1);
		//进行交换
		nInfo1 = g.GetVertexInfo(nIndex1);
		nInfo2 = g.GetVertexInfo(nIndex2);
		g.UpdateVertex(nIndex1, nInfo2);
		g.UpdateVertex(nIndex2, nInfo1);
	}

}




/*

void CGameLogic::UpdateArc(CGraph &g, int nRow, int nCol)
{
int nV1Index = nRow * 4 + nCol;
if (nCol > 0) {
int nV2Index = nV1Index - 1;
//判断左边相邻
if (g.GetVertex(nV1Index) == g.GetVertex(nV2Index))
g.AddArc(nV1Index, nV2Index);
}
if (nCol < 3) {
int nV2Index = nV1Index + 1;
//判断右边相邻
if (g.GetVertex(nV1Index) == g.GetVertex(nV2Index))
g.AddArc(nV1Index, nV2Index);
}
if (nRow > 0) {
int nV2Index = nV1Index - 4;
//判断正上方
if (g.GetVertex(nV1Index) == g.GetVertex(nV2Index))
g.AddArc(nV1Index, nV2Index);
}
if (nRow < 3) {
int nV2Index = nV1Index + 4;
//判断正下方
if (g.GetVertex(nV1Index) == g.GetVertex(nV2Index))
g.AddArc(nV1Index, nV2Index);
}
}


bool CGameLogic::LinkInRow(int anMap[][4], Vertex v1, Vertex v2)
{
//判断横向是否可相连
int nCol1 = v1.col;
int nCol2 = v2.col;
int nRow = v1.row;
//保证nCol1<nCol2
if (nCol1 > nCol2) {
int temp = nCol1;
nCol1 = nCol2;
nCol2 = temp;
}
//判断是否能相连
for (int i = nCol1 + 1; i < nCol2; i++) {
if (anMap[nRow][i] != BLANK) return false;
}
return true;
}

bool CGameLogic::LinkInCol(int anMap[][4], Vertex v1, Vertex v2)
{
//判断纵向是否可相连
int nCol = v1.col;
int nRow1 = v2.row;
int nRow2 = v1.row;
//保证nRow1<nRow2
if (nRow1 > nRow2) {
int temp = nRow1;
nRow1 = nRow2;
nRow2 = temp;
}
//判断是否能相连
for (int i = nRow1 + 1; i < nRow2; i++) {
if (anMap[i][nCol] != BLANK) return false;
}
return true;
}

bool CGameLogic::OneCornerLink(int anMap[][4], Vertex v1, Vertex v2)
{
//判断通过一个拐点是否可相连
int nRow1 = v1.row;
int nRow2 = v2.row;
int nCol1 = v1.col;
int nCol2 = v2.col;

//从（nRow1, nCol1）到（nRow2, nCol1）再到（nRow2, nCol2）可否相连
if (LineY(anMap, nRow1, nRow2, nCol1) && LineX(anMap, nRow2, nCol1, nCol2)) {
//添加拐点
Vertex v;
v.col = nCol1;
v.row = nRow2;
PushVertex(v);
return true;
}
//从（nRow1, nCol1）到（nRow1, nCol2）再到（nRow2, nCol2）可否相连
if (LineX(anMap, nRow1, nCol1, nCol2) && LineY(anMap, nRow1, nRow2, nCol2)) {
//添加拐点
Vertex v;
v.col = nCol2;
v.row = nRow1;
PushVertex(v);
return true;
}
return false;
}

bool CGameLogic::TwoCornerLink(int anMap[][4], Vertex v1, Vertex v2)
{
//判断通过两个拐点是否可相连
int nRow1 = v1.row;
int nRow2 = v2.row;
int nCol1 = v1.col;
int nCol2 = v2.col;

//两点距离是否太近
if (abs(nRow1 - nRow2) <= 1 || abs(nCol1 - nCol2) <= 1)
return false;

//控制遍历拐点方向
int direct = 1;
if (nRow1 > nRow2)
direct = -1;
//从nRow1向下遍历拐点
for (int i = nRow1 + direct; i != nRow2; i+=direct) {
if (LineY(anMap, nRow1, i, nCol1) && LineX(anMap, i, nCol1, nCol2) && LineY(anMap, i, nRow2, nCol2)) {
Vertex v;
v.col = nCol1;
v.row = i;
PushVertex(v);
v.col = nCol2;
PushVertex(v);
return true;
}
}
//控制遍历拐点方向
direct = 1;
if (nCol1 > nCol2)
direct = -1;
for (int i = nCol1 + direct; i != nCol2; i+=direct) {
if (LineX(anMap, nRow1, i, nCol1) && LineY(anMap, nRow1, nRow2, i) && LineX(anMap, nRow2, nCol2, i)) {
Vertex v;
v.col = i;
v.row = nRow1;
PushVertex(v);
v.row = nRow2;
PushVertex(v);
return true;
}
}
return false;
}

bool CGameLogic::LineY(int anMap[][4], int nRow1, int nRow2, int nCol)
{
//判断（nRow1, nCol）到（nRow2, nCol）能否相连
//包括起始点和终点
//保证nRow1<nRow2
if (nRow1 > nRow2) {
int temp = nRow1;
nRow1 = nRow2;
nRow2 = temp;
}
for (int i = nRow1; i <= nRow2; i++) {
if (anMap[i][nCol] != BLANK) return false;
}
return true;
}

bool CGameLogic::LineX(int anMap[][4], int nRow, int nCol1, int nCol2)
{
//判断（nRow, nCol1）到能（nRow, nCol2）否相连
//包括起始点
//保证nCol1<nCol2
if (nCol1 > nCol2) {
int temp = nCol1;
nCol1 = nCol2;
nCol2 = temp;
}
for (int i = nCol1; i <= nCol2; i++) {
if (anMap[nRow][i] != BLANK) return false;
}
return true;
}

*/
