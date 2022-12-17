#include "pch.h"
#include "Grid.h"

Grid::Grid(int gridSize) : maxSize(gridSize - 1)
{
	cells = std::vector<Cell>(gridSize * gridSize);
	for (int y = 0; y < gridSize; ++y)
	{
		for (int x = 0; x < gridSize; ++x)
		{
			cells[x * maxSize + y].x = x;
			cells[x * maxSize + y].y = y;
		}
	}
}

int Grid::getIndex(float pos)
{
	return std::clamp((int)pos / CELL_SIZE, 0, maxSize);
}

Cell& Grid::getCell(int row, int col)
{
	return cells[row * maxSize + col];
}

Cell& Grid::getCell(Point pos)
{
	int posX = getIndex(pos.x);
	int posY = getIndex(pos.y);
	return getCell(posX, posY);
}

void Grid::updateCells()
{
	for (auto& cell : cells)
	{
		for (auto it = cell.objects.begin(); it != cell.objects.end();)
		{

			if ((*it)->disabled)
			{
				it = cell.objects.erase(it);
				continue;
			}
			auto& newCell = getCell((*it)->position);
			if (newCell != cell)
			{
 				newCell.objects.emplace_back(*it);
				it = cell.objects.erase(it);
				continue;
			}
			++it;
		}
	}
}




bool Cell::operator!=(const Cell& cell)
{
	return !(this->x == cell.x && this->y == cell.y);
}
