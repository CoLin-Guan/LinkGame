#pragma once

#define BLANK -1
#define PLAY_TIMER_ID 1
#define GAME_LOSE -1
#define GAME_SUCCESS 0
#define GAME_PLAY 1

#define GAME_DLG_WIDTH 1024
#define GAME_DLG_HIGH 819

//定义游戏计分结构体
typedef struct tagScore
{
	int score;		//得分
	int level;		//等级
}Score;

//定义游戏窗口空间FLAG结构体
typedef struct tagFlag
{
	bool bTimer;	//计时进度条和计时时间栏
	bool bProp;		//道具按钮
	bool bScore;	//积分栏
	CString szTitle;//对话框标题
}FLAG;

//定义顶点结构体
typedef struct tagVertex 
{
	int row;
	int col;
	int info;
}Vertex;