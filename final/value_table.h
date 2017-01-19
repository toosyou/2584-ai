#ifndef __VALUE_TABLE_h__
#define __VALUE_TABLE_h__

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include "my2584.h"

using namespace std;

struct state_game;

struct value_table{
    long double* value_row[4];
    long double* value_column[4];

    value_table();
    ~value_table();
    long double value(state_game &s);
    void update(state_game &st, state_game &stn, long double learning_rate);
    void update(vector<state_game> &vs, long double learning_rate);
    state_game train(long double learning_rate, bool random_appear = true);
    void dump(const char* address);
    void load(const char* address);
};


#endif
