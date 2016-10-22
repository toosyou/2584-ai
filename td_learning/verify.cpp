#include <iostream>
#include <vector>
#include <cstdio>
#include <set>
#include <cstdlib>
#include <time.h>
#include "my2584.h"
#include "value_table.h"


using namespace std;

struct table_score{
    int index;
    double winning_rate;
    bool operator<(const table_score t)const{
        return this->winning_rate < t.winning_rate;
    }
};

int main(void){

    srand(time(NULL));
    const int number_game = 10000;
    multiset<table_score> table_rank;
    for(int i=200;i<=391;++i){
        char address_tb[200];
        sprintf(address_tb, "48/%d.vt", i);

        value_table tb;
        tb.load(address_tb);

        //play games
        int won = 0;
        double winning_rate = 0.0;
        for(int j=0;j<number_game;++j){
            state_game game;
            game = game.appear_random();

            while(1){
                int best_move = game.best_move(tb);
                if(best_move == -1){//died
                    break;
                }
                game = game.move(best_move);
                game = game.appear_random();
            }
            if(game.won == true)
                won++;
        }
        winning_rate = (double)won / (double)number_game;

        //rank it out
        table_score tmp;
        tmp.index = i;
        tmp.winning_rate = winning_rate;
        table_rank.insert(tmp);
        cout << i << " " << winning_rate <<endl;
    }
    multiset<table_score>::iterator it = table_rank.begin();
    for(int i=0;i<10;++i,++it){
        cout << it->index << " " << it->winning_rate <<endl;
    }

    return 0;
}
