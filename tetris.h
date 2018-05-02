#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

#define ROW_X 16
#define ROW 28
#define COLUMN 12
#define BEGIN_X 24
#define BEGIN_Y 0
#define D_TIMER 1
#define UP_TIMER 2

int game_array[ROW][COLUMN] = { 0 };//游戏区数组
int forecast_array[4][4] = { 0 };//预测区数组
int Block[19][4][4];//方块数组
char Color[7];//颜色数组

void SetXY(int x, int y);//设置光标坐标
void Hidden(); //隐藏光标
void Display();////显示开始界面，提示进入游戏
void StartGame();//游戏开始，初始化
void DrawGameFrame();//画游戏边框
void DisplaySpace();//画显示区
void ForecastSpace();//显示下一个随机方块
void CreateBlock();//产生下落方块
void GameSpace();//游戏界面显示
void Down();//方块下落
void FullSpace();//固定方块
void Left();//方块左移
void Right();//方块右移
void Shape();//方块变形
void Contrl();//游戏操作
void SpeedUp();//方块加速下落
int Change(int blockID);//产生新的方块编号
int IsBottom();//判断方块是否下落到底部
int IsRBorder();//判断方块是否到达右边界
int IsLBorder();//判断方块是否到达左边界
int CheckDown();//判断方块是否能下落
int CheckAround();//判断方块是否能左右移动
void IsOver();//判断游戏是否结束
void RemoveRow();//消除满行
void GameOver();//游戏结束
void SetLevel();//设置等级
VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);//定时器回调函数

/*UINT_PTR SetTimer(  HWND hWnd,              // handle to window
UINT_PTR nIDEvent,      // timer identifier
UINT uElapse,           // time-out value
TIMERPROC lpTimerFunc   // timer procedure
);设置定时器

LONG yourWndProc(HWND hWnd,UINT uMessageType,WPARAM wP,LPARAM)

VOID CALLBACK TimerProc(
HWND hwnd,         // handle to window
UINT uMsg,         // WM_TIMER message
UINT_PTR idEvent,  // timer identifier
DWORD dwTime       // current system time
);定时器回调函数*/


//颜色数组
char Color[7] =
{
	(0x03),
	(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY),
	(FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY),
	(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY),
	(FOREGROUND_RED | FOREGROUND_INTENSITY),
	(FOREGROUND_BLUE | FOREGROUND_INTENSITY),
	(FOREGROUND_GREEN | FOREGROUND_INTENSITY)
};

//方块数组
int Block[19][4][4] = 
{
	{//0田字
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 1, 1, 0 },
		{ 0, 1, 1, 0 }
	},
	{//1竖条
		{ 0, 1, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 1, 0, 0 }
	},
	{//2横条
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 1, 1, 1, 1 }
	},
	
	{//3Z形
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 1, 1, 0 },
		{ 0, 0, 1, 1 }
	},
	{//4Z形
		{ 0, 0, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 1, 1, 0 },
		{ 0, 1, 0, 0 }
	},
	
	{//5反Z形
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 1, 1 },
		{ 0, 1, 1, 0 }
	},
	{//6反Z形
		{ 0, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 1, 1, 0 },
		{ 0, 0, 1, 0 }
	},
	
	{//7L形
		{ 0, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 1, 1, 0 }
	},
	{//8L形
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 1, 1, 1, 0 }
	},
	{//9L形
		{ 0, 0, 0, 0 },
		{ 0, 1, 1, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 1, 0 }
	},
	{//10L形
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 1, 1, 1 },
		{ 0, 1, 0, 0 }
	},
	
	{//11反L形
		{ 0, 0, 0, 0 },
		{ 0, 1, 1, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 1, 0, 0 }
	},
	{//12反L形
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 1, 1, 1 }
	},
	{//13反L形
		{ 0, 0, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 1, 1, 0 }
	},
	{//14反L形
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 1, 1, 1 },
		{ 0, 0, 0, 1 }
	},
	{//15T形
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 1, 1, 1 },
		{ 0, 0, 1, 0 }
	},
	{//16T形
		{ 0, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 1, 1, 0 },
		{ 0, 1, 0, 0 }
	},
	{//17T形
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 1, 1, 1 }
	},
	{//18T形
		{ 0, 0, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 1, 1, 0 },
		{ 0, 0, 1, 0 }
	},
};




