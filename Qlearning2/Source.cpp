#include <vector>
#include <string>
#include <iostream>
#include <Windows.h>
#include <random>
#include <sstream>
void Qlearning();
int main()
{
	setlocale(0, ""); // ���{��g�p
	Qlearning();
	return 0;
}
std::wstring action_nm[] = { L"��", L"��", L"�E", L"��" };// 0: 1: 2: 3:

#define START 1
#define GOAL  8
#define HOLE  9
#define KABE  -1
#define FROZEN_LAKE_ROWS 8
#define FROZEN_LAKE_COLS 8
#define FROZEN_BLOCK_CNT (FROZEN_LAKE_ROWS*FROZEN_LAKE_COLS)
struct block
{
	int action[4];
	int typ;
	float Qt[4];
	block(int l, int t, int r, int b, int ex1 = 0)
	{
		action[0] = l; action[1] = t; action[2] = r; action[3] = b;
		typ = ex1;
		Qt[0] = Qt[1] = Qt[2] = Qt[3] = 0;
	}
};
#define S block(1,1,1,1,START)
#define o block(1,1,1,1)
#define G block(1,1,1,1,GOAL)
#define H block(0,0,0,0,HOLE)

// �n�}�ł�
block BlockMap1[] = {
 S, o, H, o, H, o, o, o,
 o, o, H, o, H, o, H, o,
 H, o, o, o, H, o, H, o,
 o, o, H, o, H, o, H, o,
 o, H, o, o, o, o, H, o,
 o, H, o, H, o, o, H, o,
 o, o, o, H, o, H, o, o,
 o, o, o, H, o, H, o, G
};

#undef S
#undef o
#undef G
#undef H
std::vector<std::wstring> rireki;
float Q[FROZEN_BLOCK_CNT][4];
int ZeroToThreeRandom()
{
	static std::random_device seed_gen;
	static std::default_random_engine engine(seed_gen());
	static std::uniform_int_distribution<> dist(0, 3);
	return dist(engine);
}
float Random()
{
	static std::random_device seed_gen;
	static std::default_random_engine engine(seed_gen());
	static std::uniform_int_distribution<> dist(0, 100);
	return dist(engine) / 100.0f;
}

int get_action(int s, float epsilon);
int get_s_next(int* s, int a);
float nanmax2(int pos);
void goal_maze_ret_s_a_Q(float epsilon, float eta, float gamma, float)
{
	int s = 0, s_next = 0; // s: state, position.
	int a = 0, a_next = 0; // a: action
	float r = 0; // r: reward
	int cnt = 0;
	rireki.clear();
	while (1)
	{
		a = a_next; // �s�����X�V����
		int ss = s;
		int xa = get_s_next(&ss, a);
		if (xa == 0 || xa == KABE || xa == HOLE)
		{
			s_next = ss;
		}

		if (s != s_next)
		{
			std::wstringstream sm;
			sm << L"action:" << action_nm[a] << " s_position:" << s
				<< L"->s_next_position:" << s_next << L" cnt=" << ++cnt << L"\n";
			rireki.push_back(sm.str());
		}
		if (xa == GOAL)
		{
			r = 1; // ��V
			std::wcout << L"cnt=" << cnt;
		}
		else
		{
			r = 0;
			a_next = get_action(s_next, epsilon); // ���̃A�N�V����
		}
		// eta: �w�K��, gamma:���Ԋ�����
		if (xa == GOAL)
		{
			Q[s][a] += eta * (r - Q[s][a]);
		}
		else
		{
			Q[s][a] += eta * (r + gamma * nanmax2(s_next) - Q[s][a]); // Q-learning
			// Sarsa
			//Q[s][a] += eta * (r + gamma * Q[s_next][a_next] - Q[s][a]);
		}
		if (xa == GOAL)
		{
			return;
		}
		else if (xa == HOLE)
		{
			//�@���ɗ������̂ł�蒼���B�����̎c��Q�͂��̂܂܁B
			a = 0, a_next = 0, s_next = 0;
			cnt = 0;
		}
		s = s_next;
	}
}

