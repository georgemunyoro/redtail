#include "board.h"
#include "defs.h"
#include <vector>
#include <string>

std::unordered_map<int, std::vector<int>> PieceMovements = {
    {WhiteKnight, {N + N + E, E + E + N, S + S + E, W + W + S, S + E + E, S + S + W, N + N + W, N + W + W}},
    {WhiteQueen, {N, W, N + W, S + W, S, E, E + S, N + E}},
    {WhiteRook, {N, E, S, W}},
    {WhiteKing, {N, W, N + W, S + W, S, E, E + S, N + E}},
    {WhiteBishop, {N + E, E + S, S + W, W + N}},
    {BlackKnight, {N + N + E, E + E + N, S + S + E, W + W + S, S + E + E, S + S + W, N + N + W, N + W + W}},
    {BlackQueen, {N, W, N + W, S + W, S, E, E + S, N + E}},
    {BlackRook, {N, E, S, W}},
    {BlackKing, {N, W, N + W, S + W, S, E, E + S, N + E}},
    {BlackBishop, {N + E, E + S, S + W, W + N}}};
