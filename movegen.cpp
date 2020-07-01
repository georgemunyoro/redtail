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

MoveList Board::generatePseudoMoves() {
    MoveList moves;
    moves.count = 0;

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
		moves.moves[moves.count] = (pos << 20) | (possible[0] << 12) | (squares[pos] << 8) | (squares[possible[0]] << 4) | 0;
		moves.count++;
		
		if (squares[possible[1]] == Empty && floor(pos / 16) == start_row) {
		    moves.moves[moves.count] = (pos << 20) | (possible[1] << 12) | (squares[pos] << 8) | (squares[possible[1]] << 4) | 0;
		    moves.count++;
		}
	    }

	    for (auto dest : { possible[2], possible[3] }) {
		if (GetColor(squares[dest]) != None) {
		    if (GetColor(squares[dest]) != turn && squares[dest] != 14) {
			moves.moves[moves.count] = (pos << 20) | (dest << 12) | (squares[pos] << 8) | (squares[dest] << 4) | 1;
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

		    if ((turn == Black && GetColor(squares[index]) == Black) || (turn == White && GetColor(squares[index]) == White)) break;

		    moves.moves[moves.count] = (pos << 20) | (index << 12) | (squares[pos] << 8) | (squares[index] << 4) | squares[index] != Empty;
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


