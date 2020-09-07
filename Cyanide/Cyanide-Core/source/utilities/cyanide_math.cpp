// cyanide_math.cpp

#include "utilities/cyanide_math.h"
#include <math.h>

const vector2 vector2::zero = { 0, 0 };
const vector2 vector2::one = { 1, 1 };

vector2 operator+(const vector2& lhs, const vector2& rhs) { return { lhs.x + rhs.x, lhs.y + rhs.y }; }
vector2 operator-(const vector2& lhs, const vector2& rhs) { return { lhs.x - rhs.x, lhs.y - rhs.y }; }
vector2 operator*(const vector2& lhs, const vector2& rhs) { return { lhs.x * rhs.x, lhs.y * rhs.y }; }
vector2 operator/(const vector2& lhs, const vector2& rhs) { return { lhs.x / rhs.x, lhs.y / rhs.y }; }

vector2& operator+=(vector2& lhs, const vector2& rhs) { lhs.x += rhs.x; lhs.y += rhs.y; return lhs; }
vector2& operator-=(vector2& lhs, const vector2& rhs) { lhs.x -= rhs.x; lhs.y -= rhs.y; return lhs; }
vector2& operator*=(vector2& lhs, const vector2& rhs) { lhs.x *= rhs.x; lhs.y *= rhs.y; return lhs; }
vector2& operator/=(vector2& lhs, const vector2& rhs) { lhs.x /= rhs.x; lhs.y /= rhs.y; return lhs; }

vector2 operator*(const vector2& lhs, const float& rhs)
{
	return { lhs.x * rhs, lhs.y * rhs };
}

vector2& operator*=(vector2& lhs, const float& rhs)
{
	lhs.x *= rhs;
	lhs.y *= rhs;
	return lhs;
}

vector2 operator/(const vector2& lhs, const float& rhs)
{
	return { lhs.x / rhs, lhs.y / rhs };
}

vector2& operator/=(vector2& lhs, const float& rhs)
{
	lhs.x /= rhs;
	lhs.y /= rhs;
	return lhs;
}

vector3 operator+(const vector3& lhs, const vector3& rhs) { return vector3{ lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z }; }
vector3 operator-(const vector3& lhs, const vector3& rhs) { return vector3{ lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z }; }
vector3 operator*(const vector3& lhs, const vector3& rhs) { return vector3{ lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z }; }
vector3 operator/(const vector3& lhs, const vector3& rhs) { return vector3{ lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z }; }

vector3& operator+=(vector3& lhs, const vector3& rhs) { lhs.x += rhs.x; lhs.y += rhs.y; lhs.z += rhs.z; return lhs; }
vector3& operator-=(vector3& lhs, const vector3& rhs) { lhs.x -= rhs.x; lhs.y -= rhs.y; lhs.z -= rhs.z; return lhs; }
vector3& operator*=(vector3& lhs, const vector3& rhs) { lhs.x *= rhs.x; lhs.y *= rhs.y; lhs.z *= rhs.z; return lhs; }
vector3& operator/=(vector3& lhs, const vector3& rhs) { lhs.x /= rhs.x; lhs.y /= rhs.y; lhs.z /= rhs.z; return lhs; }


vector2::vector2()
	: x(0)
	, y(0)
{

}

vector2::vector2(const vector2& rhs)
{
	x = rhs.x;
	y = rhs.y;
}

vector2::vector2(float p_x, float p_y)
	: x(p_x)
	, y(p_y)
{

}

vector2::vector2(int p_x, int p_y)
	: x((float)p_x)
	, y((float)p_y)
{
}


float vector2::distance(const vector2& lhs, const vector2& rhs)
{
	return sqrtf((lhs.x - rhs.x) * 
				 (lhs.x - rhs.x) + 
				 (lhs.y - rhs.y) * 
				 (lhs.y - rhs.y));
}
