#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "board.h"
#include "chess.h"
#include "defs.h"

using namespace std;

MoveList Board::newMoveGen() {
  MoveList moves;
  setKingPositions();

  for (int pos = 0; pos < 128; ++pos) {
    if ((pos & 0x88) != 0 || GetColor(squares[pos]) != turn)
      continue;
    int piece = squares[pos];

    if (piece == Chess::WhiteKing && pos == 116) {
      if ((castling & 8) != 0) {
        if ((squares[117] == Chess::Empty) && (squares[118] == Chess::Empty) &&
            (squares[119] == Chess::WhiteRook) &&
            !isSquareAttacked(117, Chess::Black) &&
            !isSquareAttacked(118, Chess::Black)) {
          moves.moves[moves.count] = (116 << 24) | (119 << 16) |
                                     (Chess::WhiteKing << 12) | (119 << 8) |
                                     (8 << 4) | castling;
          moves.count++;
        }
      } else if ((castling & 4) != 0) {
        if ((squares[113] == Chess::Empty) && (squares[114] == Chess::Empty) &&
            (squares[112] == Chess::WhiteRook) &&
            !isSquareAttacked(113, Chess::Black) &&
            !isSquareAttacked(114, Chess::Black)) {
          moves.moves[moves.count] = (116 << 24) | (112 << 16) |
                                     (Chess::WhiteKing << 12) | (112 << 8) |
                                     (4 << 4) | castling;
          moves.count++;
        }
      }
    }

    if (piece == Chess::BlackKing && pos == 4) {
      if ((castling & 2) != 0) {
        if ((squares[5] == Chess::Empty) && (squares[6] == Chess::Empty) &&
            (squares[7] == Chess::BlackRook) &&
            !isSquareAttacked(5, Chess::White) &&
            !isSquareAttacked(6, Chess::White)) {
          moves.moves[moves.count] = (4 << 24) | (7 << 16) |
                                     (Chess::BlackKing << 12) | (7 << 8) |
                                     (2 << 4) | castling;
          moves.count++;
        }
      } else if ((castling & 1) != 0) {
        if ((squares[1] == Chess::Empty) && (squares[2] == Chess::Empty) &&
            (squares[0] == Chess::BlackRook) &&
            !isSquareAttacked(1, Chess::White) &&
            !isSquareAttacked(2, Chess::White)) {
          moves.moves[moves.count] = (4 << 24) | (0 << 16) |
                                     (Chess::BlackKing << 12) | (0 << 8) |
                                     (1 << 4) | castling;
          moves.count++;
        }
      }
    }

    if (piece == Chess::WhitePawn) {
      if (squares[pos - 16] == Chess::Empty) {
        moves.moves[moves.count] = (pos << 24) | ((pos - 16) << 16) |
                                   (squares[pos] << 12) |
                                   (squares[pos - 16] << 8) | castling;
        moves.count++;

        if (squares[pos - 32] == Chess::Empty && (pos >> 4) == 6) {
          moves.moves[moves.count] = (pos << 24) | ((pos - 32)) << 16 |
                                     (squares[pos] << 12) |
                                     (squares[pos - 32] << 8) | castling;
          moves.count++;
        }
      }

      if (GetColor(squares[pos - 15]) == Chess::Black &&
          (pos - 15 & 0x88) == 0) {
        moves.moves[moves.count] = (pos << 24) | ((pos - 15)) << 16 |
                                   (squares[pos] << 12) |
                                   (squares[pos - 15] << 8) | castling;
        moves.count++;
      }

      if (GetColor(squares[pos - 17]) == Chess::Black &&
          (pos - 17 & 0x88) == 0) {
        moves.moves[moves.count] = (pos << 24) | ((pos - 17)) << 16 |
                                   (squares[pos] << 12) |
                                   (squares[pos - 17] << 8) | castling;
        moves.count++;
      }

    } else if (piece == Chess::BlackPawn) {
      if (squares[pos + 16] == Chess::Empty) {
        moves.moves[moves.count] = (pos << 24) | ((pos + 16) << 16) |
                                   (squares[pos] << 12) |
                                   (squares[pos + 16] << 8) | castling;
        moves.count++;

        if (squares[pos + 32] == Chess::Empty && (pos >> 4) == 1) {
          moves.moves[moves.count] = (pos << 24) | ((pos + 32)) << 16 |
                                     (squares[pos] << 12) |
                                     (squares[pos + 32] << 8) | castling;
          moves.count++;
        }
      }

      if (GetColor(squares[pos + 15]) == Chess::White &&
          (pos + 15 & 0x88) == 0) {
        moves.moves[moves.count] = (pos << 24) | ((pos + 15)) << 16 |
                                   (squares[pos] << 12) |
                                   (squares[pos + 15] << 8) | castling;
        moves.count++;
      }

      if (GetColor(squares[pos + 17]) == Chess::White &&
          (pos + 17 & 0x88) == 0) {
        moves.moves[moves.count] = (pos << 24) | ((pos + 17)) << 16 |
                                   (squares[pos] << 12) |
                                   (squares[pos + 17] << 8) | castling;
        moves.count++;
      }
    }

    if (piece == Chess::WhiteKing) {
      for (int i = 0; i < 8; ++i) {
        int dest = KingDirections[i] + pos;
        if ((dest & 0x88) == 0 && GetColor(squares[dest]) != Chess::White &&
            !isSquareAttacked(dest, Chess::Black)) {
          moves.moves[moves.count] = (pos << 24) | ((dest)) << 16 |
                                     (squares[pos] << 12) |
                                     (squares[dest] << 8) | castling;
          moves.count++;
        }
      }
    } else if (piece == Chess::BlackKing) {
      for (int i = 0; i < 8; ++i) {
        int dest = KingDirections[i] + pos;
        if ((dest & 0x88) == 0 && GetColor(squares[dest]) != Chess::Black &&
            !isSquareAttacked(dest, Chess::White)) {
          moves.moves[moves.count] = (pos << 24) | ((dest)) << 16 |
                                     (squares[pos] << 12) |
                                     (squares[dest] << 8) | castling;
          moves.count++;
        }
      }
    }

    if (piece == Chess::WhiteKnight) {
      for (int i = 0; i < 8; ++i) {
        int dest = KnightDirections[i] + pos;
        if ((dest & 0x88) == 0 && GetColor(squares[dest]) != Chess::White) {
          moves.moves[moves.count] = (pos << 24) | ((dest)) << 16 |
                                     (squares[pos] << 12) |
                                     (squares[dest] << 8) | castling;
          moves.count++;
        }
      }
    } else if (piece == Chess::BlackKnight) {
      for (int i = 0; i < 8; ++i) {
        int dest = KnightDirections[i] + pos;
        if ((dest & 0x88) == 0 && GetColor(squares[dest]) != Chess::Black) {
          moves.moves[moves.count] = (pos << 24) | ((dest)) << 16 |
                                     (squares[pos] << 12) |
                                     (squares[dest] << 8) | castling;
          moves.count++;
        }
      }
    }

    if (piece == Chess::WhiteRook || piece == Chess::WhiteQueen) {
      for (int i = 0; i < 4; ++i) {
        int dir = RookDirections[i];
        int index = dir + pos;

        while ((index & 0x88) == 0 &&
               GetColor(squares[index]) != Chess::White) {
          moves.moves[moves.count] = (pos << 24) | ((index) << 16) |
                                     (squares[pos] << 12) |
                                     (squares[index] << 8) | castling;
          moves.count++;

          if (GetColor(squares[index]) == Chess::Black)
            break;

          index += dir;
        }
      }
    } else if (piece == Chess::BlackRook || piece == Chess::BlackQueen) {
      for (int i = 0; i < 4; ++i) {
        int dir = RookDirections[i];
        int index = dir + pos;

        while ((index & 0x88) == 0 &&
               GetColor(squares[index]) != Chess::Black) {
          moves.moves[moves.count] = (pos << 24) | ((index) << 16) |
                                     (squares[pos] << 12) |
                                     (squares[index] << 8) | castling;
          moves.count++;

          if (GetColor(squares[index]) == Chess::White)
            break;

          index += dir;
        }
      }
    }

    if (piece == Chess::WhiteBishop || piece == Chess::WhiteQueen) {
      for (int i = 0; i < 4; ++i) {
        int dir = BishopDirections[i];
        int index = dir + pos;

        while ((index & 0x88) == 0 &&
               GetColor(squares[index]) != Chess::White) {
          moves.moves[moves.count] = (pos << 24) | ((index) << 16) |
                                     (squares[pos] << 12) |
                                     (squares[index] << 8) | castling;
          moves.count++;

          if (GetColor(squares[index]) == Chess::Black)
            break;

          index += dir;
        }
      }
    } else if (piece == Chess::BlackBishop || piece == Chess::BlackQueen) {
      for (int i = 0; i < 4; ++i) {
        int dir = BishopDirections[i];
        int index = dir + pos;

        while ((index & 0x88) == 0 &&
               GetColor(squares[index]) != Chess::Black) {
          moves.moves[moves.count] = (pos << 24) | ((index) << 16) |
                                     (squares[pos] << 12) |
                                     (squares[index] << 8) | castling;
          moves.count++;

          if (GetColor(squares[index]) == Chess::White)
            break;

          index += dir;
        }
      }
    }
  }

  return moves;
}

