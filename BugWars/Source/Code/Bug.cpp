#include "pch.h"
#include "Bug.h"
#include "Game.h"

IMPLEMENT_RTTI(Bug);

void Bug::OnUpdate(float dt)
{
}

BugBase* Bug::FindBugToEat() const
{
	Bug* target = nullptr;
	float min_dist = std::numeric_limits<float>::max();
	
	float radius = 500;
	float radius2 = radius * radius;
	int startX = g_Game->grid.getIndex(position.x - radius);
	int startY = g_Game->grid.getIndex(position.y - radius);
	int endX = g_Game->grid.getIndex(position.x + radius);
	int endY = g_Game->grid.getIndex(position.y + radius);

	for (int x = startX; x <= endX; ++x)
	{
		for (int y = startY; y <= endY; ++y)
		{
			Cell cell = g_Game->grid.getCell(x, y);
			for (const auto& obj : cell.objects)
			{
				if (auto bug = static_cast<Bug*>(obj))
				{
					if (bug == this)
						continue;
					if (bug->disabled)
						continue;
					if (bug->id > id)
						continue;
					float dist = position.Distance2(bug->position);
					if (dist < min_dist && dist < radius2)
					{
						min_dist = dist;
						target = bug;
					}
				}
			}
		}
	}

	if (target)
		return target;

	for (auto object : g_Game->objects)
	{
		if (auto bug = dynamic_cast<Bug*>(object))
		{
			if (bug == this)
				continue;
			if (bug->disabled)
				continue;
			if (bug->id > id)
				continue; // Can't eat that
			float dist = position.Distance2(bug->position);
			if (dist < min_dist)
			{
				min_dist = dist;
				target = bug;
			}
		}
	}

	return target;
}

void Bug::OnEat(BugBase& first, BugBase& second)
{
	if (first.id > second.id)
	{
		second.disabled = true;
		second.visible = false;
	}
	else
	{
		first.disabled = true;
		first.visible = false;
	}
}
