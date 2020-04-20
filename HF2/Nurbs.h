#pragma once
#pragma warning( disable : 4244 )
#include "bevgrafmath2017.h"
#include <deque>

class Nurbs {

public:
	void AddPoint(float x, float y);
	void RemovePoint(float x, float y);
	void MovePoint(float x, float y);
	void MoveGrabbedPoint(float x, float y);
	void SetGrabbedPoint(float x, float y);
	void DrawPoints();
	void DrawSpline(float lineThickness);
	bool MouseOverPoint(float x, float y);
	vec2 * grabbedPoint = nullptr;
	Nurbs(int pointSize)
	{
		points = std::deque<vec2>();
		this->pointSize = pointSize;
	}
private:
	void DrawCircle(float x, float y);
	int pointSize;
	std::deque<vec2> points;
};