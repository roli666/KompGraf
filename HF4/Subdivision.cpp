#include "Subdivision.h"
#include <glfw3.h>

void Subdivision::AddPoint(float x, float y)
{
	auto& basePoints = points.at(0);
	basePoints.push_back(vec2{ x,y });
}

void Subdivision::RemovePoint(float x, float y)
{
	float s = pointSize * pointSize;
	auto& basePoints = points.at(0);
	for (int i = 0; i < basePoints.size(); i++)
	{
		if (dist2(basePoints[i], vec2{ x,y }) < s)
		{
			basePoints.erase(basePoints.begin() + i);
		}
	}
}

void Subdivision::RemovePoints()
{
	grabbedPoint = nullptr;
	auto& basePoints = points.at(0);
	basePoints.clear();
}

void Subdivision::MovePoint(float x, float y)
{
	float s = pointSize * pointSize;
	auto& basePoints = points.at(0);
	for (int i = 0; i < basePoints.size(); i++)
	{
		if (dist2(basePoints[i], vec2{ x,y }) < s)
		{
			basePoints[i].x = x;
			basePoints[i].y = y;
		}
	}
}

void Subdivision::MoveGrabbedPoint(float x, float y)
{
	grabbedPoint->x = x;
	grabbedPoint->y = y;
}

void Subdivision::SetGrabbedPoint(float x, float y)
{
	float s = pointSize * pointSize;
	auto& basePoints = points.at(0);
	for (int i = 0; i < basePoints.size(); i++)
	{
		if (dist2(basePoints[i], vec2{ x,y }) < s)
		{
			grabbedPoint = &basePoints[i];
		}
	}
}

void Subdivision::DrawPoints()
{
	glPointSize(pointSize * 2);
	glBegin(GL_POINTS);
	auto& basePoints = points.at(0);
	for (int i = 0; i < basePoints.size(); i++)
	{
		glVertex2f(basePoints[i].x, basePoints[i].y);
	}
	glEnd();
}

void Subdivision::DrawSpline(float lineThickness)
{
	glLineWidth(lineThickness);
	for (int i = 1; i < 5; i++) {
		points[i].clear();
	}
	auto& basePoints = points.at(0);
	if (basePoints.size() > 3)
	{
		for (int k = 1; k < 5; k++)
		{
			int n = int(points.at(k - 1).size());
			for (int j = 1; j <= n; ++j)
			{
				points[k].push_back(points.at(k - 1).at(j % n));
				points[k].push_back(
					Alpha(k - 1, 0) * points.at(k - 1).at((j - 1) % n) + Alpha(k - 1, 1) * points.at(k - 1).at(j % n)
					+
					Alpha(k - 1, 2) * points.at(k - 1).at((j + 1) % n) + Alpha(k - 1, 3) * points.at(k - 1).at((j + 2) % n));
				points[k].push_back(
					Alpha(k - 1, 3) * points.at(k - 1).at((j - 1) % n) + Alpha(k - 1, 2) * points.at(k - 1).at(j % n)
					+
					Alpha(k - 1, 1) * points.at(k - 1).at((j + 1) % n) + Alpha(k - 1, 0) * points.at(k - 1).at((j + 2) % n));
			}
		}
		if (firstIteration)
		{
			glBegin(GL_LINE_LOOP);
			for (int i = 0; i < points.at(1).size(); i++) {
				glVertex2f(points.at(1).at(i).x, points.at(1).at(i).y);
			}
			glEnd();
		}
		if (secondIteration)
		{
			glBegin(GL_LINE_LOOP);
			for (int i = 0; i < points.at(2).size(); i++) {
				glVertex2f(points.at(2).at(i).x, points.at(2).at(i).y);
			}
			glEnd();
		}
		if (thirdIteration)
		{
			glBegin(GL_LINE_LOOP);
			for (int i = 0; i < points.at(3).size(); i++) {
				glVertex2f(points.at(3).at(i).x, points.at(3).at(i).y);
			}
			glEnd();
		}
		if (fourthIteration)
		{
			glBegin(GL_LINE_LOOP);
			for (int i = 0; i < points.at(4).size(); i++) {
				glVertex2f(points.at(4).at(i).x, points.at(4).at(i).y);
			}
			glEnd();
		}
	}
}

void Subdivision::DrawControlPolygon(float lineThickness)
{
	glLineWidth(lineThickness);
	glBegin(GL_LINE_STRIP);
	auto& basePoints = points.at(0);
	for (int i = 0; i < basePoints.size(); i++)
	{
		glVertex2f(basePoints[i].x, basePoints[i].y);
	}
	if (basePoints.size() > 3)
		glVertex2f(basePoints.front().x, basePoints.front().y);
	glEnd();
}

void Subdivision::DrawNet(int windowWidth, int windowHeight, float spacing)
{
	glColor3f(0.9f, 0.9f, 0.9f);
	glLineWidth(1);
	glBegin(GL_LINES);
	for (size_t x = 0; x < windowWidth; x += spacing)
	{
		glVertex2f(x, 0);
		glVertex2f(x, windowHeight);
	}
	for (size_t y = 0; y < windowHeight; y += spacing)
	{
		glVertex2f(0, y);
		glVertex2f(windowWidth, y);
	}
	glEnd();
}

bool Subdivision::MouseOverPoint(float x, float y)
{
	float s = pointSize * pointSize;
	auto& basePoints = points.at(0);
	for (int i = 0; i < basePoints.size(); i++)
	{
		if (dist2(basePoints[i], vec2{ x,y }) < s)
		{
			return true;
		}
	}
	return false;
}

void Subdivision::SetTension(float value)
{
	if (value < -2)
		tension = -2;
	if (value == -1)
		tension = 0;
	tension = value;
}

double Subdivision::Gamma(int k)
{
	return -1.0 / (3.0 * (1.0 - pow(Beta(k), 2)) * (1.0 + Beta(k)));
}

double Subdivision::Beta(int k)
{
	switch (k)
	{
	case 0:
		return tension;
	case 1:
		return sqrt(2 + tension);
	default:
		return sqrt(2 + Beta(k - 1));
	}
}

double Subdivision::Alpha(int k, int i)
{
	switch (i)
	{
	case 0:
		return (1.0 / 60.0) * ((-90 * Gamma(k + 1)) - 1);
	case 1:
		return (1.0 / 60.0) * ((90 * Gamma(k + 1)) + 43);
	case 2:
		return (1.0 / 60.0) * ((90 * Gamma(k + 1)) + 17);
	case 3:
		return (1.0 / 60.0) * ((-90 * Gamma(k + 1)) + 1);
	default:
		return 0;
	}
}

void Subdivision::DrawCircle(float x, float y)
{
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 360; i++)
	{
		glVertex2f(x + (sinf(i) * 20), y + (cosf(i) * 20));
	}
	glEnd();
}
