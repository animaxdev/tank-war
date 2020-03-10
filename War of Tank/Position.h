#pragma once
#include <Windows.h>

class Position
	:private COORD
{
private:
	int m_loss; //�õ�ֱ�ӵ����յ����С����
	int m_weight; //���õ��Ȩ��

public:
	Position::Position(const COORD& pos, int m_weight, int loss, Position *prev);
	/****************�ⲿ�ӿ�*************/
	void getCOORD(COORD &coord);
	int getLoss();
	void updateLoss(int weight);
	int getWeigth();
	bool comperTo(const COORD &coord);

	static int calcLoss(const COORD& src, const COORD& dest, int weight); //��������

	~Position();
public:
	Position *m_prev;
};

