#pragma once 

#include "global.h"

// ����ǰ��ɫ
#define F_BLUE     FOREGROUND_BLUE      // ����
#define F_H_BLUE   0x0001|0x0008        // ����
#define F_GREEN    0x0002               // ����
#define F_H_GREEN  0x0002|0x0008        // ����
#define F_RED      0x0004               // ���
#define F_H_RED    0x0004|0x0008        // ����
#define F_YELLOW   0x0002|0x0004        // ���
#define F_H_YELLOW 0x0002|0x0004|0x0008 // ����
#define F_PURPLE   0x0001|0x0004        // ����
#define F_H_PURPLE 0x0001|0x0004|0x0008 // ����
#define F_CYAN     0x0002|0x0004        // ����
#define F_H_CYAN   0x0002|0x0004|0x0008 // ����
#define F_WHITE    0x0004|0x0002|0x0001
#define F_H_WHITE  0x0004|0x0002|0x0001|0x0008

// ���ֱ���ɫ
#define B_BLUE     BACKGROUND_BLUE      // ����
#define B_H_BLUE   0x0010|0x0080        // ����
#define B_GREEN    0x0020               // ����
#define B_H_GREEN  0x0020|0x0080        // ����
#define B_RED      0x0040               // ���
#define B_H_RED    0x0040|0x0080        // ����
#define B_YELLOW   0x0020|0x0040        // ���
#define B_H_YELLOW 0x0020|0x0040|0x0080 // ����
#define B_PURPLE   0x0010|0x0040        // ����
#define B_H_PURPLE 0x0010|0x0040|0x0080 // ����
#define B_CYAN     0x0020|0x0040        // ����
#define B_H_CYAN   0x0020|0x0040|0x0080 // ����
#define B_WHITE    0x0010|0x0020|0x0040 // ��ɫ
#define B_H_WHITE  0x0010|0x0020|0x0040|0x0080 //����

//����
#define LEFT 0
#define UP 1
#define RIGHT 2
#define DOWN 3

//ͼƬ����
#define WALL__      0x00  // "  " �յ��������ƻ���  ��ͨ����				
#define WALL_A      0x01  // "��" ���֣������ƻ���  ��ͨ������������	
#define WALL_B      0x02  // "�d" ��ǽ��  ���ƻ�, ����ͨ������ǽ���ƻ���̬��
#define WALL_C      0x03  // "��" ��ǽ��  ���ƻ�������ͨ����		
#define WALL_D      0x04  // "��" ��ǽ (�����ƻ�, ����ͨ��)
#define WALL_E      0x05  // "��" �����������ƻ�,   �ӵ���ͨ����̹�˲���ͨ����
#define WALL_F      0x06  // "��" ���ӣ������ƻ���  ��ʰȡ�������ԣ�

#define TANK_A      0x00  //  "  "
#define TANK_B      0x01  //  "��"
#define TANK_C      0x02  //  "��"
#define TANK_D      0x03  //  "��"
#define TANK_E      0x04  //  "��"

#define BLOOD_0     0x0   //  "  "
#define BLOOD_1     0x1	  //  "��"
#define BLOOD_2     0x2   //  "��"
#define BLOOD_3     0x3   //  "��"
#define BLOOD_4     0x4   //  "��"
#define BLOOD_5     0x5   //  "��"
#define BLOOD_6     0x6   //  "��"
#define BLOOD_7     0x7	  //  "��"
#define BLOOD_8     0x8   //  "��"
#define BLOOD_9     0x9   //  "��"

#define MISSIL_0    0x0	  //  "  "
#define MISSIL_1    0x1	  //  "��"
#define MISSIL_2    0x2   //  "��"
#define MISSIL_3    0x3   //  "��"
#define MISSIL_4    0x4   //  "��"
#define MISSIL_5    0x5   //  "��"


//ͼƬ��
extern char *wall_pic[];             //ǽ���ͼƬ��
extern int  wall_color[];             //ǽ�����ɫ��
extern byte tank_style[][4][3][3];     //̹�˵���ʽ��(����)(ÿ����ʽ��̹�������������ĸ����ͣ�ÿ��������Ҫ9��������)
extern char *tank_pic[];             //̹�˵�ͼƬ��
extern char *blood_pic[];            //Ѫ����ͼƬ��
extern char *missile_pic[];          //�ӵ���ͼƬ��
extern byte tank_style[2][4][3][3]; //̹�˵���ʽ