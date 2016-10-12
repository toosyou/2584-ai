#ifndef __FIB2584AI_H__
#define __FIB2584AI_H__

#include <cstdlib>
#include <ctime>
#include <vector>
#include <iostream>
#include <map>
#include "Fib2584/MoveDirection.h"

using namespace std;

//null, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597, 2584
//   0, 1, 2, 3, 4, 5,  6,  7,  8,  9, 10,  11,  12,  13,  14,  15,   16,   17
struct state_game{
    bool valid;
    bool won;
    short value_cells[4][4];
	static const int mapping_2584[32];
	int point;
	int depth;

	state_game(){
		this->depth = 0;
		this->point = 0;
        this->valid = true;
        this->won = false;
        for(int i=0;i<4;++i)
            for(int j=0;j<4;++j)
                this->value_cells[i][j] = 0;
    }

	state_game(int board[4][4], int point_board = 0){
		this->valid = true;
		this->point = point_board;
		this->depth = 0;

		map<int, int> index_tile;
		for(int i=0;i<32;++i){
			index_tile[mapping_2584[i]] = i;
		}
		for(int i=0;i<4;++i){
			for(int j=0;j<4;++j){
				this->value_cells[i][j] = index_tile[ board[i][j] ];
				if(this->value_cells[i][j] >= 17)
					this->won = true;
			}
		}

	}

	state_game(const state_game& s){
		this->point = s.point;
        this->valid = s.valid;
        this->won = s.won;
		this->depth = s.depth;
        for(int i=0;i<4;++i)
            for(int j=0;j<4;++j)
                this->value_cells[i][j] = s.value_cells[i][j];
		const int mapping_2584[32] = {0, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597, 2584, 4181, 6765, 10946, 17711, 28657, 46368, 75025, 121393, 196418, 317811, 514229, 832040, 1346269, 2178309};
    }

    vector<state_game> appear(){
        vector<state_game> states_return;
        for(int i=0;i<4;++i)
            for(int j=0;j<4;++j)
                if(this->value_cells[i][j] == 0){
                    state_game tmp = *this;
                    tmp.value_cells[i][j] = 1;
                    states_return.push_back(tmp);
					tmp.value_cells[i][j] = 3;
					states_return.push_back(tmp);
                }

        return states_return;
    }

    state_game appear_random(){
        vector<state_game> all_appear = this->appear();
        int random_index = rand() % all_appear.size();
        return all_appear[random_index];
    }

    short* operator[](int index_i){
        return this->value_cells[index_i];
    }

    state_game rotate_clockwise(){
        state_game state_return = *this;

        for(int i=0;i<4;++i){
            for(int j=0;j<4;++j){
                state_return[i][j] = this->value_cells[3-j][i];
            }
        }

        return state_return;
    }

    state_game rotate_counterclockwise(){
        state_game state_return = *this;

        for(int i=0;i<4;++i){
            for(int j=0;j<4;++j){
                state_return[i][j] = this->value_cells[j][3-i];
            }
        }

        return state_return;
    }

    state_game rotate_clockwise(unsigned int times){
        times %= 4;
        state_game state_return = *this;
        for(int i=0;i<times;++i){
            state_return = state_return.rotate_clockwise();
        }
        return state_return;
    }

	state_game move(int urdl){
		urdl %= 4;
		switch(urdl){
			case 0:
				return this->up();
				break;
			case 1:
				return this->right();
				break;
			case 2:
				return this->down();
				break;
			case 3:
				return this->left();
				break;
			default:
				break;
		}
		return this->up();
	}

    state_game up(){
        bool moved = false;
        state_game state_return = *this;

        //converge all
        for(int i=0;i<3;++i){
            for(int j=0;j<4;++j){
                if(state_return[i][j] == 0)
                    continue;
                //find below
                for(int k=i+1;k<4;++k){
                    if(state_return[k][j] == 0)
                        continue;
                    if( (state_return[i][j] == 1 && state_return[k][j] == 1) ||
                        abs(state_return[i][j] - state_return[k][j]) == 1 ){ //converge
                        state_return[i][j] = max(state_return[i][j], state_return[k][j]) + 1;
                        state_return[k][j] = 0;
                        moved = true;
                        if(state_return[i][j] == 17)
                            state_return.won = true;
						this->point += mapping_2584[state_return[i][j]];
                    }
                    break;
                }
            }
        }

        //move all cells up
        for(int j=0;j<4;++j){
            int k = 0;
            for(int i=0;i<4;++i){
                if(state_return[i][j] != 0){
                    state_return[k][j] = state_return[i][j];
                    //check if it's moved
                    if(k != i)
                        moved = true;
                    k += 1;
                }
            }
            for(;k<4;++k){
                state_return[k][j] = 0;
            }
        }

        if(moved == false)
            state_return.valid = false;
		else
			state_return.depth += 1;

        for(int i=0;i<4;++i){
            for(int j=0;j<4;++j){
                if(this->value_cells[i][j] == 0)
                    this->point += 100;
            }
        }
        return state_return;
    }

    state_game down(){
        return this->rotate_clockwise(2).up().rotate_clockwise(2);
    }

    state_game left(){
        return this->rotate_clockwise().up().rotate_counterclockwise();
    }

    state_game right(){
        return this->rotate_counterclockwise().up().rotate_clockwise();
    }

    vector<state_game> move_set(){
        vector<state_game> states_return;
        //if(this->won == true)
        //    return states_return;

        state_game tmp = this->up();
        if(tmp.valid == true )
            states_return.push_back(tmp);

        tmp = this->down();
        if(tmp.valid == true )
            states_return.push_back(tmp);

        tmp = this->left();
        if(tmp.valid == true )
            states_return.push_back(tmp);

        tmp = this->right();
        if(tmp.valid == true )
            states_return.push_back(tmp);

        return states_return;
    }

	bool operator<(const state_game &s)const{
		for(int i=0;i<4;++i){
			for(int j=0;j<4;++j){
				if(this->value_cells[i][j] > s.value_cells[i][j])
					return false;
				else if(this->value_cells[i][j] < s.value_cells[i][j])
					return true;
			}
		}
		return false;
	}

	bool operator==(const state_game &s)const{
		for(int i=0;i<4;++i){
			for(int j=0;j<4;++j){
				if(this->value_cells[i][j] != s.value_cells[i][j])
					return false;
			}
		}
		return true;
	}

	bool operator!=(const state_game &s)const{
		return !(*this == s);
	}

    void print(){
        cout << "======================================" <<endl;
        for(int i=0;i<4;++i){
            for(int j=0;j<4;++j){
                cout << mapping_2584[this->value_cells[i][j]] << "\t";
            }
            cout << endl;
        }

        return;
    }
};

class Fib2584Ai{
	map<state_game, int> decided;
	public:
	Fib2584Ai();
	// initialize ai
	void initialize(int argc, char* argv[]);
	// generate one move
	MoveDirection generateMove(int board[4][4]);
	// do some action when game over
	void gameOver(int board[4][4], int iScore);

	/**********************************
	You can implement any additional functions
	or define any variables you may need.
	**********************************/
};



#endif
