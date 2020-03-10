#include "Missile.h"
#include "Engine.h"


Missile::Missile(unsigned int id)
	:m_id(id), m_style(0x2), m_isValid(false), m_max(MAX), m_isfly(false), m_color(0)
{
}
Missile::Missile(unsigned int id, unsigned int tankID, int max, int m_style)
	: m_id(id), m_tankID(tankID), m_max(max), m_dir(m_dir), m_style(m_style), m_isfly(false), m_color(0)
{
}
//��ʼ��һ���ӵ�
bool Missile::init(unsigned int tankId, int dir, int x, int y, unsigned int color)
{
	bool flag;
	m_tankID = tankId;
	m_dir = dir;
	m_isValid = true;
	m_color = color;
	X = x;
	Y = y;
	m_max = MAX;
	m_isfly = false;
	if (flag = checkMissile())
	{
		if (m_isfly)
		{
			switch (m_dir)
			{
			case UP:    Y--;  X--; break;
			case DOWN:  Y++; X++; break;
			case LEFT:  X--; Y++; break;
			case RIGHT: X++; Y--; break;
			}
		}
		else
		{
			switch (dir)
			{
			case UP:    Y--; break;
			case DOWN:  Y++; break;
			case LEFT:  X--; break;
			case RIGHT: X++; break;
			}
		}
	}
	return flag;
}

//��ʼ����Ч�ӵ�
bool Missile::init(unsigned int tankId, int dir, int x, int y, unsigned int color, int max, bool isfly)
{
	bool flag;
	m_tankID = tankId;
	m_dir = dir;
	m_isValid = true;
	X = x;
	Y = y;
	m_max = max;
	m_color = color;
	m_isfly = isfly;
	if (flag = checkMissile())
	{
		if (m_isfly)
		{
			switch (m_dir)
			{
			case UP:    Y--;  X--; break;
			case DOWN:  Y++; X++; break;
			case LEFT:  X--; Y++; break;
			case RIGHT: X++; Y--; break;
			}
		}
		else
		{
			switch (dir)
			{
			case UP:    Y--; break;
			case DOWN:  Y++; break;
			case LEFT:  X--; break;
			case RIGHT: X++; break;
			}
		}
	}
	return flag;
}


//��ʼ��������Ч�ӵ�
bool Missile::init(unsigned int tankId, int dir, int x, int y, unsigned int color, int max, bool isfly, int)
{
	m_tankID = tankId;
	m_dir = dir;
	m_isValid = true;
	X = x;
	Y = y;
	m_max = max;
	m_color = color;
	m_isfly = isfly;

	if (m_isfly)
	{
		switch (m_dir)
		{
		case UP:    Y--;  X--; break;
		case DOWN:  Y++; X++; break;
		case LEFT:  X--; Y++; break;
		case RIGHT: X++; Y--; break;
		}
	}
	else
	{
		switch (dir)
		{
		case UP:    Y--; break;
		case DOWN:  Y++; break;
		case LEFT:  X--; break;
		case RIGHT: X++; break;
		}
	}

	return true;
}

//�ٻ�ǽ
void Missile::hitWall(int row, int col)
{
	int wallType = (*(g_Game->m_map))[row][col];

	switch (wallType)
	{
	case WALL_C:
		(*(g_Game->m_map))[row][col] = WALL_B;
		writeChar(row, col, wall_pic[WALL_B], wall_color[WALL_B]); break;
	case WALL_B:
		(*(g_Game->m_map))[row][col] = WALL__;
		writeChar(row, col, wall_pic[WALL__], wall_color[WALL__]); break;
		break;
	}
}

