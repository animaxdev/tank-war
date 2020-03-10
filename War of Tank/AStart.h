#pragma once

#include <vector>
#include <list>
#include "Position.h"
#include "Map.h"
using std::vector;
using std::list;

class AStart
{
private:
	/******��ͼ��Ϣ*****/
	int m_width;
	int m_height;
	Map *m_map ;
	/*********����Ϣ**********/
	COORD m_dest;
	/***********������Ϣ**************/
	vector<Position *> m_extendList; //�����չ�ĵ�
	vector<Position *> m_visitedList; //��ŷ��ʹ��ĵ�

public:
	AStart(Map *map , int width, int height);
	void setElement(const COORD &start, const COORD &dest);
	int findPath(COORD &src);
	~AStart();
private:
	int _checkPos(COORD &coord, int weight);//�жϸ�������Ƿ������չ
	void _extNext();//��չ�µĵ�
	int _findNext();//�ҵ�����չ����������С����Ϊ��һ���µ���չ�Ļ���
	int _findPath();//��ȡ·��
};

