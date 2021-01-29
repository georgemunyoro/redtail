#include "uci.h"

#include <iostream>
#include <string>
#include <vector>

#include "board.h"
#include "chess.h"
#include "defs.h"

UCI::UCI() {}
UCI::~UCI() {}

void UCI::isReady() { std::cout << "readyok" << std::endl; }

void UCI::uci() {
  std::cout << "id name Skai" << std::endl;
  std::cout << "id author George Munyoro" << std::endl;
  std::cout << "uciok" << std::endl;
}

void UCI::stop() { board.Search(); }

void UCI::go() { board.Search(); }

void UCI::perft() {
  int depth;
  std::cin >> depth;
  board.PerftTest(depth, false);
}

void UCI::score() { std::cout << board.CalculateMaterial() << std::endl; }

void UCI::listMoves() {
  MoveList legalMoves = board.LegalMoves();
  std::vector<int> PV;
  for (int i = 0; i < legalMoves.count; ++i) {
    int move = legalMoves.moves[i];
    board.MakeMove(move);
    std::cout << GetRef((move >> 20) & 0xff) << GetRef((move >> 12) & 0xff)
              << " " << board.AlphaBeta(-100000, 100000, 3, PV) << " "
              << std::endl;
    board.UndoMove();
  }
  std::cout << legalMoves.count << std::endl;
}

void UCI::position() {
  std::string position;
  std::cin >> position;

  if (position == "startpos")
    board.SetFen(Chess::START_POS);
  if (position == "fen") {
    std::string pos, color, castling, enPas, ply, halfMoves;
    std::cin >> pos >> color >> castling >> enPas >> ply >> halfMoves;
    std::string fen = pos + " " + color + " " + castling + " " + enPas + " " +
                      ply + " " + halfMoves;

    board.SetFen(fen);

    std::string moves;
    std::cin >> moves;

    if (moves == "moves") {
      std::string restOfLine;
      std::getline(std::cin, restOfLine);
      std::vector<std::string> uciMoves = split(restOfLine, ' ');

      for (int i = 1; i < uciMoves.size(); ++i) {
        std::string currentMove = uciMoves[i];

        int fromRow = Chess::FILES.find(currentMove[0]);
        int fromCol = Chess::RANKS.find(currentMove[1]);

        int targetRow = Chess::FILES.find(currentMove[2]);
        int targetCol = Chess::RANKS.find(currentMove[3]);

        int fromPos = (16 * (7 - fromCol)) + fromRow;
        int targetPos = (16 * (7 - targetCol)) + targetRow;

        int currentMoveMove = (fromPos << 20) | (targetPos << 12) |
                              (board.squares[fromPos] << 8) |
                              (board.squares[targetPos] << 4) | 0;

        board.MakeMove(currentMoveMove);
      }
    } else {
      handleInput(moves);
    }
  }
}

void UCI::handleInput(std::string comm) {
  if (comm == "uci")
    uci();
  if (comm == "position")
    position();
  if (comm == "isready")
    isReady();
  if (comm == "go")
    go();
  if (comm == "score")
    score();
  if (comm == "stop")
    stop();

  if (comm == "draw")
    board.Draw();
  if (comm == "perft")
    perft();
  if (comm == "listmoves")
    listMoves();
}

int UCI::loop() {
  board = Board();

  board.SetFen(Chess::START_POS);
  std::string comm;
  do {
    std::cin >> comm;
    if (comm == "quit")
      break;
    else
      handleInput(comm);
  } while (comm != "quit");
  return 0;
}

int main() {
  UCI uci = UCI();
  return uci.loop();
}
