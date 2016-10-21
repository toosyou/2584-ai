#include <iostream>
#include <cstdlib>
#include <vector>
#include <cstdio>
#include "my2584.h"
#include "value_table.h"

using namespace std;

int main(void){

    value_table tb;
    long double learning_rate = 0.0025L;
    const int sample_size = 10000;
    const int dump_size = 100000;
    int has_won = 0;


    //do the train [times] times
    for(int times=1;;++times){

        vector<state_game> game_played;
        state_game game;
        game = game.appear_random();
        game_played.push_back(game);
        while(1){
            //choose the max value move
            long double max_value_game = 0.0L;
            bool died = true;
            int next_move = 0;

            for(int i=0;i<4;++i){
                state_game next_game = game.move(i);
                if(next_game.valid == true){
                    long double next_value = tb.value(next_game);
                    died = false;
                    if(max_value_game <= next_value){ //update
                        next_move = i;
                        max_value_game = next_value;
                    }
                }
            }

            //game over
            if(died)
                break;

            //make next move
            game = game.move(next_move);
            game = game.appear_random();
            game_played.push_back(game);
        }

        //update value table
        game_played.back().valid = false;
        vector<state_game>::iterator it_game;
        for(it_game = game_played.end()-1; it_game != game_played.begin(); --it_game){
            tb.update( *(it_game-1), *it_game, learning_rate );
        }

        //statistic
        if(game.won == true){
            has_won += 1;
        }

        if(times % sample_size == 0){
            cout << times << "th wining rate: " << (double)has_won/(double)sample_size <<endl;
            has_won = 0;
        }
        if(times % dump_size == 0){
            char address[200] = {0};
            sprintf(address, "%d.tb", times/dump_size);
            cout << times << "th dumping: " << address <<endl;
            tb.dump(address);
        }
    }
}
