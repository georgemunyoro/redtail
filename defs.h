#pragma once

#include <string>
#include <vector>
#include <unordered_map>

std::vector<std::string> split(const std::string &text, char sep);
int GetColor(int piece);

#define createMove(f, t, pi, pt, tp) ((f << 20) | (t << 12) | (pi << 8) | (pt << 4) | tp)

const int N = -16;
const int E = 1;
const int S = 16;
const int W = -1;

const int KnightDirections[8] = {N + N + E, E + E + N, S + S + E, W + W + S, S + E + E, S + S + W, N + N + W, N + W + W};
const int KingDirections[8] = {N, W, N + W, S + W, S, E, E + S, N + E};
const int BishopDirections[4] = {N + E, E + S, S + W, W + N};
const int RookDirections[4] = {N, E, S, W};

struct MoveList
{
    int moves[256];
    int count = 0;
};

struct PerftResult
{
    int depth = 0;
    int nodes = 0;
    int captures = 0;
    int enPas = 0;
    int castles = 0;
    int promotions = 0;
    int checks = 0;
    int checkmates = 0;
};

enum
{
    White,
    Black,
    None
};

enum
{
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
