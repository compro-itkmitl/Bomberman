#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <conio.h>
using namespace std;

typedef struct{
	char object[50];
	char meterial;
	int check;
}Table;
Table table[50][50];
int table_scale = 17;

// Random state
int b_max = -1, b_min = 500;
int i_max = -1, i_min = 500;

void set_table_to_default();
void generate_blocks(int num_blocks);
void generate_items(int num_items);
void print_to_screen(int scale);

int is_conner(int x, int y);

int main()
{
	while(1)
	{
		system("CLS");
		set_table_to_default();
		generate_blocks(75);
		print_to_screen(3);
	}
	

	return 0;
}

void print_to_screen(int scale)
{
	int k,i,j,m;
	for(k=0;k<table_scale;k++)
	{
		for(i=0;i<scale;i++)
		{
			for(j=0;j<table_scale;j++)
				for(m=0;m<scale;m++)printf("%c", table[k][j].meterial);
			printf("\n");
		}
	}
	
}

void set_table_to_default()
{
	int i,j;

	for(i=0;i<table_scale;i++)
	{
		for(j=0;j<table_scale;j++)
		{
			if(i>0 && i<table_scale && i%2==1 &&((i%2==0 && (i+j)%2==1) || (i%2==1 && (i+j)%2==0)))
			{
				strcpy(table[i][j].object, "stone");
				table[i][j].meterial = ' ';
				table[i][j].check = 0;
			}
			else
			{
				strcpy(table[i][j].object, "way");
				table[i][j].meterial = '-';
				table[i][j].check = 0;
			}
		}
	}
}

void generate_blocks(int num_blocks)
{
	int i,j,number,count_way = 0, count_block = 0;
	int mic[50];
	for(i=0;i<50;i++) mic[i] = 0;


	for(i=0;i<table_scale;i++)
	{
		count_way = 0;
		for(j=0;j<table_scale;j++)
		{
			if(strcmp(table[i][j].object, "way") == 0 && is_conner(i,j))
			{
				number = (rand() % 100) + 1;
				if(number < num_blocks || count_way >= 2 || mic[j] >= 2)
				{
					strcpy(table[i][j].object, "block");
					table[i][j].meterial = '=';
					table[i][j].check = 1;
					count_way = 0;
					mic[j] = 0;
					count_block ++;
				}
				else
				{
					count_way ++;
					mic[j] ++;
				}
			}
		}	
	}
	printf("Blocks : %d\n", count_block);

	//Random state
	if(count_block > b_max) b_max = count_block;
	if(count_block < b_min) b_min = count_block;
	
	generate_items(count_block);

	printf("\n------------\n");
	printf("Random State\n");
	printf("Blocks %d:%d\n", b_min, b_max);
	printf("Items  %d:%d\n", i_min, i_max);
	printf("\n------------\n");

	
}

void generate_items(int blocks)
{
	int x,y, item;
	item = blocks*33/100;
	printf("Items : %d\n", item);

	//Random state
	if(item > i_max) i_max = item;
	if(item < i_min) i_min = item;

	while(item--)
	{
		while(1)
		{
			x = (rand() % 15);
			y = (rand() % 15);
			if(strcmp(table[x][y].object, "block") == 0)
				break;
		}
		strcpy(table[x][y].object, "item");
		table[x][y].meterial = '?';
		table[x][y].check = 1;
	}
	
}


int is_conner(int x, int y)
{	
	int i, check[12][2] = {{0,0}, 		{0,1}, 		{1,0},
					 	{table_scale-1, table_scale-2}, 	{table_scale-2, table_scale-1}, 	{table_scale-1, table_scale-1},
					 	{0, table_scale-1}, 	{0, table_scale-2}, 	{1, table_scale-1},
					 	{table_scale-1, 0}, 	{table_scale-2, 0}, 	{table_scale-1, 1}};
	for(i=0;i<12;i++)
		if(x == check[i][0] && y == check[i][1])
			return 0;
	return 1;
}


