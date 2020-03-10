#include "ConsoleTool.h"
#include <windows.h>
#pragma comment(lib, "winmm.lib")
#include<io.h>
#include <vector>
using std::vector;

#define FILE_MAX 12 //�ļ��������

//���ڴ�С
#define WINDOWS_WIDTH 66
#define WINDOWS_HIGH  42
#define TITLE L"War of Tank"

HANDLE g_hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);    //��ȡ��׼������
HANDLE g_hStdIn = GetStdHandle(STD_INPUT_HANDLE);    //��ȡ��׼������

static bool setWindowSize(int width, int high)
{
	width = width * 2;  //�ѿ��ַ����꣬��Ϊխ�ַ�����

	//1. ��ȡ������̨���ڴ�С
	COORD pos = GetLargestConsoleWindowSize(g_hStdOut);
	if (pos.Y <= high || pos.X <= width)
	{
		printf("�����õĴ���̫���ˣ�װ���£� \n");
		printf("�����ý�С��������¿�ʼ~ \n");
		return false;
	}
	COORD bufferSize = { pos.X, pos.Y };  //��󻺳�ȡ��С
	pos.X -= 1;	pos.Y -= 1;              //��󴰿ڴ�С

	//2. ���û�������С(������Ϊ����Ա����ô���)
	if (!SetConsoleScreenBufferSize(g_hStdOut, bufferSize)) {
		printf("���û�������Сʧ�� \n");
		return false;
	}

	//3. ȷ�����ڵ����մ�С
	width = width > pos.X ? pos.X : width;
	high = high > pos.Y ? pos.Y : high;

	//4. ���ô��ڴ�С
	SMALL_RECT windowSize = { 0, 0, (short)width, (short)high };
	if (!SetConsoleWindowInfo(g_hStdOut, true, &windowSize)) {
		printf("���ô��ڴ�Сʧ�� \n");
		return false;
	}
	//5. ���û�������С
	bufferSize = { (short)width + 1, (short)high + 1 };
	if (!SetConsoleScreenBufferSize(g_hStdOut, bufferSize)) {
		printf("���û�������Сʧ�� \n");
		return false;
	}

	return true;
}//���ô��ڴ�С

void initConsole()
{
	srand((unsigned int)time(NULL));
	SetConsoleTitle(TITLE);
	if (!setWindowSize(WINDOWS_WIDTH, WINDOWS_HIGH))
	{
		system("pause");
		exit(-1);
	}
}

void clearConsle()
{
	SetConsoleTextAttribute(g_hStdOut, F_WHITE); //����Ϊ�ڰ�ɫ
	system("CLS");
} //��տ���̨

void writeChar(int row, int col, const char* pszChar, WORD wArr)
{
	// ���ù������
	CONSOLE_CURSOR_INFO cci;                    //����̨�����Ϣ�ṹ����
	cci.dwSize = 1;                             //����С
	cci.bVisible = FALSE;                       //�Ƿ���ʾ���
	COORD loc;                                  //����ṹ����
	loc.X = col * 2;                            //��ȱ���*2
	loc.Y = row;                                //�߶�
	SetConsoleCursorInfo(g_hStdOut, &cci);      //����ָ������̨��Ļ����������С�Ϳɼ���
	SetConsoleCursorPosition(g_hStdOut, loc);   //����ָ������̨��Ļ�������й���λ��
	SetConsoleTextAttribute(g_hStdOut, wArr);   //������ɫ
	//�����Ժ������ú�������ı�����Ӱ��
	printf(pszChar);                            //��ӡ�����ַ�ռ�����ֽ�
}

//��������
byte getKeyCode()
{
	INPUT_RECORD inRec;        //���������¼��ṹ��  
	DWORD res;      //���巵�ؼ�¼  
	while (true)
	{
		ReadConsoleInput(g_hStdIn, &inRec, 1, &res);
		if (inRec.EventType == KEY_EVENT && inRec.Event.KeyEvent.bKeyDown)
			return inRec.Event.KeyEvent.wVirtualKeyCode;
		
	}
}//��ȡ����̨�����¼�����

//��ʾ�˵�
byte showMenu(byte COUNT, char**menuArr)
{
	int row = (WINDOWS_HIGH - 5) / 2;
	int col = (WINDOWS_WIDTH - strlen("--------------") / 2) / 2;
	byte option = 0;

	clearConsle();
	writeChar(row - 1, col - 2, "---------------------", F_RED | B_GREEN);
	for (int i = 0; i < COUNT; i++)
	{
		writeChar(row + i, col, menuArr[i], F_WHITE);
	}
	writeChar(row + option, col - 2, "��", F_RED | B_H_WHITE);
	writeChar(row + COUNT, col - 2, "---------------------", F_RED | B_GREEN);

	while (true)
	{
		switch (getKeyCode())
		{
		case VK_UP:
			if (option >  0)
			{
				writeChar(row + option , col - 2, "��", F_RED);
				option--;
				writeChar(row + option, col - 2, "��", F_RED | B_H_WHITE);
			}
			break;
		case VK_DOWN:
			if (option < COUNT-1)
			{
				writeChar(row + option, col - 2, "��", F_RED);
				option++;
				writeChar(row + option, col - 2, "��", F_RED | B_H_WHITE);
			}
			break;

		case VK_RETURN:
			clearConsle();
			return option;
			break;
		}
	}
}

