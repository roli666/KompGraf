#pragma once
#pragma warning( disable : 4244 )
#include "bevgrafmath2017.h"
#include <deque>

class Subdivision {

public:
	void AddPoint(float x, float y);
	void RemovePoint(float x, float y);
	void RemovePoints();
	void MovePoint(float x, float y);
	void MoveGrabbedPoint(float x, float y);
	void SetGrabbedPoint(float x, float y);
	void DrawPoints();
	void DrawSpline(float lineThickness);
	void DrawControlPolygon(float lineThickness);
	void DrawNet(int windowWidth,int windowHeight,float spacing);
	bool MouseOverPoint(float x, float y);
	void SetTension(float value);
	vec2 * grabbedPoint = nullptr;
	Subdivision(int pointSize)
	{
		points = std::deque<std::deque<vec2>>();
		points.resize(5);
		this->pointSize = pointSize;
	}
private:
	double Gamma(int k);
	double Beta(int k);
	double Alpha(int k, int i);
	float tension = 0;
	void DrawCircle(float x, float y);
	int pointSize;
	std::deque<std::deque<vec2>> points;
	bool firstIteration, secondIteration, thirdIteration, fourthIteration = true;
};