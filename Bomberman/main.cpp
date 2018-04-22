#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <typeinfo>
#include <queue>
#include <stack>
#include "PlayerAnimation.h"
#include "Player.h"
#include "Player2rd.h"
#include "Platform.h"
#include "Bomb.h"
#include "BombAnimation.h"
#include "BombExploding.h"
#include "BombExplodingAnim.h"
#include "Destroy_item.h"
#include "Destroy_item_Anim.h"
#include "Lose.h"
#include "LoseAnimation.h"
#include "MAP_GROUND.h"
#include "MAP_OBJECT.h"
#include "MAP_item.h"
#include "MAP_outside_top.h"
#include "Collider.h"
#include "Immortal.h"
#include "BOT1.h"
#include "BOT2.h"
#include "BOT3.h"
using namespace std;
//------------------struct  Bomb pos---------------------------------------
typedef struct {
	int x, y, type, fire_range;
}Bomb_explode_pos;
Bomb_explode_pos thisBombPos;
queue<Bomb_explode_pos> queueBombpos;
//------------------struct fire pos-----------------------------------
typedef struct {
	int x, y, type;
}Fire_range_pos;
Fire_range_pos thisFirepos;
queue<Fire_range_pos> queueFirepos;
//-------------------struct item---------------------------------------
typedef struct {
	int x, y;
	int type;
}Block_destroy;
Block_destroy thisBlockPos;
queue<Block_destroy> queueBlockpos;

typedef struct {
	int type, set, bomb_type;
	float time;
	sf::Vector2f pos;
}Explode_mark;
Explode_mark Explode_table[30][30];

typedef struct {
	int type, set;
	float time;
	sf::Vector2f pos;
}Block_mark;
Block_mark Block_table[30][30];


typedef struct {
	int type;
	int set;
	int burn;
	float time;
}Item_mark;
Item_mark Item_mark_table[30][30];

//-----------------------------Player information----------------------
typedef struct {
	float switch_time;
	float lose_time;
	int speed;
	int fire_range;
	int bomb;
	int used_bomb;
	int bomb_type;
	int is_die;
	sf::Texture playerTexture;
	sf::Texture Burning;
	sf::Vector2f spawn_postion;
	sf::Vector2f lose_position;
	//------for bot-------
	int des_x;
	int des_y;
	int have_des;

	
}Player_info;
Player_info PLAYER[5] ;  //P1 P2 BOT BOT BOT

void set_player_info();

typedef struct {
	int fire_range;
	int bomb_type;
	int planted;
	int explode;
	int own;
	float plant_time;
	float exploding_time;
	sf::Vector2f position;
}Bomb_info;
Bomb_info Bomb_table[30][30];


//----------------------------------- Variables for Map-------------------------------
typedef struct {
	char object;
	char meterial;
}Table;
Table table[30][30];
int item_table[30][30];
int table_scale_height = 11;
int table_scale_width = 21;

int resolution_x = 1280;
int resolution_y = 960;

int battle_table_x = (resolution_x / 2 - (table_scale_width / 2) * 50);
int battle_table_y = (resolution_y / 2 - (table_scale_height / 2) * 50) + 75;

void generate_map();
void set_sprite_map();
int set_to_item_table(int x, int y);


typedef struct{
	sf::Texture default_glass1;
	sf::Texture default_glass2;
	sf::Texture immortal;
	sf::Texture Outside;
	sf::Texture Object;
	sf::Texture Boom;
	sf::Texture Top;
	sf::Texture Left;
	sf::Texture Right;
	sf::Texture Bottom;
}Sprite_MAP;
Sprite_MAP thisMAP[5];

int check_pos_bomb_player(sf::Vector2f this_bomb, sf::Vector2f this_player);
int set_fire_type(int y1, int x1, int y2, int x2, int fire);
void generate_background(sf::RenderWindow& window, int map);
void generate_immortal(sf::RenderWindow& window, Collider& player, Collider& player2rd, Collider& bot_no1, int map);
void generate_bomb(sf::RenderWindow& window,
	Collider& player, sf::Vector2f player_pos,
	Collider& player2rd, sf::Vector2f player2rd_pos,
	Collider& bot_no1, sf::Vector2f bot_no1_pos,
	Collider& bot_no2, sf::Vector2f bot_no2_pos,
	Collider& bot_no3, sf::Vector2f bot_no3_pos,
	float cur_time);
void set_table_to_ground(int y, int x);
void add_bomb_to_queue(int y, int x, int type);
void add_block_to_queue(int y, int x, int type);
void Fire_exploding(Explode_mark thisFireExploding,
	Collider& player, sf::Vector2f player_pos,
	Collider& player2rd, sf::Vector2f player2rd_pos,
	Collider& bot_no1, sf::Vector2f bot_no1_pos,
	Collider& bot_no2, sf::Vector2f bot_no2_pos,
	Collider& bot_no3, sf::Vector2f bot_no3_pos,
	sf::RenderWindow& window, float cur_time, int fire_type);
void Bomb_exploding(Bomb_info thisBombExploding, sf::RenderWindow& window, float cur_time, int fire_type);
void Block_exploding(Block_mark thisBlockExploding, sf::RenderWindow& window, float cur_time);
void find_fire_path(Bomb_info thisBombExploding, float cur_time, int y, int x);
void player_got_item(int type, int player_number);
sf::Vector2f check_set_bombPosition(sf::Vector2f thisPosition, float cur_time, int own);
sf::Vector2f set_fire_pos(int y, int x);
//---------------------------------- -------------------------------------------------------------------


//---------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------Main------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------
int Map_number = 0;

typedef struct {
	int x, y;
}bfs_path;
bfs_path tmpbfs[3], tempqbfs[3];
queue<bfs_path> queueBFS[3];
int bot_bfs_mark[3][30][30];

struct bot_path{
	int ex_x,x;
	int ex_y,y;
	int value;
	int distance;
	bool operator < (const bot_path & a) const {
		if (value < a.value) return true;
		else if (value > a.value) return false;
		else if (distance < a.distance) return true;
		else return false;
	}

};
bot_path path_table[3][30][30], tempPath[3];
priority_queue<bot_path> queueBotPath[3];
int bot_table[3][30][30];

typedef struct {
	int x, y;
}bot_path_stack;
bot_path_stack tmpstack[3];
stack<bot_path_stack> stackpath[3];
int is_same_pos(sf::Vector2f cur, sf::Vector2f des);

int bot_dir[4][2] = { { 1, 0 },{ 0, 1 },{ -1, 0 },{ 0, -1 } };
void bot1_BFS(int num)
{
	//printf("%d", num);
	int x, y;
	for (int i = 0; i < table_scale_height; i++)
		for (int j = 0; j < table_scale_width; j++)
		{
			if (table[i][j].object != 0)
				bot_table[num][i][j] = -1;
			else
				bot_table[num][i][j] = 0;
		}
	for (int i = 0; i < table_scale_height; i++)
		for (int j = 0; j < table_scale_width; j++)
		{
			if (table[i][j].object == 0)
			{
				for (int k = 0; k < 4; k++)
				{
					y = i + bot_dir[k][0];
					x = j + bot_dir[k][1];
					if (x >= 0 && x < table_scale_width && y >= 0 && y < table_scale_height)
					{
						if (table[y][x].object == 1 || table[y][x].object == 2)
						{
							bot_table[num][i][j] += 2;
						}
					}
				}
			}
		}
}

void bot2_BFS(int num)
{
	//printf("%d", num);
	int x, y;
	for (int i = 0; i < table_scale_height; i++)
		for (int j = 0; j < table_scale_width; j++)
			if (table[i][j].object != 0)
				bot_table[num][i][j] = -1;
			else
				bot_table[num][i][j] = 0;
	for (int i = 0; i < table_scale_height; i++)
		for (int j = 0; j < table_scale_width; j++)
		{
			if (table[i][j].object == 0)
			{
				for (int k = 0; k < 4; k++)
				{
					y = i + bot_dir[k][0];
					x = j + bot_dir[k][1];
					if (x >= 0 && x < table_scale_width && y >= 0 && y < table_scale_height)
					{
						if (table[y][x].object == 1 || table[y][x].object == 2)
						{
							bot_table[num][i][j] += 1;
						}
					}
				}
			}
		}
}

void bot3_BFS(int num)
{
	//printf("%d", num);
	int x, y;
	for (int i = 0; i < table_scale_height; i++)
		for (int j = 0; j < table_scale_width; j++)
			if (table[i][j].object != 0)
				bot_table[num][i][j] = -1;
			else
				bot_table[num][i][j] = 0;
	for (int i = 0; i < table_scale_height; i++)
		for (int j = 0; j < table_scale_width; j++)
		{
			if (table[i][j].object == 0)
			{
				for (int k = 0; k < 4; k++)
				{
					y = i + bot_dir[k][0];
					x = j + bot_dir[k][1];
					if (x >= 0 && x < table_scale_width && y >= 0 && y < table_scale_height)
					{
						if (table[y][x].object == 1 || table[y][x].object == 2)
						{
							bot_table[num][i][j] += 1;
						}
					}
				}
			}
		}
}