//������
byte getMousePos(COORD &pos)
{
	INPUT_RECORD inRec;        //���������¼��ṹ��  
	DWORD res;      //���巵�ؼ�¼  
	SetConsoleMode(g_hStdIn, ENABLE_MOUSE_INPUT);
	while (true)
	{
		ReadConsoleInput(g_hStdIn, &inRec, 1, &res);
		if (inRec.EventType == MOUSE_EVENT)
		{
			pos = inRec.Event.MouseEvent.dwMousePosition;
			pos.X /= 2;
			if (inRec.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
				return 1; //�������
			else if (inRec.Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
				return 2; //�Ҽ�����
			return 0;
		}
	}
} //��ȡ�������

//ѡ���б�
void showChoose(int top, int edge, int count, char **chooseArr, WORD color, int choose)
{
	if (choose <0 || choose > count - 1)
	{
		return;
	}
	writeChar(top-1, edge, "-------------------------------------------------", F_RED | B_GREEN);
	int i;
	for ( i = 0; i < count; i++)
	{
		if (i == choose)
		{
			writeChar(top + i, edge, chooseArr[i], color | B_RED);
			continue;
		}
		writeChar(top + i, edge, chooseArr[i], color);
	}
	writeChar(top+i, edge, "-------------------------------------------------", F_RED | B_GREEN);
}

//��ʾ��Ϣ��0Ϊ�ײ���>0Ϊ���
void showMessage(char *message, WORD color, unsigned int falg)
{
	if (falg == BOTTOM)
	{
		writeChar(WINDOWS_HIGH - 1, WINDOWS_WIDTH / 2 - 20, "                                                      ", color);
		writeChar(WINDOWS_HIGH - 1, WINDOWS_WIDTH / 2 - 20, message, color);
	}
	else
	{
		writeChar(falg, WINDOWS_WIDTH - 24, "                                              ", color);
		writeChar(falg, WINDOWS_WIDTH - 24, message, color);
	}
		
}//����֪ͨ

//��ʾ�Ի���
void showDialog(char *str) 
{
	COORD pos;
	int row = (WINDOWS_HIGH - 10);
	int col = (WINDOWS_WIDTH - 20);
	//�����ͣ����
	writeChar(row + 0, col,  "=========================", F_RED | B_WHITE);
	writeChar(row + 1, col,  "                         ", F_RED | B_WHITE);
	writeChar(row + 2, col,/*"     ��  Ϸ   ��  ͣ     "*/str, F_RED | B_WHITE);
	writeChar(row + 3, col,  "                         ", F_RED | B_WHITE);
	writeChar(row + 4, col,  "=========���ȷ��========", F_RED | B_WHITE);
	while (true)
	{
		if (getMousePos(pos) == 1)
		{
			if (pos.X >= col && pos.X <= col + 12 && pos.Y >= row && pos.Y <= row + 4)
				break;
		}
	}
	writeChar(row + 0, col, "                         ", F_RED );
	writeChar(row + 1, col, "                         ", F_RED );
	writeChar(row + 2, col, "                         ", F_RED );
	writeChar(row + 3, col, "                         ", F_RED );
	writeChar(row + 4, col, "                         ", F_RED );
}

//��ʾ�ļ���
int showFileMenu(char *dir, char *buf, size_t size)
{
	//�ļ��洢��Ϣ�ṹ�� 
	struct _finddata_t fileinfo;
	//�����ļ���� 
	long fHandle;
	int row ;
	int col ;
	int count = 0, opt = 0;
	char list[FILE_MAX][20];

	if ((fHandle = _findfirst(dir, &fileinfo)) == -1L)
	{
		showMessage("û�з��ֿ��Զ�ȡ�ĵ�ͼ", F_H_WHITE, 0);
		return -1;
	}
	else
	{
		clearConsle();
		row = (WINDOWS_HIGH - 5) / 2;
		col = (WINDOWS_WIDTH - strlen("----------") / 2) / 2;
		writeChar(row - 1, col - 2, "---------------", F_RED | B_GREEN);
		do{
			if (count < FILE_MAX)
			{
				writeChar(row + count, col, fileinfo.name, F_WHITE);
				memcpy_s(list[count], 20, fileinfo.name, 20);
				count++;
			}
			else
			{
				break;
			}
		} while (_findnext(fHandle, &fileinfo) == 0);
		_findclose(fHandle); 
		writeChar(row + opt, col - 2, "��", F_RED | B_H_WHITE);
		writeChar(row + count, col - 2, "---------------", F_RED | B_GREEN);
		while (true)
		{
			switch (getKeyCode())
			{
			case VK_UP:
				if (opt >  0)
				{
					writeChar(row + opt, col - 2, "��", F_RED);
					opt--;
					writeChar(row + opt, col - 2, "��", F_RED | B_H_WHITE);
				}
				break;
			case VK_DOWN:
				if (opt < count - 1)
				{
					writeChar(row + opt, col - 2, "��", F_RED);
					opt++;
					writeChar(row + opt, col - 2, "��", F_RED | B_H_WHITE);
				}
				break;

			case VK_RETURN:
				clearConsle();
				strcpy_s(buf, size, list[opt]);
			
				return 0;
			}
		}
	}
}

//�������
void Muisc(char *muiscName)
{
	PlaySoundA(muiscName, NULL, SND_ASYNC | SND_NODEFAULT);
}