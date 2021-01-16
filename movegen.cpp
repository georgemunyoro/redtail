#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "board.h"
#include "defs.h"

using namespace std;

MoveList Board::newMoveGen() {
  MoveList moves;
  setKingPositions();

  for (int pos = 0; pos < 128; ++pos) {
    if ((pos & 0x88) != 0 || GetColor(squares[pos]) != turn) continue;
    int piece = squares[pos];

    if (piece == WhitePawn) {
      if (squares[pos - 16] == Empty) {
	moves.moves[moves.count] = (pos << 20) | ((pos - 16) << 12) |
				   (squares[pos] << 8) |
				   (squares[pos - 16] << 4) | castling;
	moves.count++;

	if (squares[pos - 32] == Empty && (pos >> 4) == 6) {
	  moves.moves[moves.count] = (pos << 20) | ((pos - 32)) << 12 |
				     (squares[pos] << 8) |
				     (squares[pos - 32] << 4) | castling;
	  moves.count++;
	}
      }

      if (GetColor(squares[pos - 15]) == Black && (pos - 15 & 0x88) == 0) {
	moves.moves[moves.count] = (pos << 20) | ((pos - 15)) << 12 |
				   (squares[pos] << 8) |
				   (squares[pos - 15] << 4) | castling;
	moves.count++;
      }

      if (GetColor(squares[pos - 17]) == Black && (pos - 17 & 0x88) == 0) {
	moves.moves[moves.count] = (pos << 20) | ((pos - 17)) << 12 |
				   (squares[pos] << 8) |
				   (squares[pos - 17] << 4) | castling;
	moves.count++;
      }

    } else if (piece == BlackPawn) {
      if (squares[pos + 16] == Empty) {
	moves.moves[moves.count] = (pos << 20) | ((pos + 16) << 12) |
				   (squares[pos] << 8) |
				   (squares[pos + 16] << 4) | castling;
	moves.count++;

	if (squares[pos + 32] == Empty && (pos >> 4) == 1) {
	  moves.moves[moves.count] = (pos << 20) | ((pos + 32)) << 12 |
				     (squares[pos] << 8) |
				     (squares[pos + 32] << 4) | castling;
	  moves.count++;
	}
      }

      if (GetColor(squares[pos + 15]) == White && (pos + 15 & 0x88) == 0) {
	moves.moves[moves.count] = (pos << 20) | ((pos + 15)) << 12 |
				   (squares[pos] << 8) |
				   (squares[pos + 15] << 4) | castling;
	moves.count++;
      }

      if (GetColor(squares[pos + 17]) == White && (pos + 17 & 0x88) == 0) {
	moves.moves[moves.count] = (pos << 20) | ((pos + 17)) << 12 |
				   (squares[pos] << 8) |
				   (squares[pos + 17] << 4) | castling;
	moves.count++;
      }
    }

    if (piece == WhiteKing) {
      for (int i = 0; i < 8; ++i) {
	int dest = KingDirections[i] + pos;
	if ((dest & 0x88) == 0 && GetColor(squares[dest]) != White &&
	    !isSquareAttacked(dest, Black)) {
	  moves.moves[moves.count] = (pos << 20) | ((dest)) << 12 |
				     (squares[pos] << 8) |
				     (squares[dest] << 4) | castling;
	  moves.count++;
	}
      }
    } else if (piece == BlackKing) {
      for (int i = 0; i < 8; ++i) {
	int dest = KingDirections[i] + pos;
	if ((dest & 0x88) == 0 && GetColor(squares[dest]) != Black &&
	    !isSquareAttacked(dest, White)) {
	  moves.moves[moves.count] = (pos << 20) | ((dest)) << 12 |
				     (squares[pos] << 8) |
				     (squares[dest] << 4) | castling;
	  moves.count++;
	}
      }
    }

    if (piece == WhiteKnight) {
      for (int i = 0; i < 8; ++i) {
	int dest = KnightDirections[i] + pos;
	if ((dest & 0x88) == 0 && GetColor(squares[dest]) != White) {
	  moves.moves[moves.count] = (pos << 20) | ((dest)) << 12 |
				     (squares[pos] << 8) |
				     (squares[dest] << 4) | castling;
	  moves.count++;
	}
      }
    } else if (piece == BlackKnight) {
      for (int i = 0; i < 8; ++i) {
	int dest = KnightDirections[i] + pos;
	if ((dest & 0x88) == 0 && GetColor(squares[dest]) != Black) {
	  moves.moves[moves.count] = (pos << 20) | ((dest)) << 12 |
				     (squares[pos] << 8) |
				     (squares[dest] << 4) | castling;
	  moves.count++;
	}
      }
    }

    if (piece == WhiteRook || piece == WhiteQueen) {
      for (int i = 0; i < 4; ++i) {
	int dir = RookDirections[i];
	int index = dir + pos;

	while ((index & 0x88) == 0 && GetColor(squares[index]) != White) {
	  moves.moves[moves.count] = (pos << 20) | ((index) << 12) |
				     (squares[pos] << 8) |
				     (squares[index] << 4) | castling;
	  moves.count++;

	  if (GetColor(squares[index]) == Black) break;

	  index += dir;
	}
      }
    } else if (piece == BlackRook || piece == BlackQueen) {
      for (int i = 0; i < 4; ++i) {
	int dir = RookDirections[i];
	int index = dir + pos;

	while ((index & 0x88) == 0 && GetColor(squares[index]) != Black) {
	  moves.moves[moves.count] = (pos << 20) | ((index) << 12) |
				     (squares[pos] << 8) |
				     (squares[index] << 4) | castling;
	  moves.count++;

	  if (GetColor(squares[index]) == White) break;

	  index += dir;
	}
      }
    }

    if (piece == WhiteBishop || piece == WhiteQueen) {
      for (int i = 0; i < 4; ++i) {
	int dir = BishopDirections[i];
	int index = dir + pos;

	while ((index & 0x88) == 0 && GetColor(squares[index]) != White) {
	  moves.moves[moves.count] = (pos << 20) | ((index) << 12) |
				     (squares[pos] << 8) |
				     (squares[index] << 4) | castling;
	  moves.count++;

	  if (GetColor(squares[index]) == Black) break;

	  index += dir;
	}
      }
    } else if (piece == BlackBishop || piece == BlackQueen) {
      for (int i = 0; i < 4; ++i) {
	int dir = BishopDirections[i];
	int index = dir + pos;

	while ((index & 0x88) == 0 && GetColor(squares[index]) != Black) {
	  moves.moves[moves.count] = (pos << 20) | ((index) << 12) |
				     (squares[pos] << 8) |
				     (squares[index] << 4) | castling;
	  moves.count++;

	  if (GetColor(squares[index]) == White) break;

	  index += dir;
	}
      }
    }
  }

  return moves;
}

