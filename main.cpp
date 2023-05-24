#include<stdio.h>
#include<time.h>
#include<easyx.h>
#include<graphics.h>
#include <Windows.h>
#include<mmsystem.h>//�������
#pragma comment(lib,"winmm.lib")
#define ROW 10
#define COL 10
#define MINE_NUM 10//����
#define GRID_W 40//���ӿ��
int map[ROW][COL];
IMAGE img[12];//�������е�ͼƬ
int cnt = 0;//��ǰ�㿪���׸���
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
//��ʼ������
void gameInit()
{
	playMusic("begin.mp3");
	//�������������
	srand((unsigned)time(NULL));
	//�������������10��-1��ʾ����
	for (int i = 0; i < MINE_NUM;)
	{
		//��������±�
		int r = rand() % ROW;
		int c = rand() % COL;
		if (map[r][c] == 0)
		{
			map[r][c] = -1;
			i++;
		}
	}
	//��-1Ϊ���ģ��Ź����-1�����������+1
	for (int i = 0; i < ROW; i++)
	{
		for (int k = 0; k < COL; k++)
		{
			if (map[i][k] == -1)
			{
				//�����Ź���
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

	//���ݼ���  -1~8->��������+20  19~28
	for (int i = 0; i < ROW; i++)
	{
		for (int k = 0; k < COL; k++)
		{
			map[i][k] += 20;
		}

	}
}
//�������
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
//ͨ��������򿪸���
void mouseEvent()
{
	MOUSEMSG msg = GetMouseMsg();//��ȡ�����Ϣ
	//����굱ǰ���������ת�ɶ�ά������±�
	int row = msg.y / GRID_W;
	int col = msg.x / GRID_W;
	//�ж�����/�Ҽ����
	if (msg.uMsg == WM_LBUTTONDOWN)
	{
		playMusic("great.mp3");
		if (map[row][col] > 19)//��û�򿪣��ʹ򿪸���
		{
			map[row][col] -= 20;
			openNULL(row, col);
			cnt++;
			//�����򿪿հ׸񣬲�����Ч
			if (map[row][col] == 0)
			{
				playMusic("left_fire.mp3");
			}
			jude(row, col);
		}
		else if (map[row][col] == 19)
		{
			playMusic("end1.mp3");
			if (MessageBox(GetHWnd(), "���ϧ�����Ҳ�����\n��Ҫ����һ����", "������˵��", MB_RETRYCANCEL) == IDRETRY)
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
		//�����������
		{
			srand((unsigned int)time(NULL));
			int ret1 = rand() % 3 + 1;//����1~3�������
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
		else if (map[row][col] > 28)//�ѱ�� ��ô��ȡ�����
		{
			map[row][col] -= 20;
		}
		show();
	}

}
//���㵽�ո�ը��һƬ
void openNULL(int row, int col)
{
	if (map[row][col] == 0)
	{
		for (int i = row - 1; i <= row + 1; i++)
		{
			for (int k = col - 1; k <= col + 1; k++)
			{
				//��Ϊ�ո������(������)��ֱ�Ӵ�
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
//�ж�ʤ��
void jude(int row, int col)
{
	if (cnt == (ROW * ROW - MINE_NUM))
	{
		playMusic("end2.mp3");
		MessageBox(GetHWnd(), "      ����İ������������ڻ�����а��ĸ�����˹��ʿ\n�������������Ժ��\n\t\t\t    ����made by Jackie", "������˵��", MB_OK);
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
	//����ͼ�δ���
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