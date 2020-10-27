all:
	c++ -Ofast -std=c++17 uci.cpp movegen.cpp board.cpp evaluate.cpp -o engine

test:
	c++ -Ofast -std=c++17 perft.cpp movegen.cpp board.cpp -o tes
