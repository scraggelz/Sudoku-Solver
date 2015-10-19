/* Sudoku as a CSP
CSP's are made up of three things:
	1.) Variables
	2.) Domains of each variable
	3.) Constraints on the domains/values

Rance Morgan - rm9402@uga.edu
-Variables  = all the spaces in 3x3 sudoku, 2d array/matrix
-Domains 	= Possible values to put in spaces (1-9)
-Constraint = 1.) Unique number in each row, column and block 
*/

#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
//#include "boards.h"

struct elem
{
	int value;
	bool domain[9];
	int row;
	int col;
};

int layout1[9][9];
int layout2[9][9];

//hardcoding given board configurations
void set_layout()
{
	int a,b = 0;
	for(a=0; a<9; a++){
		for(b=0; b<9; b++){
			layout1[a][b] = 0;
			layout2[a][b] = 0;
		}
	}

//setting starting numbers for layouts
	layout1[0][2] = 3;
	layout1[0][4] = 2;
	layout1[0][6] = 6;

	layout1[1][0] = 9;
	layout1[1][3] = 3;
	layout1[1][5] = 5;
	layout1[1][8] = 1;

	layout1[2][2] = 1;
	layout1[2][3] = 8;
	layout1[2][5] = 6;
	layout1[2][6] = 4;

	layout1[3][2] = 8;
	layout1[3][3] = 1;
	layout1[3][5] = 2;
	layout1[3][6] = 9;

	layout1[4][0] = 7;
	layout1[4][8] = 8;

	layout1[5][2] = 6;
	layout1[5][3] = 7;
	layout1[5][5] = 8;
	layout1[5][6] = 2;

	layout1[6][2] = 2;
	layout1[6][3] = 6;
	layout1[6][5] = 9;
	layout1[6][6] = 5;

	layout1[7][0] = 8;
	layout1[7][3] = 2;
	layout1[7][5] = 3;
	layout1[7][8] = 9;

	layout1[8][2] = 5;
	layout1[8][4] = 1;
	layout1[8][6] = 3;


	//second layout
	layout2[0][0] = 7;
	layout2[0][1] = 9;
	layout2[0][3] = 4;
	layout2[0][4] = 6;
	layout2[0][8] = 8;

	layout2[1][1] = 4;
	layout2[1][2] = 6;
	layout2[1][7] = 9;
	layout2[1][8] = 5;

	layout2[2][0] = 2;
	layout2[2][3] = 1;
	layout2[2][5] = 8;
	layout2[2][7] = 7;
	layout2[2][8] = 6;

	layout2[3][1] = 5;
	layout2[3][3] = 8;
	layout2[3][4] = 1;
	layout2[3][5] = 6;
	layout2[3][6] = 3;

	layout2[4][2] = 7;
	layout2[4][4] = 3;
	layout2[4][7] = 4;

	layout2[5][4] = 5;
	layout2[5][6] = 8;
	layout2[5][7] = 6;
	layout2[5][8] = 9;

	layout2[6][0] = 1;
	layout2[6][3] = 9;
	layout2[6][4] = 8;
	layout2[6][6] = 7;

	layout2[7][0] = 9;
	layout2[7][1] = 2;
	layout2[7][2] = 3;
	layout2[7][3] = 5;

	layout2[8][0] = 5;
	layout2[8][3] = 6;
	layout2[8][5] = 3;
	layout2[8][6] = 9;



}

//two boards to use
struct elem Board[9][9];
//struct elem Board2[9][9];

//print out 3x3 boards
void printBoard(struct elem B[][9])
{
	int x, y = 0;
	for(x=0; x<9; x++){
		for(y=0; y<9; y++){
			printf("%d ", B[x][y].value);
			if(y == 2 || y == 5)
				printf(" | ");
			if ((x==2 || x == 5) && y == 8 )
				printf("\n-------------------------");
		}
		printf("\n");
	}
}	

//initialize board, Bnum = board layout number, 1st or 2nd layout
void init_board(struct elem B[][9], int Bnum)
{
	int x,y = 0;
	for(x=0; x<9; x++){
		for(y=0; y<9; y++){
			B[x][y].row=x;
			B[x][y].col=y;
			//swap for layout2 if you want to run other sudoku layout
			if(Bnum == 1){
				if(layout1[x][y] != 0)
					B[x][y].value = layout1[x][y];
				else
					B[x][y].value = 0;
			}

			if(Bnum==2){
				if(layout2[x][y] != 0)	
					B[x][y].value= layout2[x][y];
				else
					B[x][y].value = 0;
			}
		}
	}
}

