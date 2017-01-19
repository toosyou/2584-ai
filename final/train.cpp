#include <iostream>
#include <cstdlib>
#include <vector>
#include <cstdio>
#include "my2584.h"
#include "value_table.h"

using namespace std;

int main(void){

    value_table vt;
    long double learning_rate = 0.0025L;
    const int sample_size = 10000;
    const int dump_size = 100000;
    int has_won = 0;
    int average_score = 0;
    bool random_appear = true;

    //do the train [times] times
    for(int times=1;;++times){
        state_game end_game = vt.train(learning_rate, random_appear);

        //statistic
        if(end_game.won == true){
            has_won += 1;
            average_score += end_game.score;
        }

        if(times % sample_size == 0){
            cout << times << " th winning-rate: " << (double)has_won/(double)sample_size <<endl;
            cout << times << " th average-score: " << (double)average_score/(double)sample_size <<endl;
            has_won = 0;
            average_score = 0;
        }
        if(times % dump_size == 0){
            char address[200] = {0};
            sprintf(address, "vts/%d.vt", times/dump_size);
            cout << times << " th dumping: " << address <<endl;
            vt.dump(address);
            random_appear = !random_appear;
        }
        if(times % 500000 == 0){
            learning_rate /= 2.0L;
            cout << times << "th change-learning-rate: " << learning_rate <<endl;
        }
    }
}
