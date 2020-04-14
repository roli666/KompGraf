#include "BSpline.h"
#include <glfw3.h>

void BSpline::AddPoint(float x, float y)
{
	points.push_back(vec2{ x,y });
	if (points.size() > 2)
	{
		ghostStart = (2 * points[0]) - points[1];
		ghostEnd = (2 * points.back()) - points.end()[-2];
	}
	else
	{
		ghostStart = 0;
		ghostEnd = 0;
	}
}

void BSpline::RemovePoint(float x, float y)
{
	float s = pointSize * pointSize;
	for (int i = 0; i < points.size(); i++)
	{
		if (dist2(points[i], vec2{ x,y }) < s)
		{
			points.erase(points.begin() + i);
			if (points.size() > 2)
			{
				ghostStart = (2 * points[0]) - points[1];
				ghostEnd = (2 * points.back()) - points.end()[-2];
			}
			else
			{
				ghostStart = 0;
				ghostEnd = 0;
			}
		}
	}
}

void BSpline::MovePoint(float x, float y)
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

void BSpline::MoveGrabbedPoint(float x, float y)
{
	grabbedPoint->x = x;
	grabbedPoint->y = y;
	if (points.size() > 2)
	{
		ghostStart = (2 * points[0]) - points[1];
		ghostEnd = (2 * points.back()) - points.end()[-2];
	}
	else
	{
		ghostStart = 0;
		ghostEnd = 0;
	}
}

void BSpline::SetGrabbedPoint(float x, float y)
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

void BSpline::DrawPoints()
{
	glPointSize(pointSize * 2);
	if (ghostStart != 0)
		DrawCircle(ghostStart.x, ghostStart.y);
	glBegin(GL_POINTS);
	for (int i = 0; i < points.size(); i++)
	{
		glVertex2f(points[i].x, points[i].y);
	}
	glEnd();
	if (ghostEnd != 0)
		DrawCircle(ghostEnd.x, ghostEnd.y);
}

void BSpline::DrawSpline(float lineThickness)
{
	glLineWidth(lineThickness);
	if (points.size() < 3)
	{
		glBegin(GL_LINES);
		for (int i = 0; i < points.size(); i++) 
		{
			glVertex2f(points[i].x, points[i].y);
		}
		glEnd();
	}
	else
	{
		points.push_front(ghostStart);
		points.push_back(ghostEnd);
		glBegin(GL_LINE_LOOP);
		for (int i = 0; i<int((points.size() + 2) / 4); i++)
		{
			G =
			{
				points[i + 0],
				points[i + 1],
				points[i + 2],
				points[i + 3]
			};
			for (float t = t1; t < t3; t += 0.01f)
			{
				vec2 gmt = G * M * vec4{ powf(t,3),powf(t,2) ,t,1 };
				glVertex2f(gmt.x, gmt.y);
			}
		}
		glEnd();
		points.pop_front();
		points.pop_back();
	}
}

bool BSpline::MouseOverPoint(float x, float y)
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

void BSpline::DrawCircle(float x, float y)
{
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 360; i++)
	{
		glVertex2f(x + (sinf(i) * 20), y + (cosf(i) * 20));
	}
	glEnd();
}
