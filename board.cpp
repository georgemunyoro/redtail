#include "board.h"

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "chess.h"
#include "defs.h"

Board::~Board() {}
Board::Board() {}

std::vector<std::string> split(const std::string& text, char sep) {
  std::vector<std::string> tokens;
  std::size_t start = 0, end = 0;
  while ((end = text.find(sep, start)) != std::string::npos) {
    tokens.push_back(text.substr(start, end - start));
    start = end + 1;
  }
  tokens.push_back(text.substr(start));
  return tokens;
}

int GetColor(int piece) {
  if (piece == Chess::Empty) return -1;
  if (piece < 6) return Chess::White;

  return Chess::Black;
}

std::string GetRef(int pos) {
  int row = 8 - floor(pos / 16);
  int col = pos % 8;
  std::string ref = "abcdefgh"[col] + std::to_string(row);
  return ref;
}

void Board::ClearBoard() {
  for (int i = 0; i < 128; ++i) {
    squares[i] = 14;
    if ((i & 0x88) == 0) squares[i] = Chess::Empty;
  }
}

bool Board::kingAttacked() {
  if (turn == Chess::Black) {
    if (isSquareAttacked(WhiteKingPosition, Chess::Black)) {
      return true;
    }
  } else if (turn == Chess::White) {
    if (isSquareAttacked(BlackKingPosition, Chess::White)) {
      return true;
    }
  }
  return false;
}

void Board::setKingPositions() {
  for (int i = 0; i < 128; ++i) {
    if ((i & 0x88) == 0) {
      if (squares[i] == Chess::BlackKing) {
	BlackKingPosition = i;
      } else if (squares[i] == Chess::WhiteKing) {
	WhiteKingPosition = i;
      }
    }
  }
}

MoveList Board::LegalMoves() {
  MoveList moves;
  MoveList pseudoMoves = newMoveGen();
  for (int i = 0; i < pseudoMoves.count; ++i) {
    int move = pseudoMoves.moves[i];
    MakeMove(move);
    if (!kingAttacked()) {
      moves.moves[moves.count] = move;
      moves.count++;
    }
    UndoMove();
  }
  return moves;
}

void Board::Draw() {
  int oturn = turn;
  int index = 0;
  for (int i = 0; i < 128; ++i) {
    if (index == 8) {
      std::cout << std::endl;
      index = 0;
    }
    if ((i & 0x88) == 0) {
      printf("%2c", PIECE_CHAR_MAP[squares[i]]);
      ++index;
    }
  }

  std::cout << std::endl;
  std::cout << "WB"[turn] << std::endl;
  std::cout << GetRef(BlackKingPosition)
	    << " WHITE : " << GetRef(WhiteKingPosition) << std::endl;
  turn = Chess::Black;
  std::cout << "BLACK CHECK : " << kingAttacked() << std::endl;
  turn = Chess::White;
  std::cout << "WHITE CHECK : " << kingAttacked() << std::endl;
  turn = oturn;
  std::cout << std::endl;

  std::cout << "K : " << ((castling >> 3)) << std::endl;
  std::cout << "Q : " << ((castling >> 2) & 1) << std::endl;
  std::cout << "k : " << ((castling >> 1) & 1) << std::endl;
  std::cout << "q : " << (castling & 1) << std::endl << std::endl;

  index = 0;
  for (int i = 0; i < 128; ++i) {
    if (index == 8) {
      std::cout << std::endl;
      index = 0;
    }
    if ((i & 0x88) == 0) {
      printf("%2c", isSquareAttacked(i, Chess::Black) ? 'x' : '.');
      ++index;
    }
  }

  std::cout << std::endl;

  index = 0;
  for (int i = 0; i < 128; ++i) {
    if (index == 8) {
      std::cout << std::endl;
      index = 0;
    }
    if ((i & 0x88) == 0) {
      printf("%2c", isSquareAttacked(i, Chess::White) ? 'x' : '.');
      ++index;
    }
  }

  std::cout << std::endl;
  std::cout << (turn ^ 1) << std::endl << std::endl;
}

std::string Board::GenerateFen() {
  std::string board = "";
  std::string currentRow = "";
  for (int i = 0; i < 128; ++i) {
    if (currentRow.size() == 8) {
      board += currentRow;
      currentRow = "";
    }
    if ((i & 0x88) == 0) {
      currentRow += PIECE_CHAR_MAP[squares[i]];
    }
  }
  return board;
}

void Board::switchTurn() { turn ^= 1; }

void Board::MakeMove(int mov) {
  moveHistory[historyIndex] = mov;
  squares[(mov >> 12) & 0xff] = (mov >> 8) & 0xf;
  squares[(mov >> 20) & 0xff] = Chess::Empty;
  ++historyIndex;

  int movingPiece = (mov >> 8) & 0xf;
  if (movingPiece == Chess::WhiteKing) {
    castling &= (0xf ^ 12);
  } else if (movingPiece == Chess::BlackKing) {
    castling &= (0xf ^ 3);
  }

  switchTurn();
  setKingPositions();
}

