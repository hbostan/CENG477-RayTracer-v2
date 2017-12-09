#ifndef __HW1__VECTOR__
#define __HW1__VECTOR__


class Vec3f {
public:
	float x, y, z;

	Vec3f() :x(0.0f), y(0.0f), z(0.0f) {}
	Vec3f(const Vec3f& other) :x(other.x), y(other.y), z(other.z) {}
	Vec3f(float x, float y, float z) : x(x), y(y), z(z) {}
	Vec3f(float d) :x(d), y(d), z(d) {}

	float length();
	float sqrLength();

	float normalize();
	Vec3f normalized();


	Vec3f& operator =(const Vec3f& v);
	Vec3f& operator +=(const Vec3f& v);
	Vec3f& operator -=(const Vec3f& v);
	Vec3f& operator *=(const Vec3f& v);
	Vec3f& operator /=(const Vec3f& v);
	Vec3f operator -();

	float dot(Vec3f v);
	Vec3f cross(Vec3f v);
};

inline Vec3f operator+(const Vec3f& a, const Vec3f& b)
{
  return Vec3f(a.x+b.x, a.y+b.y, a.z+b.z);
}

inline Vec3f operator-(const Vec3f& a, const Vec3f& b)
{
  return Vec3f(a.x-b.x, a.y-b.y, a.z-b.z);
}

inline Vec3f operator*(const Vec3f& a, const float& b)
{
  return Vec3f(a.x*b, a.y*b, a.z*b);
}

inline Vec3f operator*(const Vec3f& a, const Vec3f& b)
{
  return Vec3f(a.x*b.x, a.y*b.y, a.z*b.z);
}

inline Vec3f operator/(const Vec3f& a, const float& b)
{
  return Vec3f(a.x/b, a.y/b, a.z/b);
}

typedef Vec3f Point;

class Vec3i {
public:
	int x, y, z;

	Vec3i():x(0),y(0),z(0) {}
	Vec3i(const Vec3i& other) :x(other.x), y(other.y), z(other.z) {}
	Vec3i(int x, int y, int z) : x(x), y(y), z(z) {}
	Vec3i(int d) :x(d), y(d), z(d) {}

	float length();
	int sqrLength();

	/*float normalize();
	Vec3i normalized();*/


	Vec3i& operator =(const Vec3i& v);
	Vec3i& operator +=(const Vec3i& v);
	Vec3i& operator -=(const Vec3i& v);
	Vec3i& operator *=(const Vec3i& v);
	Vec3i& operator /=(const Vec3i& v);
	Vec3i operator -();

	int dot(Vec3i v);
	Vec3i cross(Vec3i v);
};

class Vec4f {
public:
	float x, y, z, w;
};

#endif