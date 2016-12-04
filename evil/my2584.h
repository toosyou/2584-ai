#ifndef __MY2584_H__
#define __MY2584_H__

#include <cstdlib>
#include <vector>
#include <iostream>
#include <map>
#include <utility>
#include <time.h>
#include "value_table.h"

using namespace std;

struct value_table;

struct move_table{
    unsigned int* table;
    int* score;
    bool* moved;
    bool* won;
    static const int mapping_2584[32];

    move_table();
    ~move_table();

    static void unhash(int bit_board, int* unhahsed_table);
    static unsigned int hash(int board[4]);
    static int move_left(int *board );

};

struct state_game{
    bool valid;
    bool won;
    int score;
	int depth;

    //short value_cells[4][4];
    int bitboard[4];

	static const int mapping_2584[32];
    static const move_table table;

	state_game();
	state_game(int board[4][4], int point_board = 0);
	state_game(const state_game& s);
    vector<state_game> appear();
    state_game appear_random();
    state_game appear_random(int tile);
    int operator[](int index_i);
	state_game move(int urdl);
    state_game up();
    state_game down();
    state_game left();
    state_game right();
    state_game rotate_clockwise();
    vector<state_game> move_set();
	bool operator==(const state_game &s)const;
	bool operator!=(const state_game &s)const;

    static int reverse_bitboard(int bits);
    static int column(int index, int board[4]);
    int column(int index);

    int best_move(value_table &tb);

    void print();
};


#endif
