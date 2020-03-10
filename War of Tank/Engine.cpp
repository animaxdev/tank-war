#include "Engine.h"

Engine::Engine()
	:m_currentNPC(DEF_CNPC), m_maxNPC(DEF_NPCMAX), m_player(nullptr), m_map(nullptr), m_speed(DEF_SPEED), m_tmp(0), m_isPause(false)
{
}


//������Ϸ
void Engine::run()
{
	while (true)
	{
		if (KEY_DOWN('P')){ m_isPause = true; } 
		//����������ң���������
		m_player->Run(m_mf, m_MISSlist, m_map);
		//����NPC����
		checkNPC();
		//�ƶ��ӵ�
		movAllMissile();
		//�ƶ�̹��
		movAllTank();
		//���ʰȡ
		checkCime();
		//����ͼ�ȼ�
		checkLevel();
		//�����ͣ
		checkPause();
		//������
		if (checkOver())
			break;
	}
}

//����Ƿ����
bool Engine::checkOver()
{
	bool flag = false;
	static clock_t start_time = 0;
	clock_t current_time = clock();
	if (current_time - start_time < 1000)
		return false;

	if (m_player->isOver()) //�������
	{
		auto itor = m_NPClist.begin();//����NPC
		while (itor != m_NPClist.end())
		{
			(*itor)->ClsTank(m_map);
			m_tf.recTank(*itor);
			itor++;
		}
		m_NPClist.clear();
		flag = true;
		
	} 
	else if ((m_maxNPC == 0 && m_NPClist.size() == 0))//NPCΪ0
	{
		flag = true;
	} 

	//������������������е��ӵ�
	if (flag)
	{
		auto itor = m_MISSlist.begin();
		while (itor != m_MISSlist.end())
		{
			(*itor)->ClsMissile(m_map);
			m_mf.recMissile((*itor));
			itor++;
		}
		m_MISSlist.clear();
		showDialog("     ��  Ϸ   ��  ��     ");
	}

	return flag;
} 

//�����ͣ
void Engine::checkPause()
{
	static clock_t start_time = 0;
	clock_t current_time = clock();
	if (current_time - start_time < 400)
		return ;
	start_time = current_time;
	if (m_isPause)
	{
		showDialog("     ��  Ϸ   ��  ͣ     ");
		m_isPause = false;
	}
}

//�ƶ������ӵ�
void Engine:: movAllMissile()
{
	static clock_t start_time = 0;
	clock_t current_time = clock();
	if (current_time - start_time < 40)
		return;
	start_time = current_time;
	auto itor = m_MISSlist.begin();
	while (itor != m_MISSlist.end())
	{
	
 		if ((*itor)->moveMissile(m_map))
		{
			m_mf.recMissile((*itor));
			itor = m_MISSlist.erase(itor);
		}
		else
		{
			itor++;
		}
	}
} 

//�ƶ�����̹��
void Engine::movAllTank()
{
	static clock_t NPC_clockStart;
	clock_t NPC_clockFinish = clock();
	if (NPC_clockFinish - NPC_clockStart < m_speed) { return; }
	NPC_clockStart = NPC_clockFinish;
	auto itor = m_NPClist.begin();
	while (itor != m_NPClist.end())
	{
		(*itor)->Unmanned(m_mf, m_MISSlist, m_map, m_player);
		itor++;
	}
}

//���ˢ��NPC
void Engine::checkNPC()
{
	static clock_t start_time = 0;
	int current_time = clock();
	if (current_time - start_time < 2000)
		return;
	start_time = current_time;
	Tank *tmp;
	if (m_NPClist.size() < m_currentNPC)
	{
		if (m_maxNPC)
		{
			m_maxNPC--;
			tmp = m_tf.buyTank(m_map);
			m_NPClist.push_back(tmp);
		}
	}
	char buf[26];
	sprintf_s(buf, "ս���ϵĵ��˻���%d\n", m_currentNPC);
	showMessage(buf, F_H_RED, 14);
	sprintf_s(buf, "�Ż������ĵ��˻���%d\n", m_maxNPC);
	showMessage(buf, F_H_RED, 12);
}

