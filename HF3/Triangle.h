#pragma once
#include <bevgrafmath2017.h>
struct Triangle {
	vec3 points[3];
	vec3 normalVector;
	vec3 center;
	vec3 color;

	void setNormalVector() {
		this->normalVector = cross(this->points[0] - this->points[1], this->points[0] - this->points[2]);
	}

	void setCenterPoint() {
		this->center = (this->points[0] + this->points[1] +
			this->points[2]) / 3.0f;
	}
};