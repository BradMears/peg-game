This project contains two implementations of the Peg/Triangle game  - one in python and one in C++.
Both implementations are written in run-of-the-mill idiomatic python/C++ with no particular emphasis on optimization for speed. I tried to avoid any obviously wasteful steps but didn't try to profile it and speed it up.

Explanation of The Peg/Triangle Game aka Golf Tee Game aka Cracker Barrel Game
------------------------------------------------------------------------------
You have a triangle with a grid of 15 regularly spaced holes.

                               1
                            2      3
                        4      5       6
                    7       8      9       10
                11     12      13      14      15

Place 14 golf tees in the holes, leaving a single hole empty. The goal is to remove tees from the board by jumping over them with another tee. Tees can only move by skipping over an adjacent, occupied tee and landing in the next hole, which must be empty. The tee being moved, the adjacent tee, and the empty hole must be in a straight line. Play continues until no more moves are possible. The goal is to remove all but one tee from the board.

Example of a Legal Move
In the picture above, imagine there is a tee in position 2, another one in position 5, and position 9 is empty. You can move the tee from 2 over 5 and place it into 9. Now you can remove the tee from 5 and take it off the board.

Example of an Illegal Move
Imagine there is a tee in position 2, another one in position 5, and position 8 is empty. You CANNOT jump from 2 "over" 5 and into 8 because the three holes do not form a straight line.

Goal of This Program
--------------------
Both the python and C++ versions of this program do the same thing. They play all possible games using all possible unique starting board configurations, and record the results for each game. With the resulting output, you can see exactly how many games result in a single tee remaining, two tees, etc.

Note on Performance
-------------------
On my circa 2020 Dell laptop, both the python and C++ versions take the same amount of time to run - between 37 and 40 seconds. You would expect the C++ version to be faster and it is - if you comment out the code that outputs a record of every game played. In that case, the python code executes in about 20 seconds and the C++ version in about 1.2 seconds. 

The C++ output is done using ostream. An interesting test would be to replace that with printf() to see if the two I/O mechanisms are significantly different.

It is quite likely that the python implementation could be sped up using numpy. That is another interesting exercise for another day.
