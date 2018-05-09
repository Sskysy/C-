
#include "tetris.h"

HANDLE out;//标准输出句柄
int x = 0;
int y = 0;
int grade = 0;
int level = 0;
int blockID = 0;
int new_blockID = 0;
int colorNum = 0;

//下落方块数组左上角方块的下标
int b_row;
int b_clo;

int down_array[4][4] = { 0 };//下落方块数组
int temp[4][4] = { 0 };//临时数组

//定时器相关参数
int dTime = 500;
int id_1;
int id_2;
int id_3;

//设置光标位置
void SetXY(x, y) 
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(out, coord);
}

//隐藏光标
void Hidden()
{
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(out, &cci);
	cci.bVisible = 0;
	SetConsoleCursorInfo(out, &cci);
}

//显示开始界面，提示进入游戏
void Display() 
{
	int CHOICE = 1;
	int cache = 0;
	char key;
	system("cls");
	Hidden();

	while (1)
	{
		if (cache == 0)
		{
			system("cls");
			SetXY(25, 7);
			printf("*===* Welcome To Tetris! *===*");
		}
		if (CHOICE != cache)
		{
			if (CHOICE == 1)
			{
				SetXY(35, 9);
				SetConsoleTextAttribute(out, 0xF0);
				printf("START GAME");
				SetXY(35, 10);
				SetConsoleTextAttribute(out, 0x0F);
				printf("EXIT");
			}
			else if (CHOICE == 2)
			{
				SetXY(35, 9);
				SetConsoleTextAttribute(out, 0x0F);
				printf("START GAME");
				SetXY(35, 10);
				SetConsoleTextAttribute(out, 0xF0);
				printf("EXIT");
			}
			cache = CHOICE;
		}
		SetConsoleTextAttribute(out, 0x0F);
		if (_kbhit()) //检测按键
		{
			key = _getch();
			if (key == 72 || key == 'w' || key == 'W')
			{
				CHOICE--;
				if (CHOICE == 0)
				{
					CHOICE = 2;
				}
			}
			else if (key == 80 || key == 's' || key == 'S')
			{
				CHOICE++;
				if (CHOICE == 3)
				{
					CHOICE = 1;
				}
			}
			else if (key == '\r' || key == 13)
			{
				if (CHOICE == 1)
				{
					StartGame();
				}
				else if (CHOICE == 2)
				{
					exit(0);
				}
				break;
			}
		}
	}
}

//画游戏边框
void DrawGameFrame()
{
	int x = 0,
		y = 0,
		i = 0;
	SetConsoleTextAttribute(out, 0xF0);
	for (x = 40, y = 0; y < 25; y++)
	{
		SetXY(x, y);
		printf("%2s", " ");
		printf("\n");
	}
	for (x = 16, y = 24; x < 40; x++)
	{
		SetXY(x, y);
		printf("%2s", " ");
		printf("\n");
	}
	for (x = 14, y = 0; y < 25; y++)
	{
		SetXY(x, y);
		printf("%2s", " ");
		printf("\n");
	}
	SetConsoleTextAttribute(out, 0x08);
	for (x = 65, y = 2; y < 10; y++)
	{
		SetXY(x, y);
		printf("@");
	}
	for (x = 53, y = 9; x < 65; x++)
	{
		SetXY(x, y);
		printf("o");
	}
	for (x = 52, y = 2; y < 10; y++)
	{
		SetXY(x, y);
		printf("@");
	}
	for (x = 53, y = 2; x < 65; x++)
	{
		SetXY(x, y);
		printf("o");
	}
}

//画显示区
void DisplaySpace()
{
	SetConsoleTextAttribute(out, 0x13);
	SetXY(53, 12);
	printf("GRADE : %d", grade);
	SetXY(53, 13);
	printf("LEVEL : %d", level);
	SetConsoleTextAttribute(out, 0x03);
	SetXY(53, 15);
	printf("操作：");
	SetXY(53, 16);
	printf("  a  A  ←  左移");
	SetXY(53, 17);
	printf("  d  D  →  右移");
	SetXY(53, 18);
	printf("  s  S  ↓  加速");
	SetXY(53, 19);
	printf("  w  W  ↑  变形");
}