//�ٻ�̹��
void Missile::hitTank(int x, int y)
{
	unsigned int destId = g_Game->m_map->getRecID(x,y) ;
	bool isPlayer = g_Game->m_player->isMy(m_tankID);
	if (isPlayer)//�������ҵ��ӵ�
	{
  		auto itor = g_Game->m_NPClist.begin();
		while (itor != g_Game->m_NPClist.end())
		{
			if ((*itor)->isMy(destId))
				break;
			itor++;
		}
		if (itor == g_Game->m_NPClist.end())
			return;
		if ((*itor)->wounded())
		{
			(*itor)->ClsTank(g_Game->m_map);
			g_Game->m_tf.recTank((*itor));
			g_Game->m_player->get((*itor));
			g_Game->m_NPClist.erase(itor);
			showMessage("�ɹ���ɱ��", F_H_YELLOW, BOTTOM);
		}
		else
		{
			(*itor)->DrawTank(g_Game->m_map);
			showMessage("����Ŀ�꣡", F_H_YELLOW, BOTTOM);
		}
	}
	else//����ǵз����ӵ�
	{
		if (g_Game->m_player->isMy(destId))
		{
			if (g_Game->m_player->wounded())
			{
				g_Game->m_player->ClsTank(g_Game->m_map);
			}
			else
			{
				g_Game->m_player->DrawTank(g_Game->m_map);
			}
		}
	}
}

//�ٻ��ӵ�
void Missile::hitMissile(int x, int y)
{
	unsigned int destId = g_Game->m_map->getRecID(x, y);
	auto itor = g_Game->m_MISSlist.begin();
	while (itor != g_Game->m_MISSlist.end())
	{
		if ((*itor)->m_id == destId)
			break;
		itor++;
	}
	if (itor == g_Game->m_MISSlist.end())
		return;
	(*itor)->ClsMissile(g_Game->m_map);
	g_Game->m_mf.recMissile((*itor));
	g_Game->m_MISSlist.erase(itor);
}

//����ӵ�
int Missile::DrawMissile(Map *map)
{
	
	map->putRes(Y, X, m_id);
	char * pic = missile_pic[m_style];
	writeChar(Y, X, pic, m_color);

	//�ݵص���ʾ�ȼ����ӵ��ߣ���ͼԪ�ط�װ����󣬿���ֱ�Ӳ鿴����ʾ�ȼ���
	if ((*map)[Y][X] == WALL_A) //�ݵ�
	{
		writeChar(Y, X, pic, wall_color[WALL_A] | m_color);
	}else if ((*map)[Y][X] == WALL_E) //����
	{
		writeChar(Y, X, pic, wall_color[WALL_D] | m_color);
	}

	return true;
}

//����ӵ�
void Missile::ClsMissile(Map *map)
{
	int tmp = (*map)[Y][X];
	writeChar(Y, X, wall_pic[tmp], wall_color[tmp]);
	map->movRes(Y,X);
}

//����ӵ���ײ
bool Missile:: checkMissile()
{
	int y = Y;
	int x = X;
	if (m_isfly)
	{
		switch (m_dir)
		{
		case UP:    y--;  x--; break;
		case DOWN:  y++; x++; break;
		case LEFT:  x--; y++; break;
		case RIGHT: x++; y--; break;
		}
	}
	else
	{
		switch (m_dir)
		{
		case UP:    y--; break;
		case DOWN:  y++; break;
		case LEFT:  x--; break;
		case RIGHT: x++; break;
		}
	}

	if ((*g_Game->m_map)[y][x] != WALL__ &&
		(*g_Game->m_map)[y][x] != WALL_A &&
		(*g_Game->m_map)[y][x] != WALL_E)
	{
		hitWall(y, x);
		return false;
	}
	else if (g_Game->m_map->isPlaced(x, y))
	{
		if (g_Game->m_map->getRecID(x, y) <0x60)
		{
			hitTank(x, y);
		}
		else
		{
			hitMissile(x, y);
		}
		return false;
	}

	return true;
}


//�ƶ��ӵ�
bool Missile::moveMissile(Map *map)
{
	//1.����ɹ켣
	ClsMissile(map);
	//2.�ı��ӵ�λ��
	//3.�����¹켣

	
	if (m_max-- == 0) //���ж���������
	{
		m_isValid = false;
		return true;
	}
	if (checkMissile())
	{
		if (m_isfly)
		{
			switch (m_dir)
			{
			case UP:    Y--;  X--; break;
			case DOWN:  Y++; X++; break;
			case LEFT:  X--; Y++; break;
			case RIGHT: X++; Y--; break;
			}
		}
		else
		{
			switch (m_dir)
			{
			case UP:    Y--; break;
			case DOWN:  Y++; break;
			case LEFT:  X--; break;
			case RIGHT: X++; break;
			}
		}
		DrawMissile(map);
		return false;
	}
	else{
		m_isValid = false;
		return true;
	}
}

Missile::~Missile()
{
}
