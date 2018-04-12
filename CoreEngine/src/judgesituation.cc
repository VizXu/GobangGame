#include "judgesituation.h"
#include <cstdlib>
#include <unistd.h>
#include <memory.h>

void Info::set_flag(int f)
{
   this->flag = f;
};

int Info::get_flag() const
{
   return this->flag;
};

int Info::get_xpos() const
{
   return this->x_pos;
};

int Info::get_ypos() const
{
   return this->y_pos;
};

int Info::get_direction() const
{
   return this->direction;
};

void Info::set_xpos(int xpos)
{
   this->x_pos = xpos;
};

void Info::set_index(int index)
{
   this->index = index;
};

void Info::set_ypos(int ypos) 
{
   this->y_pos = ypos;
};

void Info::set_direction(int direction)
{
   this->direction = direction;
};

const Info& Info::operator()(int _x = 0, int _y = 0, int _d = 0)
{
   return std::move(Info(_x,_y,_d));
}

void Info::set(int xpos, int ypos, int direction)
{
   this->set_xpos(xpos);
   this->set_ypos(ypos);
   this->set_direction(direction);
}

void Info::set(int index, int direction)
{
   this->set_index(index);
   
   this->set_xpos(index/BOARD_SIZE);
   this->set_ypos(index%BOARD_SIZE);
   
   this->set_flag(1);
   this->set_direction(direction);
}

bool Info::operator==(const Info& info)
{
   return this->x_pos == info.get_xpos() && this->y_pos == info.get_ypos() && this->direction == info.get_direction();
}

PosInfo Chessmen_info::getiHasWonInfo(int which) const
{
	int x    = this->ihas_won[which]->get_xpos();
	int y    = this->ihas_won[which]->get_ypos();
	int dir  = this->ihas_won[which]->get_direction();
	int flag = this->ihas_won[which]->get_flag();
	PosInfo posInfo(x,y,dir,flag);
return posInfo;
}

Chessmen_info::Chessmen_info()
{
   memset(chessmen_of_each_direction,0,sizeof(chessmen_of_each_direction));
   for(int i = 0; i < BOARD_SIZE*BOARD_SIZE; i++){
	ihas_won[i] = new Info();
	ilive4[i] = new Info();
	ilive4heap[i] = new Info();
	ilive3[i] = new Info();
	isleep3[i] = new Info();
	ilive2[i] = new Info();
	isleep2[i] = new Info();
   }
}

Chessmen_info::~Chessmen_info()
{
   for(int i = 0; i < BOARD_SIZE*BOARD_SIZE; i++){
	delete ihas_won[i];
	delete ilive4[i];
	delete ilive4heap[i];
	delete ilive3[i];
	delete isleep3[i];
	delete ilive2[i];
	delete isleep2[i];
   }
}
void Chessmen_info::dumpiLivesInfo() const
{
	for(int i = 0; i < TOTALPOSSIZE; i++){
		PosInfo posInfo = this->getiHasWonInfo(i);
		if(1 == posInfo.getFlag()){
			printf("ihas_won: x = %d, y = %d, direction = %d\n",posInfo.getxPos(),posInfo.getyPos(),posInfo.getDirection());
		}
	}
};

JudgeWin::JudgeWin()
{
  this->be_won = false;
  this->winner = '+';
  for(int i = 0;i < BOARD_SIZE; i++){
      for(int j = 0; j < BOARD_SIZE; j++){
            this->board_for_judge[i][j]= '+';
	 for(int k = 0 ; k < 8; k++){
	    this->judge_panel[i][j][k] = 0;
	 }
      }
  }
}

JudgeWin::~JudgeWin(){

}

void JudgeWin::copy_chessboard(const Chessboard& chessboard)
{
    for(int i = 0;i < BOARD_SIZE; i++){
      for(int j = 0; j < BOARD_SIZE; j++){
            this->board_for_judge[i][j]= chessboard.get_chess(i,j);
      }
    }
}

