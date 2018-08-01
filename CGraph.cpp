#include "stdafx.h"
#include "CGraph.h"

void CGraph::InitGraph()
{
	for (int i = 0; i < MAX_VERTEX_NUM; i++) {
		//初始化顶点
		m_Vertices[i] = -1;
	}
	m_nVexNum = 0;
}

int CGraph::AddVertex(int info)
{
	//将顶点数据info加入顶点数组
	m_Vertices[m_nVexNum++] = info;
	//返回顶点数
	return m_nVexNum;
}

int CGraph::GetVertexInfo(int nIndex)
{
	return m_Vertices[nIndex];
}

int CGraph::GetVexNum()
{
	return m_nVexNum;
}

void CGraph::UpdateVertex(int nIndex, int info)
{
	m_Vertices[nIndex] = info;
}

void CGraph::GetVertex(int nIndex, Vertex &v)
{
	//得到由nIndex指定的顶点数组中的顶点
	v.col = nIndex % COL_SIZE;
	v.row = nIndex / COL_SIZE;
}


/*


*/