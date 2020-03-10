#include "Tank.h"

//�ȳ�ʼ������̹�˵������
Tank::TankFactory::TankFactory()
{
	for (int i = 0; i < MIN; i++)
	{
		tankPool.push_back(new Tank(TANKID++));
	}
}

//����̹��
Tank *Tank::TankFactory::buyTank(Map *map)
{
	Tank* tmp;

	if (tankPool.empty())
	{
		tmp =  new Tank(TANKID++);
	}
	else
	{
		tmp = tankPool.back();
		tankPool.pop_back();
	}
	tmp->init(map, m_blood);

	return tmp;
}

//�����Ѿ�������̹�˵�̹�˳�
void Tank :: TankFactory::recTank(Tank *tank)
{
	tankPool.push_back(tank);
} 

//����̹��
void Tank ::TankFactory:: updateTank(int levl)
{
	m_blood = DEFBLOOD + levl/4;
}

//�ͷ�̹�˳�
void Tank ::TankFactory::clearPool()
{
	for (int i = 0; i < tankPool.size(); i++)
		delete tankPool[i];
	tankPool.clear();
}

Tank :: TankFactory::~TankFactory()
{
	clearPool();
}

unsigned int Tank :: TankFactory:: TANKID = 0x10;
