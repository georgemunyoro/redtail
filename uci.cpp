#include "board.h"
#include "defs.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main()
{
    auto MainBoard = Board();
    const string START_POS = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    MainBoard.SetFen(START_POS);

    cout << "id name Skai" << endl;
    cout << "id author George TMG" << endl;
    cout << "uciok" << endl;

    for (;;) {
        string comm;
        cin >> comm;
        if (comm == "isready") {
            cout << "readyok" << endl;
        } else if (comm == "uci") {
            cout << "uciok" << endl;
        } else if (comm == "position") {
            string position;
            cin >> position;
            if (position == "startpos") {
                MainBoard.SetFen(START_POS);
                string moves;
                cin >> moves;
                if (moves == "moves") {
                    string restOfline;
                    getline(cin, restOfline);
                    vector<string> uciMoves = split(restOfline, ' ');

                    for (int i = 1; i < uciMoves.size(); ++i) {
                        string currentMove = uciMoves[i];

                        string FILES = "abcdefgh";
                        string RANKS = "12345678";

                        int from_row = FILES.find(currentMove[0]);
                        int from_col = RANKS.find(currentMove[1]);

                        int target_row = FILES.find(currentMove[2]);
                        int target_col = RANKS.find(currentMove[3]);

                        int from_pos = (16 * (7 - from_col)) + from_row;
                        int target_pos = (16 * (7 - target_col)) + target_row;

                        Move currentMoveMove({ from_pos, target_pos,
                            MainBoard.squares[from_pos],
                            MainBoard.squares[target_pos], "quiet" });

                        MainBoard.MakeMove(currentMoveMove);
                    }
                }
            } else {
                string col, enp, cas;
                cin >> col >> enp >> cas;
                string fen = position + " " + col + " " + enp + " " + cas;
                MainBoard.SetFen(fen);
            }
        } else if (comm == "go" || comm == "stop") {
            auto move = MainBoard.BestMove(4);
            cout << "bestmove " << GetRef(move.initial) << GetRef(move.target)
                 << endl;
        } else if (comm == "listmoves") {
            auto mvs = MainBoard.PseudoMoves();
            for (auto move : mvs) {
                cout << GetRef(move.initial) << " " << GetRef(move.target) << endl;
            }
        } else if (comm == "quit") {
            break;
        }
    }
    return 0;
}
