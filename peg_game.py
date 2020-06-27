#!/usr/bin/env python3
"""Generate all solutions to the peg game - 15 holes & 14 pegs."""

# This implementation is a pure python solution without any optimization.
# It can be used as a baseline for comparison with other implementations.

# The allowed moves for each position are a tuple consisting of the
# position being jumped over and the position being jumped to. The
# from position is the index in the array.
ALLOWED_MOVES = (
    ((1, 3), (2, 5)),  # 0
    ((3, 6), (4, 8)),  # 1
    ((4, 7), (5, 9)),  # 2
    ((1, 0), (4, 5), (7, 12), (6, 10)),  # 3
    ((7, 11), (8, 13)),  # 4
    ((2, 0), (4, 3), (8, 12), (9, 14)),  # 5
    ((3, 1), (7, 8)),  # 6
    ((4, 2), (8, 9)),  # 7
    ((4, 1), (7, 6)),  # 8
    ((5, 2), (8, 7)),  # 9
    ((6, 3), (11, 12)),  # 10
    ((7, 4), (12, 13)),  # 11
    ((7, 3), (8, 5), (11, 10), (13, 14)),  # 12
    ((8, 4), (12, 11)),  # 13
    ((9, 5), (13, 12)),  # 14
)

# Histogram of # remaining pegs at the end of each game
remaining_count = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]


def validate(moves):
    """Given a set of moves, make sure they are all legal and end with no more possible moves."""
    assert len(moves) > 0
    assert len(moves) < 14

    # Create the starting board by finding the 'to' element of the first move
    # and setting that position to blank
    empty_spot = moves[0][2]
    board = [empty_spot != x for x in range(15)]

    # For each move in the game, verify that it is legal and then apply it to the board
    for pos, over, to in moves:
        assert board[pos]
        assert board[over]
        assert not board[to]
        board[pos] = False
        board[over] = False
        board[to] = True

    # Check every position to see that no more valid moves exist
    for pos in range(15):
        for over, to in ALLOWED_MOVES[pos]:
            if board[pos] and board[over]:
                assert board[to]


def move(board, moves, pos, over, to):
    """Record a move and then kick off the remainder of the game."""
    board[pos] = False
    board[over] = False
    board[to] = True
    moves.append([pos, over, to])
    game_over = play(board, moves)  # Keep playing with the updated board
    if game_over:  # that's the end of this game
        remaining_count[14 - len(moves)] += 1
        # print('Final:', (14 - len(moves), moves))
        validate(moves)


def play(board, moves):
    """Start from the existing board, walk through all available moves."""
    # This is recursive and doesn't unwind until no more valid moves remain.
    game_over = True
    for pos, _ in enumerate(board):  # for every spot on the board
        if board[pos]:  # if it has a peg
            for over, to in ALLOWED_MOVES[
                pos
            ]:  # loop over all allowed moves from that position
                if board[over] and not board[to]:  # If a move is open
                    move(board.copy(), moves.copy(), pos, over, to)
                    game_over = False

    return game_over


def main():
    """Entry point."""
    unique_starting_positions = [
        0,
        1,
        3,
        4,
    ]  # all other positions are rotations or mirrors of these
    for pos in unique_starting_positions:
        board = [pos != x for x in range(15)]
        moves = []
        play(board, moves)

    # Print the histogram
    for idx, val in enumerate(remaining_count):
        print(idx, val)


if __name__ == "__main__":
    main()