//游戏开始
void StartGame()
{
	int i, j;
	for (i = 0; i < ROW; i++)
	{
		for (j = 0; j < COLUMN; j++)
		{
			game_array[i][j] = 0;
		}
	}
	system("cls");
	DrawGameFrame();//画游戏边框
	DisplaySpace();//画显示区
	srand((unsigned int)time(NULL));//随机种子
	GameSpace();//游戏界面显示
	ForecastSpace();//显示第一个随机方块
	Sleep(500);
	CreateBlock();//产生第一个下落方块
	id_1 = SetTimer(NULL, D_TIMER, dTime, &TimerProc);//打开定时器
	id_3 = SetTimer(NULL, 3, 10, &TimerProc);//打开定时器
}

//显示下一个随机方块
void ForecastSpace()
{
	int x = 0,
		y = 0,
		i = 0,
		j = 0;
	blockID = rand() % 19;
	colorNum = rand() % 6 + 1;
	for (i = 0; i < 4; i++)
	{
		SetXY(55, 4 + i);
		for (j = 0; j < 4; j++)
		{
			forecast_array[i][j] = Block[blockID][i][j];
			if (forecast_array[i][j] == 0)
			{
				SetConsoleTextAttribute(out, 0x0F);
					//printf("□");
				printf("%2s", "");
			}
			else
			{
				SetConsoleTextAttribute(out, Color[colorNum]);
				printf("■");
			}
		}
		SetConsoleTextAttribute(out, 0x0F);
	}
}

//产生下落方块
void CreateBlock()
{
	int i = 0,
		j = 0;
	for (i = 0; i < 4; i++)
	{
		SetXY(BEGIN_X, BEGIN_Y + i);
		for (j = 0; j < 4; j++)
		{
			down_array[i][j] = forecast_array[i][j];
			if (down_array[i][j] == 1)
			{
				game_array[BEGIN_Y + i][((BEGIN_X - ROW_X) / 2) + j] = down_array[i][j];
			}
			else
			{
				SetConsoleTextAttribute(out, 0x0F);
				//printf("□");
				printf("%2s", "");
			}
		}
	}
	b_row = 0;
	b_clo = 4;
}

//游戏界面显示
void GameSpace()
{
	int i = 0,
		j = 0;
	for (i = 4; i < ROW; i++)
	{
		SetXY(ROW_X, i - 4);
		for (j = 0; j < COLUMN; j++)
		{
			if (game_array[i][j] == 1)
			{
				SetConsoleTextAttribute(out, Color[colorNum]);
				printf("■");
			}
			else if (game_array[i][j] == 2)
			{
				SetConsoleTextAttribute(out, Color[0]);
				printf("■");
			}
			else
			{
				SetConsoleTextAttribute(out, 0x0F);
				//printf("□");
				printf("%2s", "");
			}
		}
		printf("\n");
	}
}

//游戏操作
void Contrl()
{
	char key;
	if (_kbhit())
	{
		key = _getch();
		if (key == 'a' || key == 'A' || key == 75)
		{
			Left();
		}
		else if (key == 'd' || key == 'D' || key == 77)
		{
			Right();
		}
		else if (key == 's' || key == 'S' || key == 80)
		{
			id_2 = SetTimer(NULL, UP_TIMER, 50, &TimerProc);
		}
		else if (key == 'w' || key == 'W' || key == 72)
		{
			Shape(new_blockID);
		}
	}
}

//判断方块是否下落到底部
int IsBottom()
{
	int i = 0;
	for (i = 0; i < 12; i++)
	{
		if (game_array[ROW - 1][i] == 1)
		{
			return 0;
		}
	}
	return 1;
}

//判断方块是否到达右边界
int IsRBorder()
{
	int i = 0;
	for (i = 0; i < ROW; i++)
	{
		if (game_array[i][11] == 1)
		{
			return 0;
		}
	}
	return 1;
}

//判断方块是否到达左边界
int IsLBorder()
{
	int i = 0;
	for (i = 0; i < ROW; i++)
	{
		if (game_array[i][0] == 1)
		{
			return 0;
		}
	}
	return 1;
}