void bot1_find_path(sf::Vector2f thisPosition)
{
	int num = 0;
	for (int i = 0; i < table_scale_height; i++)
		for (int j = 0; j < table_scale_width; j++)
			if (Explode_table[i][j].set == 1)
				bot_bfs_mark[num][i][j] = 1;
			else
				bot_bfs_mark[num][i][j] = 0;
	thisPosition.x = ((thisPosition.x - battle_table_x + 25) / 50);
	thisPosition.y = ((thisPosition.y - battle_table_y + 25) / 50);
	int pos_x = (int)thisPosition.x, x;
	int pos_y = (int)thisPosition.y, y;
	//printf("cur pos - %d %d ", pos_y, pos_x);
	bot_bfs_mark[num][pos_y][pos_x] = 1;

	tmpbfs[num].x = pos_x;
	tmpbfs[num].y = pos_y;
	queueBFS[num].push(tmpbfs[num]);

	
	path_table[num][pos_y][pos_x].x = path_table[num][pos_y][pos_x].ex_x = pos_x;
	path_table[num][pos_y][pos_x].y = path_table[num][pos_y][pos_x].ex_y = pos_y;
	path_table[num][pos_y][pos_x].distance = path_table[num][pos_y][pos_x].value = 0;

	
	tempPath[num].x = tempPath[num].ex_x = pos_x;
	tempPath[num].y = tempPath[num].ex_y = pos_y;
	tempPath[num].value = 0;
	tempPath[num].distance = 0;
	queueBotPath[num].push(tempPath[num]);
	
	

	while (!queueBFS[num].empty())
	{
		tempqbfs[num] = queueBFS[num].front();
		queueBFS[num].pop();

		for (int k = 0; k < 4; k++)
		{
			x = tempqbfs[num].x + bot_dir[k][0];
			y = tempqbfs[num].y + bot_dir[k][1];
			if (x >= 0 && x < table_scale_width && y >= 0 && y < table_scale_height)
			{
				if (table[y][x].object == 0 && bot_bfs_mark[num][y][x] == 0)
				{
					//printf("%d %d\n", x, y);
					bot_bfs_mark[num][y][x] = 1;

					tempPath[num].x = x;
					tempPath[num].y = y;
					tempPath[num].ex_x = tempqbfs[num].x;
					tempPath[num].ex_y = tempqbfs[num].y;
					tempPath[num].value = bot_table[num][y][x];
					tempPath[num].distance = abs(tempPath[num].x - tempPath[num].ex_x) + abs(tempPath[num].y - tempPath[num].ex_y);
					queueBotPath[num].push(tempPath[num]);

					path_table[num][y][x] = tempPath[num];

					tmpbfs[num].x = x;
					tmpbfs[num].y = y;
					queueBFS[num].push(tmpbfs[num]);
				}
			}
		}
	}

	tempPath[num] = queueBotPath[num].top();
	while (!queueBotPath[num].empty()) queueBotPath[num].pop();

	PLAYER[num + 1].have_des = 1;
	PLAYER[num + 1].des_x = tempPath[num].x;
	PLAYER[num + 1].des_y = tempPath[num].y;
	//printf("have des - %d %d\n", PLAYER[num + 1].des_x, PLAYER[num + 1].des_y);

	while (!stackpath[num].empty()) stackpath[num].pop();

	tmpstack[num].x = PLAYER[num + 1].des_x;
	tmpstack[num].y = PLAYER[num + 1].des_y;
	stackpath[num].push(tmpstack[num]);

	while (1)
	{
		if (tempPath[num].x == tempPath[num].ex_x && tempPath[num].y == tempPath[num].ex_y) break;
		
		tmpstack[num].x = tempPath[num].x;
		tmpstack[num].y = tempPath[num].y;
		stackpath[num].push(tmpstack[num]);

		tempPath[num] = path_table[num][tempPath[num].ex_y][tempPath[num].ex_x];
		printf("stack - %d %d\n", tmpstack[num].y, tmpstack[num].x);
	}


	if (!stackpath[num].empty())
	{
		PLAYER[num + 1].des_x = stackpath[num].top().x;
		PLAYER[num + 1].des_y = stackpath[num].top().y;
	}
	printf("have des - %d %d\n", PLAYER[num + 1].des_y, PLAYER[num + 1].des_x);
	
}

void bot2_find_path(sf::Vector2f thisPosition)
{
	int num = 1;
	for (int i = 0; i < table_scale_height; i++)
		for (int j = 0; j < table_scale_width; j++)
			if (Explode_table[i][j].set == 1)
				bot_bfs_mark[num][i][j] = 1;
			else
				bot_bfs_mark[num][i][j] = 0;
	thisPosition.x = ((thisPosition.x - battle_table_x + 25) / 50);
	thisPosition.y = ((thisPosition.y - battle_table_y + 25) / 50);
	int pos_x = (int)thisPosition.x, x;
	int pos_y = (int)thisPosition.y, y;
	//printf("cur pos - %d %d ", pos_y, pos_x);
	bot_bfs_mark[num][pos_y][pos_x] = 1;

	tmpbfs[num].x = pos_x;
	tmpbfs[num].y = pos_y;
	queueBFS[num].push(tmpbfs[num]);


	path_table[num][pos_y][pos_x].x = path_table[num][pos_y][pos_x].ex_x = pos_x;
	path_table[num][pos_y][pos_x].y = path_table[num][pos_y][pos_x].ex_y = pos_y;
	path_table[num][pos_y][pos_x].distance = path_table[num][pos_y][pos_x].value = 0;


	tempPath[num].x = tempPath[num].ex_x = pos_x;
	tempPath[num].y = tempPath[num].ex_y = pos_y;
	tempPath[num].value = 0;
	tempPath[num].distance = 0;
	queueBotPath[num].push(tempPath[num]);



	while (!queueBFS[num].empty())
	{
		tempqbfs[num] = queueBFS[num].front();
		queueBFS[num].pop();

		for (int k = 0; k < 4; k++)
		{
			x = tempqbfs[num].x + bot_dir[k][0];
			y = tempqbfs[num].y + bot_dir[k][1];
			if (x >= 0 && x < table_scale_width && y >= 0 && y < table_scale_height)
			{
				if (table[y][x].object == 0 && bot_bfs_mark[num][y][x] == 0)
				{
					//printf("%d %d\n", x, y);
					bot_bfs_mark[num][y][x] = 1;

					tempPath[num].x = x;
					tempPath[num].y = y;
					tempPath[num].ex_x = tempqbfs[num].x;
					tempPath[num].ex_y = tempqbfs[num].y;
					tempPath[num].value = bot_table[num][y][x];
					tempPath[num].distance = abs(tempPath[num].x - tempPath[num].ex_x) + abs(tempPath[num].y - tempPath[num].ex_y);
					queueBotPath[num].push(tempPath[num]);

					path_table[num][y][x] = tempPath[num];

					tmpbfs[num].x = x;
					tmpbfs[num].y = y;
					queueBFS[num].push(tmpbfs[num]);
				}
			}
		}
	}

	tempPath[num] = queueBotPath[num].top();
	while (!queueBotPath[num].empty()) queueBotPath[num].pop();

	PLAYER[num + 1].have_des = 1;
	PLAYER[num + 1].des_x = tempPath[num].x;
	PLAYER[num + 1].des_y = tempPath[num].y;
	//printf("have des - %d %d\n", PLAYER[num + 1].des_x, PLAYER[num + 1].des_y);

	while (!stackpath[num].empty()) stackpath[num].pop();

	tmpstack[num].x = PLAYER[num + 1].des_x;
	tmpstack[num].y = PLAYER[num + 1].des_y;
	stackpath[num].push(tmpstack[num]);

	while (1)
	{
		if (tempPath[num].x == tempPath[num].ex_x && tempPath[num].y == tempPath[num].ex_y) break;

		tmpstack[num].x = tempPath[num].x;
		tmpstack[num].y = tempPath[num].y;
		stackpath[num].push(tmpstack[num]);

		tempPath[num] = path_table[num][tempPath[num].ex_y][tempPath[num].ex_x];
		printf("stack - %d %d\n", tmpstack[num].y, tmpstack[num].x);
	}


	if (!stackpath[num].empty())
	{
		PLAYER[num + 1].des_x = stackpath[num].top().x;
		PLAYER[num + 1].des_y = stackpath[num].top().y;
	}
	printf("have des - %d %d\n", PLAYER[num + 1].des_y, PLAYER[num + 1].des_x);

}

