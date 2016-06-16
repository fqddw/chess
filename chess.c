#include "stdlib.h"
#include "stdio.h"
#define JU 0x1
#define MA 0x10
#define XIANG 0x100
#define SHI 0x1000
#define JIANG 0x10000
struct _move_list;
typedef struct _chess
{
	char chess[9][10];
	int turn:1;
}CHESS;
int calvalue()
{
	int i=0,j=0;
	return 0;
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
TREECOORD* init_treecoord(int depth)
{
	TREECOORD* treecoord = (TREECOORD*)malloc(sizeof(TREECOORD));
	treecoord->depth = depth;
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
	MOVE tmpmove = {0,0,0,0,movelist};
	MOVE* move = &tmpmove;
	for(;i<treecoord->depth;i++)
	{
		move = move->next->move_list+i;
		treecoord->index[i].size = movelist->count;
	}
	return 0;
}
int incr(MOVETREE* movetree,TREECOORD* treecoord)
{
	int i = 0;
	for(;i<treecoord->depth;i++)
	{
		if(treecoord->index[treecoord->depth-i].size == 0)
		{
			getmovetreesize(movetree,treecoord);
		}
		if(treecoord->index[treecoord->depth-i].index<treecoord->index[treecoord->depth-i].size - 1)
		{
			treecoord->index[treecoord->depth-i].index++;
			return 1;
		}
	}
}

int is_end(TREECOORD* treecoord)
{
	int i = 0;
	for(;i<treecoord->depth;i++)
	{
		if(treecoord->index[treecoord->depth-i].index!=treecoord->index[treecoord->depth-i].size - 1)
		{
			return 0;
		}
	}
	return 1;

}

int append_to_move_tree(MOVETREE* movetree,TREECOORD* treecoord,MOVE* move)
{
}
int clearchess(CHESS* pchess)
{
	free(pchess);
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
	CHESS* newchess = copychess(pchess);
	if(pchess->chess[i][j] == 0)
	{
		newchess->chess[i][j] = pchess->chess[i][j];
	}
	newchess->turn == 0?1:0;
	return newchess;
}

CHESS* getchessbytreecoord(CHESS* pchess,MOVETREE* movetree,TREECOORD* treecoord)
{
	int i = 0;
	CHESS* pchesslast = copychess(pchess);
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
}
MOVELIST* get_move_list(CHESS* pchess)
{
	int i = 0;
	int j = 0;
	for(;i<10;i++)
	{
		for(j=0;j<9;j++)
		{
			switch(pchess->chess[j][i])
			{
				case JU:
				int k = i;
				int l = j;
				for(;k<10;k++)
				{
					if(pchess->chess[j][k] & MASK == pchess->turn)
					{
						break;
					}
				}
				k = i;
				for(;k>=0;k--)
				{
				}
				break;
				case MA:
				break;
				default:
				;
			}
		}
	}
}
int cleantreecoord(TREECOORD* treecoord)
{
	free(treecoord);
}
MOVELIST* nextmove(CHESS* pchess)
{	
	TREECOORD* treecoord = init_treecoord(1);
	MOVETREE* movetree;
	do
	{
		CHESS* pchesscur = getchessbytreecoord(pchess,movetree,treecoord);
		MOVELIST* list_ptr = get_move_list(pchesscur);
		int i = 0;
		for(;i<list_ptr->count;i++)
		{
			CHESS* new_chess = getchessbymove(pchesscur,list_ptr->move_list+i);
			if(calvalue()> -1000)
			{
				append_to_move_tree(movetree,treecoord,list_ptr->move_list+i);
			}
		}
		incr(movetree,treecoord);
		if(is_end(treecoord))
		{
			TREECOORD* newtreecoord = init_treecoord(treecoord->depth+1);
			cleantreecoord(treecoord);
			treecoord = newtreecoord;
		}
	}while(1);
}


int main()
{
	nextmove(0);
}
