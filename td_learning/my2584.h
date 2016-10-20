#ifndef __MY2584_H__
#define __MY2584_H__

#include <cstdlib>
#include <vector>
#include <iostream>
#include <map>

using namespace std;

struct move_table{
    unsigned int* table;
    int* score;
    static const int mapping_2584[32];

    move_table();
    ~move_table();

    void unhash(int bit_board, int* unhahsed_table);
    unsigned int hash(int board[4]);
    int move_left(int *board );

};

struct state_game{
    bool valid;
    bool won;
    short value_cells[4][4];
	static const int mapping_2584[32];
	int point;
	int depth;
    static const move_table table;

	state_game();
	state_game(int board[4][4], int point_board = 0);
	state_game(const state_game& s);
    vector<state_game> appear();
    state_game appear_random();
    short* operator[](int index_i);
    state_game rotate_clockwise();
    state_game rotate_counterclockwise();
    state_game rotate_clockwise(unsigned int times);
	state_game move(int urdl);
    state_game up();
    state_game down();
    state_game left();
    state_game right();
    vector<state_game> move_set();
	bool operator<(const state_game &s)const;
	bool operator==(const state_game &s)const;
	bool operator!=(const state_game &s)const;

    void print();
};


#endif