void bot3_find_path(sf::Vector2f thisPosition)
{
	int num = 2;
	for (int i = 0; i < table_scale_height; i++)
		for (int j = 0; j < table_scale_width; j++)
			if (Explode_table[i][j].set == 1)
				bot_bfs_mark[num][i][j] = 1;
			else
				bot_bfs_mark[num][i][j] = 0;
	thisPosition.x = ((thisPosition.x - battle_table_x + 25) / 50);
	thisPosition.y = ((thisPosition.y - battle_table_y + 25) / 50);
	int pos_x = (int)thisPosition.x, x;
	int pos_y = (int)thisPosition.y, y;
	//printf("cur pos - %d %d ", pos_y, pos_x);
	bot_bfs_mark[num][pos_y][pos_x] = 1;

	tmpbfs[num].x = pos_x;
	tmpbfs[num].y = pos_y;
	queueBFS[num].push(tmpbfs[num]);


	path_table[num][pos_y][pos_x].x = path_table[num][pos_y][pos_x].ex_x = pos_x;
	path_table[num][pos_y][pos_x].y = path_table[num][pos_y][pos_x].ex_y = pos_y;
	path_table[num][pos_y][pos_x].distance = path_table[num][pos_y][pos_x].value = 0;


	tempPath[num].x = tempPath[num].ex_x = pos_x;
	tempPath[num].y = tempPath[num].ex_y = pos_y;
	tempPath[num].value = 0;
	tempPath[num].distance = 0;
	queueBotPath[num].push(tempPath[num]);



	while (!queueBFS[num].empty())
	{
		tempqbfs[num] = queueBFS[num].front();
		queueBFS[num].pop();

		for (int k = 0; k < 4; k++)
		{
			x = tempqbfs[num].x + bot_dir[k][0];
			y = tempqbfs[num].y + bot_dir[k][1];
			if (x >= 0 && x < table_scale_width && y >= 0 && y < table_scale_height)
			{
				if (table[y][x].object == 0 && bot_bfs_mark[num][y][x] == 0)
				{
					//printf("%d %d\n", x, y);
					bot_bfs_mark[num][y][x] = 1;

					tempPath[num].x = x;
					tempPath[num].y = y;
					tempPath[num].ex_x = tempqbfs[num].x;
					tempPath[num].ex_y = tempqbfs[num].y;
					tempPath[num].value = bot_table[num][y][x];
					tempPath[num].distance = abs(tempPath[num].x - tempPath[num].ex_x) + abs(tempPath[num].y - tempPath[num].ex_y);
					queueBotPath[num].push(tempPath[num]);

					path_table[num][y][x] = tempPath[num];

					tmpbfs[num].x = x;
					tmpbfs[num].y = y;
					queueBFS[num].push(tmpbfs[num]);
				}
			}
		}
	}

	tempPath[num] = queueBotPath[num].top();
	while (!queueBotPath[num].empty()) queueBotPath[num].pop();

	PLAYER[num + 1].have_des = 1;
	PLAYER[num + 1].des_x = tempPath[num].x;
	PLAYER[num + 1].des_y = tempPath[num].y;
	//printf("have des - %d %d\n", PLAYER[num + 1].des_x, PLAYER[num + 1].des_y);

	while (!stackpath[num].empty()) stackpath[num].pop();

	tmpstack[num].x = PLAYER[num + 1].des_x;
	tmpstack[num].y = PLAYER[num + 1].des_y;
	stackpath[num].push(tmpstack[num]);

	while (1)
	{
		if (tempPath[num].x == tempPath[num].ex_x && tempPath[num].y == tempPath[num].ex_y) break;

		tmpstack[num].x = tempPath[num].x;
		tmpstack[num].y = tempPath[num].y;
		stackpath[num].push(tmpstack[num]);

		tempPath[num] = path_table[num][tempPath[num].ex_y][tempPath[num].ex_x];
		printf("stack - %d %d\n", tmpstack[num].y, tmpstack[num].x);
	}


	if (!stackpath[num].empty())
	{
		PLAYER[num + 1].des_x = stackpath[num].top().x;
		PLAYER[num + 1].des_y = stackpath[num].top().y;
	}
	printf("have des - %d %d\n", PLAYER[num + 1].des_y, PLAYER[num + 1].des_x);

}

