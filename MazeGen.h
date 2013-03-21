#include <iostream>
#include <stack>
#include <vector>
#include <cstdlib>
#include <ctime>

#define MARGIN_WALL 8
#define ROAD 1
#define WALL 0

class MazeCoord;

class Maze{
public:
	int lines, cols;
	int **maze;
	int startX, startY,
		finishX, finishY;


	Maze(int l, int c);
	
	void buildGround();
	void buildWalls();
	void generateLabyrinth();
	MazeCoord getRandStart();
	std::vector<MazeCoord> generateMoves(MazeCoord );
	bool goodToMove(MazeCoord );

	int **getMap();
	void printMaze();
};

class MazeCoord{
public:
	int x, y;

	MazeCoord getWest();
	MazeCoord getEast();
	MazeCoord getNorth();
	MazeCoord getSouth();
};