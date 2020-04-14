#pragma once
#pragma warning( disable : 4244 )
#include "bevgrafmath2017.h"
#include <deque>

class BSpline {

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
	BSpline(int pointSize)
	{
		points = std::deque<vec2>();
		this->pointSize = pointSize;
	}
private:
	void DrawCircle(float x, float y);
	int pointSize;
	mat24 G;
	std::deque<vec2> points;
	vec2 ghostStart;
	vec2 ghostEnd;
	float t1 = -1;
	float t2 = 0;
	float t3 = 1;
	mat4 T = mat4
	(
		vec4(pow(t1, 3), pow(t1, 2), t1, 1),
		vec4(pow(t2, 3), pow(t2, 2), t2, 1),
		vec4(pow(t3, 3), pow(t3, 2), t3, 1),
		vec4(3 * pow(t3, 2), 2 * t3, 1, 0),
		true
	);
	mat4 M = inverse(T) / 6;
	//vec4 tder = { 3.0f * pow(t1, 2), 2.0f * t1, 1, 0 };

};