int main()
{
	printf("Start Program!!\n");
	sf::RenderWindow window(sf::VideoMode(resolution_x, resolution_y), "Bomberman", sf::Style::Close | sf::Style::Resize);

	int select_mode = 1;
	int screen = 5;
	float global_time = 0, start_screen;
	float deltaTime = 0.0f;
	sf::Clock clock;

	

	PLAYER[0].playerTexture.loadFromFile("./Sprite/Bomberman/WhitePlayer.png");
	PLAYER[1].playerTexture.loadFromFile("./Sprite/Bomberman/BlackPlayer.png");
	PLAYER[2].playerTexture.loadFromFile("./Sprite/Bomberman/RedPlayer.png");
	PLAYER[3].playerTexture.loadFromFile("./Sprite/Bomberman/BluePlayer.png");
	PLAYER[4].playerTexture.loadFromFile("./Sprite/Bomberman/GreenPlayer.png");
	PLAYER[0].Burning.loadFromFile("./Sprite/Bomberman/Burning.png");
	PLAYER[1].Burning.loadFromFile("./Sprite/Bomberman/Burning.png");
	PLAYER[2].Burning.loadFromFile("./Sprite/Bomberman/Burning.png");
	PLAYER[3].Burning.loadFromFile("./Sprite/Bomberman/Burning.png");
	PLAYER[4].Burning.loadFromFile("./Sprite/Bomberman/Burning.png");

	set_sprite_map();
	generate_map();
	set_player_info();

	Player player(&PLAYER[0].playerTexture, sf::Vector2u(3, 7), PLAYER[0].switch_time, (float)PLAYER[0].speed, PLAYER[0].spawn_postion);
	PlayerAnimation animation(&PLAYER[0].playerTexture, sf::Vector2u(3, 7), PLAYER[0].switch_time);

	Player2rd player2rd(&PLAYER[1].playerTexture, sf::Vector2u(3, 7), PLAYER[1].switch_time, (float)PLAYER[1].speed, PLAYER[1].spawn_postion);
	PlayerAnimation animation2rd(&PLAYER[1].playerTexture, sf::Vector2u(3, 7), PLAYER[1].switch_time);
	PLAYER[1].is_die = 1;

	BOT1 bot_no1(&PLAYER[2].playerTexture, sf::Vector2u(3, 7), PLAYER[2].switch_time, (float)PLAYER[2].speed, PLAYER[2].spawn_postion);
	PlayerAnimation animationbot_no1(&PLAYER[2].playerTexture, sf::Vector2u(3, 7), PLAYER[2].switch_time);

	BOT2 bot_no2(&PLAYER[3].playerTexture, sf::Vector2u(3, 7), PLAYER[3].switch_time, (float)PLAYER[3].speed, PLAYER[3].spawn_postion);
	PlayerAnimation animationbot_no2(&PLAYER[3].playerTexture, sf::Vector2u(3, 7), PLAYER[3].switch_time);

	BOT3 bot_no3(&PLAYER[4].playerTexture, sf::Vector2u(3, 7), PLAYER[4].switch_time, (float)PLAYER[4].speed, PLAYER[4].spawn_postion);
	PlayerAnimation animationbot_no3(&PLAYER[4].playerTexture, sf::Vector2u(3, 7), PLAYER[4].switch_time);

	while (window.isOpen())
	{
		sf::Vector2i MousePos = sf::Mouse::getPosition(window);
		//printf("%d %d\n", MousePos.y, MousePos.x);

		deltaTime = clock.restart().asSeconds();

		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			/*
			if (event.type == sf::Event::Resized)
			{
				cout << "Width : " << event.size.width << endl;
				cout << "Height : " << event.size.height << endl;
				cout << "---------------------" << endl;
			}
			if (event.type == sf::Event::TextEntered)
			{
				printf("%c", event.text.unicode);
			}
			*/
		}

			
		window.clear(sf::Color::Green);

		if (screen == 1)
		{

			sf::Texture Background_pic;
			sf::Texture Logo_pic;
			sf::Texture text1_pic;
			sf::Texture text2_pic;
			sf::Texture text3_pic;

			sf::Vector2f text1_size;
			sf::Vector2f text2_size;
			sf::Vector2f text3_size;

			sf::Vector2f text1_pos;
			sf::Vector2f text2_pos;
			sf::Vector2f text3_pos;

			Background_pic.loadFromFile("./Sprite/Menu/MainMenu/Background.png");
			Logo_pic.loadFromFile("./Sprite/Menu/MainMenu/Logo.png");
			text1_pic.loadFromFile("./Sprite/Menu/MainMenu/TextMenu/BattleMode.png");
			text2_pic.loadFromFile("./Sprite/Menu/MainMenu/TextMenu/Setting.png");
			text3_pic.loadFromFile("./Sprite/Menu/MainMenu/TextMenu/Quit.png");

			//text1
			if (MousePos.x >= (1280 - 500) / 2 && MousePos.x <= (1280 - 500) / 2 + 500 && MousePos.y >= 500 && MousePos.y <= 550)
			{
				text1_size = sf::Vector2f(500 + 10, 50 + 10);
				text1_pos = sf::Vector2f((float)(1280 - 500) / 2 - 5, (float)500 - 5);
				if (event.type == sf::Event::MouseButtonPressed)
					if (event.mouseButton.button == sf::Mouse::Left)
						screen = 2;
			}
			else
			{
				text1_size = sf::Vector2f(500, 50);
				text1_pos = sf::Vector2f((float)(1280 - 500) / 2, (float)500);
			}

			//text2
			if (MousePos.x >= (1280 - 340) / 2 && MousePos.x <= (1280 - 340) / 2 + 340 && MousePos.y >= 600 && MousePos.y <= 650)
			{
				text2_size = sf::Vector2f(350 + 10, 50 + 10);
				text2_pos = sf::Vector2f((float)(1280 - 340) / 2 - 5, (float)600 - 5);
			}
			else
			{
				text2_size = sf::Vector2f(340, 50);
				text2_pos = sf::Vector2f((float)(1280 - 340) / 2, (float)600);
			}

			//text3
			if (MousePos.x >= (1280 - 200) / 2 && MousePos.x <= (1280 - 200) / 2 + 200 && MousePos.y >= 700 && MousePos.y <= 750)
			{
				text3_size = sf::Vector2f(200 + 10, 50 + 10);
				text3_pos = sf::Vector2f((float)(1280 - 200) / 2 - 5, (float)700 - 5);
			}
			else
			{
				text3_size = sf::Vector2f(200, 50);
				text3_pos = sf::Vector2f((float)(1280 - 200) / 2, (float)700);
			}

			MAP_outside_top bg(&Background_pic, sf::Vector2f(1280 , 960), sf::Vector2f(0, 0));
			MAP_outside_top logo(&Logo_pic, sf::Vector2f(823, 210), sf::Vector2f((float)(1280-823)/2, (float)100));
			MAP_outside_top text1(&text1_pic, text1_size, text1_pos);
			MAP_outside_top text2(&text2_pic, text2_size, text2_pos);
			MAP_outside_top text3(&text3_pic, text3_size, text3_pos);

			bg.Draw(window);
			logo.Draw(window);
			text1.Draw(window);
			text2.Draw(window);
			text3.Draw(window);

			start_screen = 0;

		}

		if (screen == 2)
		{
			sf::Texture Black_bg;
			Black_bg.loadFromFile("./Sprite/Menu/Black.png");

			MAP_outside_top black(&Black_bg, sf::Vector2f(1280, 960), sf::Vector2f(0, 0));
			black.Draw(window);
			
			
			start_screen += deltaTime;
			if (start_screen > 3.0f) screen = 3;
			Map_number = 0;
		}

		if (screen == 3)
		{
			

			sf::Texture Background_pic;
			sf::Texture smode;
			sf::Texture mmode;
			sf::Texture textmap;
			sf::Texture map_pic[5];
			sf::Texture arrow_left;
			sf::Texture arrow_right;
			sf::Texture start_button;

			if (MousePos.x >= ((1280 / 2 - 370) / 2 + 75) && MousePos.x <= ((1280 / 2 - 370) / 2 + 75 + 370) && MousePos.y >= 200 && MousePos.y <= 470)
				if (event.type == sf::Event::MouseButtonPressed)
					if (event.mouseButton.button == sf::Mouse::Left)
						select_mode = 1;

			if (MousePos.x >= ((1280 / 2 - 370) / 2 - 75 + (1280 / 2)) && MousePos.x <= ((1280 / 2 - 370) / 2 + 1280 / 2 - 75 + 370) && MousePos.y >= 200 && MousePos.y <= 470)
				if (event.type == sf::Event::MouseButtonPressed)
					if (event.mouseButton.button == sf::Mouse::Left)
						select_mode = 2;

			if (MousePos.x >= 1030 && MousePos.x <= 1030 + 225 && MousePos.y >= 860 && MousePos.y <= 860 + 75)
				if (event.type == sf::Event::MouseButtonPressed)
					if (event.mouseButton.button == sf::Mouse::Left)
						screen = 4;


			Background_pic.loadFromFile("./Sprite/Menu/MainMenu/Background.png");
			
			if (select_mode == 1)	smode.loadFromFile("./Sprite/Menu/BattleMode/smode_2.png");
			else					smode.loadFromFile("./Sprite/Menu/BattleMode/smode_1.png");
			
			if (select_mode == 2)	mmode.loadFromFile("./Sprite/Menu/BattleMode/mmode_2.png");
			else					mmode.loadFromFile("./Sprite/Menu/BattleMode/mmode_1.png");

			map_pic[0].loadFromFile("./Sprite/Menu/BattleMode/Stage/Default.png");
			arrow_left.loadFromFile("./Sprite/Menu/BattleMode/Stage/ArrowLeft.png");
			arrow_right.loadFromFile("./Sprite/Menu/BattleMode/Stage/ArrowRight.png");
			textmap.loadFromFile("./Sprite/Menu/BattleMode/Stage/MAP.png");
			start_button.loadFromFile("./Sprite/Menu/BattleMode/Start/StartButton_Select.png");

			MAP_outside_top bg(&Background_pic, sf::Vector2f(1280, 960), sf::Vector2f(0, 0));
			MAP_outside_top s_mode(&smode, sf::Vector2f(370, 270), sf::Vector2f((1280/2 - 370)/2 + 75, 200));
			MAP_outside_top m_mode(&mmode, sf::Vector2f(370, 270), sf::Vector2f((1280 / 2 - 370) / 2 + 1280/2 - 75, 200));
			MAP_outside_top map_s(&map_pic[Map_number], sf::Vector2f(350, 270), sf::Vector2f((1280-350)/2 , 600) );
			MAP_outside_top text_map(&textmap, sf::Vector2f(140, 50), sf::Vector2f((1280 - 140) / 2, 530));
			MAP_outside_top arr_left(&arrow_left, sf::Vector2f(50, 70), sf::Vector2f((1280-350)/2 - 70, 600 + (270 - 70)/2));
			MAP_outside_top arr_right(&arrow_right, sf::Vector2f(50, 70), sf::Vector2f((1280-350)/2 + 370, 600 + (270-70)/2));
			MAP_outside_top butt(&start_button, sf::Vector2f(225, 75), sf::Vector2f(1030 , 860));

			bg.Draw(window);
			s_mode.Draw(window);
			m_mode.Draw(window);
			map_s.Draw(window);
			text_map.Draw(window);
			arr_left.Draw(window);
			arr_right.Draw(window);
			butt.Draw(window);

			start_screen = 0;

		}

		if (screen == 4)
		{
			sf::Texture Black_bg;
			Black_bg.loadFromFile("./Sprite/Menu/Black.png");

			MAP_outside_top black(&Black_bg, sf::Vector2f(1280, 960), sf::Vector2f(0, 0));
			black.Draw(window);


			start_screen += deltaTime;
			if (start_screen > 3.0f) screen = 5;
		}

		if (screen == 5)
		{	
			
			

			if (PLAYER[0].is_die == 0)
			{
				player.Update(deltaTime, PLAYER[0].switch_time, PLAYER[0].speed); //deltaTime , switch time
				if (player.planting && PLAYER[0].used_bomb+1 <= PLAYER[0].bomb)
				{
					player.planting = 0;
					check_set_bombPosition(player.GetPosition(),  global_time, 0);
				}
				player.planting = 0;
			}

			if (PLAYER[1].is_die == 0)
			{
				player2rd.Update(deltaTime, PLAYER[1].switch_time, PLAYER[1].speed); //deltaTime , switch time
				if (player2rd.planting && PLAYER[1].used_bomb + 1 <= PLAYER[1].bomb)
				{
					player2rd.planting = 0;
					check_set_bombPosition(player2rd.GetPosition(), global_time, 1);
				}
				player2rd.planting = 0;
			}

			sf::Thread bot1(&bot1_BFS, 0);
			sf::Thread bot2(&bot2_BFS, 1);
			sf::Thread bot3(&bot3_BFS, 2);


				if (PLAYER[2].is_die == 0)
				{

					if (PLAYER[2].have_des == 0 && stackpath[0].size() == 0)
					{
						bot1_find_path(bot_no1.GetPosition());
					}
					//printf("%d %d\n", PLAYER[2].des_y, PLAYER[2].des_x);
					if (!stackpath[0].empty())
					{
						if (is_same_pos(bot_no1.GetPosition(), sf::Vector2f((float)battle_table_x + 50 * stackpath[0].top().x, (float)battle_table_y + 50 * stackpath[0].top().y)))
							stackpath[0].pop();
						if (!stackpath[0].empty())
						bot_no1.Update(deltaTime, PLAYER[2].switch_time, PLAYER[2].speed, bot_no1.GetPosition(), sf::Vector2f((float)battle_table_x + 50 * stackpath[0].top().x, (float)battle_table_y + 50 * stackpath[0].top().y));

					}
					else
						PLAYER[2].have_des = 0;

					if (bot_no1.GetPosition() == sf::Vector2f((float)PLAYER[2].des_x * 50 + battle_table_x, (float)PLAYER[2].des_y * 50 + battle_table_y) || stackpath[0].empty())
					{
						bot_no1.planting = 1;
						PLAYER[2].have_des = 0;
					}
					if (bot_no1.planting && PLAYER[2].used_bomb + 1 <= PLAYER[2].bomb)
					{
						bot_no1.planting = 0;
						check_set_bombPosition(bot_no1.GetPosition(), global_time, 2);
					
					}
					bot_no1.planting = 0;
					bot1.launch();

				}
				if (PLAYER[3].is_die == 0)
				{

					if (PLAYER[3].have_des == 0 && stackpath[1].size() == 0)
					{
						bot2_find_path(bot_no2.GetPosition());
					}
					//printf("%d %d\n", PLAYER[2].des_y, PLAYER[2].des_x);
					if (!stackpath[1].empty())
					{
						if (is_same_pos(bot_no2.GetPosition(), sf::Vector2f((float)battle_table_x + 50 * stackpath[1].top().x, (float)battle_table_y + 50 * stackpath[1].top().y)))
							stackpath[1].pop();
						if (!stackpath[1].empty())
							bot_no2.Update(deltaTime, PLAYER[3].switch_time, PLAYER[3].speed, bot_no2.GetPosition(), sf::Vector2f((float)battle_table_x + 50 * stackpath[1].top().x, (float)battle_table_y + 50 * stackpath[1].top().y));

					}
					else
						PLAYER[3].have_des = 0;

					if (bot_no2.GetPosition() == sf::Vector2f((float)PLAYER[3].des_x * 50 + battle_table_x, (float)PLAYER[3].des_y * 50 + battle_table_y) || stackpath[1].empty())
					{
						bot_no2.planting = 1;
						PLAYER[3].have_des = 0;
					}
					if (bot_no2.planting && PLAYER[3].used_bomb + 1 <= PLAYER[3].bomb)
					{
						bot_no2.planting = 0;
						check_set_bombPosition(bot_no2.GetPosition(), global_time, 3);

					}
					bot_no2.planting = 0;
					bot2.launch();

				}
				if (PLAYER[4].is_die == 0)
				{

					if (PLAYER[4].have_des == 0 && stackpath[2].size() == 0)
					{
						bot3_find_path(bot_no3.GetPosition());
					}
					//printf("%d %d\n", PLAYER[2].des_y, PLAYER[2].des_x);
					if (!stackpath[2].empty())
					{
						if (is_same_pos(bot_no3.GetPosition(), sf::Vector2f((float)battle_table_x + 50 * stackpath[2].top().x, (float)battle_table_y + 50 * stackpath[2].top().y)))
							stackpath[2].pop();
						if (!stackpath[2].empty())
							bot_no3.Update(deltaTime, PLAYER[4].switch_time, PLAYER[4].speed, bot_no3.GetPosition(), sf::Vector2f((float)battle_table_x + 50 * stackpath[2].top().x, (float)battle_table_y + 50 * stackpath[2].top().y));

					}
					else
						PLAYER[4].have_des = 0;

					if (bot_no3.GetPosition() == sf::Vector2f((float)PLAYER[4].des_x * 50 + battle_table_x, (float)PLAYER[4].des_y * 50 + battle_table_y) || stackpath[2].empty())
					{
						bot_no3.planting = 1;
						PLAYER[4].have_des = 0;
					}
					if (bot_no3.planting && PLAYER[4].used_bomb + 1 <= PLAYER[4].bomb)
					{
						bot_no3.planting = 0;
						check_set_bombPosition(bot_no3.GetPosition(), global_time, 4);

					}
					bot_no3.planting = 0;
					bot3.launch();

				}

			

		
			generate_background(window, Map_number);
			generate_immortal(window, player.GetCollider(), player2rd.GetCollider(), bot_no1.GetCollider(), Map_number);
			generate_bomb(window,
				player.GetCollider(), player.GetPosition(),
				player2rd.GetCollider(), player2rd.GetPosition(),
				bot_no1.GetCollider(), bot_no1.GetPosition(),
				bot_no2.GetCollider(), bot_no2.GetPosition(),
				bot_no3.GetCollider(), bot_no3.GetPosition(),
				global_time);

			if (PLAYER[0].is_die == 0)
			{
				player.Draw(window);
			}
			else
			{
				if (global_time - PLAYER[0].lose_time <= 1.50f)
				{
					//printf("%f %f\n", global_time, PLAYER[0].lose_time);
					Lose player_lose(&PLAYER[0].Burning, PLAYER[0].lose_position, sf::Vector2u(10, 1), 1.5f / 10.0f);
					LoseAnimation animation1(&PLAYER[0].Burning, sf::Vector2u(1, 10), 1.5f / 10.0f);
					player_lose.Update(PLAYER[0].lose_time, global_time, 1.5f / 10.0f);
					player_lose.Draw(window);
				}
			}

			if (PLAYER[1].is_die == 0)
			{
				player2rd.Draw(window);
			}
			else
			{
				if (global_time - PLAYER[1].lose_time <= 1.50f)
				{
					//printf("%f %f\n", global_time, PLAYER[1].lose_time);
					Lose player2rd_lose(&PLAYER[1].Burning, PLAYER[1].lose_position, sf::Vector2u(10, 1), 1.5f / 10.0f);
					LoseAnimation animation2(&PLAYER[1].Burning, sf::Vector2u(1, 10), 1.5f / 10.0f);
					player2rd_lose.Update(PLAYER[1].lose_time, global_time, 1.5f / 10.0f);
					player2rd_lose.Draw(window);
				}
			}

			if (PLAYER[2].is_die == 0)
			{
				bot_no1.Draw(window);
			}
			else
			{
				if (global_time - PLAYER[2].lose_time <= 1.50f)
				{
					//printf("%f %f\n", global_time, PLAYER[1].lose_time);
					Lose bot1_lose(&PLAYER[2].Burning, PLAYER[2].lose_position, sf::Vector2u(10, 1), 1.5f / 10.0f);
					LoseAnimation animation3(&PLAYER[2].Burning, sf::Vector2u(1, 10), 1.5f / 10.0f);
					bot1_lose.Update(PLAYER[2].lose_time, global_time, 1.5f / 10.0f);
					bot1_lose.Draw(window);
				}
			}

			if (PLAYER[3].is_die == 0)
			{
				bot_no2.Draw(window);
			}
			else
			{
				if (global_time - PLAYER[3].lose_time <= 1.50f)
				{
					//printf("%f %f\n", global_time, PLAYER[1].lose_time);
					Lose bot2_lose(&PLAYER[3].Burning, PLAYER[3].lose_position, sf::Vector2u(10, 1), 1.5f / 10.0f);
					LoseAnimation animation4(&PLAYER[3].Burning, sf::Vector2u(1, 10), 1.5f / 10.0f);
					bot2_lose.Update(PLAYER[3].lose_time, global_time, 1.5f / 10.0f);
					bot2_lose.Draw(window);
				}
			}

			if (PLAYER[4].is_die == 0)
			{
				bot_no3.Draw(window);
			}
			else
			{
				if (global_time - PLAYER[4].lose_time <= 1.50f)
				{
					//printf("%f %f\n", global_time, PLAYER[1].lose_time);
					Lose bot3_lose(&PLAYER[4].Burning, PLAYER[4].lose_position, sf::Vector2u(10, 1), 1.5f / 10.0f);
					LoseAnimation animation5(&PLAYER[4].Burning, sf::Vector2u(1, 10), 1.5f / 10.0f);
					bot3_lose.Update(PLAYER[4].lose_time, global_time, 1.5f / 10.0f);
					bot3_lose.Draw(window);
				}
			}
		}
		
			
		
		window.display();


		global_time += deltaTime;
		//printf("Global time : %f\n", global_time);
		//printf("Delta time : %f\n", deltaTime);

	}
	
	printf("End Program\n");
	
	return 0;
}


