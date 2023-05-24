#include<stdio.h>
#include<time.h>
#include<easyx.h>
#include<graphics.h>
#include <Windows.h>
#include<mmsystem.h>//添加音乐
#pragma comment(lib,"winmm.lib")
#define ROW 10
#define COL 10
#define MINE_NUM 10//雷数
#define GRID_W 40//格子宽度
int map[ROW][COL];
IMAGE img[12];//保存所有的图片
int cnt = 0;//当前点开非雷格数
void playMusic(const char fileName[])
{
	char str[50] = "";
	sprintf_s(str, "close ./image/%s", fileName);
	mciSendString(str, 0, 0, 0);
	sprintf_s(str, "open ./image/%s", fileName);
	mciSendString(str, 0, 0, 0);
	sprintf_s(str, "play ./image/%s", fileName);
	mciSendString(str, 0, 0, 0);

}
//初始化数据
void gameInit()
{
	playMusic("begin.mp3");
	//设置随机数种子
	srand((unsigned)time(NULL));
	//数组中随机生成10个-1表示地雷
	for (int i = 0; i < MINE_NUM;)
	{
		//随机生成下标
		int r = rand() % ROW;
		int c = rand() % COL;
		if (map[r][c] == 0)
		{
			map[r][c] = -1;
			i++;
		}
	}
	//以-1为中心，九宫格除-1外的所有数据+1
	for (int i = 0; i < ROW; i++)
	{
		for (int k = 0; k < COL; k++)
		{
			if (map[i][k] == -1)
			{
				//遍历九宫格
				for (int r = i - 1; r <= i + 1; r++)
				{
					for (int c = k - 1; c <= k + 1; c++)
					{
						if ((r >= 0 && r < ROW && c >= 0 && c < COL) && map[r][c] != -1)
						{
							map[r][c]++;
						}
					}
				}
			}
		}
	}
	char str[50] = "";
	for (int i = 0; i < 12; i++)
	{
		sprintf_s(str, "./image/%d.png", i);
		loadimage(&img[i], str, GRID_W, GRID_W);
	}

	//数据加密  -1~8->所有数据+20  19~28
	for (int i = 0; i < ROW; i++)
	{
		for (int k = 0; k < COL; k++)
		{
			map[i][k] += 20;
		}

	}
}
//界面绘制
void gameDraw()
{
	for (int i = 0; i < ROW; i++)//i->y
	{
		for (int k = 0; k < COL; k++)//k->x
		{
			if (map[i][k] >= 0 && map[i][k] <= 8)
			{
				putimage(k * GRID_W, i * GRID_W, &img[map[i][k]]);
			}
			else if (map[i][k] == -1)
			{
				putimage(k * GRID_W, i * GRID_W, &img[9]);
			}
			else if (map[i][k] >= 19 && map[i][k] <= 28)
			{
				putimage(k * GRID_W, i * GRID_W, &img[10]);
			}
			else if (map[i][k] > 28)
			{
				putimage(k * GRID_W, i * GRID_W, &img[11]);
			}

		}
	}
}
void show();
void openNULL(int row, int col);
void jude(int row, int col);
//通过鼠标点击打开格子
void mouseEvent()
{
	MOUSEMSG msg = GetMouseMsg();//获取鼠标消息
	//把鼠标当前点击的坐标转成二维数组的下标
	int row = msg.y / GRID_W;
	int col = msg.x / GRID_W;
	//判断是左/右键点击
	if (msg.uMsg == WM_LBUTTONDOWN)
	{
		playMusic("great.mp3");
		if (map[row][col] > 19)//还没打开，就打开格子
		{
			map[row][col] -= 20;
			openNULL(row, col);
			cnt++;
			//连续打开空白格，播放音效
			if (map[row][col] == 0)
			{
				playMusic("left_fire.mp3");
			}
			jude(row, col);
		}
		else if (map[row][col] == 19)
		{
			playMusic("end1.mp3");
			if (MessageBox(GetHWnd(), "真可惜，不幸踩雷了\n还要再来一次吗？", "禅雅塔说：", MB_RETRYCANCEL) == IDRETRY)
			{
				playMusic("revive.mp3");
			}
			else
			{
				closegraph();
			}
		}

		show();
	}
	else if (msg.uMsg == WM_RBUTTONDOWN)
	{
		//随机播放音乐
		{
			srand((unsigned int)time(NULL));
			int ret1 = rand() % 3 + 1;//生成1~3的随机数
			if (ret1 == 1)
			{
				playMusic("no.mp3");
			}
			else if (ret1 == 2)
			{
				playMusic("understand.mp3");
			}
			else
			{
				playMusic("open.mp3");
			}
		}
		if (map[row][col] >= 19 && map[row][col] <= 28)
		{
			map[row][col] += 20;
		}
		else if (map[row][col] > 28)//已标记 那么就取消标记
		{
			map[row][col] -= 20;
		}
		show();
	}

}
//若点到空格，炸开一片
void openNULL(int row, int col)
{
	if (map[row][col] == 0)
	{
		for (int i = row - 1; i <= row + 1; i++)
		{
			for (int k = col - 1; k <= col + 1; k++)
			{
				//若为空格或数字(不是雷)则直接打开
				if ((i >= 0 && i < ROW && k >= 0 && k < COL) && map[i][k] > 19)
				{
					cnt++;
					map[i][k] -= 20;
					openNULL(i, k);
				}
			}
		}
	}
}
//判断胜负
void jude(int row, int col)
{
	if (cnt == (ROW * ROW - MINE_NUM))
	{
		playMusic("end2.mp3");
		MessageBox(GetHWnd(), "      在你的帮助下我们终于击败了邪恶的弗兰狂斯鼠博士\n拯救了香巴里寺院！\n\t\t\t    ——made by Jackie", "禅雅塔说：", MB_OK);
	}
}
void show()
{
	system("cls");
	for (int i = 0; i < ROW; i++)
	{
		for (int k = 0; k < COL; k++)
		{
			printf("%3d", map[i][k]);
		}
		printf("\n");
	}
}
int main()
{
	//创建图形窗口
	initgraph(COL * GRID_W, ROW * GRID_W);
	gameInit();
	show();
	while (1)
	{
		gameDraw();
		mouseEvent();
	}
	return 0;
}