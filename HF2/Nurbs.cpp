#include "Nurbs.h"
#include <glfw3.h>

void Nurbs::AddPoint(float x, float y)
{
	points.push_back(vec2{ x,y });
}

void Nurbs::RemovePoint(float x, float y)
{
	float s = pointSize * pointSize;
	for (int i = 0; i < points.size(); i++)
	{
		if (dist2(points[i], vec2{ x,y }) < s)
		{
			points.erase(points.begin() + i);
		}
	}
}

void Nurbs::MovePoint(float x, float y)
{
	float s = pointSize * pointSize;
	for (int i = 0; i < points.size(); i++)
	{
		if (dist2(points[i], vec2{ x,y }) < s)
		{
			points[i].x = x;
			points[i].y = y;
		}
	}
}

void Nurbs::MoveGrabbedPoint(float x, float y)
{
	grabbedPoint->x = x;
	grabbedPoint->y = y;
}

void Nurbs::SetGrabbedPoint(float x, float y)
{
	float s = pointSize * pointSize;
	for (int i = 0; i < points.size(); i++)
	{
		if (dist2(points[i], vec2{ x,y }) < s)
		{
			grabbedPoint = &points[i];
		}
	}
}

void Nurbs::DrawPoints()
{
	glPointSize(pointSize * 2);
	glBegin(GL_POINTS);
	for (int i = 0; i < points.size(); i++)
	{
		glVertex2f(points[i].x, points[i].y);
	}
	glEnd();
}

void Nurbs::DrawSpline(float lineThickness)
{

}

bool Nurbs::MouseOverPoint(float x, float y)
{
	float s = pointSize * pointSize;
	for (int i = 0; i < points.size(); i++)
	{
		if (dist2(points[i], vec2{ x,y }) < s)
		{
			return true;
		}
	}
	return false;
}

void Nurbs::DrawCircle(float x, float y)
{
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 360; i++)
	{
		glVertex2f(x + (sinf(i) * 20), y + (cosf(i) * 20));
	}
	glEnd();
}
