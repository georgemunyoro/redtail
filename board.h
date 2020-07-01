#pragma once

#include "defs.h"
#include <string>

using namespace std;

class Board {
    private:
	Move BestStaticMove();
	void ClearBoard();
	void switchTurn();

	string uci(Move move);

	int moveHistory[1024];

	int historyIndex = 0;

	int half;
	int ply;

	string PIECE_CHAR_MAP = "PNBRQKpnbrqk. *";
	string castling;
	string enPas;

	int AlphaBeta(int alpha, int beta, int depth);
	int Quiesce(int alpha, int beta);

    public:
	Board();
	~Board();

	Move BestMove(int depth);

	int GetBoardScore();

	MoveList PseudoCaptures();

	int turn;
	void SetFen(string fen);
	void Draw();

	void MakeMove(int move);
	void UndoMove();

	Move RandomMove();

	int squares[128];

	PerftResult Perft(int depth, bool debug);

	string GenerateFen();

	bool kingAttacked();

	MoveList LegalMoves();

	MoveList generatePseudoMoves();

	bool isSquareAttacked(int square);
};

string GetRef(int pos);
