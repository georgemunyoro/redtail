#include <bits/stdc++.h>

#include "board.h"

using namespace std;

string printMove(int move) {
    return GetRef((move >> 20)) + GetRef((move >> 12) & 0xff);
}

int Board::CalculateMaterial() {
    int WhiteScore = 0;
    int BlackScore = 0;
    int index = 0;
    for (int i = 0; i < 128; ++i) {
        if ((i & 0x88) == 0) {
            switch (squares[i]) {
            case Chess::WhitePawn:
                WhiteScore += 100;
                break;
            case Chess::WhiteKnight:
                WhiteScore += 300;
                break;
            case Chess::WhiteBishop:
                WhiteScore += 300;
                break;
            case Chess::WhiteRook:
                WhiteScore += 500;
                break;
            case Chess::WhiteQueen:
                WhiteScore += 900;
                break;
            case Chess::WhiteKing:
                WhiteScore += 100000;
                break;

            case Chess::BlackPawn:
                BlackScore += 100;
                break;
            case Chess::BlackKnight:
                BlackScore += 300;
                break;
            case Chess::BlackBishop:
                BlackScore += 300;
                break;
            case Chess::BlackRook:
                BlackScore += 500;
                break;
            case Chess::BlackQueen:
                BlackScore += 900;
                break;
            case Chess::BlackKing:
                BlackScore += 100000;
                break;
            }
            index++;
        }
    }

    if (turn == Chess::White) {
        return WhiteScore - BlackScore;
    } else {
        return BlackScore - WhiteScore;
    }
}

MoveList Board::getOrderedMoves() {
    MoveList moves = LegalMoves();
    MoveList orderedMoves;
    int moveScores[moves.count];
    for (int i = 0; i < moves.count; ++i) {
        int currentMove = moves.moves[i];

        MakeMove(currentMove);
        int score = CalculateMaterial();
        UndoMove();

        moveScores[i] = ((score << 8) | i);
    }
    sort(moveScores, moveScores + moves.count);
    reverse(moveScores, moveScores + moves.count);

    for (int i = 0; i < moves.count; ++i) {
        orderedMoves.moves[(moveScores[i] & 0xff)] =
            moves.moves[(moveScores[i] & 0xff)];
    }

    orderedMoves.count = moves.count;
    return orderedMoves;
}

int Board::AlphaBeta(int alpha, int beta, int depth, vector<int> &PV) {
    int best_score = -100000;
    auto moves = getOrderedMoves();

    if (depth == 0) {
        lastSearchNodes++;
        return CalculateMaterial();
    }

    for (int i = 0; i < moves.count; ++i) {
        vector<int> localPV;

        MakeMove(moves.moves[i]);
        int score = -AlphaBeta(-beta, -alpha, depth - 1, localPV);
        UndoMove();

        if (score > best_score)
            best_score = score;

        if (best_score > alpha) {
            alpha = best_score;
            PV.clear();
            PV.push_back(moves.moves[i]);
            copy(localPV.begin(), localPV.end(), back_inserter(PV));
        }

        if (alpha >= beta)
            return alpha;
    }
    return best_score;
}

vector<int> Board::BestMove(int depth, vector<int> &PV) {
    int bestmove;
    int bestscore = -100000;

    auto moves = getOrderedMoves();
    for (int i = 0; i < moves.count; ++i) {
        auto move = moves.moves[i];

        MakeMove(move);
        int score = -AlphaBeta(-100000, 100000, depth, PV);
        UndoMove();

        if (score > bestscore) {
            bestscore = score;
            bestmove = move;
        }
    }
    return {bestmove, bestscore};
}

void Board::Search() {
    int bestmove;
    auto startTime = getTime();
    for (int i = 0; i < 100; ++i) {
        vector<int> newPV;
        newPV.clear();
        auto res = BestMove(i, newPV);
        bestmove = res[0];
        cout << "info depth " << i + 1 << " score cp " << res[1] << " time "
             << (getTime() - startTime) << " nodes " << lastSearchNodes << " pv "
             << printMove(bestmove) << " ";

        for (auto &m : newPV) {
            cout << printMove(m) << " ";
        }
        cout << endl;
        lastSearchNodes = 0;

        if ((getTime() - startTime) > 4000)
            break;
    }
    cout << "bestmove " << printMove(bestmove) << endl;
}
