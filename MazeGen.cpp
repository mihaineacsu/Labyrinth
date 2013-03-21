#include "MazeGen.h"

using namespace std;

Maze::Maze(int l, int c){
	lines = l;
	cols = c;

	buildGround();
	buildWalls();

	generateLabyrinth();
}

void Maze::buildGround(){
	maze = new int*[lines];

	for (int i = 0; i < lines; ++i){
		maze[i] = new int[cols];

		for (int j = 0; j < cols; ++j)
			maze[i][j] = 0;
	}
}

void Maze::buildWalls(){
	for (int i = 0; i < lines; ++i)
		maze[i][0] = maze[i][cols - 1] = MARGIN_WALL;

	for (int i = 0; i < cols; ++i)
		maze[0][i] = maze[lines - 1][i] = MARGIN_WALL;
}

MazeCoord Maze::getRandStart(){
	srand(time(NULL));

	MazeCoord c;
	c.x = rand() % (lines); 
	if (c.x == 0)
		c.x++;

	c.y = rand() % (cols );
	if (c.y == 0)
		c.y++;

	return c;
}

bool Maze::goodToMove(MazeCoord pos){
	if (pos.x <= 0 || pos.y <= 0 ||
			pos.x >= (lines - 1) || pos.y >= (cols - 1))
		return false;

	if (maze[pos.x][pos.y] != WALL)
		return false;

	MazeCoord west = pos.getWest();
	MazeCoord east = pos.getEast();
	MazeCoord north = pos.getNorth();
	MazeCoord south = pos.getSouth();

	int sum = maze[west.x][west.y] +
		maze[east.x][east.y] + 
		maze[north.x][north.y] +
		maze[south.x][south.y];

	sum %= MARGIN_WALL;

	return (sum <= 1);
}

vector<MazeCoord> Maze::generateMoves(MazeCoord crnt){
	vector<MazeCoord> goodMoves;

	if(goodToMove(crnt.getWest()))
		goodMoves.push_back(crnt.getWest());

	if(goodToMove(crnt.getEast()))
		goodMoves.push_back(crnt.getEast());

	if(goodToMove(crnt.getNorth()))
		goodMoves.push_back(crnt.getNorth());
	
	if(goodToMove(crnt.getSouth()))
		goodMoves.push_back(crnt.getSouth());

	return goodMoves;
}

void Maze::generateLabyrinth(){
	stack<MazeCoord> s;
	
	MazeCoord start = getRandStart();
	s.push(start);


	MazeCoord crnt;

	bool firstDone = false;
	while(!s.empty()){
		crnt = s.top();

		vector<MazeCoord> nextMoves = generateMoves(crnt);
		if(!nextMoves.empty()){
			crnt = nextMoves[rand() % nextMoves.size()];

			s.push(crnt);

			maze[crnt.x][crnt.y] = ROAD;

			if (!firstDone){
				startX = crnt.x;
				startY = crnt.y;

				firstDone = true;
			}
		}
		else{
			s.pop(); //it's a deadend

			finishX = crnt.x;
			finishY = crnt.y;
		}
	}
}

int** Maze::getMap(){
	return maze;
}

void Maze::printMaze(){
		for (int i = 0; i < lines; ++i){
			for (int j = 0; j < cols; ++j)
				std::cout << maze[i][j];

			std::cout << std::endl;
		}
}

MazeCoord MazeCoord::getWest(){
		MazeCoord west;

		west.x = x;
		west.y = y - 1;

		return west;
}
	
MazeCoord MazeCoord::getEast(){
		MazeCoord east;

		east.x = x;
		east.y = y + 1;

		return east;
}
	
MazeCoord MazeCoord::getNorth(){
		MazeCoord north;

		north.x = x - 1;
		north.y = y;

		return north;
}

MazeCoord MazeCoord::getSouth(){
		MazeCoord south;

		south.x = x + 1;
		south.y = y;

		return south;
}