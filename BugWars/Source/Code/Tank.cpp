#include "pch.h"
#include "Tank.h"
#include "Globals.h"
#include "Game.h"
#include "Bullet.h"
#include "Bug.h"

IMPLEMENT_RTTI(Tank);

void Tank::OnUpdate(float dt)
{
}

BugBase* Tank::GetBugToShoot() const
{
	float min_distance = FLT_MAX;
	Bug* closestBug = nullptr;
	for (auto obj : g_Game->objects)
		if (auto bug = dynamic_cast<Bug*>(obj))
		{
			if (bug->disabled)
			{
				continue;
			}
			float distance_to_bug = bug->position.Distance2(position);
			if (min_distance > distance_to_bug)
			{
				min_distance = distance_to_bug;
				closestBug = bug;
			}
		}

	return closestBug;
}

Point Tank::CalcShootDirection(Point target_pos, Point target_dir, float target_vel, float bullet_vel) const
{
	target_dir = target_dir.Normalized();
	Point target_dir_vel = target_dir * target_vel;
	Point target_rel_pos = target_pos - position;
	float target_vel2 = target_vel * target_vel;
	float bullet_vel2 = bullet_vel * bullet_vel;
	float a = target_vel2 - bullet_vel2;
	float b = 2.f * (target_dir * target_vel).Dot(target_rel_pos);
	float c = target_rel_pos.Length2();
	
	float det = b * b - 4.f * a * c;
	float sqrt_det = std::sqrt(det);
	float t = 0.f;
	if (det > 0.f)
	{
		float t1 = (-b + sqrt_det) / (2.f * a);
		float t2 = (-b - sqrt_det) / (2.f * a);
		if (t1 > 0.f)
		{
			if (t2 > 0.f)
				t = std::min(t1, t2);
			else
				t = t1;
		}
		else
		{
			t = std::max(t2, 0.f);
		}
	}
	else if (det == 0.f)
	{
		t = std::max(-b / (2.f * a), 0.f);
	}

	return (target_rel_pos + target_dir * target_vel * t).Normalized();
}
