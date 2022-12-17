#pragma once

#include <GameBase/GameObject.h>

const static int MAP_SIZE = 6400;
const static int CELL_SIZE = 320;

struct Cell
{
	int x, y;
	std::vector<GameObject*> objects;

	bool operator!=(const Cell& cell);
};

class Grid
{
public:
	const int maxSize;

	Grid(int gridSize);

	int getIndex(float pos);

	Cell& getCell(int row, int col);
	Cell& getCell(Point pos);

	void updateCells();
private:
	std::vector<Cell> cells;
};

