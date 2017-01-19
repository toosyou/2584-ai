#include "Fib2584Ai.h"

Fib2584Ai::Fib2584Ai()
{
}

void Fib2584Ai::initialize(int argc, char* argv[]){
	this->appear_sequence_ = 2;
	this->table_.load("table.vt");
	srand(time(NULL));
	return;
}


MoveDirection Fib2584Ai::generateMove( int board[4][4] )
{
	state_game game(board);
	return (MoveDirection)game.best_move(this->table_, this->appear_sequence_, true);
}

int Fib2584Ai::generateEvilMove(int board[4][4]){
	state_game game(board);
	int rtn = game.best_evil(this->table_, this->appear_sequence_);
	this->appear_sequence_ = (this->appear_sequence_+1)%3;
	return rtn;
}

void Fib2584Ai::gameOver(int board[4][4], int iScore){
	this->appear_sequence_ = 0;
	return;
}

/**********************************
You can implement any additional functions
you may need.
**********************************/
