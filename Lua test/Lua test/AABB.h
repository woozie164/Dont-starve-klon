#pragma once

struct AABB
{
	float min[2];
	float max[2];

	bool Intersect(const AABB & box)
	{
		if (max[0] < box.min[0] || min[0] > box.max[0]) return false;
		if (max[1] < box.min[1] || min[1] > box.max[1]) return false;
		return true;
	}

	bool InsideBox(float x, float y)
	{
		if (min[0] > x || max[0] < x) return false;
		if (min[1] > y || max[1] < y) return false;
		return true;
	}
	
	// The center of the box will be at this position
	void SetPosition(float x, float y)
	{	
		float halfWidth = (max[0] - min[0]) / 2.0f;
		float halfDepth = (max[1] - min[1]) / 2.0f;

		min[0] = x - halfWidth;
		min[1] = y - halfDepth;

		max[0] = x + halfWidth;
		max[1] = y + halfDepth;		
	}
	

	void MoveBox(float xdir, float ydir)
	{
		min[0] += xdir;
		min[1] += ydir;
		max[0] += xdir;
		min[1] += ydir;
	}
};