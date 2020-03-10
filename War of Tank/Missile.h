#pragma once
#include <Windows.h>
#include "Map.h"


#define MAX 9999
#include <vector>
using std::vector;
#include <list>
using std::list;

class Missile
	:protected COORD
{
protected:
	unsigned int m_id;          //�������

	int m_dir;		//����
	int m_max;		//Ѫ��
	int m_style;    //�ӵ���ʽ
	bool m_isValid;   //�Ƿ���Ч
	bool m_isfly ;//�Ƿ�б�ŷ�
	unsigned int m_color;

	unsigned int m_tankID;				//̹�˱�ţ���¼��ŵ�����˭����ģ�

public:
	Missile(unsigned int id);
	Missile(unsigned int id, unsigned int tankID, int max, int m_style);

	virtual bool init(unsigned int tankId, int dir, int x, int y, unsigned int color);
	bool init(unsigned int tankId, int dir, int x, int y, unsigned int color, int max, bool isfly);
	bool init(unsigned int tankId, int dir, int x, int y, unsigned int color, int max, bool isfly, int);

	void hitWall(int row, int col);
	void hitTank(int x, int y);
	void hitMissile(int x, int y);
	virtual bool checkMissile();
	virtual void ClsMissile(Map *map);
	virtual int DrawMissile(Map *map);
	bool moveMissile(Map *map);

	~Missile();
protected:

public:
#define MMIN 60
	class MissileFactory
	{
	private:
		static unsigned int MISSID;
		vector<Missile *> m_missilePool;
		void clearPool();
	public:
		MissileFactory();
		void recMissile(Missile *miss);
		Missile *buyMissile(int tankID, int dir, COORD &pos, Map *map, unsigned int color);
		Missile *buyMissile(int tankID, int dir, COORD &pos, Map *map, unsigned int color, int max, int isfly);
		Missile *buyMissile(int tankID, int dir, COORD &pos, Map *map, unsigned int color, int max, int isfly, int);
		~MissileFactory();
	};


};