//判断方块是否能下落
int CheckDown()
{
	int i = 0,
		j = 0;
	for (i = ROW - 1; i >= 0; i--)
	{
		for (j = 0; j < COLUMN; j++)
		{
			if (game_array[i][j] == 1)
			{
				if (game_array[i + 1][j] == 2)
				{
					return 0;
				}
			}
		}
	}
	return 1;
}

//判断方块是否能左右移动
int CheckAround()
{
	int i = 0,
		j = 0;
	for (i = ROW - 1; i >= 0; i--)
	{
		for (j = 0; j < COLUMN; j++)
		{
			if (game_array[i][j] == 1)
			{
				if (game_array[i][j - 1] == 2 || game_array[i][j + 1] == 2)
				{
					return 0;
				}
			}
		}
	}
	return 1;

}

//方块下落
void Down()
{
	int i = 0,
		j = 0;
	if (IsBottom() == 1 && CheckDown() == 1)
	{
		for (i = ROW - 1; i >= 0; i--)
		{
			for (j = 0; j < COLUMN; j++)
			{
				if (game_array[i][j] == 1 && game_array[i + 1][j] == 0)
				{
					game_array[i + 1][j] = game_array[i][j];
					game_array[i][j] = 0;
				}
			}
		}
		b_row++;
	}
	if (IsBottom() == 0 || CheckDown() == 0)
	{
		FullSpace();
		IsOver();
		RemoveRow();
		ForecastSpace();
		KillTimer(NULL, id_2);
		Sleep(500);
		CreateBlock();
	}
	
}

//固定方块
void FullSpace()
{
	int i = 0,
		j = 0;
	for (i = 0; i < ROW; i++)
	{
		for (j = 0; j < COLUMN; j++)
		{
			if (game_array[i][j] == 1)
			{
				game_array[i][j] = 2;
			}
		}
	}
}
		
//方块加速下落
void SpeedUp()
{
	Down();
	GameSpace();
	if (IsBottom() == 0 || CheckDown() == 0)
	{
		KillTimer(NULL, id_2);
	}
}

//方块左移
void Left()
{
	int i = 0, 
		j = 0;
	if (IsLBorder() == 1 && IsBottom() == 1 && CheckAround() == 1)
	{
		for (i = 0; i < ROW; i++)
		{
			for (j = 0; j < COLUMN; j++)
			{
				if (game_array[i][j] == 1)
				{
					game_array[i][j - 1] = game_array[i][j];
					game_array[i][j] = 0;
				}
			}
		}
		b_clo--;
		GameSpace(colorNum);
	}
}

//方块右移
void Right()
{
	int i = 0,
		j = 0;
	if (IsRBorder() == 1 && IsBottom() == 1 && CheckAround() == 1)
	{
		for (i = 0; i < ROW; i++)
		{
			for (j = COLUMN; j >= 0; j--)
			{
				if (game_array[i][j] == 1)
				{
					game_array[i][j + 1] = game_array[i][j];
					game_array[i][j] = 0;
				}
			}
		}
		b_clo++;
		GameSpace(colorNum);
	}
}

//产生新的方块编号
int Change(int blockID)
{
	if (blockID <= 6)
	{
		if (blockID == 0)
		{
			new_blockID = blockID;
		}
		else
		{
			if (blockID % 2 == 0)
			{
				new_blockID = blockID - 1;
			}
			else
			{
				new_blockID = blockID + 1;
			}
		}
	}
	else
	{
		if ((blockID + 1) % 4 == 0)
		{
			new_blockID = blockID + 3;
		}
		else
		{
			new_blockID = blockID - 1;
		}
	}
	return new_blockID;
}

//方块变形
void Shape()
{
	int i = 0,
		j = 0;
	new_blockID = Change(blockID);
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			temp[i][j] = game_array[i + b_row][j + b_clo];
			if (IsLBorder() == 1 && IsRBorder() == 1 && CheckAround() == 1 && CheckDown() == 1 && IsBottom() == 1)
			{
				temp[i][j] = Block[new_blockID][i][j];
				game_array[i + b_row][j + b_clo] = temp[i][j];
			}
			else
			{
				temp[i][j] = Block[blockID][i][j];
				game_array[i + b_row][j + b_clo] = temp[i][j];
			}
		}
	}
	blockID = new_blockID;
	Change(blockID);
}

