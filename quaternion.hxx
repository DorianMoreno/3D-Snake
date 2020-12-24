#ifndef __QUATERNION__HXX__
#define __QUATERNION__HXX__

#include "vector.h"
#include "quaternion.h"
#include <iostream>
#include <cmath>

//------------------------------------------------------------------------------

#define _2PI      (_PI * 2.0)
#define _PI_360   (_PI / 360.0)
#define _SQRT3    1.73205080757
#define _SQRT3_2  0.86602540378
#define _SQRT3_3  0.57735026919
#define _SQRT3_4  0.43301270189
#define _SQRT3_6  0.28867513459
#define _SQRT2_2  0.70710678118
#define _SQRT6_6  0.40824829046

#define TOLERANCE 0.00000000001

//------------------------------------------------------------------------------

Quaternion::Quaternion(){
	this->x = 0;
	this->y = 0;
	this->z = 0;
	this->w = 0;
}

Quaternion::Quaternion(float x, float y, float z, float w){
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Quaternion::Quaternion(Vector v, const float &angle){
	v.normalize();
	FromAxis(v, angle);
}

Quaternion Quaternion::getConjugate(){
	return Quaternion(-x, -y, -z, w);
}

const Quaternion Quaternion::getConjugate()const{
	return Quaternion(-x, -y, -z, w);
}

Vector Quaternion::rotatePoint(const Vector& point){
	Quaternion newQ(point[0], point[1], point[2], 0);
	newQ.normalize();
	newQ = (*this)*newQ*(this->getConjugate());
	Vector sol;
	float aux;
	newQ.getAxisAngle(sol, aux);
	return sol;
}

Vector Quaternion::toVector(){
	return Vector(x,y,z);
}

void Quaternion::normalize() {
	if(x == 0 && y==0 && z==0){
		w = 1;
		return;
	}
	float magP = w*w + x*x + y*y + z*z;
	if (fabs(magP - 1.0f) > TOLERANCE){
		float mag = sqrt(magP);
		w /= mag;
		x /= mag;
		y /= mag;
		z /= mag;
	}
}

Quaternion Quaternion::operator* (const Quaternion &rq) const {
	return Quaternion(w * rq.x + x * rq.w + y * rq.z - z * rq.y, w * rq.y + y * rq.w + z * rq.x - x * rq.z, w * rq.z + z * rq.w + x * rq.y - y * rq.x, w * rq.w - x * rq.x - y * rq.y - z * rq.z);
}

void Quaternion::FromAxis(const Vector &v, const float &angle) {
	if(angle == 0){
		x = 0;
		y = 0;
		z = 0;
		w = 1;
	}
	else{
		float sinAngle;
		sinAngle = sin(angle*_PI_360);
		x = (v[0] * sinAngle);
		y = (v[1] * sinAngle);
		z = (v[2] * sinAngle);
		w = cos(angle*_PI_360);
	}
}

void Quaternion::getAxisAngle(Vector &axis, float &angle){
	float scale = sqrt(x* x + y * y + z * z);
	axis[0] = x / scale;
	axis[1] = y / scale;
	axis[2] = z / scale;
	angle = acos(w)*2.0f/_PI_180;
	axis.normalize();
}

void Quaternion::print(){
	std::cout<<"Q: x:"<<x<<" y:"<<y<<" z:"<<z<<" w:"<<w<<"\n";
}


#endif // __QUATERNION__HXX__
