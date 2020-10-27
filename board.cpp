#include "board.h"
#include "defs.h"
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

Board::~Board() {}
Board::Board() {}

std::vector<std::string> split(const std::string &text, char sep)
{
    std::vector<std::string> tokens;
    std::size_t start = 0, end = 0;
    while ((end = text.find(sep, start)) != std::string::npos)
    {
        tokens.push_back(text.substr(start, end - start));
        start = end + 1;
    }
    tokens.push_back(text.substr(start));
    return tokens;
}

int GetColor(int piece)
{
    if (piece < 6)
        return White;
    else if (piece > 5)
        return Black;

    return -1;
}

string GetRef(int pos)
{
    int row = 8 - floor(pos / 16);
    int col = pos % 8;
    string ref = "abcdefgh"[col] + to_string(row);
    return ref;
}

void Board::ClearBoard()
{
    for (int i = 0; i < 128; ++i)
    {
        squares[i] = 14;
        if ((i & 0x88) == 0)
            squares[i] = Empty;
    }
}

bool Board::kingAttacked()
{
    if (turn == Black)
    {
        if (isSquareAttacked(WhiteKingPosition, Black))
        {
            return true;
        }
    }
    else if (turn == White)
    {
        if (isSquareAttacked(BlackKingPosition, White))
        {
            return true;
        }
    }
    return false;
}

MoveList Board::LegalMoves()
{
    MoveList moves;
    MoveList pseudoMoves = newMoveGen();
    for (int i = 0; i < pseudoMoves.count; ++i)
    {
        int move = pseudoMoves.moves[i];
        MakeMove(move);
        if (!kingAttacked())
        {
            moves.moves[moves.count] = move;
            moves.count++;
        }
        UndoMove();
    }
    return moves;
}

void Board::Draw()
{
    int index = 0;
    for (int i = 0; i < 128; ++i)
    {
        if (index == 8)
        {
            cout << endl;
            index = 0;
        }
        if ((i & 0x88) == 0)
        {
            printf("%2c", PIECE_CHAR_MAP[squares[i]]);
            ++index;
        }
    }
    cout << endl;
    cout << "WB"[turn] << endl;
    cout << "BLACK CHECK : " << isSquareAttacked(BlackKingPosition, White) << endl;
    cout << "WHITE CHECK : " << isSquareAttacked(WhiteKingPosition, Black) << endl;

    cout << endl;
}

string Board::GenerateFen()
{
    string board = "";
    string currentRow = "";
    for (int i = 0; i < 128; ++i)
    {
        if (currentRow.size() == 8)
        {
            board += currentRow;
            currentRow = "";
        }
        if ((i & 0x88) == 0)
        {
            currentRow += PIECE_CHAR_MAP[squares[i]];
        }
    }
    return board;
}

void Board::switchTurn() {
    turn ^= 1;
}

void Board::MakeMove(int mov)
{
    moveHistory[historyIndex] = mov;
    squares[(mov >> 12) & 0xff] = (mov >> 8) & 0xf;
    squares[(mov >> 20) & 0xff] = Empty;
    ++historyIndex;
    switchTurn();

    if ((mov >> 8) & 0xf == WhiteKing)
        WhiteKingPosition = (mov >> 12) & 0xff;

    if ((mov >> 8) & 0xf == BlackKing)
        BlackKingPosition = (mov >> 12) & 0xff;
}

void Board::UndoMove()
{
    --historyIndex;
    int lastMove = moveHistory[historyIndex];
    squares[(lastMove >> 20) & 0xff] = (lastMove >> 8) & 0xf;
    squares[(lastMove >> 12) & 0xff] = (lastMove >> 4) & 0xf;
    switchTurn();
}

void Board::SetFen(string fen)
{
    ClearBoard();
    vector<string> splitFen = split(fen, ' ');
    string position = splitFen[0];
    int index = 0;
    for (char piece : position)
    {
        switch (piece)
        {
        case 'R':
            squares[index] = WhiteRook;
            break;
        case 'N':
            squares[index] = WhiteKnight;
            break;
        case 'B':
            squares[index] = WhiteBishop;
            break;
        case 'Q':
            squares[index] = WhiteQueen;
            break;
        case 'K':
            squares[index] = WhiteKing;
            WhiteKingPosition = index;
            break;
        case 'P':
            squares[index] = WhitePawn;
            break;

        case 'r':
            squares[index] = BlackRook;
            break;
        case 'n':
            squares[index] = BlackKnight;
            break;
        case 'b':
            squares[index] = BlackBishop;
            break;
        case 'q':
            squares[index] = BlackQueen;
            break;
        case 'k':
            squares[index] = BlackKing;
            BlackKingPosition = index;
            break;
        case 'p':
            squares[index] = BlackPawn;
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

    string fenTurn = splitFen[1];
    string fenCastling = splitFen[2];
    string fenEnPas = splitFen[3];
    string fenHalf = splitFen[4];
    string fenPly = splitFen[5];

    turn = fenTurn[1] == 'b' ? Black : White;
}
