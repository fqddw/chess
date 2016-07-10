#include "stdlib.h"
#include "stdio.h"
#include "string.h"
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
	struct _move_list* next;
}MOVE;
typedef struct _index
{
	int size;
	int index;
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
	int i = 0;
	for(;i<depth;i++)
	{
		treecoord->index[i].index = 0;
		treecoord->index[i].size = 0;
	}
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
CHESS* copychess(CHESS* pchess)
{
	CHESS* newchess = (CHESS*)malloc(sizeof(CHESS));
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
	return newchess;
}
CHESS* getchessbymove(CHESS* pchess,MOVE* move)
{
	int i = move->destx;
	int j = move->desty;
	if(pchess->chess[move->sourcex][move->sourcey] == 0)
	{
		printf("Invalid Move\n");
	}
	CHESS* newchess = copychess(pchess);
	newchess->chess[move->sourcex][move->sourcey] = 0;
	//if(pchess->chess[i][j] == 0)
	{
		newchess->chess[i][j] = pchess->chess[move->sourcex][move->sourcey];
	}
	newchess->turn = newchess->turn == BLACK?RED:BLACK;
	return newchess;
}

int get_side_by_depth(int first_turn,int depth)
{
	int flag = depth%2;
	return (flag == 0)?(first_turn == BLACK?BLACK:RED):(first_turn == BLACK?RED:BLACK);
}

CHESS* getchessbytreecoord(CHESS* pchess,MOVETREE* movetree,TREECOORD* treecoord)
{
	CHESS* pchesslast = copychess(pchess);
	if(!movetree->root)
		return pchesslast;
	int i = 0;
	MOVELIST* movelist = movetree->root;
	MOVE tmpmove = {0,0,0,0,movelist};
	MOVE* move = &tmpmove;
	for(;i<treecoord->depth;i++)
	{
		move = move->next->move_list+treecoord->index[i].index;
		CHESS* pchesscur = getchessbymove(pchesslast,move);
		clearchess(pchesslast);
		pchesslast = pchesscur;
	}
	pchesslast->turn = get_side_by_depth(pchess->turn,treecoord->depth);
	return pchesslast;
}
int is_dead(CHESS* pchess,int side)
{
	return 0;
}
int append_movelist(MOVELIST* movelist,MOVE* move)
{
	int count = movelist->count;
	movelist->count+=1;
	MOVE* new_movelist = (MOVE*)malloc(movelist->count*sizeof(MOVE));
	if(count!=0)
	{
		memcpy(new_movelist,movelist->move_list,count*sizeof(MOVE));
		free(movelist->move_list);
	}
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
									append_movelist(movelist,&move);
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
								append_movelist(movelist,&move);
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
									append_movelist(movelist,&move);
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
								append_movelist(movelist,&move);
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
									append_movelist(movelist,&move);
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
								append_movelist(movelist,&move);
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
									append_movelist(movelist,&move);
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
								append_movelist(movelist,&move);
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
							if(pchess->chess[j-2][i-1] == 0 || pchess->chess[j-2][i-1] & MASK != pchess->turn)
							{
								append_movelist(movelist,&lefttop);
							}
						}
					}
					move.destx = j - 1;
					move.desty = i - 2;
					topleft = move;
					if(invalid_move(j-1,i-2))
						if(!pchess->chess[j][i-1])
							if(pchess->chess[j-1][i-2] == 0 || pchess->chess[j-1][i-2] & MASK != pchess->turn)
								append_movelist(movelist,&topleft);

					move.destx = j - 2;
					move.desty = i + 1;
					leftbottom = move;
					if(invalid_move(j-2,i+1))
						if(!pchess->chess[j-1][i])
							if(pchess->chess[j-2][i+1] ==0 || pchess->chess[j-2][i+1] & MASK != pchess->turn)
								append_movelist(movelist,&leftbottom);

					move.destx = j - 1;
					move.desty = i + 2;
					bottomleft = move;
					if(invalid_move(j-1,i+2))
					{
						if(!pchess->chess[j][i+1])
						{
							if(pchess->chess[j-1][i+2] ==0 || pchess->chess[j-1][i+2] & MASK != pchess->turn)
							{
								append_movelist(movelist,&bottomleft);
							}
						}
					}

					move.destx = j + 1;
					move.desty = i + 2;
					bottomright = move;
					if(invalid_move(j+1,i+2))
						if(!pchess->chess[j][i+1])
							if(pchess->chess[j+1][i+2] == 0 || pchess->chess[j+1][i+2] & MASK != pchess->turn)
								append_movelist(movelist,&bottomright);
					move.destx = j + 2;
					move.desty = i + 1;
					rightbottom = move;
					if(invalid_move(j+2,i+1))
						if(!pchess->chess[j+1][i])
							if(pchess->chess[j+2][i+1] == 0 || pchess->chess[j+2][i+1] & MASK != pchess->turn)
								append_movelist(movelist,&rightbottom);
					move.destx = j + 2;
					move.desty = i - 1;
					righttop = move;
					if(invalid_move(j+2,i-1))
						if(!pchess->chess[j+1][i])
							if(pchess->chess[j+2][i-1] == 0 || pchess->chess[j+2][i-1] & MASK != pchess->turn)
								append_movelist(movelist,&righttop);					

					move.destx = j + 1;
					move.desty = i - 2;
					topright = move;
					if(invalid_move(j+1,i-2))
						if(!pchess->chess[j][i-1])
							if(pchess->chess[j+1][i-2] == 0 || pchess->chess[j+1][i-2] & MASK != pchess->turn)
								append_movelist(movelist,&topright);
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
									append_movelist(movelist,&move);
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
								append_movelist(movelist,&move);
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
									append_movelist(movelist,&move);
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
								append_movelist(movelist,&move);
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
									append_movelist(movelist,&move);
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
								append_movelist(movelist,&move);
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
									append_movelist(movelist,&move);
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
								append_movelist(movelist,&move);
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
						if(pchess->chess[j-2][i-2] == 0 || pchess->chess[j-2][i-2] & MASK != pchess->turn)
							append_movelist(movelist,&lefttop);
					}
					leftbottom.sourcex = j;
					leftbottom.sourcey = i;
					leftbottom.destx = j - 2;
					leftbottom.desty = i + 2;
					if(invalid_move(j-2,i+2))
					{
						if(pchess->chess[j-2][i+2] == 0 || pchess->chess[j-2][i+2] & MASK != pchess->turn)
							append_movelist(movelist,&leftbottom);
					}
					rightbottom.sourcex = j;
					rightbottom.sourcey = i;
					rightbottom.destx = j + 2;
					rightbottom.desty = i + 2;
					if(invalid_move(j+2,i+2))
					{
						if(pchess->chess[j+2][i+2] == 0 || pchess->chess[j+2][i+2] & MASK != pchess->turn)
							append_movelist(movelist,&rightbottom);
					}
					righttop.sourcex = j;
					righttop.sourcey = i;
					righttop.destx = j + 2;
					righttop.desty = i - 2;
					if(invalid_move(j+2,i-2))
					{
						if(pchess->chess[j+2][i-2] == 0 || pchess->chess[j+2][i-2] & MASK != pchess->turn)
							append_movelist(movelist,&righttop);
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
							append_movelist(movelist,&lefttop);
						}
					}

					if(pchess->turn == RED)
					{
						if(i-1>=0 && i-1<=2 && j-1 >=3 && j-1<=5)
						{
							append_movelist(movelist,&lefttop);
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
							append_movelist(movelist,&leftbottom);
						}
					}

					if(pchess->turn == RED)
					{
						if(i+1>=0 && i+1<=2 && j-1 >=3 && j-1<=5)
						{
							append_movelist(movelist,&leftbottom);
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
							append_movelist(movelist,&righttop);
						}
					}

					if(pchess->turn == RED)
					{
						if(i-1>=0 && i-1<=2 && j+1 >=3 && j+1<=5)
						{
							append_movelist(movelist,&righttop);
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
							append_movelist(movelist,&rightbottom);
						}
					}

					if(pchess->turn == RED)
					{
						if(i+1>=0 && i+1<=2 && j+1 >=3 && j+1<=5)
						{
							append_movelist(movelist,&rightbottom);
						}
					}

				}
				break;
				case JIANG:
				{
					MOVE left,right,top,bottom;
					bottom.sourcex = j;
					bottom.sourcey = i;
					bottom.destx = j;
					bottom.desty = i+1;
				}
				break;
				case BING:
				{
					MOVE left,right,top;
					left.sourcex = j;
					left.sourcey = i;
					left.destx = j - 1;
					left.desty = i;
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
MOVELIST* nextmove(CHESS* pchess)
{	
	int count = 0;
	TREECOORD* treecoord = init_treecoord(0);
	MOVETREE* movetree = (MOVETREE*)malloc(sizeof(MOVETREE));
	movetree->root = NULL;
	do
	{
		total++;
		CHESS* pchesscur = getchessbytreecoord(pchess,movetree,treecoord);
		MOVELIST* list_ptr = get_move_list(pchesscur);
		if(list_ptr->count == 0)
		{
			printf("---------------\n");
			return 0;
		}
		int i = 0;
		MOVELIST* available_list = (MOVELIST*)malloc(sizeof(MOVELIST));
		available_list->count = 0;
		available_list->move_list = 0;
		for(;i<list_ptr->count;i++)
		{
			if(i>1)
				break;
			MOVE* move = list_ptr->move_list+i;
			CHESS* new_chess = getchessbymove(pchesscur,move);
			int score = calvalue(new_chess);
			if(score > -2000)
			{
				append_movelist(available_list,move);
			}
			free(new_chess);
		}
		free(list_ptr->move_list);
		free(list_ptr);
		free(pchesscur);
		if(!movetree->root)
			movetree->root = available_list;
		else
		{
			append_to_move_tree(movetree,treecoord,available_list);
		}
		if(is_end(treecoord))
		{
			TREECOORD* newtreecoord = init_treecoord(treecoord->depth+1);
			cleantreecoord(treecoord);
			treecoord = newtreecoord;
			getmovetreesize(movetree,treecoord);
			printf("TotalCount %d %d\n",treecoord->depth-1,count);
			count = 0;
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
	char* data = fen;
	while(line<10)
	{
		int i = 0;
		int chessoffset = 0;
		for(;;i++)
		{
			if(data[i] == ' ')
				return pchess;
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
		CHESS* pchessnow = getchessbymove(pchesscur,movelist->move_list+i);
		if(calvalue(pchessnow)>-2000)
		{
		}
		nextmoverev(pchessnow);
		total--;
		free(pchessnow);
	}
	return 0;
}
int main()
{
	CHESS* pchess = get_chess_from_fen("RNBAKABNR/9/1C5C1/P1P1P1P1P/9/9/p1p1p1p1p/1c5c1/9/rnbakabnr ");
	nextmove(pchess);
}
