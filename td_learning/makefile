CXX = g++-6
CXXFLAGS = -std=c++11 -O3

all: game

game: play_game

play_game: Fib2584Ai.cpp Fib2584Ai.h main.cpp my2584.cpp my2584.h
	$(CXX) $(CXXFLAGS) Fib2584/*.cpp Fib2584Ai.cpp main.cpp my2584.cpp -o play_game

train: train.cpp my2584.cpp my2584.h value_table.cpp value_table.h
	$(CXX) $(CXXFLAGS) train.cpp my2584.cpp value_table.cpp -o train

clean:
	rm -rf play_game
