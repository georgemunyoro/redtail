#include "board.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

void Board::PerftTest(int depth, bool debug)
{
	Draw();
	cout << "Test running to depth : " << depth << endl;
	int leafNodes = 0;

	MoveList moves = LegalMoves();
	>
		for (int i = 0; i < moves.count; ++i)
	{
		int currentMove = moves.moves[i];
		MakeMove(moves.moves[i]);
		auto perft_res = Perft(depth - 1, debug);
		leafNodes += perft_res.nodes;
		UndoMove();
		cout << GetRef((currentMove >> 20) & 0xff) << GetRef((currentMove >> 12) & 0xff) << ": " << perft_res.nodes << endl;
	}

	cout << endl
		 << "Nodes searched : " << leafNodes << endl;
	return;
}

PerftResult Board::Perft(int depth, bool debug)
{
	auto moves = LegalMoves();

	int nodes = 0;

	PerftResult result;
	result.depth = depth;

	if (depth == 0)
	{
		result.nodes++;
		return result;
	}
	//
	//    cout << GenerateFen() << " " << "wb"[turn] << " ";
	//
	//    for (int i = 0; i < moves.count; ++i) {
	//	cout << GetRef((moves.moves[i] >> 20) & 0xff) << GetRef((moves.moves[i] >> 12) & 0xff) << " ";
	//    }
	//    cout << endl;
	//
	for (int i = 0; i < moves.count; ++i)
	{
		if (moves.moves[i] == 0)
			continue;
		auto move = moves.moves[i];

		if ((move & 0xf) == 1)
			result.captures++;

		MakeMove(move);
		auto res = Perft(depth - 1, debug);

		if (kingAttacked())
			result.checks++;

		result.nodes += res.nodes;
		result.captures += res.captures;
		result.checks += res.checks;
		result.checkmates += res.checkmates;
		result.enPas += res.enPas;
		result.promotions = res.promotions;

		UndoMove();
	}
	return result;
}

//
//void RunPerft(int maxDepth, int debug)
//{
//  Board MainBoard = Board();
//  MainBoard.SetFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
//  printf("---------------------------------------------------------------------"
//	  "----------------\n");
//  printf("| DEPTH |   NODE   |  CAPTURES | ENPAS | CASTLES | PROMOTIONS | "
//	  "CHECKMATES | CHECKS |\n");
//  printf("--------+----------+-----------+-------+---------+------------+------"
//	  "------+---------\n");
//  for (int i = 1; i < maxDepth; ++i) {
//	auto res = MainBoard.Perft(i, debug);
//	printf("|%7d|%10d|%11d|%7d|%9d|%12d|%12d|%8d|\n",
//		res.depth,
//		res.nodes,
//		res.captures,
//		res.enPas,
//		res.castles,
//		res.promotions,
//		res.checkmates,
//		res.checks);
//  }
//  printf("---------------------------------------------------------------------"
//	  "----------------\n");
//}
//

int main(int argc, char *argv[])
{
	//    freopen("perft_moves.txt", "w", stdout);
	if (argc >= 2)
	{
		Board MainBoard = Board();
		MainBoard.SetFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
		//	MainBoard.SetFen("rnbqkbnr/ppp1pppp/8/3p4/2P5/8/PP1PPPPP/RNBQKBNR w - - 0 1");
		MainBoard.PerftTest(stoi(argv[1]), false);
	}

	return 0;
}
