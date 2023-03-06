/******************************************************************************\
*  vec3.h                                                                      *
*                                                                              * 
*  Implements a vector 3 class.                                                *
*                                                                              * 
*              Written by Abed Naran                          November 2022    *
*                                                                              *
\******************************************************************************/

#ifndef VEC_3_H
#define VEC_3_H

#include "config.h"
#include <ostream>

class Vec3 {

	public:

		/* Note the w is only taken into account with matrix mutliplcation.   */
		EMBERS_REAL x, y, z, w; 
		Vec3();
		Vec3(EMBERS_REAL x, EMBERS_REAL y, EMBERS_REAL z);
		Vec3(const Vec3 &Other);
		~Vec3() = default;
		Vec3 operator=(const Vec3 &Other);
		Vec3 operator+(const Vec3 &Other) const;
		Vec3 operator-(const Vec3 &Other) const;
		Vec3 operator*(const Vec3 &Other) const;
		Vec3 operator*(const EMBERS_REAL &Scalar) const;
		Vec3 operator/(const EMBERS_REAL &Scalar) const;
		Vec3 Normalize() const;
		EMBERS_REAL Length() const;
		static EMBERS_REAL Dot(const Vec3 &v1, const Vec3 &v2);
		static Vec3 Cross(const Vec3 &v1, const Vec3 &v2);
};
Vec3 operator*(const EMBERS_REAL &Scalar, const Vec3 &v);

EMBERS_REAL LinePlaneIntersection(Vec3 v1, Vec3 v2, EMBERS_REAL A, EMBERS_REAL B, EMBERS_REAL C, EMBERS_REAL D);
EMBERS_REAL PointPlaneCheck(Vec3 v, EMBERS_REAL A, EMBERS_REAL B, EMBERS_REAL C, EMBERS_REAL D);

#endif /* VEC_3_H */
