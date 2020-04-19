#pragma once
#include <bevgrafmath2017.h>
#include <vector>
#include "Triangle.h"
#include <glfw3.h>

class BezierSurface {
public:
	BezierSurface();
	void CalculateTriangles();
	void TransformTriangles(mat4 coordTrans);
	void TransformControlPoints(mat4 coordTrans);
	void SortTriangles();
	void Draw();
	double bezierBase(int i, double t);
	vec3 bezier(double s, double t);
	bool showControlNet = false;
private:
	std::vector<Triangle> triangles;
	std::vector<Triangle> transformedTriangles;
	std::vector<vec3> transformedControlPoints;
	const vec3 color1 = vec3(0.67f, 0.29f, 0.13f);
	const vec3 color2 = vec3(0.50f, 0.28f, 0.14f);
	static bool SortByDistance(Triangle a, Triangle b);
	vec3 controlpoints[4][4] = {
	{ vec3(-1.5f, -1.5f, -2.0f), vec3(-0.5f, -1.5f, -0.5f), vec3(0.5f, -1.5f, -0.5f), vec3(1.5f, -1.5f, -2.0f) },
	{ vec3(-1.5f, -0.5f, -0.5f), vec3(-0.5f, -0.5f, -0.5f), vec3(0.5f, -0.5f, -0.5f), vec3(1.5f, -0.5f, -0.5f) },
	{ vec3(-1.5f, 0.5f, -0.5f),  vec3(-0.5f, 0.5f, -0.5f),  vec3(0.5f, 0.5f, -0.5f),  vec3(1.5f, 0.5f, -0.5f) },
	{ vec3(-1.5f, 1.5f, -2.0f),  vec3(-0.5f, 1.5f, -0.5f),  vec3(0.5f, 1.5f, -0.5f),  vec3(1.5f, 1.5f, -2.0f) }
	};

};