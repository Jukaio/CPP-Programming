// cyanide_math.h

#ifndef INCLUDED_CYANIDE_MATH_H
#define INCLUDED_CYANIDE_MATH_H

// Vector2
struct vector2i
{
    int x; int y;
};

struct vector2
{
    vector2();
    vector2(const vector2& rhs);
    vector2(float p_x, float p_y);
    vector2(int p_x, int p_y);

    static const vector2 zero;
    static const vector2 one;

    float x; 
    float y;

    static float distance(const vector2& lhs, const vector2& rhs);
};

vector2 operator+(const vector2& lhs, const vector2& rhs);
vector2 operator-(const vector2& lhs, const vector2& rhs);
vector2 operator*(const vector2& lhs, const vector2& rhs);
vector2 operator/(const vector2& lhs, const vector2& rhs);

vector2& operator+=(vector2& lhs, const vector2& rhs);
vector2& operator-=(vector2& lhs, const vector2& rhs);
vector2& operator*=(vector2& lhs, const vector2& rhs);
vector2& operator/=(vector2& lhs, const vector2& rhs);

vector2 operator*(const vector2& lhs, const float& rhs);
vector2& operator*=(vector2& lhs, const float& rhs);
vector2 operator/(const vector2& lhs, const float& rhs);
vector2& operator/=(vector2& lhs, const float& rhs);


// !Vector2

// Vector3
struct vector3
{
    float x; float y; float z;
};

vector3 operator+(const vector3& lhs, const vector3& rhs);
vector3 operator-(const vector3& lhs, const vector3& rhs);
vector3 operator*(const vector3& lhs, const vector3& rhs);
vector3 operator/(const vector3& lhs, const vector3& rhs);

vector3& operator+=(vector3& lhs, const vector3& rhs);
vector3& operator-=(vector3& lhs, const vector3& rhs);
vector3& operator*=(vector3& lhs, const vector3& rhs);
vector3& operator/=(vector3& lhs, const vector3& rhs);
// !Vector3

// Vector4
struct rectangle
{
    float x; float y; float w; float h;
};
// !Vector4
#endif // !INCLUDED_CYANIDE_MATH_H