void generate_bomb(sf::RenderWindow& window,
	Collider& player, sf::Vector2f player_pos,
	Collider& player2rd, sf::Vector2f player2rd_pos,
	Collider& bot_no1, sf::Vector2f bot_no1_pos,
	Collider& bot_no2, sf::Vector2f bot_no2_pos,
	Collider& bot_no3, sf::Vector2f bot_no3_pos,
	float cur_time)
{
	sf::Texture normalBombTexture;
	sf::Texture itemTexture;

	for (int i = 0; i < table_scale_height; i++)
		for (int j = 0; j < table_scale_width; j++)
		{

			if (table[i][j].object == 4)
			{
				if (cur_time - Bomb_table[i][j].plant_time <= 4.0f)
				{
					if(Bomb_table[i][j].bomb_type == 1)
					{
						normalBombTexture.loadFromFile("./Sprite/Bomb/normalBomb.png");
					}
					else
					{
						normalBombTexture.loadFromFile("./Sprite/Bomb/pierceBomb.png");
					}
					Bomb normalBomb(&normalBombTexture, Bomb_table[i][j].position, sf::Vector2u(4, 1), 0.2f);
					BombAnimation bombAnimation(&normalBombTexture, sf::Vector2u(1, 4), 0.2f);
					normalBomb.Update(Bomb_table[i][j].plant_time, cur_time, 0.2f);
					if (check_pos_bomb_player(normalBomb.GetPosition(), player_pos))
					{
						normalBomb.GetCollider().CheckCollision(player, 1.0f, 3, 1);
					}
					if (check_pos_bomb_player(normalBomb.GetPosition(), player2rd_pos))
					{
						normalBomb.GetCollider().CheckCollision(player2rd, 1.0f, 3, 1);
					}
					if (check_pos_bomb_player(normalBomb.GetPosition(), bot_no1_pos))
					{
						normalBomb.GetCollider().CheckCollision(bot_no1, 1.0f, 3, 1);
					}
					if (check_pos_bomb_player(normalBomb.GetPosition(), bot_no2_pos))
					{
						normalBomb.GetCollider().CheckCollision(bot_no2, 1.0f, 3, 1);
					}
					if (check_pos_bomb_player(normalBomb.GetPosition(), bot_no3_pos))
					{
						normalBomb.GetCollider().CheckCollision(bot_no3, 1.0f, 3, 1);
					}
					normalBomb.Draw(window);
				}
				else if (cur_time - Bomb_table[i][j].plant_time > 4.0f && Bomb_table[i][j].explode == 0)
				{
					add_bomb_to_queue(i, j, Bomb_table[i][j].bomb_type);
					Bomb_table[i][j].explode = 1;
					Bomb_table[i][j].exploding_time = cur_time;
					set_table_to_ground(i, j);
				}
			}
		}
	
	while (!queueBombpos.empty())
	{
		while (!queueBombpos.empty())
		{
			thisBombPos = queueBombpos.front();
			//printf("Boom : %d %d\n", thisBombPos.x, thisBombPos.y);
			Explode_table[thisBombPos.y][thisBombPos.x].set = 1;
			Explode_table[thisBombPos.y][thisBombPos.x].pos = set_fire_pos(thisBombPos.y, thisBombPos.x);
			Explode_table[thisBombPos.y][thisBombPos.x].bomb_type = thisBombPos.type;
			Explode_table[thisBombPos.y][thisBombPos.x].time = cur_time;
			Explode_table[thisBombPos.y][thisBombPos.x].type = 1;
			queueBombpos.pop();

			find_fire_path(Bomb_table[thisBombPos.y][thisBombPos.x], cur_time, thisBombPos.y, thisBombPos.x);
			PLAYER[Bomb_table[thisBombPos.y][thisBombPos.x].own].used_bomb--;
		}
	}
	
	while (!queueBlockpos.empty())
	{
		thisBlockPos = queueBlockpos.front();
		queueBlockpos.pop();
		Block_table[thisBlockPos.y][thisBombPos.x].set = 2;
		Block_table[thisBlockPos.y][thisBombPos.x].time = Explode_table[thisBlockPos.y][thisBombPos.x].time;
		Block_table[thisBlockPos.y][thisBombPos.x].type = thisBlockPos.type;
		Block_table[thisBlockPos.y][thisBombPos.x].pos = set_fire_pos(thisBlockPos.y, thisBlockPos.x);
	}
	
	for (int i = 0; i < table_scale_height; i++) {
		for (int j = 0; j < table_scale_width; j++)
		{
			float count_down = cur_time - Explode_table[i][j].time;
			if (Explode_table[i][j].set)
			{
				if (count_down <= 0.1f)
				{
					Fire_exploding(Explode_table[i][j],
						player, player_pos,
						player2rd, player2rd_pos,
						bot_no1, bot_no1_pos,
						bot_no2, bot_no2_pos,
						bot_no3, bot_no3_pos,
						window, cur_time, -1);
				}
				else if (count_down > 0.1f && count_down <= 0.3f)
				{
					Fire_exploding(Explode_table[i][j],
						player, player_pos,
						player2rd, player2rd_pos,
						bot_no1, bot_no1_pos,
						bot_no2, bot_no2_pos,
						bot_no3, bot_no3_pos,
						window, cur_time, Explode_table[i][j].type);
					if (Explode_table[i][j].set == 2)
						Block_exploding(Block_table[i][j], window, cur_time);
				}
				else
				{
					Explode_table[i][j].set = 0;
					Block_table[i][j].set = 0;
				}
			}
			
			if (Item_mark_table[i][j].set == 1)
			{
				if (Item_mark_table[i][j].burn == 0)
				{
					int item_type = Item_mark_table[i][j].type;
					if (item_type == 1) itemTexture.loadFromFile("./Sprite/Item/Bomb-Up.png");
					if (item_type == 2) itemTexture.loadFromFile("./Sprite/Item/Fire-Up.png");
					if (item_type == 3) itemTexture.loadFromFile("./Sprite/Item/Speed-Up.png");
					if (item_type == 4) itemTexture.loadFromFile("./Sprite/Item/Pierce-Bomb.png");
					if (item_type == 5) itemTexture.loadFromFile("./Sprite/Item/Bomb-Down.png");
					if (item_type == 6) itemTexture.loadFromFile("./Sprite/Item/Fire-Down.png");
					if (item_type == 7) itemTexture.loadFromFile("./Sprite/Item/Speed-Down.png");

					MAP_item thisItem(&itemTexture, sf::Vector2f(40.0f, 60.0f), set_fire_pos(i, j));
					if (thisItem.GetCollider().CheckCollision(player, 1.0f, 5, 1))
					{
						Item_mark_table[i][j].set = 0;
						Item_mark_table[i][j].burn = 0;
						player_got_item(Item_mark_table[i][j].type, 0);
					}
					if (thisItem.GetCollider().CheckCollision(player2rd, 1.0f, 5, 1))
					{
						Item_mark_table[i][j].set = 0;
						Item_mark_table[i][j].burn = 0;
						player_got_item(Item_mark_table[i][j].type, 1);
					}
					if (thisItem.GetCollider().CheckCollision(bot_no1, 1.0f, 5, 1))
					{
						Item_mark_table[i][j].set = 0;
						Item_mark_table[i][j].burn = 0;
						player_got_item(Item_mark_table[i][j].type, 2);
					}
					if (thisItem.GetCollider().CheckCollision(bot_no2, 1.0f, 5, 1))
					{
						Item_mark_table[i][j].set = 0;
						Item_mark_table[i][j].burn = 0;
						player_got_item(Item_mark_table[i][j].type, 3);
					}
					if (thisItem.GetCollider().CheckCollision(bot_no3, 1.0f, 5, 1))
					{
						Item_mark_table[i][j].set = 0;
						Item_mark_table[i][j].burn = 0;
						player_got_item(Item_mark_table[i][j].type, 4);
					}
					thisItem.Draw(window);
				}
				else
				{
					itemTexture.loadFromFile("./Sprite/Item/Burn.png");
					if (cur_time - Item_mark_table[i][j].time <= 0.3f)
					{
						Destroy_item thisItem(&itemTexture, set_fire_pos(i, j), sf::Vector2u(4, 1), 0.3f / 4.0f);
						Destroy_item_Anim animationItem(&itemTexture, sf::Vector2u(4, 1), 0.3f / 4.0f);
						thisItem.Update(Item_mark_table[i][j].time, cur_time, 0.3f / 4.0f);
						thisItem.Draw(window);
					}
					else
					{
						Item_mark_table[i][j].set = 0;
						Item_mark_table[i][j].burn = 0;
						Item_mark_table[i][j].time = 0;
						set_table_to_ground(i, j);
					}
				}
			}
		}
	}
	
}