//������ҵ�������������ս���ȼ�
void Engine::checkLevel()
{
	
	static clock_t start_time = 0;
	int current_time = clock();
	if (current_time - start_time < 2000)
		return;
	start_time = current_time;
	if ((m_player->Score() - m_tmp) > 100)
	{
		m_tmp = m_player->Score();
		m_level++;
		m_speed -= 4;
		m_tf.updateTank(m_level);
		char buf[20];
		sprintf_s(buf,"��%d��", m_level);
		showMessage(buf, F_H_GREEN, 20);
		showMessage("ս���Ѷ�������", F_H_GREEN, 0);
	}
	
}

//��鲹�����Ƿ����ɲ������Ƿ����Ҳ���
void Engine::checkCime()
{
	static clock_t start_time = 0;
	int current_time = clock();
	if (current_time - start_time < 40)
		return;
	start_time = current_time;
	if (!m_cime)
	{
		if (rand() % 200 != 0)
			return;
		int x, y;
		int tmp;
		while (true)
		{
			x = rand() % (MAP_WIDTH - 4) + 2;
			y = rand() % (MAP_HIGH - 4) + 2;
			if ((tmp = (*m_map)[y][x]) != WALL_D && tmp != WALL_E)
				break;
		}
		m_cime = (COORD *)malloc(sizeof(COORD));
		m_cime->X = x;
		m_cime->Y = y;
		writeChar(y, x, wall_pic[WALL_F], F_H_YELLOW | wall_color[(*m_map)[y][x]]);
		return;
	}

	if (m_map->getRecID(m_cime->X, m_cime->Y) == PID)
	{
		m_player->upgrade(m_map);
		showMessage("������ȡ������", F_H_YELLOW, 0);
		free(m_cime);
		m_cime = nullptr;
		return;
	}
	writeChar(m_cime->Y, m_cime->X, wall_pic[WALL_F], F_H_YELLOW | wall_color[(*m_map)[m_cime->Y][m_cime->X]]);
}

//A���㷨
AStart& Engine::Astart()
{
	return *m_Astart;
}

//��ʼ������,�ָ���Ϸ��Ĭ��ֵ
void Engine::Start()
{
	Muisc("begin.wav");
	showMessage("��  ��  ��  �� �ƶ�", F_H_WHITE, 6);
	showMessage("X �����ӵ���C �ͷ�Ԫ������", F_H_WHITE, 7);
	showMessage("P ��ͣ", F_H_WHITE, 8);
	if (m_map == nullptr)
		m_map = new Map();
	if (m_player == nullptr)
		m_player = new Player();
	if (m_Astart)
		delete m_Astart;
	//��ʼ����ͼ��AI
	m_Astart = new AStart(m_map, MAP_WIDTH, MAP_HIGH);
	m_map->drawMap();
	m_player->init(m_map);
	m_isPause = false;
	//�ָ���Ϸ�ȼ�
	m_level = 0; 
	m_tmp = 0;
	m_tf.updateTank(m_level);
	m_speed = DEF_SPEED;

	run();//������Ϸ
} 

//���õ����������༭�з��е���
void Engine::setOption(int maxNPC, int cNPC)
{
	m_maxNPC = maxNPC;
	m_currentNPC = cNPC;
}

//���õ�ͼ����ͼ�༭���е���
void Engine::setMap(Map *map)
{
	if (m_map)
		delete m_map;
	m_map = map;
} 

//������ң��༭��װ�е���
void Engine::setPlayer(Player *palyer)
{
	if (m_player)
		delete m_player;
	m_player = palyer;
}

Engine::~Engine()
{
	if (m_map)
		delete m_map;
	if (m_player)
		delete m_player;
	delete m_Astart;
}
