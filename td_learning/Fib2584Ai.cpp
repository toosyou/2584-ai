#include "Fib2584Ai.h"

Fib2584Ai::Fib2584Ai()
{
}

void Fib2584Ai::initialize(int argc, char* argv[])
{
	srand(time(NULL));
	tb.load(argv[2]);
	return;
}


MoveDirection Fib2584Ai::generateMove(int board[4][4])
{
	static state_game last_game;
	state_game game(board);

	if(last_game == game){
		game.print();
		exit(-1);
	}
	last_game = game;

	//choose the move that generate the max value
	int move = 0;
	long double max_value = -99999.99L;
	for(int i=0;i<4;++i){
		state_game next_game = game.move(i);
		if(next_game.valid == true){
			long double next_value = tb.value(next_game);
			if(max_value <= next_value){
				move = i;
				max_value = next_value;
			}
		}
	}
	return (MoveDirection)move;
}

void Fib2584Ai::gameOver(int board[4][4], int iScore)
{
	return;
}

/**********************************
You can implement any additional functions
you may need.
**********************************/
