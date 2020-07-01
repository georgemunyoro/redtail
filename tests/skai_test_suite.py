
def parse_position(position: str) -> str:
    position, turn = position.split()
    rows = [position[i:i+8] for i in range(0, len(position), 8)]
    fen = '/'.join(rows)
    cfen, count = '', 0
    for char in fen:
        if char == '.': count += 1
        elif char != '.':
            if count > 0:
                cfen += str(count)
                count = 0
            cfen += char
    cfen = ' '.join([cfen, turn, '-', '-', '0 1'])
    
    return cfen
