#include "pch.h"
#include "Game.h"
#include "GameBase/Log.h"
#include "Tank.h"
#include "Bug.h"
#include "Bullet.h"

Game* g_Game;

Game::Game()
	: GameBase({ [] {return new Tank; },
				 [] {return new Bug; },
				 [] {return new Bullet; } }),
	  grid(Grid(MAP_SIZE / CELL_SIZE))
{
	g_Game = this;
}

Game::~Game()
{
	for (auto gameObj : objects)
	{
		delete gameObj;
	}
	objects.clear();
}

void Game::OnUpdate(float dt)
{
	PIXScopedEvent(PIX_COLOR_INDEX(5), __FUNCTION__);
	grid.updateCells();
	for (auto it = objects.begin(); it != objects.end();)
	{
		if (!(*it)->disabled)
		{
			(*it)->Update(dt);
			++it;
		}
		else
		{
			delete* it;
			*it = nullptr;
			it = objects.erase(it);
		}
	}
}

void Game::OnRender() const
{
	for (auto obj : objects)
		if (obj->visible)
			DoRender(obj);
}

void Game::AddObject(GameObject* object)
{
	object->disabled = false;
	object->visible = true;
	if (object->GetRTTI() == Bug::s_RTTI)
	{
		grid.getCell(grid.getIndex(object->position.x), grid.getIndex(object->position.y)).objects.push_back(object);
	}
	objects.push_back(object);
}

void Game::OnBugsSpawned()
{
}
