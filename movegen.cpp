#include "board.h"
#include "defs.h"
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

std::unordered_map<int, std::vector<int>> PieceMovements = {
	{WhiteKnight, {N+N+E, E+E+N, S+S+E, W+W+S, S+E+E, S+S+W, N+N+W, N+W+W }},
	{WhiteQueen,  {N, W, N+W, S+W, S, E, E+S, N+E }},
	{WhiteRook,   {N, E, S, W}},
	{WhiteKing,   {N, W, N+W, S+W, S, E, E+S, N+E }},
	{WhiteBishop, {N+E, E+S, S+W, W+N }},
	{BlackKnight, {N+N+E, E+ E+N, S+S+E, W+W+S, S+E+E, S+S+W, N+N+W, N+W+W }},
	{BlackQueen,  {N, W, N+W, S+W, S, E, E+S, N+E }},
	{BlackRook,   {N, E, S, W}},
	{BlackKing,   {N, W, N+W, S+W, S, E, E+S, N+E }},
	{BlackBishop, {N+ E, E+S, S+W, W+N }}
};

vector<Move> Board::PseudoCaptures()
{
    vector<Move> moves;

    for (int pos = 0; pos < 128; ++pos) {
        int piece = squares[pos];

        if ((pos & 0x88) != 0 || piece == Empty)
            continue;
        if (GetColor(piece) != turn)
            continue;

        if (piece == WhitePawn || piece == BlackPawn) {
            vector<int> possible;
            int start_row = turn == White ? 6 : 1;
            if (turn == White) {
                possible = { pos + N + E, pos + N + W };
            } else {
                possible = { pos + S + E, pos + S + W };
            }

            for (auto dest : { possible[0], possible[1] }) {
                if (GetColor(squares[dest]) != None) {
                    if ((turn == White && GetColor(squares[dest]) == Black) || (turn == Black && GetColor(squares[dest]))) {
                        moves.push_back({ pos, dest, squares[pos], squares[dest], "attack" });
                    }
                }
            }
        } else {
            vector<int> MOVES;
            if (piece == WhiteKnight || piece == BlackKnight) {
                MOVES = { N + N + E, E + E + N, S + S + E, W + W + S,
                    S + E + E, S + S + W, N + N + W, N + W + W };
            } else if (piece == WhiteQueen || piece == BlackQueen)
                MOVES = { N, W, N + W, S + W, S, E, E + S, N + E };
            else if (piece == WhiteRook || piece == BlackRook)
                MOVES = { N, E, S, W };
            else if (piece == WhiteKing || piece == BlackKing)
                MOVES = { N, W, N + W, S + W, S, E, E + S, N + E };
            else if (piece == WhiteBishop || piece == BlackBishop)
                MOVES = { N + E, E + S, S + W, W + N };

            for (int possibleMove : MOVES) {
                int index = pos;
                for (;;) {
                    index += possibleMove;
                    if ((index & 0x88) != 0)
                        break;
                    if ((turn == White && GetColor(squares[index]) == White) || (turn == Black && GetColor(squares[index]) == Black))
                        break;
                    if (squares[index] != Empty) {
                        moves.push_back(
                            { pos, index, squares[pos], squares[index], "attack" });
                        break;
                    }
                    if (piece == WhiteKing || piece == WhiteKnight || piece == BlackKnight || piece == BlackKing)
                        break;
                }
            }
            vector<int>().swap(MOVES);
        }
    }
    return moves;
}

