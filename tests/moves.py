import chess
from skai_test_suite import parse_position

def main():
    results = []
    with open('moves.txt') as generated_checks_file:
        for line in generated_checks_file.readlines():
            try:
                position, moves = line.split('|')
                fen = parse_position(position)

                skai_moves = moves.split()

                board = chess.Board(fen)
                legal_moves = [str(i) for i in board.legal_moves]

                for move in skai_moves:
                    if move not in legal_moves:
                        results.append(f"{fen} ==:== ")
                        results.append(f'{move} is an illegal move!\n')

                for move in legal_moves:
                    if move not in skai_moves:
                        results.append(f"{fen} ==:== ")
                        results.append(f'{move} is a missed move!\n')

            except:
                pass

    open('moves_test_results.txt', 'r+').writelines(results)

if __name__ == '__main__':
    main()