# Apollo-and-Diana-Maze-Solver

## Description
This project was made to solve "Apollo and Diana" style mazes, which is from “MAD MAZES: Intriguing Mind Twisters for Puzzle Buffs, Game Nuts and Other Smart People”, by Robert Abbott. The full problem description can be found in the "Problem Description.pdf" file, but in short an "Apollo and Diana" style maze consists of two different colored arrows in each position of a grid and one bullseye. The idea is that you start at a specific arrow and have to work your way to the bullseye where the only arrow you can go to next is one of a different color than the one you're currently on and is in the direction that the arrow you're currently on is pointing. It's easy to get stuck in loops when manually completing this kind of maze, but this program is able to solve these mazes without issue. The program can solve any maze in the "Apollo and Diana" format, not just the one found in the MAD MAZES book (seen in "Problem Description.pdf"), even if the bullseye is not placed in the bottom right.

This project was implemented in C++, and the Boost Graph Library (BGL) was used for various graph functions. To solve these mazes, an adjacency list graph is used to represent the given maze. Each vertex in the graph represents one of the arrows in the maze (or the bullseye) by storing the color and direction of the arrow and the row and column it is at in the maze grid. Each edge of the maze is all possible moves from one arrow to another based on the color and direction. The edges store the distance between the connecting arrows (distance as in how many spaces away the two arrows are in the grid). BGL was used in this project to create the graph and store the vertices and edges of the maze as well as to iterate through vertices and edges in the graph where needed. However, I developed my own depth first search (DFS) algorithm to find a path through the maze that reaches the bullseye. My version of DFS works mostly like standard DFS, however this DFS stops traversing the graph once the bullseye is found and it saves the path to the bullseye as it goes back up the recursive chain.

## Input and Output Files
Three sample input files are provided: "apollodiana.txt", "tiny.txt", and "tiny-invalid.txt" along with their respective correct output files: "apollodiana-soln.txt", "tiny-soln.txt", and "tiny-invalid-soln.txt". "apollodiana.txt" is the original Apollo and Diana maze from the book, "tiny.txt" is a scaled down version of the Apollo and Diana maze of the same format, and "tiny-invalid.txt" is similar to "tiny.txt" but the bullseye isn't in the bottom right corner. This program works even on the invalid maze because the invalid maze was originally given to us for the assignment before later being changed to what "tiny.txt" is currently. So, the program was not intended to be able to solve mazes where the bullseye isn't in the bottom right, but it works anyway since I created the project before the change to the file was made. I mention this because you can make your own input files for the program to solve as long as they are in the correct format, which technically should have the bullseye in the bottom right corner but it will work if you make a maze where that isn't the case. Formatting details are below.
### Input File Format
Apollo and Diana mazes use the top left corner as the start position, so keep this in mind when viewing or creating input files. The first line of the input file includes two numbers which represent the number rows _r_ and columns _c_ of the maze, respectively. The following _r_ lines contain _c_ entries each separated by spaces. Each of these entries represents a position in the maze, where each entry contains the color of the arrow (usually R or B for red/blue), followed by a hyphen (-), then followed by the direction that the arrow points (N, E, S, W, NE, SE, SW, or NW, which represent each of the cardinal/intercardinal directions). If the entry is the bullseye, it just contains the letter O.
### Output File Format
The output file created by the program gives the path from the start position to the bullseye all in one line. The path includes all of the steps taken in the path where each step is separated by a space and includes the number of spaces moved and in what direction. You can compare the output file created by the program against the provided solution files to check for correctness or manually check each of the moves if you made your own input file.

## Instructions
These intructions are developed for Linux, but you should easily be able to do the equivalent on Windows and Mac.
1. Download the project3.cpp, makefile, and whatever input file you decide to use (or make your own) to the same folder on your machine
   - Make sure that you rename the input file you're using to "input.txt", otherwise the program will not read the file
2. Run the command _make_
3. Run _./project3_
4. Now you can find the correct path through the maze by checking the "output.txt" file!
