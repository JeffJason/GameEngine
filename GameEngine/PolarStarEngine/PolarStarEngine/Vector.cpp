#include "Vector.h"

Vector3::Vector3() {
	this->x = this->y = this->z = 0.0f;
}

Vector3::Vector3(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}