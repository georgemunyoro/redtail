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

    for (;;)
    {
        string comm;
        cin >> comm;
        if (comm == "isready")
        {
            cout << "readyok" << endl;
        }
        else if (comm == "uci")
        {
            cout << "uciok" << endl;
        }
        else if (comm == "position")
        {
            string position;
            cin >> position;
            if (position == "startpos")
            {
                MainBoard.SetFen(START_POS);
                string moves;
                cin >> moves;
                if (moves == "moves")
                {
                    string restOfline;
                    getline(cin, restOfline);
                    vector<string> uciMoves = split(restOfline, ' ');

                    for (int i = 1; i < uciMoves.size(); ++i)
                    {
                        string currentMove = uciMoves[i];

                        string FILES = "abcdefgh";
                        string RANKS = "12345678";

                        int from_row = FILES.find(currentMove[0]);
                        int from_col = RANKS.find(currentMove[1]);

                        int target_row = FILES.find(currentMove[2]);
                        int target_col = RANKS.find(currentMove[3]);

                        int from_pos = (16 * (7 - from_col)) + from_row;
                        int target_pos = (16 * (7 - target_col)) + target_row;

                        int currentMoveMove = (from_pos << 20) | (target_pos << 12) | (MainBoard.squares[from_pos] << 8) | (MainBoard.squares[target_pos] << 4) | 0;

                        MainBoard.MakeMove(currentMoveMove);
                    }
                }
            }
            else
            {
                string col, enp, cas;
                cin >> col >> enp >> cas;
                string fen = position + " " + col + " " + enp + " " + cas;
                MainBoard.SetFen(fen);
            }
        }
        else if (comm == "go" || comm == "stop")
        {
            int move = MainBoard.BestMove(6);
            cout << "bestmove " << GetRef((move >> 20) & 0xff) << GetRef((move >> 12) & 0xff)
                 << endl;
        }
        else if (comm == "listmoves")
        {
            MoveList mvs = MainBoard.LegalMoves();
            for (int i = 0; i < mvs.count; ++i)
            {
                int move = mvs.moves[i];
                cout << GetRef((move >> 20) & 0xff) << GetRef((move >> 12) & 0xff) << endl;
            }
            cout << mvs.count << endl;
        }
        else if (comm == "quit")
        {
            break;
        }
        else if (comm == "draw")
        {
            MainBoard.Draw();
        }
    }
    return 0;
}