void Board::UndoMove() {
  --historyIndex;
  int lastMove = moveHistory[historyIndex];
  squares[(lastMove >> 20) & 0xff] = (lastMove >> 8) & 0xf;
  squares[(lastMove >> 12) & 0xff] = (lastMove >> 4) & 0xf;
  castling = lastMove & 0xf;
  switchTurn();
}

void Board::SetFen(std::string fen) {
  ClearBoard();
  std::vector<std::string> splitFen = split(fen, ' ');
  std::string position = splitFen[0];
  int index = 0;
  for (char piece : position) {
    switch (piece) {
      case 'R':
	squares[index] = Chess::WhiteRook;
	break;
      case 'N':
	squares[index] = Chess::WhiteKnight;
	break;
      case 'B':
	squares[index] = Chess::WhiteBishop;
	break;
      case 'Q':
	squares[index] = Chess::WhiteQueen;
	break;
      case 'K':
	squares[index] = Chess::WhiteKing;
	WhiteKingPosition = index;
	break;
      case 'P':
	squares[index] = Chess::WhitePawn;
	break;

      case 'r':
	squares[index] = Chess::BlackRook;
	break;
      case 'n':
	squares[index] = Chess::BlackKnight;
	break;
      case 'b':
	squares[index] = Chess::BlackBishop;
	break;
      case 'q':
	squares[index] = Chess::BlackQueen;
	break;
      case 'k':
	squares[index] = Chess::BlackKing;
	BlackKingPosition = index;
	break;
      case 'p':
	squares[index] = Chess::BlackPawn;
	break;

      case '/':
	index += 7;
	break;
      case '2':
	++index;
	break;
      case '3':
	index += 2;
	break;
      case '4':
	index += 3;
	break;
      case '5':
	index += 4;
	break;
      case '6':
	index += 5;
	break;
      case '7':
	index += 6;
	break;
      case '8':
	index += 7;
	break;
    }
    ++index;
  }

  std::string fenTurn = splitFen[1];
  std::string fenCastling = splitFen[2];
  std::string fenEnPas = splitFen[3];

  std::string fenHalf = splitFen[4];
  std::string fenPly = splitFen[5];

  for (int i = 0; i < fenCastling.size(); ++i) {
    if (fenCastling[i] == 'K') castling |= 8;
    if (fenCastling[i] == 'Q') castling |= 4;
    if (fenCastling[i] == 'k') castling |= 2;
    if (fenCastling[i] == 'q') castling |= 1;
  }

  turn = fenTurn == "b" ? Chess::Black : Chess::White;
}

void Board::PerftTest(int depth, bool debug) {
  std::cout << "Test running to depth : " << depth << std::endl;
  int leafNodes = 0;
  long startTime = getTime();

  MoveList moves = LegalMoves();

  for (int i = 0; i < moves.count; ++i) {
    int currentMove = moves.moves[i];
    MakeMove(moves.moves[i]);
    auto perft_res = Perft(depth - 1, debug);
    leafNodes += perft_res.nodes;
    std::cout << GetRef((currentMove >> 20) & 0xff)
	      << GetRef((currentMove >> 12) & 0xff) << ": " << perft_res.nodes
	      << " " << generatePositionKey() << std::endl;
    UndoMove();
  }

  long timeTaken = getTime() - startTime;

  std::cout << std::endl << "Nodes searched : " << leafNodes << std::endl;
  std::cout << "Time elapsed : " << timeTaken << std::endl;
  return;
}

PerftResult Board::Perft(int depth, bool debug) {
  auto moves = LegalMoves();

  int nodes = 0;

  PerftResult result;
  result.depth = depth;

  if (depth == 1) {
    result.nodes += moves.count;
    return result;
  } else if (depth == 0) {
    result.nodes++;
    return result;
  }

  if (debug) {
    std::cout << GenerateFen() << " "
	      << "wb"[turn] << " ";

    for (int i = 0; i < moves.count; ++i) {
      std::cout << GetRef((moves.moves[i] >> 20) & 0xff)
		<< GetRef((moves.moves[i] >> 12) & 0xff) << " ";
    }
    std::cout << std::endl;
  }

  for (int i = 0; i < moves.count; ++i) {
    if (moves.moves[i] == 0) continue;
    auto move = moves.moves[i];

    if ((move & 0xf) == 1) result.captures++;

    MakeMove(move);
    auto res = Perft(depth - 1, debug);

    if (kingAttacked()) result.checks++;

    result.nodes += res.nodes;
    result.captures += res.captures;
    result.checks += res.checks;
    result.checkmates += res.checkmates;
    result.enPas += res.enPas;
    result.promotions = res.promotions;

    UndoMove();
  }
  return result;
}

unsigned long long Board::generatePositionKey() {
  unsigned long long key;
  for (int i = 0; i < 128; ++i) {
    if ((i & 0x88) == 0) {
      key = key << 1;
      key ^= squares[i];
    }
  }
  key = (key << 1) | turn;
  return key;
}

