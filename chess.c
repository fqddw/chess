#include "stdlib.h"
#include "stdio.h"
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
void printchess(CHESS*);
int invalid_move(MOVE* move)
{
	if(move->sourcex<9 && move->sourcey<10 && move->destx<9 && move->desty<10)
		return 1;
	return 0;
}
int canmove(CHESS* pchess,MOVE* move)
{
}
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
	newchess->chess[move->sourcex][move->sourcey] = 0;
	//if(pchess->chess[i][j] == 0)
	{
		newchess->chess[i][j] = pchess->chess[move->sourcex][move->sourcey];
	}
	newchess->turn == BLACK?RED:BLACK;
	return newchess;
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
	return pchesslast;
}
int is_dead(CHESS* pchess,int side)
{
}
MOVELIST* get_move_list(CHESS* pchess)
{
	MOVE possible_move[20] = {0};
	MOVELIST* movelist= (MOVELIST*)malloc(sizeof(MOVELIST));
	int i = 0;
	int j = 0;
	for(;i<10;i++)
	{
		for(j=0;j<9;j++)
		{
			MOVE movearray[20] = {0};
			int index = 0;
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
						int l = j;
						for(;k<10;k++)
						{
							if(pchess->chess[j][k] != 0)
							{
								if(pchess->chess[j][k] & MASK != pchess->turn)
								{
									MOVE move = {0};
									move.sourcex = j;
									move.sourcey = i;
									move.destx = j;
									move.desty = k;
									move.next = 0;
									possible_move[index] = move;
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
								possible_move[index] = move;
							}
							index++;
						}
						k = i-1;
						for(;k>=0;k--)
						{
							if(pchess->chess[j][k] & MASK == pchess->turn)
							{
								break;
							}
						}
						for(;l<10;l++)
						{
							if(pchess->chess[l][i] & MASK == pchess->turn)
							{
								break;
							}
						}
						l = j;
						for(;l>=0;l--)
						{
							if(pchess->chess[l][i] & MASK == pchess->turn)
							{
								break;
							}
						}

					}
				break;
				case MA:
				{
					MOVE lefttop,leftbottom,topleft,topright,righttop,rightbottom,bottomleft,bottomright;
					lefttop.sourcex = j;
					lefttop.sourcey = i;
					lefttop.destx = j - 1;
					lefttop.desty = i - 2;
					bottomright.sourcex = j;
					bottomright.sourcey = i;
					bottomright.destx = j + 1;
					bottomright.desty = i + 2;
				}
				break;
				case PAO:
				{
					int k = i+1;
					int l = j;
					int flag = 1;
					for(;k<10;k++)
					{
						if(pchess->chess[j][k] != 0)
						{
							if(flag == 2)
							{
								if(pchess->turn == (pchess->chess[j][k]&MASK))
								{
									printf("%d--\n",pchess->chess[j][k]);
									break;
								}
								else
								{
									printf("%d\n",pchess->chess[j][k]);
									MOVE move = {0};
									move.sourcex = j;
									move.sourcey = i;
									move.destx = j;
									move.desty = k;
									move.next = 0;
									possible_move[index] = move;
									printchess(getchessbymove(pchess,&move));
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
								possible_move[index] = move;
							}
						}
						index++;
					}

				}
				break;
				case XIANG:
				{
					MOVE lefttop,leftbottom,righttop,rightbottom;
					lefttop.sourcex = i;
					lefttop.sourcey = j;
					lefttop.destx = i-2;
					lefttop.desty = j-2;
					if(invalid_move(&lefttop))
					{
						if(!is_dead(pchess,RED))
						{
						}
					}
					leftbottom.sourcex = j;
					leftbottom.sourcey = i;
					leftbottom.destx = j + 2;
					leftbottom.desty = i + 2;
					if(invalid_move(&leftbottom))
					{
						if(!is_dead(pchess,RED))
						{
						}
					}

				}
				break;
				case SHI:
				{
					MOVE lefttop,leftbottom,righttop,rightbottom;
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
}
int cleantreecoord(TREECOORD* treecoord)
{
	free(treecoord);
}
MOVELIST* nextmove(CHESS* pchess)
{	
	TREECOORD* treecoord = init_treecoord(1);
	MOVETREE* movetree = (MOVETREE*)malloc(sizeof(MOVETREE));
	movetree->root = NULL;
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
int main()
{
	CHESS* pchess = get_chess_from_fen("RNBAKABNR/9/1C5C1/P1P1P1P1P/9/9/p1p1p1p1p/1c5c1/9/rnbakabnr ");
	nextmove(pchess);
}