void JudgeWin::analysis_ilives(Chessmen_info* info, int level, int index, int direction)
{
	if(-1 == index || index > TOTALPOSSIZE){
		printf("error, index = %d\n",index);
		return;
	}
	
	switch(level){
	   case IHAS_WON:info->ihas_won[index]->set(index,direction);break; 
	   case ILIVE4:info->ilive4[index]->set(index,direction);break; 
	   case ILIVE4HEAP:info->ilive4heap[index]->set(index,direction);break; 
	   case ILIVE3:info->ilive3[index]->set(index,direction);break; 
	   case ISLEEP3:info->isleep3[index]->set(index,direction);break; 
	   case ILIVE2:info->ilive2[index]->set(index,direction);break; 
	   case ISLEEP2:info->isleep2[index]->set(index,direction);break;
	   default:break; 
	}	
}

void JudgeWin::rotate(int r)
{
	if(ROTATE0 == r){
		rotate0(this->board_for_judge);
	}
	else if(ROTATE90 == r){
		rotate90(this->board_for_judge);
	}
	else if(ROTATE180 == r){
		rotate180(this->board_for_judge);
	}
	else if(ROTATE270 == r){
		rotate270(this->board_for_judge);
	}
}

void JudgeWin::rotate0(s8 originBoard[][BOARD_SIZE])
{
	for(int i = 0; i < BOARD_SIZE; i++){
		for(int j = 0; j < BOARD_SIZE; j++){
			this->each_direction_board_for_judge[i][j] = originBoard[i][j];
		}
	}	
}

void JudgeWin::rotate90(s8 originBoard[][BOARD_SIZE])
{
	for(int i = 0; i < BOARD_SIZE; i++){
		for(int j = 0; j < BOARD_SIZE; j++){
			this->each_direction_board_for_judge[i][j] = originBoard[BOARD_SIZE - 1 - j][i];
		}
	}	
}

void JudgeWin::rotate180(s8 originBoard[][BOARD_SIZE])
{
	for(int i = 0; i < BOARD_SIZE; i++){
		for(int j = 0; j < BOARD_SIZE; j++){
			this->each_direction_board_for_judge[i][j] = originBoard[BOARD_SIZE - 1 - i][BOARD_SIZE - 1 - j];
		}
	}	
}

void JudgeWin::rotate270(s8 originBoard[][BOARD_SIZE])
{
	for(int i = 0; i < BOARD_SIZE; i++){
		for(int j = 0; j < BOARD_SIZE; j++){
			this->each_direction_board_for_judge[i][j] = originBoard[j][BOARD_SIZE - 1 - i];
		}
	}	
}

char JudgeWin::getOpesiteChessType(char c) const
{
	if('&' == c) return '@';
	else if('@' == c) return '&';
	else return '+';
}


int JudgeWin::transferIndex(int i, int j, int rotate)
{
	int index = -1;

	if(ROTATE0 == rotate){
		index = i*BOARD_SIZE + j;
	}
	else if(ROTATE90 == rotate){  //90
		index = (BOARD_SIZE - 1 - j)*BOARD_SIZE + i;
	}
	else if(ROTATE180 == rotate){  //180
		index = (BOARD_SIZE - 1 - i)*BOARD_SIZE + (BOARD_SIZE - 1 - j);
	}
	else if(ROTATE270 == rotate){  //270
		index = j*BOARD_SIZE + (BOARD_SIZE - 1 - i);
	}
	
return index;
}

void JudgeWin::dumpIndex(int rotate)
{
	for(int i = 0; i < BOARD_SIZE; i++){
		for(int j = 0; j < BOARD_SIZE; j++){
			printf("rotate: %d, index0 = %d, index = %d\n",rotate,i*BOARD_SIZE + j, this->transferIndex(i,j,rotate));
		}
	}
}

