#include "board.h"
#include "defs.h"
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

Board::~Board() {}
Board::Board() {}

std::vector<std::string> split(const std::string& text, char sep)
{
    std::vector<std::string> tokens;
    std::size_t start = 0, end = 0;
    while ((end = text.find(sep, start)) != std::string::npos) {
        tokens.push_back(text.substr(start, end - start));
        start = end + 1;
    }
    tokens.push_back(text.substr(start));
    return tokens;
}

int GetColor(int piece)
{
    if (piece == Empty)
        return None;
    if (piece < 6)
        return White;
    else if (piece > 5)
        return Black;
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
    for (int i = 0; i < 128; ++i) {
        squares[i] = 14;
        if ((i & 0x88) == 0)
            squares[i] = Empty;
    }
}

bool Board::kingAttacked()
{
    switchTurn();
    auto oppositionMoves = PseudoMoves();
    switchTurn();
    for (auto move : oppositionMoves) {
        if (move.pieceAtTarget == BlackKing || move.pieceAtTarget == WhiteKing) {
            return true;
        }
    }
    return false;
}

vector<Move> Board::LegalMoves()
{
    vector<Move> moves;
    auto allMoves = PseudoMoves();
    for (auto move : allMoves) {
        MakeMove(move);
        if (true)
            moves.push_back(move);
        UndoMove();
    }
    return moves;
}

void Board::Draw()
{
    int index = 0;
    for (int i = 0; i < 128; ++i) {
        if (index == 8) {
            cout << endl;
            index = 0;
        }
        if ((i & 0x88) == 0) {
            printf("%2c", PIECE_CHAR_MAP[squares[i]]);
            ++index;
        }
    }
}

string Board::GenerateFen()
{
    string board = "";
    string currentRow = "";
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

void Board::MakeMove(Move move)
{
    moveHistory[historyIndex] = move;
    squares[move.target] = move.pieceAtInitial;
    squares[move.initial] = Empty;
    ++historyIndex;
    switchTurn();
}

void Board::UndoMove()
{
    --historyIndex;
    auto lastMove = moveHistory[historyIndex];
    squares[lastMove.initial] = lastMove.pieceAtInitial;
    squares[lastMove.target] = lastMove.pieceAtTarget;
    switchTurn();
}

PerftResult Board::Perft(int depth, bool debug)
{
    auto moves = LegalMoves();

    cout << GenerateFen() << " " << "wb"[turn] << "|";
    for (auto move : moves) {
        cout << " " << GetRef(move.initial) << GetRef(move.target);
    }
    cout << endl;

    int nodes = 0;

    PerftResult result;
    result.depth = depth;

    if (depth == 0) {
        result.nodes++;
        return result;
    }

    for (auto move : moves) {
        if (move.type == "attack")
            result.captures++;

        MakeMove(move);
        auto res = Perft(depth - 1, debug);

        if (kingAttacked()) {
            result.checks++;
            // cout << GenerateFen() << " " << "wb"[turn] << endl;
        }

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

void Board::SetFen(string fen)
{
    ClearBoard();
    vector<string> splitFen = split(fen, ' ');
    string position = splitFen[0];
    int index = 0;
    for (char piece : position) {
        switch (piece) {
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
