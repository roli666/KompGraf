#include "BezierSurface.h"
#include <algorithm>

BezierSurface::BezierSurface()
{
}

void BezierSurface::CalculateTriangles()
{
	float iterator = 1.0f / 15.0f;
	triangles.clear();
	for (float s = 0.0f; s < 1.0f; s += iterator) {
		for (float t = 0.0f; t < 1.0f; t += iterator) {
			Triangle tri;
			tri.points[0] = bezier(s, t);
			tri.points[1] = bezier(s, t + iterator);
			tri.points[2] = bezier(s + iterator, t + iterator);

			triangles.push_back(tri);

			tri.points[0] = bezier(s, t);
			tri.points[1] = bezier(s + iterator, t + iterator);
			tri.points[2] = bezier(s + iterator, t);

			triangles.push_back(tri);
		}
	}
}

bool BezierSurface::SortByDistance(Triangle a, Triangle b)
{
	return dist(a.center, vec3(0.0f, 0.0f, 1)) > dist(b.center, vec3(0.0f, 0.0f, 1));
}

void BezierSurface::SortTriangles()
{
	std::sort(transformedTriangles.begin(), transformedTriangles.end(), SortByDistance);
}

void BezierSurface::Draw()
{
	for (auto tri : transformedTriangles) 
	{
		glColor3f(tri.color.x, tri.color.y, tri.color.z);
		glBegin(GL_POLYGON);
		for (auto& pt : tri.points)
			glVertex2f(pt.x, pt.y);
		glEnd();
	}
	if (showControlNet)
	{
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_POINTS);
		for (auto point : transformedControlPoints)
		{
			glVertex2f(point.x,point.y);
		}
		glEnd();
	}
}

double BezierSurface::bezierBase(int i, double t) {
	if (i == 0) {
		return -1 * pow(t, 3.0) + 3 * pow(t, 2.0) - 3.0 * t + 1.0;
	}

	if (i == 1) {
		return 3.0 * pow(t, 3.0) - 6.0 * pow(t, 2.0) + 3.0 * t;
	}

	if (i == 2.0) {
		return -3.0 * pow(t, 3.0) + 3.0 * pow(t, 2.0);
	}

	if (i == 3.0) {
		return pow(t, 3.0);
	}

	return 0.0;
}

vec3 BezierSurface::bezier(double s, double t) {

	vec3 sum(0.0f, 0.0f, 0.0f);

	for (int j = 0; j <= 3; ++j) {
		for (int i = 0; i <= 3; ++i) {
			sum += controlpoints[i][j] * (float)bezierBase(i, s) * (float)bezierBase(j, t);
		}
	}

	return sum;
}

void BezierSurface::TransformTriangles(mat4 coordTrans)
{
	transformedTriangles.clear();
	for (size_t i = 0; i < triangles.size(); i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			vec4 transformedPoint = ihToH(triangles[i].points[j]);
			transformedPoint = coordTrans * transformedPoint;
			if (transformedPoint.w != 0.0f)
				triangles[i].points[j] = hToIh(transformedPoint);
		}
		triangles[i].setCenterPoint();
		triangles[i].setNormalVector();

		if (dot(normalize(triangles[i].normalVector), normalize(vec3(0.0f, 0.0f, 1) - triangles[i].center)) < 0.0f)
		{
			if (i % 2)
				triangles[i].color = color1;
			else
				triangles[i].color = color2;
		}
		transformedTriangles.push_back(triangles[i]);
	} 
}
void BezierSurface::TransformControlPoints(mat4 coordTrans)
{
	transformedControlPoints.clear();
	for (int i = 0; i <= 3; ++i) 
	{
		for (int j = 0; j <= 3; ++j) 
		{
			vec4 TransformedControlPoint = ihToH(controlpoints[i][j]);
			TransformedControlPoint = coordTrans * TransformedControlPoint;
			transformedControlPoints.push_back(hToIh(TransformedControlPoint));
		}
	}
}
