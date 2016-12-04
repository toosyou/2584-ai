#include "my2584.h"

#define WIN_INDEX 14

const int state_game::mapping_2584[32] = {0, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597, 2584, 4181, 6765, 10946, 17711, 28657, 46368, 75025, 121393, 196418, 317811, 514229, 832040, 1346269, 2178309};
const int move_table::mapping_2584[32] = {0, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597, 2584, 4181, 6765, 10946, 17711, 28657, 46368, 75025, 121393, 196418, 317811, 514229, 832040, 1346269, 2178309};
//                                        0, 1, 2, 3, 4, 5,  6,  7,  8,  9, 10,  11,  12,  13,  14,  15,   16,   17
const move_table state_game::table;


state_game::state_game(){
    this->depth = 0;
    this->score = 0;
    this->valid = true;
    this->won = false;

    for(int i=0;i<4;++i)
        this->bitboard[i] = 0;

    srand(time(NULL));
}

state_game::state_game(int board[4][4], int score_board){
    this->valid = true;
    this->score = score_board;
    this->depth = 0;

    map<int, int> index_tile;
    for(int i=0;i<32;++i){
        index_tile[mapping_2584[i]] = i;
    }
    for(int i=0;i<4;++i){
        int tmp_board[4];
        for(int j=0;j<4;++j){
            tmp_board[j] = index_tile[ board[i][j] ];
            if(tmp_board[j] >= WIN_INDEX)
                this->won = true;
        }
        this->bitboard[i] = move_table::hash(tmp_board);
    }

}

state_game::state_game(const state_game& s){
    this->score = s.score;
    this->valid = s.valid;
    this->won = s.won;
    this->depth = s.depth;
    for(int i=0;i<4;++i)
        this->bitboard[i] = s.bitboard[i];
}

vector<state_game> state_game::appear(){
    vector<state_game> states_return;
    int this_board[4][4];
    for(int i=0;i<4;++i){
        move_table::unhash(this->bitboard[i], this_board[i]);
    }

    for(int i=0;i<4;++i)
        for(int j=0;j<4;++j)
            if(this_board[i][j] == 0){
                state_game tmp = *this;
                tmp.bitboard[i] |= 1 << (5*(3-j));
                states_return.push_back(tmp);
                tmp.bitboard[i] |= 3 << (5*(3-j));
                states_return.push_back(tmp);
            }

    return states_return;
}

state_game state_game::appear_random(){
    state_game rtn = *this;
    vector<pair<int, int> > valid_coordinates;
    int this_board[4][4];
    int index_rand = 0;
    int rand_tile = 0;
    for(int i=0;i<4;++i){
        move_table::unhash(this->bitboard[i], this_board[i]);
    }
    //find all valid points
    for(int i=0;i<4;++i){
        for(int j=0;j<4;++j){
            if(this_board[i][j] == 0){
                valid_coordinates.push_back( pair<int,int>(i,j) );
            }
        }
    }
    if(valid_coordinates.size() == 0){
        rtn.valid = false;
        return rtn;
    }
    //choose one randomly with random tile 1 or 3
    index_rand = rand() % valid_coordinates.size();
    rand_tile = (rand() % 8) < 6 ? 1 : 3 ;
    rtn.bitboard[valid_coordinates[index_rand].first] |= rand_tile << (5*(3- valid_coordinates[index_rand].second ));
    return rtn;
}

state_game state_game::appear_random(int tile){
    state_game rtn = *this;
    vector<pair<int, int> > valid_coordinates;
    int this_board[4][4];
    int index_rand = 0;
    for(int i=0;i<4;++i){
        move_table::unhash(this->bitboard[i], this_board[i]);
    }
    //find all valid points
    for(int i=0;i<4;++i){
        for(int j=0;j<4;++j){
            if(this_board[i][j] == 0){
                valid_coordinates.push_back( pair<int,int>(i,j) );
            }
        }
    }
    if(valid_coordinates.size() == 0){
        rtn.valid = false;
        return rtn;
    }
    //choose one randomly with random tile 1 or 3
    index_rand = rand() % valid_coordinates.size();
    rtn.bitboard[valid_coordinates[index_rand].first] |= tile << (5*(3- valid_coordinates[index_rand].second ));
    return rtn;
}

int state_game::operator[](int index_i){
    return this->bitboard[index_i];
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
    state_game rtn = *this;
    int tmp[4];
    for(int i=0;i<4;++i){
        int bitboard_column = column(i, this->bitboard);
        tmp[i] = table.table[bitboard_column];
        rtn.score += table.score[bitboard_column];

        if(table.moved[bitboard_column] == true)
            moved = true;
        if(table.won[bitboard_column] == true)
            rtn.won = true;
    }
    for(int i=0;i<4;++i){
        rtn.bitboard[i] = column(i, tmp);
    }
    if(moved == true)
        rtn.depth += 1;
    else
        rtn.valid = false;

    return rtn;
}

