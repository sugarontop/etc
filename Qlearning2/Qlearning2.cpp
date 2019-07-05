// Qlearning2.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <vector>
#include <string>
#include <iostream>
#include <Windows.h>
#include <random>
#include <sstream>

void Qlearning();

int main()
{
	setlocale(0,""); // 日本語使用

	Qlearning();

	return 0;
}

std::wstring action_nm [] = { L"左", L"上", L"右", L"下" };// 0: 1: 2: 3:

#define START 1
#define GOAL  8
#define HOLE  9
#define KABE  -1

#define FROWZEN_LAKE_ROWS 8
#define FROWZEN_LAKE_COLS 8
#define FROWZEN_BLOCK_CNT (FROWZEN_LAKE_ROWS*FROWZEN_LAKE_COLS)

struct block
{
	int action[4];
	int typ;	
	float Qt[4];
	block(int l, int t, int r, int b, int ex1=0)
	{
		action[0]=l; action[1] =t; action[2] =r; action[3] =b;
		typ = ex1;
		Qt[0]=Qt[1]=Qt[2]=Qt[3]=0;
	}
};

#define S	block(1,1,1,1,START)
#define o	block(1,1,1,1)
#define G	block(1,1,1,1,GOAL)
#define H	block(0,0,0,0,HOLE)

block BlockMap1 [] = {
S,o,H,o,H,o,o,H,
o,o,H,o,H,o,H,o,
H,o,o,o,H,o,H,o,
o,o,H,o,H,o,H,o,
o,H,o,o,o,o,H,o,
o,H,o,H,o,o,H,o,
o,o,o,H,o,o,o,o,
o,o,o,H,o,H,o,G
};

#undef S
#undef o
#undef G
#undef H

std::vector<std::wstring> rireki;

float Q[FROWZEN_BLOCK_CNT][4];

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

	return dist(engine) /100.0f;

}


int get_action(int s, float epsilon);
int get_s_next(int* s, int a);
float nanmax2(int pos);

void goal_maze_ret_s_a_Q(float epsilon, float eta, float gamma, float  )
{
	int s = 0, s_next = 0;	// s: state, position.
	int a =0, a_next = 0;	// a: action
	float r = 0;	// r: reward
	int cnt = 0;

	rireki.clear();
		
	while(1)
	{
		a = a_next; // 行動を更新する
		int ss = s;

		int xa = get_s_next(&ss,a);
		
		if (xa == 0 || xa == KABE || xa == HOLE)
		{
			s_next = ss;
		}
	
		
		if ( s != s_next )
		{
			std::wstringstream sm;
			
			sm << L"action:" << action_nm[a] << " s_position:" << s << L"->s_next_position:" << s_next << L" cnt=" << ++cnt << L"\n"; 
						
			rireki.push_back( sm.str());
		}

		if ( xa == GOAL )
		{
			r = 1; // 報酬
			std::wcout << L"cnt=" << cnt;
		}
		else
		{
			r = 0; 
			a_next = get_action(s_next, epsilon); // 次のアクション
		}

		// eta: 学習率, gamma:時間割引率
		if (xa == GOAL)
		{
			Q[s][a] += eta*(r-Q[s][a]);
		}				
		else
		{
			Q[s][a] += eta * (r + gamma* nanmax2(s_next)-Q[s][a]); // Q-learning
			
			//Q[s][a] += eta * (r + gamma * Q[s_next][a_next] - Q[s][a]); // Sarsa
		}
		
		if (xa == GOAL)
		{
			return;
		}
		else if (xa == HOLE )
		{
			//　穴に落ちたのでやり直し。履歴の残るQはそのまま。
			a = 0, a_next = 0, s_next = 0;
			cnt = 0;
		}

		s = s_next;
	}
}


int get_s_next( int* s, int a )
{
	auto bl = BlockMap1[*s]; //sはマップ上のポジション

	if ( bl.typ == 0 || bl.typ == START)
	{
		if (a == 0 && bl.action[0] == 1 ){ *s += -1; return 0;} // 左へ
		else if (a == 1 && bl.action[1] == 1){ *s += -FROWZEN_LAKE_ROWS; return 0;} // 1段上へ
		else if (a == 2 && bl.action[2] == 1){*s += 1; return 0;} // 右へ
		else if (a == 3 && bl.action[3] == 1){*s += FROWZEN_LAKE_ROWS;  return 0;}// 1段下へ
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

int get_action( int s,float epsilon)
{
	int a = 0;
	if (Random() < epsilon )
	{
		a = ZeroToThreeRandom(); // 時々、Qを無視した移動方向を返す
	}
	else
	{
		auto FromQmax = [](int s1)->int
		{
			int yy = 0;
			float v = Q[s1][yy];
			
			for(int y=1; y < 4; y++ )
			{
				if ( v < Q[s1][y] )
				{
					v = Q[s1][y];
					yy = y;
				}
			}
			return yy; // 最大のQをもつ移動方向を返す
		};

		a = FromQmax(s);
	}

	return a;
}

std::vector<float> nanmax()
{
	std::vector<float> ar;
	for (int k = 0; k < FROWZEN_BLOCK_CNT; k++)
	{
		float v = Q[k][0];
		for( int i = 1; i < 4; i++ )
		{			
			if ( v < Q[k][i] )
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
	// マップに外枠の壁を設定
	for (int y = 0; y < FROWZEN_LAKE_ROWS; y++)
		for (int x = 0; x < FROWZEN_LAKE_COLS; x++)
		{
			if (y == 0)
				BlockMap1[y * FROWZEN_LAKE_COLS + x].action[1] = 0; // topに壁
			else if (y == FROWZEN_LAKE_ROWS-1)
				BlockMap1[y * FROWZEN_LAKE_COLS + x].action[3] = 0;

			if (x == 0)
				BlockMap1[y * FROWZEN_LAKE_COLS + x].action[0] = 0;
			else if (x == FROWZEN_LAKE_COLS-1)
				BlockMap1[y * FROWZEN_LAKE_COLS + x].action[2] = 0;
		}


	// Qテーブルに乱数を設定
	for( int k = 0; k < FROWZEN_BLOCK_CNT; k++ )
	{
		Q[k][0] = BlockMap1[k].action[0] * Random();
		Q[k][1] = BlockMap1[k].action[1] * Random();
		Q[k][2] = BlockMap1[k].action[2] * Random();
		Q[k][3] = BlockMap1[k].action[3] * Random();
	}

	float eta = 0.2f;  //# 学習率
	float gamma = 0.8f;  //# 時間割引率
	float epsilon = 0.1; 
	int  episode = 1;

	auto v = nanmax();  // 価値の最大値を求める

	while ( episode++ < 200 )
	{
		epsilon = epsilon / 2.0f;
		
		goal_maze_ret_s_a_Q(epsilon, eta, gamma,0);

		auto new_v = nanmax();  

		float sum=0.0f;
		for(int i =0; i < FROWZEN_BLOCK_CNT; i++)
		{
			sum += abs(new_v[i]-v[i]);
		}

		v = new_v;

		std::wcout<< L" 状態価値の変化" << sum  << L"\n" ;

	}


	//最後に決定された経路を表示
	for(auto& s : rireki )
	{
		std::wcout << s;
	}
} 

