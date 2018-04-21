#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <typeinfo>
#include <queue>
#include "PlayerAnimation.h"
#include "Player.h"
#include "Player2rd.h"
#include "Platform.h"
#include "Bomb.h"
#include "BombAnimation.h"
#include "BombExploding.h"
#include "BombExplodingAnim.h"
#include "Lose.h"
#include "LoseAnimation.h"
#include "MAP_GROUND.h"
#include "MAP_OBJECT.h"
#include "MAP_outside_top.h"
#include "Collider.h"
#include "Immortal.h"
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
Explode_mark Explode_table[50][50];

typedef struct {
	int type, set;
	float time;
	sf::Vector2f pos;
}Block_mark;
Block_mark Block_table[50][50];

//-----------------------------Player information----------------------
typedef struct {
	float switch_time;
	float speed;
	float lose_time;
	int fire_range;
	int bomb;
	int used_bomb;
	int bomb_limit;
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
Player_info PLAYER[4] ;  //P1 P2 BOT BOT BOT

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
Bomb_info Bomb_table[50][50];


//----------------------------------- Variables for Map-------------------------------
typedef struct {
	char object;
	char meterial;
}Table;
Table table[50][50];
int table_scale_height = 11;
int table_scale_width = 21;

int resolution_x = 1280;
int resolution_y = 960;

int battle_table_x = (resolution_x / 2 - (table_scale_width / 2) * 50);
int battle_table_y = (resolution_y / 2 - (table_scale_height / 2) * 50) + 75;

void generate_map();
void set_sprite_map();
void set_to_item_table(int x, int y);


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
void generate_immortal(sf::RenderWindow& window, Collider& player, Collider& player2rd, int map);
void generate_bomb(sf::RenderWindow& window,
	Collider& player, sf::Vector2f player_pos,
	Collider& player2rd, sf::Vector2f player2rd_pos,
	float cur_time);
void set_table_to_ground(int y, int x);
void add_bomb_to_queue(int y, int x, int type);
void add_block_to_queue(int y, int x, int type);
void Fire_exploding(Explode_mark thisFireExploding,
	Collider& player, sf::Vector2f player_pos,
	Collider& player2rd, sf::Vector2f player2rd_pos,
	sf::RenderWindow& window, float cur_time, int fire_type);
void Bomb_exploding(Bomb_info thisBombExploding, sf::RenderWindow& window, float cur_time, int fire_type);
void Block_exploding(Block_mark thisBlockExploding, sf::RenderWindow& window, float cur_time);
void find_fire_path(Bomb_info thisBombExploding, float cur_time, int y, int x);
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


int main()
{
	
	set_sprite_map();

	printf("Start Program!!\n");
	sf::RenderWindow window(sf::VideoMode(resolution_x, resolution_y), "SFML works!", sf::Style::Close | sf::Style::Resize);

	
	PLAYER[0].playerTexture.loadFromFile("./Sprite/Bomberman/WhitePlayer.png");
	PLAYER[1].playerTexture.loadFromFile("./Sprite/Bomberman/BlackPlayer.png");
	PLAYER[2].playerTexture.loadFromFile("./Sprite/Bomberman/RedPlayer.png");
	PLAYER[3].playerTexture.loadFromFile("./Sprite/Bomberman/BluePlayer.png");
	PLAYER[0].Burning.loadFromFile("./Sprite/Bomberman/Burning.png");
	PLAYER[1].Burning.loadFromFile("./Sprite/Bomberman/Burning.png");
	PLAYER[2].Burning.loadFromFile("./Sprite/Bomberman/Burning.png");
	PLAYER[3].Burning.loadFromFile("./Sprite/Bomberman/Burning.png");
	PLAYER[3].Burning.loadFromFile("./Sprite/Bomberman/Burning.png");
	set_player_info();

	Player player(&PLAYER[0].playerTexture, sf::Vector2u(3, 7), PLAYER[0].switch_time, PLAYER[0].speed, PLAYER[0].spawn_postion);
	PlayerAnimation animation(&PLAYER[0].playerTexture, sf::Vector2u(3, 7), PLAYER[0].switch_time);

	Player2rd player2rd(&PLAYER[1].playerTexture, sf::Vector2u(3, 7), PLAYER[1].switch_time, PLAYER[1].speed, PLAYER[1].spawn_postion);
	PlayerAnimation animation2rd(&PLAYER[1].playerTexture, sf::Vector2u(3, 7), PLAYER[1].switch_time);
	
	float global_time = 0;
	float deltaTime = 0.0f;
	sf::Clock clock;

	generate_map();
	player.planting = 0;
	player2rd.planting = 0;
	while (window.isOpen())
	{
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

		//printf("Planting : %d\n", player.planting);
		if (PLAYER[0].is_die == 0)
		{
			player.Update(deltaTime, PLAYER[0].switch_time); //deltaTime , switch time
			if (player.planting && PLAYER[0].used_bomb+1 <= PLAYER[0].bomb)
			{
				player.planting = 0;
				sf::Vector2f currentPosition = check_set_bombPosition(player.GetPosition(),  global_time, 0);
			}
			player.planting = 0;
		}
		if (PLAYER[1].is_die == 0)
		{
			player2rd.Update(deltaTime, PLAYER[1].switch_time); //deltaTime , switch time
			if (player2rd.planting && PLAYER[1].used_bomb + 1 <= PLAYER[1].bomb)
			{
				player2rd.planting = 0;
				sf::Vector2f currentPosition = check_set_bombPosition(player2rd.GetPosition(), global_time, 1);
			}
			player2rd.planting = 0;
		}

		
		//printf("%d/%d\n", PLAYER[0].used_bomb, PLAYER[0].bomb);
		window.clear(sf::Color::Green);

		
		generate_background(window, Map_number);
		generate_immortal(window, player.GetCollider(), player2rd.GetCollider(), Map_number);
		generate_bomb(window,
			player.GetCollider(), player.GetPosition(),
			player2rd.GetCollider(), player2rd.GetPosition(),
			global_time);

		if (PLAYER[0].is_die == 0)
		{
			player.Draw(window);
		}
		else
		{
			if (global_time - PLAYER[0].lose_time <= 1.50f)
			{
				printf("%f %f\n", global_time, PLAYER[0].lose_time);
				Lose player_lose(&PLAYER[0].Burning, PLAYER[0].lose_position, sf::Vector2u(10, 1), 1.5f / 10.0f);
				LoseAnimation animation(&PLAYER[0].Burning, sf::Vector2u(1, 10), 1.5f / 10.0f);
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
				printf("%f %f\n", global_time, PLAYER[1].lose_time);
				Lose player2rd_lose(&PLAYER[1].Burning, PLAYER[1].lose_position, sf::Vector2u(10, 1), 1.5f / 10.0f);
				LoseAnimation animation(&PLAYER[1].Burning, sf::Vector2u(1, 10), 1.5f / 10.0f);
				player2rd_lose.Update(PLAYER[1].lose_time, global_time, 1.5f / 10.0f);
				player2rd_lose.Draw(window);
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
	float cur_time)
{
	sf::Texture normalBombTexture;
	/*
	if(type == 1)
	normalBombTexture.loadFromFile("./Sprite/Bomb/normalBomb.png");
	else
	normalBombTexture.loadFromFile("./Sprite/Bomb/pierceBomb.png");
	*/

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
						window, cur_time, -1);
				}
				else if (count_down > 0.1f && count_down <= 0.2f)
				{
					Fire_exploding(Explode_table[i][j],
						player, player_pos,
						player2rd, player2rd_pos,
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
			
		}
	}
	
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
				it = table[tmp_y][tmp_x].object;
				if ((it == 0 || it == 1 || it == 2) && !Explode_table[tmp_y][tmp_x].set){
					Explode_table[tmp_y][tmp_x].time = cur_time;
					Explode_table[tmp_y][tmp_x].set = 1;
					Explode_table[tmp_y][tmp_x].type = set_fire_type(y, x, tmp_y, tmp_x, fire_range);
					Explode_table[tmp_y][tmp_x].bomb_type = Explode_table[y][x].bomb_type;
					Explode_table[tmp_y][tmp_x].pos = set_fire_pos(tmp_y, tmp_x);

					if (it == 1 || it == 2){
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
	BombExplodingAnim bombAnimation(&explodingBombTexture, sf::Vector2u(1, 4), 0.025f);
	normalBomb.Update(thisFireExploding.time, cur_time, 0.025f);
	if (normalBomb.GetCollider().CheckCollision(player, 1.0f, 4, 1))
	{
		PLAYER[0].is_die = 1;
		PLAYER[0].lose_time = cur_time;
		PLAYER[0].lose_position = player_pos;
	}
	if (normalBomb.GetCollider().CheckCollision(player2rd, 1.0f, 4, 1))
	{
		PLAYER[1].is_die = 1;
		PLAYER[1].lose_time = cur_time;
		PLAYER[1].lose_position = player2rd_pos;
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
		Bomb_table[pos_y][pos_x].fire_range = PLAYER[0].fire_range;
		Bomb_table[pos_y][pos_x].bomb_type = PLAYER[0].bomb_type;
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

void generate_immortal(sf::RenderWindow& window, Collider& player, Collider& player2rd, int map)
{
	sf::Vector2f obj_scale = sf::Vector2f(60.0f, 70.0f);
	sf::Vector2f obj_pos;
	
	//-----------------------Outside--------------------------
	sf::Vector2f obj_outside_top = sf::Vector2f(battle_table_x - 4*50 - 75, battle_table_y - 6*50 - 75);
	sf::Vector2f obj_outside_left = sf::Vector2f(battle_table_x - 4*50 - 75, battle_table_y - 75);

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
				immortal.Draw(window);
			}
			else 
			{
				if (table[i][j].object == 1 || table[i][j].object == 2)   // generate object
				{
					MAP_OBJECT object(&thisMAP[map].Object, obj_scale, obj_pos );
					object.GetCollider().CheckCollision(player, 1.0f, 2, 1);
					object.GetCollider().CheckCollision(player2rd, 1.0f, 2, 1);
					object.Draw(window);
				}
				else if (table[i][j].object == 99)
				{
					Immortal immortal(&thisMAP[map].immortal, obj_scale, obj_pos);
					immortal.GetCollider().CheckCollision(player, 1.0f, 2, 1);
					immortal.GetCollider().CheckCollision(player2rd, 1.0f, 2, 1);
					immortal.Draw(window);
				}
			}
		}
	//-----------------------Outside--------------------------
	sf::Vector2f obj_outside_right = sf::Vector2f(battle_table_x + 21*50 + 25, battle_table_y - 75);
	sf::Vector2f obj_outside_bottom = sf::Vector2f(battle_table_x - 4*50 - 75, battle_table_y + 11*50 + 25);

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
	sf::Vector2f player_spawn_pos[4] = {
		sf::Vector2f((float)battle_table_x, (float)battle_table_y),
		sf::Vector2f((float)battle_table_x + 20 * 50, (float)battle_table_y),
		sf::Vector2f((float)battle_table_x, (float)battle_table_y + 10 *50),
		sf::Vector2f((float)battle_table_x + 20 * 50, (float)battle_table_y + 10 * 50),
	};

	for (int i = 0; i < 4; i++)
	{
		PLAYER[i].switch_time = 0.125f;
		PLAYER[i].speed = 100.0f;
		PLAYER[i].fire_range = 2;
		PLAYER[i].used_bomb = 0;
		PLAYER[i].bomb = 1;
		PLAYER[i].bomb_limit = 10;
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
	print_to_screen(2);
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
			table[i][j].object = 0;

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
	printf("Blocks : %d\n", count_block);
	generate_items(count_block);

}

void generate_items(int blocks)
{
	int x, y, item;
	item = blocks * 40 / 100;
	printf("Items : %d\n", item);
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
		set_to_item_table(x, y);
	}
}

void set_to_item_table(int x, int y)
{

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