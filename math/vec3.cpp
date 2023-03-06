/******************************************************************************\
*  vec3.cpp                                                                    *
*                                                                              * 
*  Implements a vector 3 class.                                                *
*                                                                              * 
*              Written by Abed Naran                          November 2022    *
*                                                                              *
\******************************************************************************/

#include "vec3.h"
#include <cmath>

/******************************************************************************\
* Vec3                                                                         *
*                                                                              *
*  The vector3 constructor.                                                    *
*  The w term is used for homogenious coordinates and is only emplyed in       *
*  perspective division, therefore we don't need to initialize it to anything. *
*                                                                              *
* Parameters                                                                   *                                                   *
*                                                                              *
*  -x: The x term of the vector.                                               *
*  -y: The y term of the vector.                                               *
*  -y: The z term of the vector.                                               *
*                                                                              *
\******************************************************************************/
Vec3::Vec3(EMBERS_REAL x, EMBERS_REAL y, EMBERS_REAL z) : x(x), y(y), z(z), w(1)
{
}

/******************************************************************************\
* Vec3                                                                         *
*                                                                              *
*  The default vector3 constructor.                                            *
*  Creates a 0 vector.                                                         *
*                                                                              *
\******************************************************************************/
Vec3::Vec3() : x(0), y(0), z(0), w(0)
{
}

/******************************************************************************\
* Vec3                                                                         *
*                                                                              *
*  The vector 3 copy constructor.                                              *
*                                                                              *
* Parameters                                                                   *                                                   *
*                                                                              *
*  -Other: The other vector to copy from.                                      *
*                                                                              *
\******************************************************************************/
Vec3::Vec3(const Vec3 &Other) : x(Other.x), y(Other.y), z(Other.z), w(Other.w)
{
}

/******************************************************************************\
* operator=                                                                    *
*                                                                              *
*  Vector assignemnt.                                                          *
*                                                                              *
* Parameters                                                                   *                                                   *
*                                                                              *
*  -Other: The other vector.                                                   *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  Vec3: The resulting vector.                                                 *
*                                                                              *
\******************************************************************************/
Vec3 Vec3::operator=(const Vec3 &Other)
{
	this -> x = Other.x;
	this -> y = Other.y;
	this -> z = Other.z;
	this -> w = Other.w;
	return *this;
}

/******************************************************************************\
* operator+                                                                    *
*                                                                              *
*  Element wise addition between this vector and the other vector.             *
*                                                                              *
* Parameters                                                                   *                                                   *
*                                                                              *
*  -Other: The other vector.                                                   *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  Vec3: The resulting vector.                                                 *
*                                                                              *
\******************************************************************************/
Vec3 Vec3::operator+(const Vec3 &Other) const
{
	return Vec3(x + Other.x, y + Other.y, z + Other.z);
}

/******************************************************************************\
* operator+                                                                    *
*                                                                              *
*  Element wise subtraction between this vector and the other vector.          *
*                                                                              *
* Parameters                                                                   *                                                   *
*                                                                              *
*  -Other: The other vector.                                                   *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  Vec3: The resulting vector.                                                 *
*                                                                              *
\******************************************************************************/
Vec3 Vec3::operator-(const Vec3 &Other) const
{
	return *this + Other * -1;
}

/******************************************************************************\
* operator+                                                                    *
*                                                                              *
*  Element wise vector multiplication between this vector and the other vector.*
*                                                                              *
* Parameters                                                                   *                                                   *
*                                                                              *
*  -Other: The other vector.                                                   *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  Vec3: The resulting vector.                                                 *
*                                                                              *
\******************************************************************************/
Vec3 Vec3::operator*(const Vec3 &Other) const
{
	return Vec3(x * Other.x, y * Other.y, z * Other.z);
}

/******************************************************************************\
* operator*                                                                    *
*                                                                              *
*  Vector scalar multiplication.                                               *
*                                                                              *
* Parameters                                                                   *                                                   *
*                                                                              *
*  -Other: The other vector.                                                   *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  Vec3: The resulting vector.                                                 *
*                                                                              *
\******************************************************************************/
Vec3 Vec3::operator*(const EMBERS_REAL &Scalar) const
{
	return Vec3(x * Scalar, y * Scalar, z * Scalar);
}

