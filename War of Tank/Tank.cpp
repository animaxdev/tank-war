#include "Engine.h"
#include "Tank.h"


Tank::Tank(unsigned int id)
	:m_id(id), m_bloodMAX(DEFBLOOD), m_blood(m_bloodMAX), m_dir(UP), m_stat(die), m_score(DEFSCORE), m_color(F_H_GREEN)
{
}

Tank::Tank(unsigned int id, WORD color, int boolMAX, int score)
	: m_id(id), m_bloodMAX(boolMAX), m_blood(boolMAX), m_dir(UP), m_stat(die), m_score(score), m_color(color)
{
}


//��̹�˵�ͼ�ϺͿ���̨�ϻ���̹��
void Tank:: DrawTank(Map *map)
{
	byte(*tankArr)[3] = tank_style[(m_blood > m_bloodMAX/3 ?0:1)][m_dir];
	int y, x;
	unsigned int color = m_color;
	if (m_stat == inv)
		color = F_H_RED;

	for (int row = -1, i = 0; row <= 1; row++, i++)
	{
		for (int col = -1, j = 0; col <= 1; col++, j++)
		{
			y = Y + row; //������
			x = X + col; //������

			int tmp = tankArr[i][j];
			if (tmp)
			{
				map->putRes(y, x, m_id);
				char * pic = tank_pic[tmp];
				writeChar(y, x, pic, color);

				//�ݵص���ʾ�ȼ���̹�˸ߣ���ͼԪ�ط�װ����󣬿���ֱ�Ӳ鿴����ʾ�ȼ���
				if ((*map)[y][x] == WALL_A) //�ݵ�
				{
					writeChar(y, x, wall_pic[WALL_A], wall_color[WALL_A] | color);
				}
			}

		}
	}
	x = X;
	y = Y;
	//���Ѫ��ռ�ĸ���
	switch (m_dir)
	{
	case UP:    y = Y+1; break;
	case DOWN:  y = Y-1; break;
	case LEFT:  x = X+1; break;
	case RIGHT: x = X-1; break;
	}
	color = m_color;
	if ((*map)[y][x] == WALL_A) //�ݵ�
		color |= wall_color[WALL_A] ;
	writeChar(y, x, blood_pic[m_blood], color);

	map->putRes(y, x, m_id); 

}

//̹�˵�ͼ�Ϳ���̨�ϵ�̹��
void Tank::ClsTank(Map *map)
{
	byte(*tankArr)[3] = tank_style[0][m_dir];
	int tmp;

	for (int row = -1; row < 2; row++)
	{
		for (int col = -1; col < 2; col++)
		{
			int y = Y + row; //������
			int x = X + col; //������

			tmp = tankArr[row + 1][col + 1];
			if (tmp)
			{
				//printf("%d", tmp);
				map->movRes(y, x);
				tmp = (*map)[y][x];
				writeChar(y, x, wall_pic[tmp], wall_color[tmp]);
			}
		}
	}
	int y = Y;
	int x = X;
	//���Ѫ��ռ�ĸ�
	switch (m_dir)
	{
	case UP:    y = Y + 1; break;
	case DOWN:  y = Y - 1; break;
	case LEFT:  x = X + 1; break;
	case RIGHT: x = X - 1; break;
	}
	map->movRes(y, x);
	tmp = (*map)[y][x];
	writeChar(y, x, wall_pic[tmp], wall_color[tmp]);
}

//̹�˵���ײ���
bool Tank::checkTank(Map *map, bool isMov)
{

	int ym = 0, xm = 0;
	if (isMov)
	{
		switch (m_dir)
		{
		case UP:    ym--; break;
		case DOWN:  ym++; break;
		case LEFT:  xm--; break;
		case RIGHT: xm++; break;
		}
	}
	int tmp;
	byte(*tankArr)[3] = tank_style[0][m_dir];

	for (int row = -1; row<2; row++)
	{
		for (int col = -1; col<2; col++)
		{

			int y = row + Y + ym;
			int x = col + X + xm;

			tmp = tankArr[row + 1][col + 1];
			if (tmp)//�����̹�������һ����
			{
				if ((*map)[y][x] != WALL__ &&
					(*map)[y][x] != WALL_A)
				{
					return false;
				}
				if (map->isPlaced(x, y))
				{
					return false;
				}
			}
		}
	}
	return true;
}

//��ʼ��̹�ˣ����һ�����꣬Ѫ��Ϊ��Ѫ���޵�״̬)
void Tank::init(Map *map, int blood)
{
	do
	{
		X = rand() % (MAP_WIDTH - 4) + 2;
		Y = rand() % (MAP_HIGH - 4) + 2;
	} while (!checkTank(map, false));
	m_stat = inv;
	if (blood)
		m_blood = m_bloodMAX = blood;
	else
		m_blood = m_bloodMAX;

	DrawTank(map);
}

//�ƶ�̹��
void Tank:: moveTank(Map *map, int orientation) 
{

	//1.����ɹ켣
	ClsTank(map);//����ƶ�ǰ��tank
	//2.�ı�̹��λ��
	//3.�����¹켣
	int tmp = m_dir; 

	if (orientation != m_dir)
	{
		
		m_dir = orientation;

		/*
		//�������ת��,�����ƶ�
		if (!checkTank(temp))
		{
		temp.orientation = pTank->orientation;
		goto move;
		}
		*/
		if (checkTank(map, false))//�������ת��ֱ��ת�򣬲��������ƶ���ת��
		{
			goto end;
		}
	}
	if (checkTank(map, true))//��������ƶ�
	{
		switch (m_dir)
		{
		case UP:    Y--; break;
		case DOWN:  Y++; break;
		case LEFT:  X--; break;
		case RIGHT: X++; break;
		}
	}
	else
	{
		m_dir = tmp;
	}
	
end :
	DrawTank(map);
}

//����
void Tank::fire(Missile::MissileFactory &mf, list<Missile*> &missList, Map *map)
{
 	clock_t current_time = clock();
	if (current_time - m_time < 260)
		return;
	m_time = current_time;
	Missile *tmp;
	if((tmp = mf.buyMissile(m_id, m_dir, *this, map, m_color)))
		missList.push_back(tmp);
}

//�ж��ǲ����Լ�
bool Tank:: isMy(unsigned int id)
{
	return m_id == id;
}

//�Զ���ʻ
void Tank::Unmanned(Missile::MissileFactory &mf, list<Missile*> &missList, Map *map, Player *player)
{
	
	int oper ;
	if (m_stat == act)
	{
		g_Game->Astart().setElement(*this, *player);
		oper = g_Game->Astart().findPath(*this);
	} else if (m_stat == inv)
	{
		oper = rand() % 16;
	}
	
	if (oper == -1)
		oper = rand() % 16;
	
	switch (oper)
	{
	case UP:    moveTank(map, UP);   break;
	case DOWN:  moveTank(map, DOWN);   break;
	case LEFT:	moveTank(map, LEFT);   break;
	case RIGHT:	moveTank(map, RIGHT);   break;
	default:
		moveTank(map, m_dir);
	}
	if (X == player->X || Y == player->Y)
	{
		if (m_stat == inv)
		{
			m_stat = act;
			return;
		}
		fire(mf, missList, map);	
	}
}

//���˼�Ѫ�����ж�
bool Tank::wounded()
{
	if (m_stat == inv)
	{
		m_stat = act;
		return false;
	}
	m_blood--;
	if (m_blood) //�Ƿ�����
		return false;
	m_stat = die;
	return true;
}

//���ظ�̹�˵���������
int Tank :: Score()
{
	return m_score;
}

Tank::~Tank()
{
}
