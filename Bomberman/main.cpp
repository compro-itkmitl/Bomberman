#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <queue>
#include "PlayerAnimation.h"
#include "Player.h"
#include "Platform.h"
#include "Bomb.h"
#include "BombAnimation.h"
using namespace std;
 
//---------------------Priority queue for generate UI to screen----------------------------
struct Platform_Object {
	sf::Texture thisTexture;
	Player player;
	PlayerAnimation thisPlayerAnim;
	Bomb thisBomb;
	BombAnimation thisBombAnim;
	Platform thisPlatform;
	int type;
	char name[15];

	bool operator < (const Platform_Object& a) const {
		if (type > a.type) return true;
		else return false;
	}
};
//struct Platform_Object AnObject;
priority_queue<Platform_Object> q_object;
//---------------------End queue and struct for UI ------------------------------------------

//---------------------------- Struct for Table(map)------------------------
typedef struct {
	char object;
	char meterial;
	int check;
}Table;
Table table[50][50];
int table_scale_height = 11;
int table_scale_width = 17;
void gen_map();
//---------------------------------- End Table  -----------------------------


int main()
{
	printf("Start Program!!\n");
	sf::RenderWindow window(sf::VideoMode(1024, 768), "SFML works!", sf::Style::Close | sf::Style::Resize);

	sf::Texture playerTexture;
	playerTexture.loadFromFile("WhitePlayer.png");
	
	sf::Texture normalBombTexture;
	normalBombTexture.loadFromFile("Bomb_sprite.png");

	Player player(&playerTexture, sf::Vector2u(3, 7), 0.15f, 100.0f);
	PlayerAnimation animation(&playerTexture, sf::Vector2u(3, 7), 0.125f);    //0.2f is switchTime

	//Bomb normalBomb(&normalBombTexture, sf::Vector2u(4, 1), 0.2f);
	//BombAnimation bombAnimation(&normalBombTexture, sf::Vector2u(1, 4),0.2f);

	//Platform platform1(nullptr, sf::Vector2f(50.0f, 50.0f), sf::Vector2f(75.0f, 75.0f)); //*17*11 512.0f, 384.0f + 42
	//Platform platform2(nullptr, sf::Vector2f(75.0f, 75.0f), sf::Vector2f(500.0f, 100.0f));


	float deltaTime = 0.0f;
	sf::Clock clock;

	gen_map();
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
		
		//normalBomb.Update(deltaTime, 0.2f);
		player.Update(deltaTime, 0.125f); //switch time
		
		//normalBomb.GetCollider().CheckCollision(player.GetCollider(), 0.0f);
		//platform1.GetCollider().CheckCollision(player.GetCollider(), 0.0f);
		//platform2.GetCollider().CheckCollision(player.GetCollider(), 0.0f);

		window.clear(sf::Color::Green);
	
		//normalBomb.Draw(window);
		player.Draw(window);
		//platform1.Draw(window);
		//platform2.Draw(window);
		

		window.display();
	}

	

	printf("End Program\n");

	return 0;
}

// ---------------------------------- Generate map -----------------------
void set_table_to_default();
void generate_blocks(int num_blocks);
void generate_items(int num_items);
void print_to_screen(int scale);
int is_conner(int x, int y);

void gen_map()
{
	set_table_to_default();
	generate_blocks(75);
	print_to_screen(3);
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
				table[i][j].check = 0;
			}
			else
			{
				table[i][j].object = 0;
				table[i][j].meterial = '-';
				table[i][j].check = 0;
			}
		}
	}
}

void generate_blocks(int num_blocks)
{
	int i, j, number, count_way = 0, count_block = 0;
	int mic[50];
	for (i = 0; i<50; i++) mic[i] = 0;


	for (i = 0; i<table_scale_height; i++)
	{
		count_way = 0;
		for (j = 0; j<table_scale_width; j++)
		{
			if (table[i][j].object == 0 && is_conner(i, j))
			{
				srand(time(NULL));
				number = (rand() % 100) + 1;
				if (number < num_blocks || count_way >= 2 || mic[j] >= 2)
				{
					table[i][j].object = 1; //1 = Blocks
					table[i][j].meterial = '=';
					table[i][j].check = 1;
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
	item = blocks * 33 / 100;
	printf("Items : %d\n", item);
	while (item--)
	{
		while (1)
		{
			x = (rand() % 15);
			y = (rand() % 15);
			if (table[x][y].object == 1)
				break;
		}
		table[x][y].object = 2; //2 = item;
		table[x][y].meterial = '?';
		table[x][y].check = 1;
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