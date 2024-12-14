// Implements the peg game (aka golf tee game aka Cracker Barrel game)
//

#include <iostream>
#include <vector>
#include <array>
#include <assert.h>

using std::cout;
using std::endl;

#ifndef NDEBUG
#define verify(expression) assert(expression)
#else
bool myassert(bool expression, const char *filename, int lineNum)
{
	std::cerr << "verify failure at " << filename << ':' << lineNum << endl;
	abort();
	return expression;
}

#define verify(expression) expression || myassert(expression, __FILE__, __LINE__)
#endif

typedef uint8_t boardIndex_t; // 0..14
typedef std::array<bool, 15> board_t;
typedef std::array<boardIndex_t, 3> move_t;
typedef std::vector<move_t> moves_t;

std::ostream &operator<<(std::ostream &os, const move_t &m)
{
	verify(m.size() == 3);
	os << "[" << (short)m[0] << ", " << (short)m[1] << ", " << (short)m[2] << "]";
	return os;
}

std::ostream &operator<<(std::ostream &os, const moves_t &moves)
{
	os << "[";
	for (size_t ndx = 0; ndx < moves.size() - 1; ++ndx)
	{
		os << moves[ndx] << ", ";
	}
	os << moves.back() << "]";

	return os;
}

// Histogram of # remaining pegs at the end of each game
std::array<size_t, 14> remainingCount;

// The allowed moves for each position are a tuple consisting of the
// position being jumped over and the position being jumped to. The
// from position is the index in the array.
typedef std::array<boardIndex_t, 2> over_to_t;
std::array<std::vector<over_to_t>, 15> allowedMoves;

// There is probably a clean way to initialize an array of vectors in an
// initializer but I'm taking the easy way out just to get the code working.
// TODO - initialize this the "right" way.
void initializeAllowedMoves()
{
	allowedMoves[0] = std::vector<over_to_t>({{1, 3}, {2, 5}});
	allowedMoves[1] = std::vector<over_to_t>({{3, 6}, {4, 8}});
	allowedMoves[2] = std::vector<over_to_t>({{4, 7}, {5, 9}});
	allowedMoves[3] = std::vector<over_to_t>({{1, 0}, {4, 5}, {7, 12}, {6, 10}});
	allowedMoves[4] = std::vector<over_to_t>({{7, 11}, {8, 13}});
	allowedMoves[5] = std::vector<over_to_t>({{2, 0}, {4, 3}, {8, 12}, {9, 14}});
	allowedMoves[6] = std::vector<over_to_t>({{3, 1}, {7, 8}});
	allowedMoves[7] = std::vector<over_to_t>({{4, 2}, {8, 9}});
	allowedMoves[8] = std::vector<over_to_t>({{4, 1}, {7, 6}});
	allowedMoves[9] = std::vector<over_to_t>({{5, 2}, {8, 7}});
	allowedMoves[10] = std::vector<over_to_t>({{6, 3}, {11, 12}});
	allowedMoves[11] = std::vector<over_to_t>({{7, 4}, {12, 13}});
	allowedMoves[12] = std::vector<over_to_t>({{7, 3}, {8, 5}, {11, 10}, {13, 14}});
	allowedMoves[13] = std::vector<over_to_t>({{8, 4}, {12, 11}});
	allowedMoves[14] = std::vector<over_to_t>({{9, 5}, {13, 12}});
}

/**
 * Given a set of moves, make sure they are all legal and end with no more possible moves
 */
void validate(const moves_t &moves)
{
	verify(moves.size() > 0);
	verify(moves.size() < 14);

	// Create the starting board by finding the 'to' element of the first move
	// and setting that position to blank
	auto emptySpot = moves[0][2];
	board_t board;
	board.fill(true);
	board[emptySpot] = false;

	// For each move in the game, verify that it is legal and then apply it to the board
	for (auto triple : moves)
	{
		auto pos = triple[0];
		auto over = triple[1];
		auto to = triple[2];
		verify(board[pos]);
		verify(board[over]);
		verify(!board[to]);
		board[pos] = false;
		board[over] = false;
		board[to] = true;
	}

	// Check every position to see that no more valid moves exist
	for (boardIndex_t pos = 0; pos < board.size(); ++pos)
	{
		for (auto over_to : allowedMoves[pos]) // loop over all allowed moves from that position
		{
			auto over = over_to[0];
			auto to = over_to[1];
			if (board[pos] and board[over])
			{
				verify(board[to]);
			}
		}
	}
}

// Forward declaration for recursiveness
bool play(board_t &board, moves_t &moves);

/**
 * Record a move and then kick off the remainder of the game
 * Yes, I really want board and moves to be pass-by-value
 */
void move(board_t board, moves_t moves, boardIndex_t pos, boardIndex_t over, boardIndex_t to)
{
	board[pos] = false;
	board[over] = false;
	board[to] = true;
	moves.push_back(move_t({pos, over, to}));
	bool gameOver = play(board, moves); // Keep playing with the updated board
	if (gameOver)						// that's the end of this game
	{
		remainingCount[14 - moves.size()] += 1;
		//cout << "Final: (" << (14 - moves.size()) << ", " << moves << ")\n";
		validate(moves);
	}
}

/**
 * Starting from the existing board, walk through all available moves.
 * This is recursive and doesn't unwind until no more valid moves remain.
 */
bool play(board_t &board, moves_t &moves)
{
	bool gameOver = true;
	for (boardIndex_t pos = 0; pos < board.size(); ++pos)
	{
		if (board[pos]) // if it has a peg
		{
			for (auto over_to : allowedMoves[pos]) // loop over all allowed moves from that position
			{
				auto over = over_to[0];
				auto to = over_to[1];
				if (board[over] and not board[to]) // If a move is open
				{
					move(board, moves, pos, over, to);
					gameOver = false;
				}
			}
		}
	}
	return gameOver;
}

int main()
{
	initializeAllowedMoves();

	short uniqueStartingPositions[] = {0, 1, 3, 4}; // all other positions are rotations or mirrors of these
	for (auto pos : uniqueStartingPositions)
	{
		board_t board = {true};
		board.fill(true);
		board[pos] = false;
		moves_t moves;
		play(board, moves);
	}

	// Print the histogram
	for (size_t ndx = 0; ndx < remainingCount.size(); ++ndx)
	{
		cout << ndx << '\t' << remainingCount[ndx] << endl;
	}
	return 0;
}
