#pragma once
#include <string>

#include "board.h"
#include "defs.h"

class UCI {
 public:
  UCI();
  ~UCI();
  int loop();

 private:
  void uci();
  void position();
  void isReady();
  void stop();
  void go();
  void perft();
  void score();
  void listMoves();
  void handleInput(std::string comm);

  Board board;
};

