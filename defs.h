#pragma once

#include <string>
#include <vector>

std::vector<std::string> split(const std::string& text, char sep);
int GetColor(int piece);

struct Move {
    int initial;
    int target;
    int pieceAtInitial;
    int pieceAtTarget;
    std::string type;
};

struct MoveList {
    Move moves[256];
    int count = 0;
};

typedef struct {
    int depth = 0;
    int nodes = 0;
    int captures = 0;
    int enPas = 0;
    int castles = 0;
    int promotions = 0;
    int checks = 0;
    int checkmates = 0;
} PerftResult;

enum {
    White,
    Black,
    None
};

enum {
    WhitePawn,
    WhiteKnight,
    WhiteBishop,
    WhiteRook,
    WhiteQueen,
    WhiteKing,
    BlackPawn,
    BlackKnight,
    BlackBishop,
    BlackRook,
    BlackQueen,
    BlackKing,
    Empty
};
