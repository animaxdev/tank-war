#include "global.h"
#include "engine.h"
#include <string.h> 
#include <fstream>  
#include <exception>
#include <iostream>
using namespace std;


//�б�λ��
#define TOP 4
#define EDGE MAP_WIDTH+1
//�б�
#define CHOOSE_COUNT 6
#define OPT_COUNT 4 
static char* chooseArr[CHOOSE_COUNT] = { "\"  \" �յ��������ƻ���  ��ͨ����", "\"��\" ���֣������ƻ���  ��ͨ������������" ,"\"�d\" ��ǽ��  ���ƻ�, ����ͨ������ǽ���ƻ���̬��", "\"��\" ��ǽ��  ���ƻ�������ͨ����",  "\"��\" ��ǽ (�����ƻ�, ����ͨ��)", "\"��\" �����������ƻ�,   �ӵ���ͨ����̹�˲���ͨ����" };
char *optArr[OPT_COUNT] = { "�����ͼ", "ʹ�õ�ͼ", "��ȡ��ͼ", "����" };
static void editMAP() //�༭��ͼ
{
	fstream fp ;
	Map *map = new Map();
	COORD pos;
	char buf[20]{};
	int option = 0;
begin:map->drawMap();

	showChoose(TOP, EDGE, CHOOSE_COUNT, chooseArr, F_H_WHITE, option);
	showMessage("���ڱ༭��ͼ������", F_H_WHITE, BOTTOM);
	writeChar(TOP + CHOOSE_COUNT + 3, EDGE+4, "����б�ѡ��ǽ��", F_H_WHITE);
	writeChar(TOP + CHOOSE_COUNT + 4, EDGE + 4, "�����������ʾ�༭�˵�", F_H_WHITE);
	while (true)
	{
		if (getMousePos(pos) == 1)
		{
			if (pos.X > MAP_WIDTH && pos.Y >= TOP && pos.Y < TOP + CHOOSE_COUNT)
			{
				option = pos.Y - TOP;
				showChoose(TOP, EDGE, CHOOSE_COUNT, chooseArr, F_H_WHITE, option);
			}
			else if (pos.X < MAP_WIDTH - 1 && pos.X > 0 && pos.Y > 0 && pos.Y < MAP_HIGH - 1)
			{
				writeChar(pos.Y, pos.X, wall_pic[option], wall_color[option]);
				(*map)[pos.Y][pos.X] = option;
			}
			else if (pos.X > MAP_WIDTH + 4 && pos.Y > 16)
			{
				switch (showMenu(OPT_COUNT, optArr))
				{
				case 0:
					printf("------------------------------------\n");
					printf("�������ͼ��:");
					scanf_s("%s", buf, sizeof buf - 4);
					strcat_s(buf,sizeof buf, ".wtm");
					fp.open(buf, ios::out | ios::binary);
					if (!fp.is_open())
					{
						cout << "fail to open file" << endl;
						cout << "Please check the file name" << endl;
						system("pause");
						goto begin;
					}
					fp.write((char*)map, sizeof(Map));
					fp.close();
					cout << "save success" << endl;
					system("pause");
					goto begin;
					break;
				case 1:
					g_Game->setMap(map);
					showMessage("��ͼ��ȡ�ɹ���", F_H_WHITE, BOTTOM);
					system("pause");
					goto end;
				case 2:
					if (showFileMenu("./*.wtm", buf, sizeof buf))
					{
						showMessage("��ȡʧ��!(�����Ƿ��е�ͼ)", F_H_WHITE, BOTTOM);
						system("pause");
						goto begin;
					}

					fp.open(buf, ios::in | ios::binary);
					fp.read((char*)map, sizeof(Map));
					fp.close();
					goto begin;
					
					break;
				case 3:
					goto begin;
					break;
				}
			}
		}
		
		sprintf_s(buf, "< X = %d�� Y = %d >", pos.X, pos.Y);
		writeChar(TOP - 3, EDGE, buf, F_H_WHITE);
	}
	end:;
}

//��ȡһ������
static int getNumber(void)
{
	int tmp;
	while (scanf_s("%d", &tmp) != 1)
	{
		while (getchar() != '\n');
		printf("error\n");
	}
	while (getchar() != '\n');
	return tmp;
}
#define NPCM 6 //���ϵо������
static void editNPC()
{
	int max, totle;
	while (true)
	{
		printf("------------------------------------\n");
		printf("������о�������:");
		max = getNumber();
		printf("�����볡�ϵо�������:");
		while ((totle = getNumber()) > NPCM)
			printf("error: not > %d\n", NPCM);
		printf("ENTER TO CONTINUE OR L TO LOOP\n");
		switch (getKeyCode())
		{
		case VK_RETURN:
			goto end;
		case 'L':
			break;
		}
	}
end:g_Game->setOption(max, totle);
}


#define ATTR_COUNT 2
char *attrArr[ATTR_COUNT]{"�ؼ�(Ѫ:��,��:��)", "���(Ѫ:��,��:��)"};
#define TYPE_COUNT 4
char *typeArr[TYPE_COUNT]{"������", "��׼��Ļ", "ǿ����", "��ɢ��"};
static void editPlayer()
{
	int blood;
	int speed; 
	unsigned int color;
	int type;
	if (showMenu(ATTR_COUNT, attrArr))
	{
		blood = 8;
		speed = 44;
		color = F_H_YELLOW;
	}
	else
	{
		blood = 10;
		speed = 60;
		color = F_H_PURPLE;
	}
	type = showMenu(TYPE_COUNT, typeArr);

	Player *p = new Player(color, blood,speed,type);
	g_Game->setPlayer(p);
}

//��ʾ�༭�˵�
#define MENU_COUNT 4
static char *menuArr[MENU_COUNT]{"�༭��ͼ", "�����з�", "������װ", "����"};
static void(*funArr[MENU_COUNT])(void) { editMAP, editNPC, editPlayer};
Engine* Edit()
{
	if (!g_Game)
		g_Game = new Engine();
	int index;
	while ((index = showMenu(MENU_COUNT, menuArr)) != MENU_COUNT -1)
	{
		funArr[index]();
	}
	
	return g_Game;
}