bool Board::isSquareAttacked(int pos, int color) {
  if (color == Chess::Black) {
    if (((pos - 17) & 0x88) == 0 && squares[pos - 17] == Chess::BlackPawn)
      return true;
    if (((pos - 15) & 0x88) == 0 && squares[pos - 15] == Chess::BlackPawn)
      return true;
  } else if (color == Chess::White) {
    if (((pos + 17) & 0x88) == 0 && squares[pos + 17] == Chess::WhitePawn)
      return true;
    if (((pos + 15) & 0x88) == 0 && squares[pos + 15] == Chess::WhitePawn)
      return true;
  }

  for (int i = 0; i < 8; ++i) {
    int piece = squares[pos + KnightDirections[i]];
    if (color == Chess::Black && piece == Chess::BlackKnight) {
      return true;
    } else if (color == Chess::White && piece == Chess::WhiteKnight) {
      return true;
    }
  }

  for (int i = 0; i < 4; ++i) {
    int dir = RookDirections[i];
    int index = dir + pos;
    int piece = squares[index];

    while ((index & 0x88) == 0) {
      if (piece != Chess::Empty) {
        if ((piece == Chess::WhiteRook || piece == Chess::BlackRook ||
             piece == Chess::WhiteQueen || piece == Chess::BlackQueen) &&
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
      if (piece != Chess::Empty) {
        if ((piece == Chess::WhiteBishop || piece == Chess::BlackBishop ||
             piece == Chess::WhiteQueen || piece == Chess::BlackQueen) &&
            GetColor(piece) == color)
          return true;

        break;
      }
      index += dir;
      piece = squares[index];
    }
  }

  for (int i = 0; i < 8; ++i) {
    if ((pos + KingDirections[i] & 0x88) != 0)
      continue;
    int piece = squares[pos + KingDirections[i]];
    if (color == Chess::White && piece == Chess::WhiteKing) {
      return true;
    } else if (color == Chess::Black && piece == Chess::BlackKing) {
      return true;
    }
  }

  return false;
}