int get_s_next(int* s, int a)
{
	auto bl = BlockMap1[*s]; //s�̓}�b�v��̃|�W�V����
	if (bl.typ == 0 || bl.typ == START)
	{
		if (a == 0 && bl.action[0] == 1)
		{
			*s += -1; return 0;
		} // ����
		else if (a == 1 && bl.action[1] == 1)
		{
			*s += -FROZEN_LAKE_ROWS; return 0;
		} // 1�i���
		else if (a == 2 && bl.action[2] == 1)
		{
			*s += 1; return 0;
		} // �E��
		else if (a == 3 && bl.action[3] == 1)
		{
			*s += FROZEN_LAKE_ROWS;  return 0;
		}// 1�i����
	}
	else if (bl.typ == GOAL)
		return GOAL;
	else if (bl.typ == HOLE)
		return HOLE;
	return KABE; // wall
}
float nanmax2(int k)
{
	float qmax = Q[k][0];
	for (int y = 1; y < 4; y++)
	{
		qmax = max(qmax, Q[k][y]);
	}
	return qmax;
}
int get_action(int s, float epsilon)
{
	int a = 0;
	if (Random() < epsilon)
	{
		a = ZeroToThreeRandom(); // ���X�AQ�𖳎������ړ�������Ԃ�
	}
	else
	{
		auto FromQmax = [](int s1)->int
		{
			int yy = 0;
			float v = Q[s1][yy];
			for (int y = 1; y < 4; y++)
			{
				if (v < Q[s1][y])
				{
					v = Q[s1][y];
					yy = y;
				}
			}
			return yy; // �ő��Q�����ړ�������Ԃ�
		};
		a = FromQmax(s);
	}
	return a;
}
std::vector<float> nanmax()
{
	std::vector<float> ar;
	for (int k = 0; k < FROZEN_BLOCK_CNT; k++)
	{
		float v = Q[k][0];
		for (int i = 1; i < 4; i++)
		{
			if (v < Q[k][i])
			{
				v = Q[k][i];
			}
		}
		ar.push_back(v);
	}
	return ar;
}

void Qlearning()
{
	// �}�b�v�ɊO�g�̕ǂ�ݒ�
	for (int y = 0; y < FROZEN_LAKE_ROWS; y++)
		for (int x = 0; x < FROZEN_LAKE_COLS; x++)
		{
			if (y == 0)
				BlockMap1[y * FROZEN_LAKE_COLS + x].action[1] = 0; // 0:wall
			else if (y == FROZEN_LAKE_ROWS - 1)
				BlockMap1[y * FROZEN_LAKE_COLS + x].action[3] = 0;
			if (x == 0)
				BlockMap1[y * FROZEN_LAKE_COLS + x].action[0] = 0;
			else if (x == FROZEN_LAKE_COLS - 1)
				BlockMap1[y * FROZEN_LAKE_COLS + x].action[2] = 0;
		}

	// Q�e�[�u���ɗ�����ݒ�
	for (int k = 0; k < FROZEN_BLOCK_CNT; k++)
	{
		Q[k][0] = BlockMap1[k].action[0] * Random();
		Q[k][1] = BlockMap1[k].action[1] * Random();
		Q[k][2] = BlockMap1[k].action[2] * Random();
		Q[k][3] = BlockMap1[k].action[3] * Random();
	}
	float eta = 0.2f;  //# �w�K��
	float gamma = 0.8f;  //# ���Ԋ�����
	float epsilon = 0.1;
	int  episode = 1;
	auto v = nanmax();  // ���l�̍ő�l�����߂�
	while (episode++ < 200)
	{
		epsilon = epsilon / 2.0f;
		goal_maze_ret_s_a_Q(epsilon, eta, gamma, 0);
		auto new_v = nanmax();
		float sum = 0.0f;
		for (int i = 0; i < FROZEN_BLOCK_CNT; i++)
		{
			sum += abs(new_v[i] - v[i]);
		}
		v = new_v;
		std::wcout << L" ��ԉ��l�̕ω�" << sum << L"\n";
	}

	//�Ō�Ɍ��肳�ꂽ�o�H��\��
	for (auto& s : rireki)
	{
		std::wcout << s;
	}
}