void player_got_item(int type, int player_number)
{
	if (type == 1)
		PLAYER[player_number].bomb++;
	else if (type == 2)
		PLAYER[player_number].fire_range++;
	else if (type == 3 && PLAYER[player_number].speed < 200)
		PLAYER[player_number].speed+=20;
	else if (type == 4)
		PLAYER[player_number].bomb_type = 2;
	else if (type == 5 && PLAYER[player_number].bomb > 1)
		PLAYER[player_number].bomb--;
	else if (type == 6 && PLAYER[player_number].fire_range > 1)
		PLAYER[player_number].fire_range--;
	else if (type == 7 && PLAYER[player_number].speed > 100)
		PLAYER[player_number].speed-=20;
}

int is_same_pos(sf::Vector2f cur, sf::Vector2f des)
{
	float dis = 1.0f;
	if (abs(cur.x - des.x) < dis && abs(cur.y - des.y) < dis)return 1;
	//printf("dis - %f %f\n", abs(cur.y - des.y), abs(cur.x - des.x));
	return 0;
}

void find_fire_path(Bomb_info thisBombExploding, float cur_time, int y, int x)
{
	//Breadth-first search ( BFS )
	int fire_range;
	int fire_dir[4][2] = { {1, 0} , {0, 1} , {-1, 0} , {0, -1} };
	int i, tmp_x, tmp_y, it;
	for (i = 0; i < 4; i++)
	{
		tmp_x = x;
		tmp_y = y;
		fire_range = thisBombExploding.fire_range;
		while (fire_range--)
		{
			tmp_x += fire_dir[i][0];
			tmp_y += fire_dir[i][1];
			if (tmp_x >= 0 && tmp_x < table_scale_width && tmp_y >= 0 && tmp_y < table_scale_height){
				if (Item_mark_table[tmp_y][tmp_x].set == 1)
				{
					Item_mark_table[tmp_y][tmp_x].time = cur_time;
					Item_mark_table[tmp_y][tmp_x].burn = 1;
				}

				it = table[tmp_y][tmp_x].object;
				if ((it == 0 || it == 1 || it == 2) && !Explode_table[tmp_y][tmp_x].set){
					Explode_table[tmp_y][tmp_x].time = cur_time;
					Explode_table[tmp_y][tmp_x].set = 1;
					Explode_table[tmp_y][tmp_x].type = set_fire_type(y, x, tmp_y, tmp_x, fire_range);
					Explode_table[tmp_y][tmp_x].bomb_type = Explode_table[y][x].bomb_type;
					Explode_table[tmp_y][tmp_x].pos = set_fire_pos(tmp_y, tmp_x);

					if (it == 1 || it == 2){
						if (it == 2)
						{
							Item_mark_table[tmp_y][tmp_x].set = 1;
							Item_mark_table[tmp_y][tmp_x].burn = 0;
						}
						
						add_block_to_queue(tmp_y, tmp_x, it);
						set_table_to_ground(tmp_y, tmp_x);
						if (Explode_table[y][x].bomb_type == 1)	break;
					}
				}
				else if (it == 4){
					add_bomb_to_queue(tmp_y, tmp_x, thisBombExploding.bomb_type);
					set_table_to_ground(tmp_y, tmp_x);
				}
				else
					break;
			}
			else
				break;
		}
	}
		
}

void Block_exploding(Block_mark thisBlockExploding, sf::RenderWindow& window, float cur_time)
{
	BombExploding normalBomb(&thisMAP[Map_number].Boom, thisBlockExploding.pos, sf::Vector2u(6, 1), 0.05f);
	BombExplodingAnim bombAnimation(&thisMAP[Map_number].Boom, sf::Vector2u(1, 6), 1.0f/6.0f);
	normalBomb.Update(thisBlockExploding.time, cur_time, 1.0f / 6.0f);
	//normalBomb.GetCollider().CheckCollision(player, 1.0f, 3, 1);
	normalBomb.Draw(window);
}

