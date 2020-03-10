#pragma once
#include "AStart.h"
#include "Map.h"
#include "Player.h"
#include "Tank.h"
#include <list>
#include "RMiss1.h"
#include "RMiss2.h"
#include "RMiss3.h"
#include "RMiss4.h"

using std::list;

#define DEF_NPCMAX 999999
#define DEF_CNPC 3
#define DEF_SPEED 160
class Engine
{
private:
	Map *m_map;
	Player *m_player;
	Tank ::TankFactory m_tf;
	Missile::MissileFactory m_mf;
	list<Tank *> m_NPClist;
	list<Missile *> m_MISSlist;
	int m_level = 0; //��Ϸ�ȼ�
	int m_speed; //��Ϸ�ٶ�
	int m_tmp ; //��Ϸ�ȼ���������

	bool m_isPause; //�Ƿ���ͣ
	int m_maxNPC; //NPC����
	int m_currentNPC; //��ǰNPC��
	COORD *m_cime = nullptr; //����������
	
	AStart *m_Astart; //AI


	void run();
	void Engine::checkNPC();
	void checkPause();
	void checkLevel();
	void movAllTank();
	void Engine::movAllMissile();
	bool checkOver();
	void checkCime();
public:
	Engine();
	void setMap(Map *map);
	void setOption(int maxNPC, int cNPC);
	void Engine::setPlayer(Player *palyer);
	void Start();
	AStart& Astart();
	friend bool Player::wounded();
	friend bool Missile::checkMissile();
	friend void Missile::hitWall(int row, int col);
	friend void Missile::hitTank(int x, int y);
	friend void Missile::hitMissile(int x, int y);
	friend bool RMiss1::checkMissile();
	friend bool RMiss3::checkMissile();
	friend bool RMiss1::run(Map *map, Player *player);
	friend bool RMiss2::run(Map *map, Player *player);
	friend bool RMiss4::run(Map *map, Player *player);
	~Engine();
};

