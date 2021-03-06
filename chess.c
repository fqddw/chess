#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "ctype.h"
#include "time.h"
#define JU 0x1
#define MA (0x1<<1)
#define XIANG (0x1<<2)
#define SHI (0x1<<3)
#define JIANG (0x1<<4)
#define PAO (0x1<<5)
#define BING (0x1<<6)
#define MASK (0x1<<7)
#define BLACK MASK
#define RED 0
struct _move_list;
typedef struct _chess
{
	int chess[9][10];
	int turn;
}CHESS;

int getweightbyitem(int item)
{
	switch(item)
	{
		case JU:
			return 1000;
		case JU|MASK:
			return -1000;
		case MA:
			return 430;
		case MA|MASK:
			return -430;
		case PAO:
			return 450;
		case PAO|MASK:
			return -450;
		case XIANG:
			return 200;
		case XIANG|MASK:
			return -200;
		case SHI:
			return 200;
		case SHI|MASK:
			return -200;
	default:
			return 0;
	}
	return 0;
}
int calvalue(CHESS* pchess)
{
	return 0;
	int count = 0;
	int i=0,j=0;
	int score = 0;
	for(i=0;i<9;i++)
	{
		for(j=0;j<10;j++)
		{
			score += getweightbyitem(pchess->chess[i][j]);
		}
	}
	return score;
}

typedef struct _move
{
	int sourcex;
	int sourcey;
	int destx;
	int desty;
	CHESS chess;
	struct _move_list* next;
}MOVE;
typedef struct _index
{
	int size;
	int index;
	int score;
}INDEX;

typedef struct _move_list
{
	int count;
	MOVE* move_list;
}MOVELIST;
typedef struct _treecoord
{
	int depth;
	INDEX* index;
}TREECOORD;
typedef struct _movetree
{
	MOVELIST* root;
}MOVETREE;
void printchess(CHESS*);
int printtreecoord(TREECOORD* treecoord)
{
	int i = 0;
	for(;i<treecoord->depth;i++)
	{
		printf("total depth %d depth %d index %d size %d\n",treecoord->depth,i,treecoord->index[i].index,treecoord->index[i].size);
	}
	return 0;
}