bool Board::isSquareAttacked(int pos, int color) {
  if (color == Black) {
    if (((pos - 17) & 0x88) == 0 && squares[pos - 17] == BlackPawn) return true;
    if (((pos - 15) & 0x88) == 0 && squares[pos - 15] == BlackPawn) return true;
  } else if (color == White) {
    if (((pos + 17) & 0x88) == 0 && squares[pos + 17] == WhitePawn) return true;
    if (((pos + 15) & 0x88) == 0 && squares[pos + 15] == WhitePawn) return true;
  }

  for (int i = 0; i < 8; ++i) {
    int piece = squares[pos + KnightDirections[i]];
    if (color == Black && piece == BlackKnight) {
      return true;
    } else if (color == White && piece == WhiteKnight) {
      return true;
    }
  }

  for (int i = 0; i < 4; ++i) {
    int dir = RookDirections[i];
    int index = dir + pos;
    int piece = squares[index];

    while ((index & 0x88) == 0) {
      if (piece != Empty) {
	if ((piece == WhiteRook || piece == BlackRook || piece == WhiteQueen ||
	     piece == BlackQueen) &&
	    GetColor(piece) == color)
	  return true;

	break;
      }
      index += dir;
      piece = squares[index];
    }
  }

  for (int i = 0; i < 4; ++i) {
    int dir = BishopDirections[i];
    int index = dir + pos;
    int piece = squares[index];

    while ((index & 0x88) == 0) {
      if (piece != Empty) {
	if ((piece == WhiteBishop || piece == BlackBishop ||
	     piece == WhiteQueen || piece == BlackQueen) &&
	    GetColor(piece) == color)
	  return true;

	break;
      }
      index += dir;
      piece = squares[index];
    }
  }

  for (int i = 0; i < 8; ++i) {
    if ((pos + KingDirections[i] & 0x88) != 0) continue;
    int piece = squares[pos + KingDirections[i]];
    if (color == White && piece == WhiteKing) {
      return true;
    } else if (color == Black && piece == BlackKing) {
      return true;
    }
  }

  return false;
}

