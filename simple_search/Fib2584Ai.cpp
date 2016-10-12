#include "Fib2584Ai.h"

#define MAX_SEARCH_DEPTH 2

Fib2584Ai::Fib2584Ai(){
}

void Fib2584Ai::initialize(int argc, char* argv[])
{
	srand(time(NULL));
	return;
}


MoveDirection Fib2584Ai::generateMove(int board[4][4]){

	int max_score_gain = 0;
	int index_move = 0;
	int last_valid_move = 0;
	state_game game(board);

	for(int i=0;i<4;++i){
		state_game new_board = game.move(i);
		if(new_board.valid == false){
			continue;
		}
		// begin search
		last_valid_move = i;
		vector<state_game> search_stack = new_board.appear();

		while(!search_stack.empty()){
			state_game this_game = search_stack.back();
			search_stack.pop_back();

			if( this_game.depth < MAX_SEARCH_DEPTH ){
				for(int j=0;j<4;++j){
					state_game next_game = this_game.move(j);
					if(next_game.valid){
						vector<state_game> next_appear_game = next_game.appear();
						search_stack.insert(search_stack.end(), next_appear_game.begin(), next_appear_game.end());
					}
				}
			}else{
				if(max_score_gain <= this_game.point){
					max_score_gain = this_game.point;
					index_move = i;
				}
			}
		}
	}

	if(max_score_gain == 0)
		index_move = last_valid_move;
	return (MoveDirection)index_move;
}

void Fib2584Ai::gameOver(int board[4][4], int iScore)
{
	return;
}

const int state_game::mapping_2584[32] = {0, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597, 2584, 4181, 6765, 10946, 17711, 28657, 46368, 75025, 121393, 196418, 317811, 514229, 832040, 1346269, 2178309};


/**********************************
You can implement any additional functions
you may need.
**********************************/
