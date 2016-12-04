CXX = g++-6
CXXFLAGS = -std=c++11 -O3

client: my2584.cpp my2584.h value_table.cpp value_table.h Fib2584Ai.cpp
	$(CXX) my2584.cpp my2584.h value_table.cpp value_table.h Fib2584Ai.cpp main/client_main.cpp Fib2584/*.cpp -o ai_client

stdio:
	g++ *.cpp main/main.cpp Fib2584/*.cpp -o ai_stdio

train: train.cpp my2584.cpp my2584.h value_table.cpp value_table.h
	$(CXX) $(CXXFLAGS) train.cpp my2584.cpp value_table.cpp -o train

test: test_game.cpp my2584.cpp my2584.h value_table.cpp value_table.h
	$(CXX) $(CXXFLAGS) test_game.cpp my2584.cpp value_table.cpp -o test