int invalid_move(int x,int y)
{
	if(x<9 && x>=0 && y>=0 && y<10)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
int canmove(CHESS* pchess,MOVE* move)
{
	return 0;
}
TREECOORD* init_treecoord(int depth)
{
	TREECOORD* treecoord = (TREECOORD*)malloc(sizeof(TREECOORD));
	treecoord->depth = depth;
	if(depth == 0)
	{
		treecoord->index = 0;
		return treecoord;
	}
	treecoord->index = (INDEX*)malloc(sizeof(INDEX)*depth);
	memset(treecoord->index,0,depth*sizeof(INDEX));
	return treecoord;
}

int getmovetreesize(MOVETREE* movetree,TREECOORD* treecoord)
{
	int i = 0;
	MOVELIST* movelist = movetree->root;
	for(;i<treecoord->depth;i++)
	{
		treecoord->index[i].size = movelist->count;
		movelist = movelist->move_list[treecoord->index[i].index].next;
	}
	return 0;
}
int incr(MOVETREE* movetree,TREECOORD* treecoord)
{
	int i = 0;
	//getmovetreesize(movetree,treecoord);
	if(treecoord->depth == 0)
		return 1;
	if(treecoord->index[treecoord->depth-1].index<treecoord->index[treecoord->depth-1].size - 1)
	{
		treecoord->index[treecoord->depth-1].index++;
		return 1;
	}
	else
	{
		for(;i<treecoord->depth;i++)
		{
			if(treecoord->index[treecoord->depth-i-1].index<treecoord->index[treecoord->depth-i-1].size - 1)
			{
				treecoord->index[treecoord->depth-i-1].index++;
				treecoord->index[treecoord->depth-i-1].size = 0;
				getmovetreesize(movetree,treecoord);
				return 1;
			}
			else
			{
				treecoord->index[treecoord->depth-i-1].index = 0;
			}
		}
	}
	return 0;
}

int is_end(TREECOORD* treecoord)
{
	int i = 0;
	for(i=0;i<treecoord->depth;i++)
	{
		if(treecoord->index[i].index !=treecoord->index[i].size - 1)
			return 0;
	}
	return 1;
}

int append_to_move_tree(MOVETREE* movetree,TREECOORD* treecoord,MOVELIST* lmovelist)
{
	int i = 0;
	MOVELIST* movelist = movetree->root;
	MOVELIST** needlist = 0;
	for(i=0;i<treecoord->depth;i++)
	{
		needlist = &(movelist->move_list+treecoord->index[i].index)->next;
		movelist = *needlist;
	}
	*needlist = lmovelist;
	return 0;
}
int clearchess(CHESS* pchess)
{
	free(pchess);
	return 0;
}
int copychess(CHESS* pchess,CHESS* newchess)
{
	newchess->turn =pchess->turn;
	int i = 9;
	int j = 10;
	for(i=0;i<9;i++)
	{
		for(j=0;j<10;j++)
		{
			newchess->chess[i][j] = pchess->chess[i][j];
		}
	}
	return 1;
}
int totalchesscount = 0;
int getchessbymove(CHESS* pchess,MOVE* move,CHESS* newchess)
{
	totalchesscount++;
	int i = move->destx;
	int j = move->desty;
	if(pchess->chess[move->sourcex][move->sourcey] == 0)
	{
		printf("Invalid Move\n");
	}
	copychess(pchess,newchess);
	newchess->chess[move->sourcex][move->sourcey] = 0;
	//if(pchess->chess[i][j] == 0)
	{
		newchess->chess[i][j] = pchess->chess[move->sourcex][move->sourcey];
	}
	newchess->turn = newchess->turn == BLACK?RED:BLACK;
	return 1;
}

int get_side_by_depth(int first_turn,int depth)
{
	int flag = depth%2;
	return (flag == 0)?(first_turn == BLACK?BLACK:RED):(first_turn == BLACK?RED:BLACK);
}

CHESS* getchessbytreecoord(CHESS* pchess,MOVETREE* movetree,TREECOORD* treecoord)
{
	if(!movetree->root)
	{
		return pchess;
	}
	int i = 0;
	MOVELIST* movelist = movetree->root;
	MOVE* nullmove = (MOVE*)0;
	MOVE tmpmove = {0};
	memset(&tmpmove,0,sizeof(MOVE));
	tmpmove.chess = *pchess;
	tmpmove.next = movelist;
	MOVE* move = &tmpmove;

	for(;i<treecoord->depth;i++)
	{
		move = move->next->move_list+treecoord->index[i].index;
	}
	return &move->chess;
}
int is_dead(CHESS* pchess,int side)
{
	return 0;
}
int append_movelist(MOVELIST* movelist,MOVE* move,CHESS* originchess)
{
	int count = movelist->count;
	movelist->count+=1;
	MOVE* new_movelist = (MOVE*)malloc(movelist->count*sizeof(MOVE));
	if(count!=0)
	{
		memcpy(new_movelist,movelist->move_list,count*sizeof(MOVE));
		free(movelist->move_list);
	}
	getchessbymove(originchess,move,&move->chess);
	calvalue(&move->chess);
	memcpy(new_movelist+count,move,sizeof(MOVE));
	movelist->move_list = new_movelist;
	return 1;
}
MOVELIST* get_move_list(CHESS* pchess)
{
	MOVELIST* movelist= (MOVELIST*)malloc(sizeof(MOVELIST));
	movelist->count = 0;
	movelist->move_list = 0;
	int i = 0;
	int j = 0;
	for(;i<10;i++)
	{
		for(j=0;j<9;j++)
		{
			int item = 0;
			if((pchess->chess[j][i] & MASK) != pchess->turn || pchess->chess[j][i] == 0)
			{
				continue;
			}

			switch(pchess->chess[j][i] &~MASK)
			{
				case JU:
					{
						int k = i+1;
						int l = j+1;
						for(;k<10;k++)
						{
							if(pchess->chess[j][k] != 0)
							{
								if((pchess->chess[j][k] & MASK) != pchess->turn)
								{
									MOVE move = {0};
									move.sourcex = j;
									move.sourcey = i;
									move.destx = j;
									move.desty = k;
									move.next = 0;
									append_movelist(movelist,&move,pchess);
								}
								break;
							}
							else
							{
								MOVE move = {0};
								move.sourcex = j;
								move.sourcey = i;
								move.destx = j;
								move.desty = k;
								move.next = 0;
								append_movelist(movelist,&move,pchess);
							}
						}
						k = i-1;
						for(;k>=0;k--)
						{
							if(pchess->chess[j][k] != 0)
							{
								if((pchess->chess[j][k] & MASK) != pchess->turn)
								{
									MOVE move = {0};
									move.sourcex = j;
									move.sourcey = i;
									move.destx = j;
									move.desty = k;
									move.next = 0;
									append_movelist(movelist,&move,pchess);
								}
								break;
							}
							else
							{
								MOVE move = {0};
								move.sourcex = j;
								move.sourcey = i;
								move.destx = j;
								move.desty = k;
								move.next = 0;
								append_movelist(movelist,&move,pchess);
							}
						}

						for(;l<9;l++)
						{
							if(pchess->chess[l][i] != 0)
							{
								if((pchess->chess[l][i] & MASK) != pchess->turn)
								{
									MOVE move = {0};
									move.sourcex = j;
									move.sourcey = i;
									move.destx = l;
									move.desty = i;
									move.next = 0;
									append_movelist(movelist,&move,pchess);
								}
								break;
							}
							else
							{
								MOVE move = {0};
								move.sourcex = j;
								move.sourcey = i;
								move.destx = l;
								move.desty = i;
								move.next = 0;
								append_movelist(movelist,&move,pchess);
							}
						}
						l=j-1;
						for(;l>=0;l--)
						{
							if(pchess->chess[l][i] != 0)
							{
								if((pchess->chess[l][i] & MASK) != pchess->turn)
								{
									MOVE move = {0};
									move.sourcex = j;
									move.sourcey = i;
									move.destx = l;
									move.desty = i;
									move.next = 0;
									append_movelist(movelist,&move,pchess);
								}
								break;
							}
							else
							{
								MOVE move = {0};
								move.sourcex = j;
								move.sourcey = i;
								move.destx = l;
								move.desty = i;
								move.next = 0;
								append_movelist(movelist,&move,pchess);
							}
						}

					}
				break;
				case MA:
				{
					MOVE lefttop,leftbottom,topleft,topright,righttop,rightbottom,bottomleft,bottomright;
					MOVE move;
					move.sourcex = j;
					move.sourcey = i;
					move.destx = j - 2;
					move.desty = i - 1;
					lefttop = move;
					if(invalid_move(j-2,i-1))
					{
						if(!pchess->chess[j-1][i])
						{
							if(pchess->chess[j-2][i-1] == 0 || (pchess->chess[j-2][i-1] & MASK) != pchess->turn)
							{
								append_movelist(movelist,&lefttop,pchess);
							}
						}
					}
					move.destx = j - 1;
					move.desty = i - 2;
					topleft = move;
					if(invalid_move(j-1,i-2))
						if(!pchess->chess[j][i-1])
							if(pchess->chess[j-1][i-2] == 0 || (pchess->chess[j-1][i-2] & MASK) != pchess->turn)
								append_movelist(movelist,&topleft,pchess);

					move.destx = j - 2;
					move.desty = i + 1;
					leftbottom = move;
					if(invalid_move(j-2,i+1))
						if(!pchess->chess[j-1][i])
							if(pchess->chess[j-2][i+1] ==0 || (pchess->chess[j-2][i+1] & MASK) != pchess->turn)
								append_movelist(movelist,&leftbottom,pchess);

					move.destx = j - 1;
					move.desty = i + 2;
					bottomleft = move;
					if(invalid_move(j-1,i+2))
					{
						if(!pchess->chess[j][i+1])
						{
							if(pchess->chess[j-1][i+2] ==0 || (pchess->chess[j-1][i+2] & MASK) != pchess->turn)
							{
								append_movelist(movelist,&bottomleft,pchess);
							}
						}
					}

					move.destx = j + 1;
					move.desty = i + 2;
					bottomright = move;
					if(invalid_move(j+1,i+2))
						if(!pchess->chess[j][i+1])
							if(pchess->chess[j+1][i+2] == 0 || (pchess->chess[j+1][i+2] & MASK) != pchess->turn)
								append_movelist(movelist,&bottomright,pchess);
					move.destx = j + 2;
					move.desty = i + 1;
					rightbottom = move;
					if(invalid_move(j+2,i+1))
						if(!pchess->chess[j+1][i])
							if(pchess->chess[j+2][i+1] == 0 || (pchess->chess[j+2][i+1] & MASK) != pchess->turn)
								append_movelist(movelist,&rightbottom,pchess);
					move.destx = j + 2;
					move.desty = i - 1;
					righttop = move;
					if(invalid_move(j+2,i-1))
						if(!pchess->chess[j+1][i])
							if(pchess->chess[j+2][i-1] == 0 || (pchess->chess[j+2][i-1] & MASK) != pchess->turn)
								append_movelist(movelist,&righttop,pchess);					

					move.destx = j + 1;
					move.desty = i - 2;
					topright = move;
					if(invalid_move(j+1,i-2))
						if(!pchess->chess[j][i-1])
							if(pchess->chess[j+1][i-2] == 0 || (pchess->chess[j+1][i-2] & MASK) != pchess->turn)
								append_movelist(movelist,&topright,pchess);
				}
				break;
				case PAO:
				{
					int k = i+1;
					int l = j + 1;
					int flag = 1;
					for(;k<10;k++)
					{
						if(pchess->chess[j][k] != 0)
						{
							if(flag == 2)
							{
								if(pchess->turn == (pchess->chess[j][k]&MASK))
								{
									break;
								}
								else
								{
									MOVE move = {0};
									move.sourcex = j;
									move.sourcey = i;
									move.destx = j;
									move.desty = k;
									move.next = 0;
									append_movelist(movelist,&move,pchess);
									break;
								}
							}
							else
								flag = 2;
						}
						else
						{
							if(flag == 1)
							{
								MOVE move = {0};
								move.sourcex = j;
								move.sourcey = i;
								move.destx = j;
								move.desty = k;
								move.next = 0;
								append_movelist(movelist,&move,pchess);
							}
						}
					}
					k = i-1;
					flag = 1;
					for(;k>=0;k--)
					{
						if(pchess->chess[j][k] != 0)
						{
							if(flag == 2)
							{
								if(pchess->turn == (pchess->chess[j][k]&MASK))
								{
									break;
								}
								else
								{
									MOVE move = {0};
									move.sourcex = j;
									move.sourcey = i;
									move.destx = j;
									move.desty = k;
									move.next = 0;
									append_movelist(movelist,&move,pchess);
									break;
								}
							}
							else
								flag = 2;
						}
						else
						{
							if(flag == 1)
							{
								MOVE move = {0};
								move.sourcex = j;
								move.sourcey = i;
								move.destx = j;
								move.desty = k;
								move.next = 0;
								append_movelist(movelist,&move,pchess);
							}
						}
					}
					l = j+1;
					flag = 1;
					for(;l<9;l++)
					{
						if(pchess->chess[l][i] != 0)
						{
							if(flag == 2)
							{
								if(pchess->turn == (pchess->chess[l][i]&MASK))
								{
									break;
								}
								else
								{
									MOVE move = {0};
									move.sourcex = j;
									move.sourcey = i;
									move.destx = l;
									move.desty = i;
									move.next = 0;
									append_movelist(movelist,&move,pchess);
									break;
								}
							}
							else
								flag = 2;
						}
						else
						{
							if(flag == 1)
							{
								MOVE move = {0};
								move.sourcex = j;
								move.sourcey = i;
								move.destx = l;
								move.desty = i;
								move.next = 0;
								append_movelist(movelist,&move,pchess);
							}
						}
					}
					l = j-1;
					flag = 1;
					for(;l >= 0;l--)
					{
						if(pchess->chess[l][i] != 0)
						{
							if(flag == 2)
							{
								if(pchess->turn == (pchess->chess[l][i]&MASK))
								{
									break;
								}
								else
								{
									MOVE move = {0};
									move.sourcex = j;
									move.sourcey = i;
									move.destx = l;
									move.desty = i;
									move.next = 0;
									append_movelist(movelist,&move,pchess);
									break;
								}
							}
							else
								flag = 2;
						}
						else
						{
							if(flag == 1)
							{
								MOVE move = {0};
								move.sourcex = j;
								move.sourcey = i;
								move.destx = l;
								move.desty = i;
								move.next = 0;
								append_movelist(movelist,&move,pchess);
							}
						}
					}

				}
				break;
				case XIANG:
				{
					MOVE lefttop,leftbottom,righttop,rightbottom;
					lefttop.sourcex = j;
					lefttop.sourcey = i;
					lefttop.desty = i-2;
					lefttop.destx = j-2;
					if(invalid_move(j-2,i-2))
					{
						if(!(pchess->turn == BLACK && (j-2<4))) {
							if(pchess->chess[j-2][i-2] == 0 || (pchess->chess[j-2][i-2] & MASK) != pchess->turn)
								append_movelist(movelist,&lefttop,pchess);
						}
					}
					leftbottom.sourcex = j;
					leftbottom.sourcey = i;
					leftbottom.destx = j - 2;
					leftbottom.desty = i + 2;
					if(invalid_move(j-2,i+2))
					{
						if(!(pchess->turn == BLACK && (j-2<4))) {
							if(pchess->chess[j-2][i+2] == 0 || (pchess->chess[j-2][i+2] & MASK) != pchess->turn)
								append_movelist(movelist,&leftbottom,pchess);
						}
					}
					rightbottom.sourcex = j;
					rightbottom.sourcey = i;
					rightbottom.destx = j + 2;
					rightbottom.desty = i + 2;
					if(invalid_move(j+2,i+2))
					{
						if(!(pchess->turn == RED && (j+2<5))) {
							if(pchess->chess[j+2][i+2] == 0 || (pchess->chess[j+2][i+2] & MASK) != pchess->turn)
								append_movelist(movelist,&rightbottom,pchess);
						}
					}
					righttop.sourcex = j;
					righttop.sourcey = i;
					righttop.destx = j + 2;
					righttop.desty = i - 2;
					if(invalid_move(j+2,i-2))
					{
						if(!(pchess->turn == RED && (j+2<5))) {
							if(pchess->chess[j+2][i-2] == 0 || (pchess->chess[j+2][i-2] & MASK) != pchess->turn)
								append_movelist(movelist,&righttop,pchess);
						}
					}

				}
				break;
				case SHI:
				{
					MOVE lefttop,leftbottom,righttop,rightbottom;
					lefttop.sourcex = j;
					lefttop.sourcey = i;
					lefttop.destx = j-1;
					lefttop.desty = i-1;
					if(pchess->turn == BLACK)
					{
						if(i-1>=7 && i-1<=9 && j-1 >=3 && j-1<=5)
						{
							if(pchess->chess[j-1][i-1] == 0 || (pchess->chess[j-1][i-1] & MASK) != pchess->turn)
								append_movelist(movelist,&lefttop,pchess);
						}
					}

					if(pchess->turn == RED)
					{
						if(i-1>=0 && i-1<=2 && j-1 >=3 && j-1<=5)
						{
							if(pchess->chess[j-1][i-1] == 0 || (pchess->chess[j-1][i-1] & MASK) != pchess->turn)
								append_movelist(movelist,&lefttop,pchess);
						}
					}
					leftbottom.sourcex = j;
					leftbottom.sourcey = i;
					leftbottom.destx = j-1;
					leftbottom.desty = i+1;
					if(pchess->turn == BLACK)
					{
						if(i+1>=7 && i+1<=9 && j-1 >=3 && j-1<=5)
						{
							if(pchess->chess[j-1][i+1] == 0 || (pchess->chess[j-1][i+1] & MASK) != pchess->turn)
								append_movelist(movelist,&leftbottom,pchess);
						}
					}

					if(pchess->turn == RED)
					{
						if(i+1>=0 && i+1<=2 && j-1 >=3 && j-1<=5)
						{
							if(pchess->chess[j-1][i+1] == 0 || (pchess->chess[j-1][i+1] & MASK) != pchess->turn)
								append_movelist(movelist,&leftbottom,pchess);
						}
					}
					righttop.sourcex = j;
					righttop.sourcey = i;
					righttop.destx = j+1;
					righttop.desty = i-1;
					if(pchess->turn == BLACK)
					{
						if(i-1>=7 && i-1<=9 && j+1 >=3 && j+1<=5)
						{
							if(pchess->chess[j+1][i-1] == 0 || (pchess->chess[j+1][i-1] & MASK) != pchess->turn)
								append_movelist(movelist,&righttop,pchess);
						}
					}

					if(pchess->turn == RED)
					{
						if(i-1>=0 && i-1<=2 && j+1 >=3 && j+1<=5)
						{
							if(pchess->chess[j+1][i-1] == 0 || (pchess->chess[j+1][i-1] & MASK) != pchess->turn)
								append_movelist(movelist,&righttop,pchess);
						}
					}
					rightbottom.sourcex = j;
					rightbottom.sourcey = i;
					rightbottom.destx = j+1;
					rightbottom.desty = i+1;
					if(pchess->turn == BLACK)
					{
						if(i+1>=7 && i+1<=9 && j+1 >=3 && j+1<=5)
						{
							if(pchess->chess[j+1][i+1] == 0 || (pchess->chess[j+1][i+1] & MASK) != pchess->turn)
								append_movelist(movelist,&rightbottom,pchess);
						}
					}

					if(pchess->turn == RED)
					{
						if(i+1>=0 && i+1<=2 && j+1 >=3 && j+1<=5)
						{
							if(pchess->chess[j+1][i+1] == 0 || (pchess->chess[j+1][i+1] & MASK) != pchess->turn)
								append_movelist(movelist,&rightbottom,pchess);
						}
					}

				}
				break;
				case JIANG:
				{
					MOVE left,right,top,bottom;
					top.sourcex = j;
					top.sourcey = i;
					top.destx = j;
					top.desty = i-1;
					if(pchess->turn == RED)
					{
						if(i-1>=0)
							if(pchess->chess[j][i-1] == 0 || (pchess->chess[j][i-1] & MASK) != pchess->turn)
								append_movelist(movelist,&top,pchess);
					}
					if(pchess->turn == BLACK)
					{
						if(i-1>=7)
							if(pchess->chess[j][i-1] == 0 || (pchess->chess[j][i-1] & MASK) != pchess->turn)
								append_movelist(movelist,&top,pchess);
					}

					bottom.sourcex = j;
					bottom.sourcey = i;
					bottom.destx = j;
					bottom.desty = i+1;
					if(pchess->turn == RED)
					{
						if(i+1<=2)
							if(pchess->chess[j][i+1] == 0 || (pchess->chess[j][i+1] & MASK) != pchess->turn)
								append_movelist(movelist,&bottom,pchess);
					}
					if(pchess->turn == BLACK)
					{
						if(i+1>=7 && i+1<=9)
							if(pchess->chess[j][i+1] == 0 || (pchess->chess[j][i+1] & MASK) != pchess->turn)
								append_movelist(movelist,&bottom,pchess);
					}
					left.sourcex = j;
					left.sourcey = i;
					left.destx = j-1;
					left.desty = i;
					if(j-1>=3)
							if(pchess->chess[j-1][i] == 0 || (pchess->chess[j-1][i] & MASK) != pchess->turn)
								append_movelist(movelist,&left,pchess);
					right.sourcex = j;
					right.sourcey = i;
					right.destx = j+1;
					right.desty = i;
					if(j+1<=5)
							if(pchess->chess[j-1][i] == 0 || (pchess->chess[j-1][i] & MASK) != pchess->turn)
								append_movelist(movelist,&right,pchess);


				}
				break;
				case BING:
				{
					MOVE left,right,front;
					if((pchess->turn == BLACK && i<5) ||(pchess->turn == RED && i>4))
					{
						left.sourcex = j;
						left.sourcey = i;
						left.destx = j - 1;
						left.desty = i;
						if(j-1>=0)
							if(pchess->chess[j-1][i] == 0 || (pchess->chess[j-1][i] & MASK) != pchess->turn)
								append_movelist(movelist,&left,pchess);
						right.sourcex = j;
						right.sourcey = i;
						right.destx = j + 1;
						right.desty = i;
						if(j+1<9)
							if(pchess->chess[j+1][i] == 0 || (pchess->chess[j+1][i] & MASK) != pchess->turn)
								append_movelist(movelist,&right,pchess);

					}

					if(pchess->turn == BLACK)
					{
						front.sourcex = j;
						front.sourcey = i;
						front.destx = j;
						front.desty = i-1;
						if(i-1>=0)
							if(pchess->chess[j][i-1] == 0 || (pchess->chess[j][i-1] & MASK) != pchess->turn)
								append_movelist(movelist,&front,pchess);
					}

					if(pchess->turn == RED)
					{
						front.sourcex = j;
						front.sourcey = i;
						front.destx = j;
						front.desty = i+1;
						if(i+1<10)
							if(pchess->chess[j][i+1] == 0 || (pchess->chess[j][i+1] & MASK) != pchess->turn)
								append_movelist(movelist,&front,pchess);
					}
				}
				break;
				default:
				;
			}
		}
	}
	return movelist;
}
int cleantreecoord(TREECOORD* treecoord)
{
	if(treecoord->index)
		free(treecoord->index);
	free(treecoord);
	return 0;
}
int total = 0;
TREECOORD * bestmove;
MOVELIST* nextmove(CHESS* pchess)
{	
	int count = 0;
	TREECOORD* treecoord = init_treecoord(0);
	bestmove = init_treecoord(0);
	MOVETREE* movetree = (MOVETREE*)malloc(sizeof(MOVETREE));
	movetree->root = NULL;
	CHESS* pchesscur = pchess;
	do
	{
		if(treecoord->depth>0)
		if(treecoord->index[treecoord->depth-1].index == 0) {
			pchesscur = getchessbytreecoord(pchess,movetree,treecoord);
		}
		MOVELIST* list_ptr = get_move_list(pchesscur);
		if(list_ptr->count == 0)
		{
			printf("---------------\n");
			return 0;
		}
		int i = 0;
		total+=list_ptr->count;
		MOVELIST* available_list = list_ptr;/*(MOVELIST*)malloc(sizeof(MOVELIST));
		available_list->count = 0;
		available_list->move_list = 0;
		for(;i<list_ptr->count;i++)
		{
			MOVE* move = list_ptr->move_list+i;
			CHESS newchess = {0};
			CHESS* pnewchess = &newchess;
			getchessbymove(pchesscur,move,pnewchess);
			int score = calvalue(pnewchess);
			if(treecoord->depth>1)
			if(pchesscur->turn == RED)
			{
				if(score > bestmove->index[treecoord->depth-1].score)
				{
					bestmove->index[treecoord->depth-1].score = score;
				}
			}

			if(treecoord->depth>1)
			if(pchesscur->turn == BLACK)
			{
				if(score < bestmove->index[treecoord->depth-1].score)
				{
					bestmove->index[treecoord->depth-1].score = score;
				}
			}

			if(score > -2000)
			{
				append_movelist(available_list,move);
			}
		}
		free(list_ptr->move_list);
		free(list_ptr);*/
		if(!movetree->root)
			movetree->root = available_list;
		else
		{
			append_to_move_tree(movetree,treecoord,available_list);
		}
		if(is_end(treecoord))
		{
			TREECOORD* newtreecoord = init_treecoord(treecoord->depth+1);
			TREECOORD* tmpbestmove = init_treecoord(treecoord->depth+1);
			memcpy(tmpbestmove->index,bestmove->index,treecoord->depth*sizeof(INDEX));
			cleantreecoord(bestmove);
			bestmove = tmpbestmove;
			cleantreecoord(treecoord);
			treecoord = newtreecoord;
			getmovetreesize(movetree,treecoord);
			count++;
			printf("TotalCount %d %d %d %d\n",treecoord->depth-1,count,totalchesscount,total);
			count = 0;
			if(treecoord->depth == 10)
				exit(0);
		}
		else
		{
			incr(movetree,treecoord);
			count++;
		}
	}while(1);
}

int getchesscode(char code)
{
	switch(code)
	{
		case 'r':
			return JU | BLACK;
		case 'n':
			return MA | BLACK;
		case 'b':
			return XIANG | BLACK;
		case 'a':
			return SHI | BLACK;
		case 'k':
			return JIANG | BLACK;
		case 'c':
			return PAO | BLACK;
		case 'p':
			return BING | BLACK;

		case 'R':
			return JU | RED;
		case 'N':
			return MA | RED;
		case 'B':
			return XIANG | RED;
		case 'A':
			return SHI | RED;
		case 'K':
			return JIANG | RED;
		case 'C':
			return PAO | RED;
		case 'P':
			return BING | RED;

		default:
			return 0;
	}
}

CHESS* get_chess_from_fen(char* fen)
{
	int line = 0;
	CHESS* pchess = (CHESS*)malloc(sizeof(CHESS));
	pchess->turn = RED;
	int i = 0;
	int j = 0;
	for(i=0;i<9;i++)
	{
		for(j=0;j<10;j++)
		{
			pchess->chess[i][j] = 0;
		}
	}
	int flag = 0;
	char* data = fen;
	while(line<10)
	{
		int i = 0;
		int chessoffset = 0;
		for(;;i++)
		{
			if(data[i] == ' ') {
				if(data[i+1] == '1')
					pchess->turn = RED;
				else
					pchess->turn = BLACK;
				return pchess;
			}
			if(data[i] == '/')
			{
				data = data+i+1;
				line++;
				break;
			}
			else
			{
				if(isdigit(data[i]))
				{
					int it = 0;

					int len = data[i] - '0';
					for(;it<len;it++)
					{
						pchess->chess[i+it][line] = 0;
					}
					chessoffset += len-1;
				}
				else
				{
					int code = getchesscode(data[i]);
					pchess->chess[i+chessoffset][line] = code;
				}
			}
		}
	}
	return pchess;
}
void printchess(CHESS* pchess)
{
	int i = 0;
	int j = 0;
	for(j=0;j<10;j++)
	{
		for(i=0;i<9;i++)
		{
			printf("%5d ",pchess->chess[i][j]);
		}
		printf("\n");
	}
}

char* to_fen_string(CHESS* pchess)
{
	int i = 0;
	int j = 0;
	for(;j<10;j++)
	{
		int blank = 0;
		for(i=0;i<9;i++)
		{
			if(pchess->chess[i][j] == 0)
			{
				blank++;
			}
			else
			{
			}
		}
	}
	return 0;
}
int countrev = 0;
int depthrev = 5;
int nextmoverev(CHESS* pchess)
{
	total++;
	if(total>depthrev)
		return 0;
	countrev++;
/*	if(total == 1)
		depthrev++;
*/
	CHESS* pchesscur = pchess;
	MOVELIST* movelist = get_move_list(pchesscur);
	int i = 0;
	for(i=0;i<movelist->count;i++)
	{
		CHESS chessnow = {0};
		CHESS* pchessnow = &chessnow;
		getchessbymove(pchesscur,movelist->move_list+i,pchessnow);
		if(calvalue(pchessnow)>-2000)
		{
		}
		nextmoverev(pchessnow);
		total--;
	}
	return 0;
}

int check_end(CHESS* pchess)
{
	int i = 0;
	int j = 0;
	int irjiang = 0;
	int jrjiang = 0;
	int ibjiang = 0;
	int jbjiang = 0;
	int rflag = 0;
	int bflag = 0;
	for(;i<9;i++)
	{
		for(j=0;j<10;j++)
		{
			if(pchess->chess[i][j] == JIANG)
			{
				irjiang = i;
				jrjiang = j;
				rflag = 1;
			}
			if(pchess->chess[i][j] == (JIANG|BLACK))
			{
				ibjiang = i;
				jbjiang = j;
				bflag = 1;
			}
		}
	}
	if(bflag && rflag)
	{
		if( irjiang == ibjiang)
		{
			int emptylineflag = 0;
			int it = jrjiang+1;
			for(;it<jbjiang;it++)
			{
				if(pchess->chess[irjiang][it] != 0)
				{
					emptylineflag = 1;
					break;
				}
			}
			if(emptylineflag == 0)
			{
				if(pchess->turn == RED)
				{
					return 1;
				}
				if(pchess->turn == BLACK)
				{
					return 2;
				}
			}
		}
		return 0;
	}
	else
	{
		if(bflag)
			return 2;
		if(rflag)
			return 1;
		return 0;
	}
}
int get_random_vs(CHESS* pchessin)
{
	CHESS* pchess = pchessin;
	printchess(pchess);
	CHESS* pchessbk = pchess;
	int round = 0;
	int rwin = 0;
	int bwin = 0;
	int i = 0;
	MOVELIST* rml = get_move_list(pchess);
	while(round < 1000)
	{
		MOVELIST* prev = NULL;
		int index = 0;
		while(pchess)
		{
			MOVELIST* ml = get_move_list(pchess);

			if(prev != NULL){
				free(prev->move_list);
				free(prev);
			}
			prev = ml;
			int count = ml->count;
			int random = rand()%count;
			//printf("count random %d %d\n",count,random);
			MOVE* mv = ml->move_list+random;
			int ckret = 0;
			{
				int mvindex = 0;
				for(mvindex = 0;mvindex<count;mvindex++)
				{
					CHESS* ckchess = &(ml->move_list+mvindex)->chess;
					ckret = 0;
					ckret = check_end(ckchess);
					if(ckret != 0)
					{
						//printf("End Here\n");
						break;
					}
				}
			}
			pchess = &mv->chess;
			index++;
			int ret = check_end(pchess);
			if(index>2000)
				ret = 3;
			if(ckret != 0)
				ret = ckret;
			if(ret == 1)
			{
				rwin++;
				//printchess(pchess);
				//printf("R Win %d\n",index);
				free(ml->move_list);
				free(ml);
				pchess = pchessbk;
				break;
			}
			if(ret == 2)
			{
				bwin++;
				//printchess(pchess);
				//printf("B Win %d\n",index);
				free(ml->move_list);
				free(ml);
				pchess = pchessbk;
				break;
			}
			if(ret == 3)
			{
				//printf("Out Of Step\n");
				free(ml->move_list);
				free(ml);
				pchess = pchessbk;
				break;
			}
			if(ret == 4)
			{
				pchess = pchessbk;
				break;
			}
		}
		round++;
		//printf("Round %d\n",round);
	}
	printf("rwin %d bwin %d\n",rwin,bwin);
	return 0;
}
int main()
{
	srand(time(NULL));
	CHESS* pchess = get_chess_from_fen("RNBAKABNR/9/115C1/P1P1P1P1P/9/9/p1p1p1p1p/1c5c1/9/rCbakabnr 2");
	MOVELIST* plist = get_move_list(pchess);
	int i = 0;
	for(;i<plist->count;i++) {
		get_random_vs(&(plist->move_list+i)->chess);
	}
	//CHESS* pchess = get_chess_from_fen("R2AKABNR/9/1C5C1/P1P1P1P1P/9/9/1111p1111/1c5c1/9/rnbakabnr ");
	//nextmove(pchess);
}