vector<Move> Board::PseudoMoves()
{
    vector<Move> moves;
    moves.reserve(256);

    for (int pos = 0; pos < 128; ++pos) {
        int piece = squares[pos];

        if ((pos & 0x88) != 0 || piece == Empty)
            continue;
        if (GetColor(piece) != turn)
            continue;

        if (piece == WhitePawn || piece == BlackPawn) {
            vector<int> possible;
            int start_row = turn == White ? 6 : 1;
            if (turn == White) {
                possible = { pos + N, pos + N + N, pos + N + E, pos + N + W };
            } else {
                possible = { pos + S, pos + S + S, pos + S + E, pos + S + W };
            }

            if (squares[possible[0]] == Empty) {
                moves.push_back({ pos, possible[0], squares[pos], squares[possible[0]], "quiet" });
                if (squares[possible[1]] == Empty && floor(pos / 16) == start_row) {
                    Move move { pos, possible[0], squares[pos], squares[possible[0]], "quiet" };
                    moves.push_back(
                        { pos, possible[1], squares[pos], squares[possible[1]], "quiet" });
                }
            }

            for (auto dest : { possible[2], possible[3] }) {
                if (GetColor(squares[dest]) != None) {
                    if (GetColor(squares[dest]) != turn && squares[dest] != 14) {
                        moves.push_back({ pos, dest, squares[pos], squares[dest], "attack" });
                    }
                }
            }
        } else {
            vector<int> MOVES;
            if (piece == WhiteKnight || piece == BlackKnight) {
                MOVES = { N + N + E, E + E + N, S + S + E, W + W + S,
                    S + E + E, S + S + W, N + N + W, N + W + W };
            } else if (piece == WhiteQueen || piece == BlackQueen)
                MOVES = { N, W, N + W, S + W, S, E, E + S, N + E };
            else if (piece == WhiteRook || piece == BlackRook)
                MOVES = { N, E, S, W };
            else if (piece == WhiteKing || piece == BlackKing)
                MOVES = { N, W, N + W, S + W, S, E, E + S, N + E };
            else if (piece == WhiteBishop || piece == BlackBishop)
                MOVES = { N + E, E + S, S + W, W + N };

            for (int possibleMove : MOVES) {
                int index = pos;
                for (;;) {
                    index += possibleMove;
                    if ((index & 0x88) != 0)
                        break;
                    if ((turn == White && GetColor(squares[index]) == White) || (turn == Black && GetColor(squares[index]) == Black))
                        break;

                    string move_type = squares[index] == Empty ? "quiet" : "attack";
                    moves.push_back(
                        { pos, index, squares[pos], squares[index], move_type });

                    if (piece == WhiteKing || piece == WhiteKnight || piece == BlackKnight || piece == BlackKing)
                        break;
                    if (squares[index] != Empty)
                        break;
                }
            }
            vector<int>().swap(MOVES);
        }
    }
    return moves;
}

MoveList Board::generatePseudoMoves() {
	MoveList moves;

    for (int pos = 0; pos < 128; ++pos) {
        int piece = squares[pos];

        if ((pos & 0x88) != 0 || piece == Empty)
            continue;
        if (GetColor(piece) != turn)
            continue;

        if (piece == WhitePawn || piece == BlackPawn) {
            vector<int> possible;
            int start_row = turn == White ? 6 : 1;
            if (turn == White) {
                possible = { pos + N, pos + N + N, pos + N + E, pos + N + W };
            } else {
                possible = { pos + S, pos + S + S, pos + S + E, pos + S + W };
            }

            if (squares[possible[0]] == Empty) {
                moves.moves[moves.count] = { pos, possible[0], squares[pos], squares[possible[0]], "quiet" };
				moves.count++;
                if (squares[possible[1]] == Empty && floor(pos / 16) == start_row) {
					moves.moves[moves.count] = {pos, possible[1], squares[pos], squares[possible[1]], "quiet"};
					moves.count++;
                }
            }

            for (auto dest : { possible[2], possible[3] }) {
                if (GetColor(squares[dest]) != None) {
                    if (GetColor(squares[dest]) != turn && squares[dest] != 14) {
						moves.moves[moves.count] = {pos, dest, squares[pos], squares[dest], "attack"};
						moves.count++;
                    }
                }
            }
        } else {

            for (int possibleMove : PieceMovements[piece]) {
                int index = pos;
                for (;;) {
                    index += possibleMove;
                    if ((index & 0x88) != 0) break;

					if (GetColor(squares[index]) == turn)
						break;

					moves.moves[moves.count] = {pos, index, squares[pos], squares[index], squares[index] == Empty ? "quiet" : "attack"};
					moves.count++;

                    if (piece == WhiteKing || piece == WhiteKnight || piece == BlackKnight || piece == BlackKing)
                        break;
                    if (squares[index] != Empty)
                        break;
                }
            }
        }
    }
	return moves;
}

