#include "Fib2584Ai.h"

Fib2584Ai::Fib2584Ai()
{
}

void Fib2584Ai::initialize(int argc, char* argv[])
{
	this->appear_sequence_ = 0;
	this->table_.load("42.vt");
	srand(time(NULL));
	return;
}


MoveDirection Fib2584Ai::generateMove( int board[4][4] )
{
	MoveDirection randomMove = static_cast<MoveDirection>(rand() % 4);
	return randomMove;
}

int Fib2584Ai::generateEvilMove(int board[4][4])
{
	/*int min_i = 0;
	int min_j = 0;
	long double min_value = 999999.9L;
	for(int i=0;i<4;++i){
		for(int j=0;j<4;++j){
			if( board[i][j] == 0 ){
				if(this->appear_sequence_ == 2)
					board[i][j] = 3;
				else
					board[i][j] = 1;
				state_game game(board);
				long double max_value = -99999.99L;
				for(int k=0;k<4;++k){
					state_game next = game.move(k);
					long double value = this->table_.value(next);
					if( max_value < value ){
						max_value = value;
					}
				}
				if( min_value > max_value ){
					min_i = i;
					min_j = j;
					min_value = max_value;
				}
				board[i][j] = 0;
			}
		}
	}
	this->appear_sequence_ = ( this->appear_sequence_+1 ) % 3;
    return min_i*4+min_j;*/
	state_game game(board);
	int rtn = game.best_evil(this->table_, this->appear_sequence_);
	this->appear_sequence_ = (this->appear_sequence_+1)%3;
	return rtn;
}

void Fib2584Ai::gameOver(int board[4][4], int iScore)
{
	return;
}

/**********************************
You can implement any additional functions
you may need.
**********************************/
