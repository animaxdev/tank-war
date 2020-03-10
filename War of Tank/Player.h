#pragma once
#include "Tank.h"
#include <time.h>
#include "RBase.h"

#define RCOUNT 14
class Player :
	public Tank
{
private :
	clock_t m_speed;//����ٶ�
	int m_Rcount;//Ԫ��
	int m_type; //��������
	RBase *m_R; //����

	void Drive(Map *map, Missile::MissileFactory &mf, list<Missile*> &missList);
	void showInfo();
public:
	Player();
	Player(unsigned int color, int bloodMAX, clock_t speed, int type);

	void init(Map *map);
	void get(Tank *tank);
	void Run(Missile::MissileFactory &mf, list<Missile*> &list, Map *map);
	void R(Map *map);
	bool wounded();
	bool isOver();
	void upgrade(Map *map);

	~Player();
};

