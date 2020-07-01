import chess
from skai_test_suite import parse_position

def main():
    with open('checks.txt') as generated_checks_file:
        for line in generated_checks_file.readlines():
            fen = parse_position(line)
            board = chess.Board(fen)
            print(board.is_check())

if __name__ == '__main__':
    main()