//need to check block, column and row
void limit_domain(struct elem B[][9])
{
	int x,y,z;
	int a, b;
	
	//check blocks
	for(x=0; x<9; x++){

		for(y=0; y<9; y++){
			struct elem curr = B[x][y];

			//top left corner
			if((x==0&&(y==0||y==3||y==6))||(x==3&&(y==0||y==3||y==6))||
				(x==6&&(y==0||y==3||y==6))){
				for(a=x; a<x+3; a++){
					for(b=y; b<y+3; b++){
						if(B[a][b].value !=0)
							B[x][y].domain[B[a][b].value-1] = true;
					}
				}
			}
	
			//top middle
			if((x==0&&(y==1||y==4||y==7))||(x==3&&(y==1||y==4||y==7))||
				(x==6&&(y==1||y==4||y==7))){
				for(a=x; a<x+3; a++){
					for(b=y-1; b<y+2; b++){
						if(B[a][b].value!=0)
							B[x][y].domain[B[a][b].value-1] = true;
					}
				}

			}

			//top right corner	
			if((x==0&&(y==2||y==5||y==8))||(x==3&&(y==2||y==5||y==8))||
				(x==6&&(y==2||y==5||y==8))){
				for(a=x; a<x+3; a++){
					for(b=y-2; b<y+1; b++){
						if(B[a][b].value!=0)
							B[x][y].domain[B[a][b].value-1] = true;
					}
				}

			}

			//mid left
			if((x==1&&(y==0||y==3||y==6))||(x==4&&(y==0||y==3||y==6))||
				(x==7&&(y==0||y==3||y==6))){
				for(a=x-1; a<x+2; a++){
					for(b=y; b<y+3; b++){
						if(B[a][b].value!=0)
							B[x][y].domain[B[a][b].value-1] = true;
					}
				}

			}

			//mid mid
			if((x==1&&(y==1||y==4||y==7))||(x==4&&(y==1||y==4||y==7))||
				(x==7&&(y==1||y==4||y==7))){
				for(a=x-1; a<x+2; a++){
					for(b=y-1; b<y+2; b++){
						if(B[a][b].value!=0)
							B[x][y].domain[B[a][b].value-1] = true;
					}
				}

			}

			//mid right
			if((x==1&&(y==2||y==5||y==8))||(x==4&&(y==2||y==5||y==8))||
				(x==7&&(y==2||y==5||y==8))){
				for(a=x-1; a<x+2; a++){
					for(b=y-2; b<y+1; b++){
						if(B[a][b].value!=0)
							B[x][y].domain[B[a][b].value-1] = true;
					}
				}

			}

			//bottom left
			if((x==2&&(y==0||y==3||y==6))||(x==5&&(y==0||y==3||y==6))||
				(x==8&&(y==0||y==3||y==6))){
				for(a=x-2; a<x+1; a++){
					for(b=y; b<y+3; b++){
						if(B[a][b].value!=0)
							B[x][y].domain[B[a][b].value-1] = true;
					}
				}

			}

			//bottom mid
			if((x==2&&(y==1||y==4||y==7))||(x==5&&(y==1||y==4||y==7))||
				(x==8&&(y==1||y==4||y==7))){
				for(a=x-2; a<x+1; a++){
					for(b=y-1; b<y+2; b++){
						if(B[a][b].value!=0)
							B[x][y].domain[B[a][b].value-1] = true;
					}
				}

			}

			//bottom right
			if((x==2&&(y==2||y==5||y==8))||(x==5&&(y==2||y==5||y==8))||
				(x==8&&(y==2||y==5||y==8))){
				for(a=x-2; a<x+1; a++){
					for(b=y-2; b<y+1; b++){
						if(B[a][b].value!=0)
							B[x][y].domain[B[a][b].value-1] = true;
					}
				}

			}

			for(a=0; a<9; a++)
			{
				if(B[x][a].value != 0)
					B[x][y].domain[B[x][a].value-1] = true;
				if(B[a][y].value !=0)
					B[x][y].domain[B[a][y].value-1] = true;
			}
	

		}
	}

	/*for(x=0; x<9; x++){
		for(y=0; y<9; y++){
			for(z=0; z<9; z++){
				printf("%d ", B[x][y].domain[z]);
			}
			printf("x=%d y=%d value=%d", x, y, B[x][y].value);
			printf("\n");
		}
	}*/
			
}

bool solve(struct elem B[][9])
{
	bool solved = true;
	int x,y,a,zeros = 0;

	for(x=0; x<9; x++){
		for(y=0; y<9; y++){

			if(B[x][y].value == 0)
				solved=false;

			//looking for naked singles
			for(a=0; a<9; a++){
				if(B[x][y].domain[a] == 0)
					zeros++;
			}
			if(zeros==1){
				for(a=0; a<9; a++){
					if(B[x][y].domain[a] == 0 && B[x][y].value==0){
						B[x][y].value = a+1;
						B[x][y].domain[a] = 1;
						//printf("Changed! %d %d\n", x, y);
					}
				}
			}

			

			//should also look for "squeezes" and other sudoku strats
			//actually not needed for first puzzle

			zeros=0;
		}	
	}
	return solved;
}

			
			
int main(int argc, char *argv[])
{
	if(argc != 2){
		printf("Two arguemnts required: ./program layout(int)\n");
		printf("Layout: 1 for first sudoku board 2 for second.\n");
		return -1;
	}

	int choice = atoi(argv[1]);
	if(choice != 1 && choice != 2){
		printf("Layout can only be 1 or 2.\n");
		return -1;
	}
	
	srand(time(NULL));
	set_layout();

		init_board(Board, choice);
		limit_domain(Board);
		printBoard(Board);
		printf("\n\n");
		while(solve(Board) == false){
			limit_domain(Board);
			solve(Board);
		}
		printBoard(Board);
			
	
}			