state_game state_game::down(){
    bool moved = false;
    state_game rtn = *this;
    int tmp[4];
    for(int i=0;i<4;++i){
        int bitboard_column_reversed = reverse_bitboard(column(i, this->bitboard));
        tmp[i] = reverse_bitboard(table.table[bitboard_column_reversed]);
        rtn.score += table.score[bitboard_column_reversed];

        if(table.moved[bitboard_column_reversed] == true)
            moved = true;
        if(table.won[bitboard_column_reversed] == true)
            rtn.won = true;
    }
    for(int i=0;i<4;++i){
        rtn.bitboard[i] = column(i, tmp);
    }
    if(moved == true)
        rtn.depth += 1;
    else
        rtn.valid = false;

    return rtn;
}

state_game state_game::left(){
    bool moved = false;
    state_game rtn = *this;
    for(int i=0;i<4;++i){
        rtn.bitboard[i] = table.table[this->bitboard[i]];
        rtn.score += table.score[this->bitboard[i]];

        if(table.moved[this->bitboard[i]] == true)
            moved = true;
        if(table.won[this->bitboard[i]] == true)
            rtn.won = true;
    }
    if(moved == true)
        rtn.depth += 1;
    else
        rtn.valid = false;

    return rtn;
}

state_game state_game::right(){
    bool moved = false;
    state_game rtn = *this;
    for(int i=0;i<4;++i){
        int bitboard_reversed = reverse_bitboard(this->bitboard[i]);
        rtn.bitboard[i] = reverse_bitboard(this->table.table[bitboard_reversed]);
        rtn.score += this->table.score[bitboard_reversed];

        if(this->table.moved[bitboard_reversed] == true)
            moved = true;
        if(this->table.won[bitboard_reversed] == true)
            rtn.won = true;
    }
    if(moved == true)
        rtn.depth += 1;
    else
        rtn.valid = false;

    return rtn;
}

state_game state_game::rotate_clockwise(){
    state_game rtn = *this;

    for(int i=0;i<4;++i){
        rtn.bitboard[i] = this->column(i);
    }

    return rtn;
}

int state_game::best_move(value_table &tb){
    //choose the max value move
    long double max_value_game = -9999.99L;
    bool died = true;
    int next_move = 0;

    for(int i=0;i<4;++i){
        state_game next_game = this->move(i);
        if(next_game.valid == true){
            long double next_value = tb.value(next_game);
            died = false;
            if(max_value_game < next_value){ //update
                next_move = i;
                max_value_game = next_value;
            }
        }
    }

    if(died == true)
        return -1;

    return next_move;
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

bool state_game::operator==(const state_game &s)const{
    for(int i=0;i<4;++i){
        if(this->bitboard[i] != s.bitboard[i])
            return false;
    }
    return true;
}

bool state_game::operator!=(const state_game &s)const{
    return !(*this == s);
}

int state_game::reverse_bitboard(int bits){
    int rtn = 0;
    for(int i=0;i<4;++i){
        rtn <<= 5;
        rtn |= (bits >> (5*i)) & 31;
    }
    return rtn;
}

int state_game::column(int index, int board[4]){
    int rtn = 0;
    for(int i=0;i<4;++i){
        rtn <<= 5;
        rtn |= (board[i] >> (5*(3-index)) ) & 31 ;
    }
    return rtn;
}

int state_game::column(int index){
    return state_game::column(index, this->bitboard);
}

void state_game::print(){
    cout << "======================================" <<endl;
    for(int i=0;i<4;++i){
        int tmp[4];
        move_table::unhash(this->bitboard[i], tmp);
        for(int j=0;j<4;++j){
            cout << mapping_2584[tmp[j] ] << "\t";
        }
        cout << endl;
    }
    cout << "Score:\t" << this->score <<endl ;

    return;
}

move_table::move_table(){
    this->table = new unsigned int[1<<20];
    this->score = new int[1<<20];
    this->moved = new bool[1<<20];
    this->won = new bool[1<<20];
    for(int i=0;i<(1<<20);++i){
        int board[4];
        unhash(i, board);
        this->score[i] = move_left(board);
        this->table[i] = hash(board);
        this->moved[i] = (this->table[i] != (unsigned int)i);
        this->won[i] = false;
        for(int j=0;j<4;++j){
            if(board[j] >= WIN_INDEX)
                this->won[i] = true;
        }
    }
}

move_table::~move_table(){
    delete [] table;
    delete [] score;
    delete [] moved;
    delete [] won;
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

    for(int i=0;i<4;++i){
        if(board[i] == 0)
            score += 100;
    }
    return score;
}