/******************************************************************************\
* operator*                                                                    *
*                                                                              *
*  Vector scalar division.                                                     *
*                                                                              *
* Parameters                                                                   *                                                   *
*                                                                              *
*  -Scalar: The scalar.                                                        *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  Vec3: The resulting vector.                                                 *
*                                                                              *
\******************************************************************************/
Vec3 Vec3::operator/(const EMBERS_REAL &Scalar) const
{
	return (*this) * (1 / Scalar);
}

/******************************************************************************\
* Normalize                                                                    *
*                                                                              *
*  Normalizes this vector.                                                     *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  Vec3: The resulting vector.                                                 *
*                                                                              *
\******************************************************************************/
Vec3 Vec3::Normalize() const
{

	EMBERS_REAL 
		Len = Length();
	
	if (Len < EMBERS_EPSILON)
		return Vec3(0.f, 0.f, 0.f);

	return *this / Len;
}

/******************************************************************************\
* Length                                                                       *
*                                                                              *
*  Calculates this vector's length.                                            *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  EMBERS_REAL: The vector's length.                                                  *
*                                                                              *
\******************************************************************************/
EMBERS_REAL Vec3::Length() const
{
	return sqrt(Dot(*this, *this));
}

/******************************************************************************\
* Dot                                                                          *
*                                                                              *
*  Calculates the dot product between two vectors.                             *
*                                                                              *
* Parameters                                                                   *                                                   *
*                                                                              *
*  -v1: The first vector.                                                      *
*  -v2: The sescond vector.                                                    *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  EMBERS_REAL: The resulting dot product.                                            *
*                                                                              *
\******************************************************************************/
EMBERS_REAL Vec3::Dot(const Vec3 &v1, const Vec3 &v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

/******************************************************************************\
* Cross                                                                        *
*                                                                              *
*  Calculates the cross product between two vectors.                           *
*                                                                              *
* Parameters                                                                   *                                                   *
*                                                                              *
*  -v1: The first vector.                                                      *
*  -v2: The sescond vector.                                                    *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  EMBERS_REAL: The resulting cross product.                                          *
*                                                                              *
\******************************************************************************/
Vec3 Vec3::Cross(const Vec3 &v1, const Vec3 &v2)
{
	return Vec3(v1.y * v2.z - v1.z * v2.y,
				v1.z * v2.x - v1.x * v2.z,
				v1.x * v2.y - v1.y * v2.x);

}

/******************************************************************************\
* operator*                                                                    *
*                                                                              *
*  Left vector scalar multiplication.                                          *
*                                                                              *
* Parameters                                                                   *                                                   *
*                                                                              *
*  -Scalar: The scalar.                                                        *
*  -v:      The vector.                                                        *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  -Vec3: The resulting vector.                                                *
*                                                                              *
\******************************************************************************/
Vec3 operator*(const EMBERS_REAL &Scalar, const Vec3 &v)
{
	return v * Scalar;
}

EMBERS_REAL LinePlaneIntersection(Vec3 v1, Vec3 v2, EMBERS_REAL A, EMBERS_REAL B, EMBERS_REAL C, EMBERS_REAL D)
{

    Vec3 Dir = v2 - v1;

    EMBERS_REAL 
        TopPart = A * v1.x + B * v1.y + C * v1.z + D,
        BottomPart = A * Dir.x + B * Dir.y + C * Dir.z;

    if (fabs(BottomPart) < EMBERS_EPSILON)
        return INFINITY;
    
    return -TopPart / BottomPart;
}

EMBERS_REAL PointPlaneCheck(Vec3 v, EMBERS_REAL A, EMBERS_REAL B, EMBERS_REAL C, EMBERS_REAL D) 
{
    return v.x * A + v.y * B + v.z * C + D;
}
