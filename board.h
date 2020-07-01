#pragma once

#include "defs.h"
#include <string>
#include <vector>

using namespace std;

const int N = -16;
const int E = 1;
const int S = 16;
const int W = -1;

class Board {
private:
    Move BestStaticMove();
    void ClearBoard();
    void switchTurn();

    string uci(Move move);

    Move moveHistory[1024];
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

    MoveList PseudoMovesB();

    int GetBoardScore();

    vector<Move> PseudoCaptures();
    vector<Move> PseudoMoves();

    int turn;
    void SetFen(string fen);
    void Draw();
    void MakeMove(Move move);
    void UndoMove();

    Move RandomMove();

    int squares[128];

    PerftResult Perft(int depth, bool debug);

    string GenerateFen();

    bool kingAttacked();

    vector<Move> AllPseudoMoves();
    vector<Move> LegalMoves();
};

string GetRef(int pos);