//消除满行
void RemoveRow()
{
	int i = 0,
		j = 0;
	int row = 0;
	int rm_flag = 1;
	for (i = ROW - 1; i >= 4; i--)
	{
		for (j = 0; j < COLUMN; j++)
		{
			if (game_array[i][j] == 0)
			{
				rm_flag = 0;
				break;
			}
			else
			{
				rm_flag = 1;
			}
		}
		if (rm_flag == 1)
		{
			for (row = i; i >= 4; i--)
			{
				for (j = 0; j < 12; j++)
				{
					game_array[i][j] = 0;
					game_array[i][j] = game_array[i - 1][j];
				}
			}
			i = ROW;
			grade += 10;
		}
	}
	SetLevel();
	DisplaySpace();
}

//设置等级
void SetLevel()
{
	switch (grade)
	{
	case 100:
		level = 1;
		dTime = 480;
		break;
	case 200:
		level = 2;
		dTime = 450;
		break;
	case 300:
		level = 3;
		dTime = 400;
		break;
	case 400:
		level = 4;
		dTime = 300;
		break;
	case 500:
		level = 5;
		dTime = 200;
		break;
	}
	/*if (grade = 100)
	{
		level = 1;
		dTime -= 20;
	}
	else if (grade = 200)
	{
		level = 2;
		dTime -= 30;
	}
	else if (grade = 300)
	{
		level = 3;
		dTime -= 50;
	}
	else if (grade = 400)
	{
		level = 4;
		dTime -= 100;
	}
	else if (grade = 500)
	{
		level = 5;
		dTime -= 200;
	}*/
}

//判断游戏是否结束
void IsOver()
{
	int i = 0;
	for (i = 0; i < COLUMN; i++)
	{
		if (game_array[4][i] == 2)
		{
			GameOver();
			memset(game_array, 0, sizeof(game_array));
			KillTimer(NULL, id_1);
			KillTimer(NULL, id_3);
		}
	}
}

//游戏结束
void GameOver()
{
	int CHOICE = 1;
	int cache = 0;
	char key;
	system("cls");

	while (1)
	{
		if (cache == 0)
		{
			system("cls");
			SetXY(23, 7);
			SetConsoleTextAttribute(out, 0x0F);
			printf("Game Over!   Grade:%d   Level:%d", grade, level);
		}
		if (CHOICE != cache)
		{
			if (CHOICE == 1)
			{
				SetXY(32, 9);
				SetConsoleTextAttribute(out, 0xF0);
				printf("PLAY AGAIN");
				SetXY(32, 10);
				SetConsoleTextAttribute(out, 0x0F);
				printf("EXIT");
			}
			else if (CHOICE == 2)
			{
				SetXY(32, 9);
				SetConsoleTextAttribute(out, 0x0F);
				printf("START GAME");
				SetXY(32, 10);
				SetConsoleTextAttribute(out, 0xF0);
				printf("EXIT");
			}
			cache = CHOICE;
		}
		SetConsoleTextAttribute(out, 0x0F);
		if (_kbhit()) //检测按键
		{
			key = _getch();
			if (key == 72 || key == 'w' || key == 'W')
			{
				CHOICE--;
				if (CHOICE == 0)
				{
					CHOICE = 2;
				}
			}
			else if (key == 80 || key == 's' || key == 'S')
			{
				CHOICE++;
				if (CHOICE == 3)
				{
					CHOICE = 1;
				}
			}
			else if (key == '\r' || key == 13)
			{
				if (CHOICE == 1)
				{
					StartGame();
				}
				else if (CHOICE == 2)
				{
					exit(0);
				}
				break;
			}
			//cache = 0;
		}
	}
}

//定时器回调函数
VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	if (idEvent == id_1)
	{
		Down();
		GameSpace();
	}
	if (idEvent == id_2)
	{
		SpeedUp();
	}
	if (idEvent == id_3)
	{
		Contrl();
	}
}

//主函数
int main()
{
	MSG msg;
	out = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTitleA("菜鸟方块 Ver:1.0");
	system("cls");
	Display();
	
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}
