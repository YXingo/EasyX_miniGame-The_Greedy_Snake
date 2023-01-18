#include <iostream>
#include <vector>
#include <conio.h>
#include <easyx.h>
using namespace std;

class Node
{
public:
	int x;
	int y;

	Node(int X, int Y)
	{
		x = X; y = Y;
	}

	void paintNode(int NODE_WIDTH)
	{
		fillrectangle(x * NODE_WIDTH, y * NODE_WIDTH, (x + 1) * NODE_WIDTH, (y + 1) * NODE_WIDTH);
	}
};

int NODE_WIDTH = 40;
int speed = 4;
vector<Node> v_Snake;
int x = 10;
int y = 7;
Node food(x, y);

/******************************************* 以上：类的定义与变量的定义 *******************************************/

void paintGrid(int NODE_WIDTH)	//绘制坐标网格
{
	for (int i = 1; i < 20; i++)
	{
		line(NODE_WIDTH * i, 0, NODE_WIDTH * i, 600);
	}
	for (int i = 1; i < 15; i++)
	{
		line(0, NODE_WIDTH * i, 800, NODE_WIDTH * i);
	}
}

void paintSnake(vector<Node> v_Snake, int NODE_WIDTH)	//绘制蛇
{
	for (vector<Node>::iterator it = v_Snake.begin(); it != v_Snake.end(); it++)
	{
		(*it).paintNode(NODE_WIDTH);
	}
}

char judgeDirection(vector<Node> v_Snake)	//获取蛇无法转的方向（蛇不能朝着与身体相反的方向转向）
{
	int x0 = v_Snake[0].x;
	int y0 = v_Snake[0].y;
	int x1 = v_Snake[1].x;
	int y1 = v_Snake[1].y;

	if (x0 == x1)
	{
		if (y0 > y1)
		{
			return 'w';
		}
		else if (y0 < y1)
		{
			return 's';
		}
	}
	else
	{
		if (x0 > x1)
		{
			return 'a';
		}
		else if (x0 < x1)
		{
			return 'd';
		}
	}
}

void changeSnake(vector<Node>& v_Snake, char direction)	//改变蛇的身体
{
	Node snake_Head(0, 0);	//创建即将插入的头部

	if (direction == 'w')
	{
		snake_Head.x = v_Snake[0].x;
		snake_Head.y = v_Snake[0].y - 1;
	}
	else if (direction == 'a')
	{
		snake_Head.x = v_Snake[0].x - 1;
		snake_Head.y = v_Snake[0].y;
	}
	else if (direction == 's')
	{
		snake_Head.x = v_Snake[0].x;
		snake_Head.y = v_Snake[0].y + 1;
	}
	else if (direction == 'd')
	{
		snake_Head.x = v_Snake[0].x + 1;
		snake_Head.y = v_Snake[0].y;
	}

	v_Snake.pop_back();	//删除蛇末尾的一个节点
	v_Snake.insert(v_Snake.begin(), snake_Head);	//插入蛇头部
}

void moveSnake(vector<Node>& v_Snake, int NODE_WIDTH, char direction)	//令蛇移动
{
	changeSnake(v_Snake, direction);
	cleardevice();
	paintGrid(NODE_WIDTH);
	paintSnake(v_Snake, NODE_WIDTH);
}

bool judgeFoodPosition(int food_x, int food_y, vector<Node> v_Snake) //判断食物位置是否与蛇头位置重合（蛇是否吃到食物）
{
	for (vector<Node>::iterator it = v_Snake.begin(); it != v_Snake.end(); it++)
	{
		if (food_x == (*it).x && food_y == (*it).y)
		{
			return false;
		}
	}
	return true;
}

bool judgeGameOver(vector<Node> v_Snake)	//判断游戏是否结束
{
	for (vector<Node>::iterator it = v_Snake.begin() + 1; it != v_Snake.end(); it++)
	{
		if (v_Snake[0].x == (*it).x && v_Snake[0].y == (*it).y)
		{
			return true;
		}
	}

	if (v_Snake[0].x > 19 || v_Snake[0].x < 0 || v_Snake[0].y>14 || v_Snake[0].y < 0)
	{
		return true;
	}

	return false;
}

void GameInitialization()	//游戏初始化
{
	cleardevice();
	NODE_WIDTH = 40;
	speed = 4;
	food.x = 10, food.y = 7;	//将变量初始化

	paintGrid(NODE_WIDTH);	//画网格
	v_Snake.clear();

	for (int i = 4; i >= 0; i--)	//创建一个含有五个节点的Node数组来表示初始蛇
	{
		Node node(i + 1, 7);	//从头部往尾部插节点
		v_Snake.push_back(node);
	}
	paintSnake(v_Snake, NODE_WIDTH);	//画蛇

	setfillcolor(YELLOW);
	food.paintNode(NODE_WIDTH);	//画食物

	FlushBatchDraw();
}

void GameStart()	//游戏主体进程
{
	char move_direction;
	while (1)
	{
		move_direction = _getch();
		if (move_direction != judgeDirection(v_Snake))
		{
			break;
		}
	}	//用户首先发出指令 决定起始运动的方向

	while (1)
	{
		setfillcolor(WHITE);
		moveSnake(v_Snake, NODE_WIDTH, move_direction);

		if (judgeGameOver(v_Snake) == true)	//如果碰到墙壁或者蛇身则游戏结束
		{
			return;
		}

		if (v_Snake[0].x == food.x && v_Snake[0].y == food.y)	//如果蛇头碰到食物则判定为吃到食物
		{
			v_Snake.insert(v_Snake.begin(), food);	//将食物替换为蛇的头部 以实现蛇吃到食物后身体增长的效果
			speed += 2;
			while (judgeFoodPosition(food.x, food.y, v_Snake) == false)	//避免食物刷新到蛇的任意一节身体上
			{
				food.x = rand() % (600 / NODE_WIDTH);
				food.y = rand() % (400 / NODE_WIDTH);
			}
			setfillcolor(YELLOW);
			food.paintNode(NODE_WIDTH);
		}
		else
		{
			setfillcolor(YELLOW);
			food.paintNode(NODE_WIDTH);
		}

		FlushBatchDraw();
		Sleep(1000 / speed);
		if (_kbhit() != 0)
		{
			char ch = _getch();
			if (ch != judgeDirection(v_Snake))
			{
				move_direction = ch;
			}
		}
	}	//如果用户再次发出指令 则先判断指令是否合理 若合理 则进行方向改变后的运动 否则保持原运动
}

int main()
{
	initgraph(800, 600);
	setbkcolor(RGB(164, 225, 202));
	cleardevice();
	BeginBatchDraw();

	srand(time(NULL));	//使用当前时间作为随机数种子

	GameInitialization();
	while (1)
	{
		GameStart();
		GameInitialization();
	}

	EndBatchDraw();
	getchar();
	closegraph();
}