#ifndef __QUATERNION__H__
#define __QUATERNION__H__

#include "vector.h"

class Quaternion{
public:
    Quaternion();
	Quaternion(float x, float y, float z, float w);
	Quaternion(Vector v, const float &angle);

	Quaternion getConjugate();
	const Quaternion getConjugate() const;
	Quaternion operator* (const Quaternion &rq) const;

    Vector rotatePoint(const Vector& point);
	Vector toVector();
	void normalize();
	void FromAxis(const Vector &v, const float &angle);
	void getAxisAngle(Vector &axis, float &angle);
	void print();
protected:
	float w;
	float x;
	float y;
	float z;
};

#include "quaternion.hxx"

#endif // __QUATERNION__H__
