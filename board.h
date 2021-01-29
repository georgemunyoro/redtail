#pragma once

#include <string>
#include <unordered_map>

#include "defs.h"

struct TranspositionEntry {
    int best_move;
    int move_score;
    int depth;
};

class Board {
public:
    std::unordered_map<int, TranspositionEntry> transposition_table;
    int WhiteKingPosition;
    int BlackKingPosition;
    int turn;
    int squares[128];

    Board();
    ~Board();

    int CalculateMaterial();
    int Quiesce(int alpha, int beta);
    unsigned long long generatePositionKey();
    void PerftTest(int depth, bool debug);
    int AlphaBeta(int alpha, int beta, int depth, std::vector<int> &PV);
    int BestStaticMove();
    void Search();
    MoveList newMoveGen();
    MoveList getOrderedMoves();
    std::vector<int> BestMove(int depth, std::vector<int> &PV);
    int GetBoardScore();
    MoveList PseudoCaptures();
    void SetFen(std::string fen);
    void Draw();
    void MakeMove(int move);
    void UndoMove();
    int RandomMove();
    PerftResult Perft(int depth, bool debug);
    std::string GenerateFen();
    bool kingAttacked();
    MoveList LegalMoves();
    MoveList generatePseudoMoves();
    bool isSquareAttacked(int pos, int color);
    int pvSearch(int depth, int alpha, int beta);

private:
    int seedKey = getTime();
    int moveHistory[1024];
    int historyIndex = 0;
    int half;
    int ply;
    int enPas;
    int castling = 0;
    int lastSearchNodes = 0;

    std::string PIECE_CHAR_MAP = "PNBRQKpnbrqk. *";
    std::string uci(int move);
    void filterPseudoMoves(MoveList *moves);
    void setKingPositions();
    void ClearBoard();
    void switchTurn();
    void castle(int castleType);
    void undoCastleMove(int mov);
};

std::string GetRef(int pos);
