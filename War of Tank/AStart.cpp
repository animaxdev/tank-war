#include "AStart.h"
#include "Player.h"
#include <iostream>
using namespace std;

AStart::AStart(Map *map, int width, int height)
	:m_width(width), m_height(height), m_map(map), m_dest({ 0, 0 })
{
	if (m_width <= 0 || m_height <= 0 || m_map == nullptr)
	{
		throw "AStart init error";
	}
}

int AStart::_findNext() //��������չ����Ȩ����С�ĵ㲢�����������չ����
{
	if (m_extendList.empty())
	{
		return -1;
	}
	int loss = m_extendList[0]->getLoss();
	int tmp, index = 0;
	for (int i = 1; i < m_extendList.size(); i++) //�ҵ���չ�б���������С�ĵ�
	{
		if (loss > (tmp = m_extendList[i]->getLoss()))
		{
			loss = tmp;
			index = i;
		}
	}
	m_visitedList.push_back(m_extendList[index]);//��������Է����б�
	m_extendList.erase(m_extendList.begin() + index);

	COORD coord;
	m_visitedList.back()->getCOORD(coord);
	for (int row = -1; row<2; row++)
	{
		for (int col = -1; col<2; col++)
		{

			int y = row + m_dest.Y;
			int x = col + m_dest.X;

			if (coord.X == x && coord.Y == y) //����ҵ��յ�ֱ�ӷ���
				return 1;
		}
	}

	return 0;
}

int AStart::_checkPos(COORD &coord, int weight) //�жϸ�������Ƿ������չ
{
	for (int i = 0; i < m_visitedList.size(); i++) //�жϸõ��Ƿ��Ѿ����ʹ�
	{
		if (m_visitedList[i]->comperTo(coord))
			return -1;
	}
	if (coord.X < 1 || coord.Y < 1 || coord.X > m_width - 1 || coord.Y > m_height - 1) //�ж�Ҫ��չ�������Ƿ�Խ��
	{
		return -1;
	}

	int tmp;
	for (int row = -1; row<2; row++)
	{
		for (int col = -1; col<2; col++)
		{

			int y = row + coord.Y;
			int x = col + coord.X;

			if ((tmp = (*m_map)[y][x]) != WALL__ && tmp != WALL_A) //�ж�Ҫ��չ�������Ƿ����ϰ���
			{
				return -1;
			}
		}
	}

	
	

	for (int i = 0; i < m_extendList.size(); i++)
	{
		if (m_extendList[i]->comperTo(coord))
		{
			m_extendList[i]->updateLoss(weight);
			return 1; //�������չ�ĵ����иõ�����¸õ����Ϣ
		}
	}
	

	return 0;
}

void AStart::_extNext() //��չ��
{
	COORD tmp, coord;
	Position *pos = m_visitedList.back();
	pos->getCOORD(tmp);
	int weight = pos->getWeigth();
	//UP
	coord.X = tmp.X;
	coord.Y = tmp.Y - 1;
	if(!_checkPos(coord, weight))
		m_extendList.push_back(new Position(coord, weight, Position::calcLoss(coord, m_dest, weight), pos));
	//DOWN
	coord.X = tmp.X;
	coord.Y = tmp.Y + 1;
	if (!_checkPos(coord, weight))//Ϊ���ʾ������չ
		m_extendList.push_back(new Position(coord, weight, Position::calcLoss(coord, m_dest, weight), pos));
	//LEFT
	coord.X = tmp.X - 1;
	coord.Y = tmp.Y;
	if (!_checkPos(coord, weight))
		m_extendList.push_back(new Position(coord, weight, Position::calcLoss(coord, m_dest, weight), pos));
	//right
	coord.X = tmp.X + 1;
	coord.Y = tmp.Y;
	if (!_checkPos(coord, weight))
		m_extendList.push_back(new Position(coord, weight, Position::calcLoss(coord, m_dest, weight), pos));
}

int AStart::_findPath()
{
	
	int code;
	while (!(code = _findNext()))//��ȡ��С���ĵ��ѷ��ʵ㣬ͨ���õ���չ
	{
		_extNext();
	}
	return code == -1 ? 1 : 0;
}

/*****************�ⲿ�ӿ�*****************/
void AStart::setElement(const COORD &start, const COORD &dest) //���������յ�
{
	m_extendList.push_back(new Position(start, 0, Position::calcLoss(start, dest, 0), nullptr));
	m_dest = dest;
}


int AStart::findPath(COORD &src)
{
	list<COORD> path;
	int code;
	COORD tmp;
	if (!(code = _findPath()))
	{
		Position *pos = m_visitedList.back();
	do{
		pos->getCOORD(tmp);
		path.push_front(tmp);
	} while (pos = pos->m_prev) ;
	}
	for (int i = 0; i < m_extendList.size(); i++)
		delete m_extendList[i];
	
	for (int i = 0; i < m_visitedList.size(); i++)
		delete m_visitedList[i];

	m_visitedList.clear();
	m_extendList.clear();
	if (path.size() < 2)
		return -1;
	path.pop_front();
	COORD end = path.front();
	if (src.Y - end.Y == 1)
	{
		return UP;
	}
	else if (src.Y - end.Y == -1)
	{
		return DOWN;
	}
	else if (src.X - end.X == 1)
	{
		return LEFT;
	}
	else if (src.X - end.X == -1)
	{
		return RIGHT;
	}

	return -1;
}

AStart::~AStart()
{
}