void Fire_exploding(Explode_mark thisFireExploding ,
	Collider& player, sf::Vector2f player_pos,
	Collider& player2rd, sf::Vector2f player2rd_pos,
	Collider& bot_no1, sf::Vector2f bot_no1_pos,
	Collider& bot_no2, sf::Vector2f bot_no2_pos,
	Collider& bot_no3, sf::Vector2f bot_no3_pos,
	sf::RenderWindow& window, float cur_time, int fire_type)
{
	sf::Texture explodingBombTexture;
	if (thisFireExploding.bomb_type == 1)
	{
		if (fire_type == -1)
			explodingBombTexture.loadFromFile("./Sprite/Bomb/redBombExploding.png");
		if (fire_type == 1)
			explodingBombTexture.loadFromFile("./Sprite/Bomb/redfire_mid.png");
		if (fire_type == 2)
			explodingBombTexture.loadFromFile("./Sprite/Bomb/redfire_horizontal.png");
		if (fire_type == 3)
			explodingBombTexture.loadFromFile("./Sprite/Bomb/redfire_vertical.png");
		if (fire_type == 4)
			explodingBombTexture.loadFromFile("./Sprite/Bomb/redfire_horizontal_left.png");
		if (fire_type == 5)
			explodingBombTexture.loadFromFile("./Sprite/Bomb/redfire_vertical_top.png");
		if (fire_type == 6)
			explodingBombTexture.loadFromFile("./Sprite/Bomb/redfire_horizontal_right.png");
		if (fire_type == 7)
			explodingBombTexture.loadFromFile("./Sprite/Bomb/redfire_vertical_bottom.png");
	}
	else
	{
		if (fire_type == -1)
			explodingBombTexture.loadFromFile("./Sprite/Bomb/blueBombExploding.png");
		if (fire_type == 1)
			explodingBombTexture.loadFromFile("./Sprite/Bomb/bluefire_mid.png");
		if (fire_type == 2)
			explodingBombTexture.loadFromFile("./Sprite/Bomb/bluefire_horizontal.png");
		if (fire_type == 3)
			explodingBombTexture.loadFromFile("./Sprite/Bomb/bluefire_vertical.png");
		if (fire_type == 4)
			explodingBombTexture.loadFromFile("./Sprite/Bomb/bluefire_horizontal_left.png");
		if (fire_type == 5)
			explodingBombTexture.loadFromFile("./Sprite/Bomb/bluefire_vertical_top.png");
		if (fire_type == 6)
			explodingBombTexture.loadFromFile("./Sprite/Bomb/bluefire_horizontal_right.png");
		if (fire_type == 7)
			explodingBombTexture.loadFromFile("./Sprite/Bomb/bluefire_vertical_bottom.png");
	}


	BombExploding normalBomb(&explodingBombTexture, thisFireExploding.pos, sf::Vector2u(4, 1), 0.05f);
	BombExplodingAnim bombAnimation(&explodingBombTexture, sf::Vector2u(1, 4), 0.05f);
	normalBomb.Update(thisFireExploding.time, cur_time, 0.05f);
	if (normalBomb.GetCollider().CheckCollision(player, 1.0f, 4, 1) && PLAYER[0].is_die == 0)
	{
		PLAYER[0].is_die = 1;
		PLAYER[0].lose_time = cur_time;
		PLAYER[0].lose_position = player_pos;
	}
	if (normalBomb.GetCollider().CheckCollision(player2rd, 1.0f, 4, 1) && PLAYER[1].is_die == 0)
	{
		PLAYER[1].is_die = 1;
		PLAYER[1].lose_time = cur_time;
		PLAYER[1].lose_position = player2rd_pos;
	}
	if (normalBomb.GetCollider().CheckCollision(bot_no1, 1.0f, 4, 1) && PLAYER[2].is_die == 0)
	{
		PLAYER[2].is_die = 1;
		PLAYER[2].lose_time = cur_time;
		PLAYER[2].lose_position = bot_no1_pos;
	}
	if (normalBomb.GetCollider().CheckCollision(bot_no2, 1.0f, 4, 1) && PLAYER[3].is_die == 0)
	{
		PLAYER[3].is_die = 1;
		PLAYER[3].lose_time = cur_time;
		PLAYER[3].lose_position = bot_no2_pos;
	}
	if (normalBomb.GetCollider().CheckCollision(bot_no3, 1.0f, 4, 1) && PLAYER[4].is_die == 0)
	{
		PLAYER[4].is_die = 1;
		PLAYER[4].lose_time = cur_time;
		PLAYER[4].lose_position = bot_no3_pos;
	}
	normalBomb.Draw(window);

}

void Bomb_exploding(Bomb_info thisBombExploding, sf::RenderWindow& window, float cur_time, int fire_type)
{
	sf::Texture explodingBombTexture;
	if (thisBombExploding.bomb_type == 1)
	{	
		if (fire_type == -1)
			explodingBombTexture.loadFromFile("./Sprite/Bomb/redBombExploding.png");
		if (fire_type == 1)
			explodingBombTexture.loadFromFile("./Sprite/Bomb/redfire_mid.png");
		if (fire_type == 2)
			explodingBombTexture.loadFromFile("./Sprite/Bomb/redfire_horizontal.png");
		if (fire_type == 3)
			explodingBombTexture.loadFromFile("./Sprite/Bomb/redfire_vertical.png");
		if (fire_type == 4)
			explodingBombTexture.loadFromFile("./Sprite/Bomb/redfire_horizontal_left.png");
		if (fire_type == 5)
			explodingBombTexture.loadFromFile("./Sprite/Bomb/redfire_vertical_top.png");
		if (fire_type == 6)
			explodingBombTexture.loadFromFile("./Sprite/Bomb/redfire_horizontal_right.png");
		if (fire_type == 7)
			explodingBombTexture.loadFromFile("./Sprite/Bomb/redfire_vertical_bottom.png");
	}
	else
	{
		if (fire_type == -1)
			explodingBombTexture.loadFromFile("./Sprite/Bomb/blueBombExploding.png");
		if (fire_type == 1)
			explodingBombTexture.loadFromFile("./Sprite/Bomb/bluefire_mid.png");
		if (fire_type == 2)
			explodingBombTexture.loadFromFile("./Sprite/Bomb/bluefire_horizontal.png");
		if (fire_type == 3)
			explodingBombTexture.loadFromFile("./Sprite/Bomb/bluefire_vertical.png");
		if (fire_type == 4)
			explodingBombTexture.loadFromFile("./Sprite/Bomb/bluefire_horizontal_left.png");
		if (fire_type == 5)
			explodingBombTexture.loadFromFile("./Sprite/Bomb/bluefire_vertical_top.png");
		if (fire_type == 6)
			explodingBombTexture.loadFromFile("./Sprite/Bomb/bluefire_horizontal_right.png");
		if (fire_type == 7)
			explodingBombTexture.loadFromFile("./Sprite/Bomb/bluefire_vertical_bottom.png");
	}


		Bomb normalBomb(&explodingBombTexture, thisBombExploding.position, sf::Vector2u(4, 1), 0.05f);
		BombAnimation bombAnimation(&explodingBombTexture, sf::Vector2u(1, 4), 0.025f);
		normalBomb.Update(thisBombExploding.exploding_time, cur_time, 0.025f);
			//normalBomb.GetCollider().CheckCollision(player, 1.0f, 3, 1);
		normalBomb.Draw(window);

}

int set_fire_type(int y1, int x1, int y2, int x2, int fire)
{
	if (y1 == y2 && x1 != x2)
	{
		if (fire != 0) return 2;
		else
		{
			if (x2 < x1) return 4;
			else return 6;
		}
	}
	else
	{
		if (fire != 0) return 3;
		else
		{
			if (y2 < y1) return 5;
			else return 7;
		}
	}
}


int check_pos_bomb_player(sf::Vector2f this_bomb, sf::Vector2f this_player)
{
	this_bomb.x = ((this_bomb.x - battle_table_x + 25) / 50);
	this_bomb.y = ((this_bomb.y - battle_table_y + 25) / 50);
	int bomb_x = (int)this_bomb.x;
	int bomb_y = (int)this_bomb.y;

	this_player.x = ((this_player.x - battle_table_x + 25) / 50);
	this_player.y = ((this_player.y - battle_table_y + 25) / 50);
	int player_x = (int)this_player.x;
	int player_y = (int)this_player.y;

	if (player_x == bomb_x && player_y == bomb_y) return 0;
	else										  return 1;
}

sf::Vector2f set_fire_pos(int y, int x)
{
	return sf::Vector2f(battle_table_x + (float)x*50, battle_table_y + (float)y*50);
}

sf::Vector2f check_set_bombPosition(sf::Vector2f thisPosition, float cur_time, int own)
{
	thisPosition.x = ((thisPosition.x - battle_table_x + 25) / 50);
	thisPosition.y = ((thisPosition.y - battle_table_y + 25) / 50);
	int pos_x = (int)thisPosition.x;
	int pos_y = (int)thisPosition.y;
	
	if (table[pos_y][pos_x].object == 0)
	{
		Bomb_table[pos_y][pos_x].fire_range = PLAYER[own].fire_range;
		Bomb_table[pos_y][pos_x].bomb_type = PLAYER[own].bomb_type;
		Bomb_table[pos_y][pos_x].plant_time = cur_time;
		Bomb_table[pos_y][pos_x].planted = 1;
		Bomb_table[pos_y][pos_x].explode = 0;
		Bomb_table[pos_y][pos_x].own = own;
		Bomb_table[pos_y][pos_x].position = sf::Vector2f(battle_table_x + (float)pos_x*50, battle_table_y + (float)pos_y*50);
		table[pos_y][pos_x].object = 4;
		table[pos_y][pos_x].meterial = '+';
		PLAYER[own].used_bomb++;
		//printf("\nBomb position :: %d %d\n", battle_table_x + pos_x * 50, battle_table_y + pos_y * 50);
	}
	
	return sf::Vector2f((float)pos_x, (float)pos_y);
}

void set_sprite_map()
{
	// MAP 0
	thisMAP[0].default_glass1.loadFromFile("./Sprite/AllMap/Map_Default/Ground.png");
	thisMAP[0].default_glass2.loadFromFile("./Sprite/AllMap/Map_Default/Ground.png");
	thisMAP[0].immortal.loadFromFile("./Sprite/AllMap/Map_Default/Immortal.png");
	thisMAP[0].Outside.loadFromFile("./Sprite/AllMap/Map_Default/Outside.png");
	thisMAP[0].Object.loadFromFile("./Sprite/AllMap/Map_Default/Object.png");
	thisMAP[0].Boom.loadFromFile("./Sprite/AllMap/Map_Default/Boom.png");
	thisMAP[0].Top.loadFromFile("./Sprite/AllMap/Map_Default/MAP_top_Default.png");
	thisMAP[0].Left.loadFromFile("./Sprite/AllMap/Map_Default/MAP_left_Default.png");
	thisMAP[0].Right.loadFromFile("./Sprite/AllMap/Map_Default/MAP_right_Default.png");
	thisMAP[0].Bottom.loadFromFile("./Sprite/AllMap/Map_Default/MAP_bottom_Default.png");
}

void generate_background(sf::RenderWindow& window, int map)
{
	sf::Vector2f g_scale = sf::Vector2f(50.0f, 50.0f);
	sf::Vector2f g_pos;
	for (int i = 1; i<=table_scale_height; i++)
		for (int j = 1; j <=table_scale_width; j++)
		{
			g_pos = sf::Vector2f((float)battle_table_x + 50 * (j - 1), (float)battle_table_y + 50 * (i - 1));
			MAP_GROUND ground_back(&thisMAP[map].default_glass1, &thisMAP[map].default_glass2, g_scale, g_pos, i, j);
			ground_back.Draw(window);
		}
}

