#pragma once
#include "global.h"
class CGraph
{
public:
	CGraph()
	{
		InitGraph();
	}
	~CGraph()
	{
		
	}

#define ROW_SIZE 12				//地图行数
#define COL_SIZE 13				//地图列数
#define PICS_NUM 13				//图片种类数
#define REPEAT_NUM 12			//每种图片数量
#define GRAPH_TOP 50			//地图左上角纵坐标
#define GRAPH_LEFT 20			//地图左上角横坐标
#define PIC_WIDTH 60			//顶点图片宽度
#define PIC_HIGH 60				//顶点图片高度
#define MAX_VERTEX_NUM (ROW_SIZE*COL_SIZE)	//最大顶点数

	int AddVertex(int indo);					//将info添加到顶点数组中
	int GetVertexInfo(int nIndex);				//获取顶点数组中索引为nIndex的顶点信息
	int GetVexNum();							//获取顶点数组中顶点数
	void UpdateVertex(int nIndex, int info);	//更新结点数组
	void InitGraph();							//初始化地图
	void GetVertex(int nIndex, Vertex &v);		//生成结点结构体



protected:
	typedef int Vertices[MAX_VERTEX_NUM];

	Vertices m_Vertices;	//顶点数组
	int m_nVexNum;			//顶点数
	int m_nArcNum;			//边数
	
	//functions

};


