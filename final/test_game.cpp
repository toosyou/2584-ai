#include <iostream>
#include "my2584.h"

int main(void){
    state_game game;
    game = game.appear_random();
    game = game.appear_random();

    while(game.valid){
        state_game next;
        string cmd;
        game.print();
        cin >> cmd;
        if( cmd == "a" ){
            next = game.left();
        }
        else if( cmd == "s" ){
            next = game.down();
        }
        else if( cmd == "d" ){
            next = game.right();
        }
        else if( cmd == "w" ){
            next = game.up();
        }

        if(next.valid == false)
            continue;

        game = next.appear_random();
    }

    return 0;
}
