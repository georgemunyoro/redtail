#pragma once

#include "defs.h"
#include <string>

using namespace std;

class Board
{
private:
	int BestStaticMove();
	void ClearBoard();
	void switchTurn();

	string uci(int move);

	int moveHistory[1024];

	int historyIndex = 0;

	int half;
	int ply;

	string PIECE_CHAR_MAP = "PNBRQKpnbrqk. *";
	string castling;
	string enPas;

	int AlphaBeta(int alpha, int beta, int depth);
	int Quiesce(int alpha, int beta);

	void filterPseudoMoves(MoveList *moves);

public:
	void PerftTest(int depth, bool debug);
	Board();
	~Board();

	MoveList newMoveGen();

	int WhiteKingPosition;
	int BlackKingPosition;

	int BestMove(int depth);

	int GetBoardScore();

	MoveList PseudoCaptures();

	int turn;
	void SetFen(string fen);
	void Draw();

	void MakeMove(int move);
	void UndoMove();

	int RandomMove();

	int squares[128];

	PerftResult Perft(int depth, bool debug);

	string GenerateFen();

	bool IsKingAttacked();

	MoveList LegalMoves();

	MoveList generatePseudoMoves();

	bool isSquareAttacked(int pos, int color);
};

string GetRef(int pos);