void generate_immortal(sf::RenderWindow& window, Collider& player, Collider& player2rd, Collider& bot_no1, int map)
{
	sf::Vector2f obj_scale = sf::Vector2f(60.0f, 70.0f);
	sf::Vector2f obj_pos;
	
	//-----------------------Outside--------------------------
	sf::Vector2f obj_outside_top = sf::Vector2f((float)battle_table_x - 4*50 - 75, (float)battle_table_y - 6*50 - 75);
	sf::Vector2f obj_outside_left = sf::Vector2f((float)battle_table_x - 4*50 - 75, (float)battle_table_y - 75);

	MAP_outside_top top(&thisMAP[map].Top, sf::Vector2f(1550.0f, 300.0f), obj_outside_top);
	MAP_outside_top left(&thisMAP[map].Left, sf::Vector2f(200.0f, 650.0f), obj_outside_left);

	top.Draw(window);
	left.Draw(window);
	//--------------------------------------------------------

	for (int i = -1; i<=table_scale_height; i++)
		for (int j = -1; j <=table_scale_width; j++)
		{
			
			obj_pos = sf::Vector2f(battle_table_x + 50 * ((float)j), battle_table_y + 50 * ((float)i));
			if (i < 0 || i == table_scale_height || j < 0 || j == table_scale_width)
			{
				Immortal immortal(&thisMAP[map].Outside, obj_scale, obj_pos );
				immortal.GetCollider().CheckCollision(player, 1.0f, 2, 1);
				immortal.GetCollider().CheckCollision(player2rd, 1.0f, 2, 1);
				immortal.GetCollider().CheckCollision(bot_no1, 1.0f, 2, 1);
				immortal.Draw(window);
			}
			else 
			{
				if (table[i][j].object == 1 || table[i][j].object == 2)   // generate object
				{
					MAP_OBJECT object(&thisMAP[map].Object, obj_scale, obj_pos );
					object.GetCollider().CheckCollision(player, 1.0f, 2, 1);
					object.GetCollider().CheckCollision(player2rd, 1.0f, 2, 1);
					object.GetCollider().CheckCollision(bot_no1, 1.0f, 2, 1);
					object.Draw(window);
				}
				else if (table[i][j].object == 99)
				{
					Immortal immortal(&thisMAP[map].immortal, obj_scale, obj_pos);
					immortal.GetCollider().CheckCollision(player, 1.0f, 2, 1);
					immortal.GetCollider().CheckCollision(player2rd, 1.0f, 2, 1);
					immortal.GetCollider().CheckCollision(bot_no1, 1.0f, 2, 1);
					immortal.Draw(window);
				}
			}
		}
	//-----------------------Outside--------------------------
	sf::Vector2f obj_outside_right = sf::Vector2f((float)battle_table_x + 21*50 + 25, (float)battle_table_y - 75);
	sf::Vector2f obj_outside_bottom = sf::Vector2f((float)battle_table_x - 4*50 - 75, (float)battle_table_y + 11*50 + 25);

	MAP_outside_top right(&thisMAP[map].Right, sf::Vector2f(200.0f, 650.0f), obj_outside_right);
	MAP_outside_top bottom(&thisMAP[map].Bottom, sf::Vector2f(1550.0f, 200.0f), obj_outside_bottom);

	right.Draw(window);
	bottom.Draw(window);
	//--------------------------------------------------------

	
}


void set_player_info()
{
	/*
	typedef struct {
		float speed;
		float switch_time;
		int fire_range;
		int bomb;
		int used_bomb;
		int bomb_limit;
		int bomb_type;
		int is_die;
		sf::Texture playerTexture;
		sf::Vector2f spawn_postion;
		//------for bot-------
		int des_x;
		int des_y;
		int have_des;
	}Player_info;
	*/
	sf::Vector2f player_spawn_pos[5] = {
		sf::Vector2f((float)battle_table_x, (float)battle_table_y),
		sf::Vector2f((float)battle_table_x + 20 * 50, (float)battle_table_y),
		sf::Vector2f((float)battle_table_x + 20 * 50, (float)battle_table_y),
		sf::Vector2f((float)battle_table_x, (float)battle_table_y + 10 *50),
		sf::Vector2f((float)battle_table_x + 20 * 50, (float)battle_table_y + 10 * 50),
	};

	for (int i = 0; i < 5; i++)
	{
		PLAYER[i].switch_time = 0.125f;
		PLAYER[i].speed = 100;
		PLAYER[i].fire_range = 1;
		PLAYER[i].used_bomb = 0;
		PLAYER[i].bomb = 1;
		PLAYER[i].bomb_type = 1;
		PLAYER[i].is_die = 0;
		PLAYER[i].have_des = 0;
		PLAYER[i].spawn_postion = player_spawn_pos[i];
	}
}

void set_table_to_ground(int y, int x)
{
	table[y][x].object = 0;
	table[y][x].meterial = '-';
}

void add_bomb_to_queue(int y, int x, int type)
{
	Bomb_explode_pos temp;
	temp.x = x;
	temp.y = y;
	temp.type = type;
	queueBombpos.push(temp);
}

void add_block_to_queue(int y, int x, int type)
{
	thisBlockPos.x = x;
	thisBlockPos.y = y;
	thisBlockPos.type = type;
	queueBlockpos.push(thisBlockPos);
}



//---------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------


// ---------------------------------- Generate map -----------------------
void set_table_to_default();
void generate_blocks(int num_blocks);
void generate_items(int num_items);
void print_to_screen(int scale);
int is_conner(int x, int y);

void generate_map()
{
	set_table_to_default();
	generate_blocks(75);
	//print_to_screen(1);
}

void print_to_screen(int scale)
{
	int k, i, j, m;
	for (k = 0; k<table_scale_height; k++)
	{
		for (i = 0; i<scale; i++)
		{
			for (j = 0; j<table_scale_width; j++)
				for (m = 0; m<scale; m++)printf("%c", table[k][j].meterial);
			printf("\n");
		}
	}

}

void set_table_to_default()
{
	int i, j;

	for (i = 0; i < table_scale_height; i++)
		for (j = 0; j < table_scale_width; j++)
		{
			table[i][j].object = 0;
			Bomb_table[i][j].planted = 0;
			Block_table[i][j].set = 0;
			Explode_table[i][j].set = 0;
			item_table[i][j] = 0;
			Item_mark_table[i][j].set = 0;
			Item_mark_table[i][j].burn = 0;
		}

	for (i = 0; i<table_scale_height; i++)
	{
		for (j = 0; j<table_scale_width; j++)
		{
			if (i>0 && i<table_scale_height && i % 2 == 1 && ((i % 2 == 0 && (i + j) % 2 == 1) || (i % 2 == 1 && (i + j) % 2 == 0)))
			{
				table[i][j].object = 99; //99 = stone
				table[i][j].meterial = ' ';
			}
			else
			{
				table[i][j].object = 0;
				table[i][j].meterial = '-';
			}
		}
	}
}

void generate_blocks(int num_blocks)
{
	int i, j, number, count_way = 0, count_block = 0;
	int mic[50];
	for (i = 0; i<50; i++) mic[i] = 0;
	srand((int)time(NULL));

	for (i = 0; i<table_scale_height; i++)
	{
		count_way = 0;
		for (j = 0; j<table_scale_width; j++)
		{
			if (table[i][j].object == 0 && is_conner(i, j))
			{
				number = (rand() % 100) + 1;
				if (number < num_blocks || count_way >= 2 || mic[j] >= 2)
				{
					table[i][j].object = 1; //1 = Blocks
					table[i][j].meterial = '=';
					count_way = 0;
					mic[j] = 0;
					count_block++;
				}
				else
				{
					count_way++;
					mic[j] ++;
				}
			}
		}
	}
	//printf("Blocks : %d\n", count_block);
	generate_items(count_block);

}

void generate_items(int blocks)
{
	int x, y, item;
	item = blocks * 40 / 100;
	//printf("Items : %d\n", item);
	while (item--)
	{
		while (1)
		{
			y = (rand() % table_scale_width);
			x = (rand() % table_scale_height);
			if (table[x][y].object == 1)
				break;
		}
		table[x][y].object = 2; //2 = item;
		table[x][y].meterial = '?';
		item_table[x][y] = set_to_item_table(x, y);
		Item_mark_table[x][y].type = item_table[x][y];
	}
}

int set_to_item_table(int x, int y)
{
	int random_type, random_item;
	random_type = rand() % 100;
	if (random_type < 80)  // chance 80% to get positive
	{
		random_item = rand() % 4; // Fire-UP  Bomb-UP  Speed-UP  PierceBomb
		return (random_item + 1);
	}
	else   //20% for bad things
	{
		random_item = rand() % 3;  // Speed-DOWN  Fire-DOWN  Bomb_DOWN
		return (4 + random_item + 1);
	}

}


int is_conner(int x, int y)
{
	int i, check[12][2] = { { 0,0 },{ 0,1 },{ 1,0 },
	{ table_scale_height - 1, table_scale_width - 2 },{ table_scale_height - 2, table_scale_width - 1 },{ table_scale_height - 1, table_scale_width - 1 },
	{ 0, table_scale_width - 1 },{ 0, table_scale_width - 2 },{ 1, table_scale_width - 1 },
	{ table_scale_height - 1, 0 },{ table_scale_height - 2, 0 },{ table_scale_height - 1, 1 } };
	for (i = 0; i<12; i++)
		if (x == check[i][0] && y == check[i][1])
			return 0;
	return 1;
}
//----------------------------- End Generate map ---------------------------------------


