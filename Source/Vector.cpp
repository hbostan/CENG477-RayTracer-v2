#include "Vector.h"
#include <cmath>

float Vec3f::length()
{
	return sqrt(x*x + y*y + z*z);
}

float Vec3f::sqrLength()
{
	return x*x + y*y + z*z;
}

float Vec3f::normalize()
{
	*this /= this->length();
	return this->length();
}

Vec3f Vec3f::normalized()
{
	Vec3f v(*this);
	v.normalize();
	return v;
}

Vec3f & Vec3f::operator=(const Vec3f & v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}

Vec3f & Vec3f::operator+=(const Vec3f & v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vec3f & Vec3f::operator-=(const Vec3f & v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Vec3f & Vec3f::operator*=(const Vec3f & v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

Vec3f & Vec3f::operator/=(const Vec3f & v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return *this;
}

Vec3f Vec3f::operator-()
{
	return Vec3f(-x, -y, -z);
}

float Vec3f::dot(Vec3f v)
{
	return this->x*v.x + this->y*v.y + this->z*v.z;
}

Vec3f Vec3f::cross(Vec3f v)
{
	return Vec3f((y*v.z - z*v.y), (z*v.x - x*v.z), (x*v.y - y*v.x));
}


float Vec3i::length()
{
	return sqrt(x*x + y*y + z*z);
}

int Vec3i::sqrLength()
{
	return x*x + y*y + z*z;
}

Vec3i & Vec3i::operator=(const Vec3i & v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}

Vec3i & Vec3i::operator+=(const Vec3i & v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vec3i & Vec3i::operator-=(const Vec3i & v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Vec3i & Vec3i::operator*=(const Vec3i & v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

Vec3i & Vec3i::operator/=(const Vec3i & v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return *this;
}

Vec3i Vec3i::operator-()
{
	return Vec3i(-x, -y, -z);
}

int Vec3i::dot(Vec3i v)
{
	return this->x*v.x + this->y*v.y + this->z*v.z;
}

Vec3i Vec3i::cross(Vec3i v)
{
	return Vec3i((y*v.z - z*v.y), (z*v.x - x*v.z), (x*v.y - y*v.x));
}