all:
	c++ -O3 uci.cpp movegen.cpp board.cpp evaluate.cpp -o engine

test:
	c++ -O3 test.cpp movegen.cpp board.cpp -o test
