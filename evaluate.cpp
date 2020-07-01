#include "board.h"

int Board::GetBoardScore()
{
    int score = 0;
    for (int i = 0; i < 128; ++i) {
        int piece = squares[i];
        if (piece == Empty || (i & 0x88) != 0)
            continue;
        switch (piece) {
        case WhiteKing:
            score += 32070;
            break;
        case WhitePawn:
            score += 100;
            break;
        case WhiteRook:
            score += 500;
            break;
        case WhiteBishop:
            score += 320;
            break;
        case WhiteQueen:
            score += 900;
            break;
        case WhiteKnight:
            score += 300;
            break;

        case BlackKing:
            score -= 32070;
            break;
        case BlackPawn:
            score -= 100;
            break;
        case BlackRook:
            score -= 500;
            break;
        case BlackBishop:
            score -= 320;
            break;
        case BlackQueen:
            score -= 900;
            break;
        case BlackKnight:
            score -= 300;
            break;
        }
    }
    return score;
}

Move Board::RandomMove()
{
    auto possibleMoves = LegalMoves();
    return possibleMoves[rand() % possibleMoves.size()];
}

Move Board::BestStaticMove()
{
    auto moves = LegalMoves();
    int best_score = -100000;
    Move best_move;
    for (auto move : moves) {
        MakeMove(move);
        int moveScore = GetBoardScore();
        if (turn == Black)
            moveScore = 0 - moveScore;
        if (moveScore > best_score) {
            best_score = moveScore;
            best_move = move;
        }
    }
    return best_move;
}

Move Board::BestMove(int depth)
{
    vector<Move> best_moves;
    int best_score = -100000;
    auto moves = LegalMoves();
    int alpha = -100000;
    int beta = 100000;
    for (auto move : moves) {
        MakeMove(move);
        int score = 0 - AlphaBeta(0 - beta, 0 - alpha, depth - 1);
        if (score > best_score) {
            vector<Move>().swap(best_moves);
            best_score = score;
            best_moves.push_back(move);
        } else if (score == best_score) {
            best_moves.push_back(move);
        }
        if (score > alpha) {
            alpha = score;
        }
        UndoMove();
    }
    return best_moves[rand() % best_moves.size()];
}

int Board::AlphaBeta(int alpha, int beta, int depth)
{
    int best_score = -100000;
    Move best_move;
    if (depth == 0) {
        return Quiesce(alpha, beta);
    }
    auto moves = LegalMoves();
    for (auto move : moves) {
        MakeMove(move);
        int score = 0 - AlphaBeta(0 - beta, 0 - alpha, depth - 1);
        UndoMove();
        if (score >= beta) {
            return score;
        }
        if (score > best_score) {
            best_score = score;
        }
        if (score > alpha) {
            alpha = score;
        }
    }
    return best_score;
}

int Board::Quiesce(int alpha, int beta)
{
    int stand_pat = GetBoardScore();
    if (stand_pat >= beta) {
        return beta;
    }
    if (alpha < stand_pat) {
        alpha = stand_pat;
    }

    auto moves = LegalMoves();
    for (auto move : moves) {
        if (move.type == "attack") {
            MakeMove(move);
            int score = 0 - Quiesce(0 - alpha, 0 - beta);
            UndoMove();

            if (score >= beta) {
                return beta;
            }
            if (score > alpha) {
                alpha = score;
            }
        }
    }
}