void JudgeWin::analysis_for_each_direction(char chessman, s8 temp[][BOARD_SIZE], Chessmen_info * info,int rotate)
{
	int x_tmp;
	int y_tmp;
	int index = -1;
	char oChessman = this->getOpesiteChessType(chessman);
	char emptyTypeChessman = '+';

	for(int i = 0; i < BOARD_SIZE; i++){
		 for(int j = 0; j < BOARD_SIZE; j++){
		 	 //printf("xujiwei--->info1\n");
			 index = this->transferIndex(i, j, rotate);
			 if(temp[i][j] == chessman){
				 // -x direction(up)
				 int k = 0;
				 int l = 0;
			 	 int m = 0;
				 int n = 0;
				 int o = 0;
				 int p = 0; // = k - 1;
				 int q = 0; // = k + 1;
				 int r = 0;
				 int s = 0;
				 int t = 0;
				 int u = 0;
				 int v = 0;
				 int w = 0;
				 int x = 0;
				 int y = 0;
				 int z = 0;
				 int a = 0;
				 for(k = i; k >= 0; k--){
					if(temp[k][j] == chessman) info->chessmen_of_each_direction[i][j][rotate] += 1;
					else break;
				 }

				 l = k - 5;
				 m = k - 4;
				 n = k - 3;
				 o = k - 2;
				 p = k - 1;
				 q = k + 1;
				 r = k + 2;
				 s = k + 3;
				 t = k + 4;
				 u = k + 5;
				 
						 
				 switch(info->chessmen_of_each_direction[i][j][rotate]){
					case 5:{
					 	this->analysis_ilives(info,IHAS_WON,index,rotate);
					}break;
					
					case 4:{
						 if((k >= 0 && temp[k][j] == chessman)){
						 	//printf("k = %d\n",k);
							this->analysis_ilives(info,IHAS_WON,index,rotate);
						 }
						 else{
						 	//printf("opsite chessman = %c\n",oChessman);
						 	if(((k >= 0 && temp[k][j] == oChessman) || (k < 0)) && ((k + 5 < BOARD_SIZE && temp[k + 5][j] == oChessman) || (k + 5 >= BOARD_SIZE))){
					 	 		//this->analysis_ilives(info,ILIVE4HEAP,index,rotate);
						 	}
							else if((k >= 0 && temp[k][j] == oChessman) || (k + 5 < BOARD_SIZE && temp[k + 5][j] == oChessman)){
								this->analysis_ilives(info,ILIVE4HEAP,index,rotate);
							}
							else{
								this->analysis_ilives(info,ILIVE4,index,rotate);
							}
						 }
					}break;
					
					case 3:{
						 if(p < BOARD_SIZE && temp[p][j] == chessman){
							 this->analysis_ilives(info,ILIVE4HEAP,index,rotate);
						 }
				
						 this->analysis_ilives(info,ILIVE3,index,rotate);
					}break;
					
					case 2:{
						 if(p < BOARD_SIZE && temp[p][j] == chessman){
							if(q <= BOARD_SIZE && temp[q][j] == chessman){
								this->analysis_ilives(info,ILIVE4HEAP,index,rotate);
							}
							else{
								this->analysis_ilives(info,ISLEEP3,index,rotate);
							}
						 }
						 this->analysis_ilives(info,ILIVE2,index,rotate);
					}break;
					
					case 1:{

					}
					break;
					default:break;
				 }
			 }
			 else{ continue;}
		 }
	}

}

Chessmen_info* JudgeWin::scan_analysis_chessmen(char chessman)
{
   int chessmen_of_each_direction[BOARD_SIZE][BOARD_SIZE][8] = { 0 };
   Chessmen_info* info = new Chessmen_info();

   this->rotate(ROTATE0);
   this->analysis_for_each_direction(chessman,this->each_direction_board_for_judge,info,ROTATE0);
   this->rotate(ROTATE90);
   this->analysis_for_each_direction(chessman,this->each_direction_board_for_judge,info,ROTATE90);
   this->rotate(ROTATE180);
   this->analysis_for_each_direction(chessman,this->each_direction_board_for_judge,info,ROTATE180);
   this->rotate(ROTATE270);
   this->analysis_for_each_direction(chessman,this->each_direction_board_for_judge,info,ROTATE270);
   
return info;
}

bool JudgeWin::won_the_game(char chessman) const
{
   //judge analysis

return true;
}

char JudgeWin::whois_winner() const
{
   if(be_won) return this->winner;
   return '+';
}
