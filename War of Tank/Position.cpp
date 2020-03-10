#include "Position.h"

/*******************��ʼ������***********************/

Position::Position(const COORD& pos, int weight, int loss, Position *prev)
	: COORD(pos), m_weight(weight), m_loss(loss), m_prev(prev)
{
}


 /****************************��ȡ�õ������********************************/
 void Position:: getCOORD(COORD &coord)
 {
	 coord.X = X;
	 coord.Y = Y;
 }

 /********************�ⲿ�ӿ�**********************/
 bool Position:: comperTo(const COORD &coord) //�ж��ǲ���ͬһ������
 {
	 if (X == coord.X && Y == coord.Y)
	 {
		 return true;
	 }
	 return false;
 }

 int Position::getLoss() //��ȡ�õ�ֱ�ӵ����յ����С����
 {
	 return m_loss;
 }

 void Position ::updateLoss(int weight) //���¸õ�ֱ�ӵ����յ����С����
 {
	 if (m_weight > weight)
	 {
		 m_loss -= (m_weight - weight);
		 m_weight = weight;
	 }
 }

 int Position::getWeigth()//��ȡ��ǰ���Ȩ��
 {
	 return m_weight;
 }

Position::~Position()
{
}

int Position::calcLoss(const COORD& src, const COORD& dest, int weight) //������С����
{
	return abs(dest.X - src.X) + abs(dest.Y - dest.Y) + weight;
}
