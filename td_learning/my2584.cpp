#include "my2584.h"

const int state_game::mapping_2584[32] = {0, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597, 2584, 4181, 6765, 10946, 17711, 28657, 46368, 75025, 121393, 196418, 317811, 514229, 832040, 1346269, 2178309};
const int move_table::mapping_2584[32] = {0, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597, 2584, 4181, 6765, 10946, 17711, 28657, 46368, 75025, 121393, 196418, 317811, 514229, 832040, 1346269, 2178309};

state_game::state_game(){
    this->depth = 0;
    this->point = 0;
    this->valid = true;
    this->won = false;
    for(int i=0;i<4;++i)
        for(int j=0;j<4;++j)
            this->value_cells[i][j] = 0;
}

state_game::state_game(int board[4][4], int point_board){
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

state_game::state_game(const state_game& s){
    this->point = s.point;
    this->valid = s.valid;
    this->won = s.won;
    this->depth = s.depth;
    for(int i=0;i<4;++i)
        for(int j=0;j<4;++j)
            this->value_cells[i][j] = s.value_cells[i][j];
    const int mapping_2584[32] = {0, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597, 2584, 4181, 6765, 10946, 17711, 28657, 46368, 75025, 121393, 196418, 317811, 514229, 832040, 1346269, 2178309};
}

vector<state_game> state_game::appear(){
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

state_game state_game::appear_random(){
    vector<state_game> all_appear = this->appear();
    int random_index = rand() % all_appear.size();
    return all_appear[random_index];
}

short* state_game::operator[](int index_i){
    return this->value_cells[index_i];
}

state_game state_game::rotate_clockwise(){
    state_game state_return = *this;

    for(int i=0;i<4;++i){
        for(int j=0;j<4;++j){
            state_return[i][j] = this->value_cells[3-j][i];
        }
    }

    return state_return;
}

state_game state_game::rotate_counterclockwise(){
    state_game state_return = *this;

    for(int i=0;i<4;++i){
        for(int j=0;j<4;++j){
            state_return[i][j] = this->value_cells[j][3-i];
        }
    }

    return state_return;
}

state_game state_game::rotate_clockwise(unsigned int times){
    times %= 4;
    state_game state_return = *this;
    for(int i=0;i<times;++i){
        state_return = state_return.rotate_clockwise();
    }
    return state_return;
}

state_game state_game::move(int urdl){
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

state_game state_game::up(){
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

state_game state_game::down(){
    return this->rotate_clockwise(2).up().rotate_clockwise(2);
}

state_game state_game::left(){
    return this->rotate_clockwise().up().rotate_counterclockwise();
}

state_game state_game::right(){
    return this->rotate_counterclockwise().up().rotate_clockwise();
}

vector<state_game> state_game::move_set(){
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

bool state_game::operator<(const state_game &s)const{
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

bool state_game::operator==(const state_game &s)const{
    for(int i=0;i<4;++i){
        for(int j=0;j<4;++j){
            if(this->value_cells[i][j] != s.value_cells[i][j])
                return false;
        }
    }
    return true;
}

bool state_game::operator!=(const state_game &s)const{
    return !(*this == s);
}

void state_game::print(){
    cout << "======================================" <<endl;
    for(int i=0;i<4;++i){
        for(int j=0;j<4;++j){
            cout << mapping_2584[this->value_cells[i][j]] << "\t";
        }
        cout << endl;
    }

    return;
}

move_table::move_table(){
    this->table = new unsigned int[1<<20];
    this->score = new int[1<<20];
    for(int i=0;i<(1<<20);++i){
        int board[4];
        unhash(i, board);
        this->score[i] = move_left(board);
        this->table[i] = hash(board);
    }
}

void move_table::unhash(int bit_board, int* unhahsed_table){
    for(int i=0;i<4;++i){
        unhahsed_table[3-i] = bit_board & ( (1<<5) - 1);
        bit_board >>= 5;
    }
    return;
}

unsigned int move_table::hash(int board[4]){
    int rtn = 0;
    for(int i=0;i<4;++i){
        rtn |= board[i] << (5*(3-i));
    }
    return rtn;
}

int move_table::move_left(int *board){

    int score = 0;

    //converge all
    for(int i=0;i<3;++i){
        if(board[i] == 0)
            continue;
        //find below
        for(int k=i+1;k<4;++k){
            if(board[k] == 0)
                continue;
            if( (board[i] == 1 && board[k] == 1) ||
                    abs(board[i] - board[k]) == 1 ){ //converge
                board[i] = max(board[i], board[k]) + 1;
                board[k] = 0;
                score += mapping_2584[board[i]];
            }
            break;
        }
    }

    //move all cells up
    int k = 0;
    for(int i=0;i<4;++i){
        if(board[i] != 0){
            board[k] = board[i];
            //check if it's moved
            k += 1;
        }
    }
    for(;k<4;++k){
        board[k] = 0;
    }

    return score;
}
