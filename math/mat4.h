/******************************************************************************\
*  mat4.h                                                                      *
*                                                                              * 
*  Implements a 4x4 matrix.                                                    *
*                                                                              *
*              Written by Abed Naran                          November 2022    *
*                                                                              *
\******************************************************************************/

#ifndef MAT_4_H
#define MAT_4_H

#include "config.h"
#include "vec3.h"
#include <ostream>

class Mat4 {

	private:
		EMBERS_REAL Inner[4][4];

	public:
		Mat4() = default;
		Mat4(const Mat4 &Other);
		Mat4(Vec3 r1, Vec3 r2, Vec3 r3);
		~Mat4() = default;
		Mat4 operator=(const Mat4 &Other);
		Mat4 operator*(const Mat4 &Other) const;
		Vec3 operator*(const Vec3 &Vec) const;
		Vec3 MULT(const Vec3 &Vec) const;
		EMBERS_REAL *operator[](int i);
		EMBERS_REAL const * const operator[](int i) const;
		static Mat4 Identity();
		static Mat4 Zero();
		static Mat4 RotateX(const EMBERS_REAL &a);
		static Mat4 RotateY(const EMBERS_REAL &a);
		static Mat4 RotateZ(const EMBERS_REAL &a);
		static Mat4 ScaleX(const EMBERS_REAL &s);
		static Mat4 ScaleY(const EMBERS_REAL &s);
		static Mat4 ScaleZ(const EMBERS_REAL &s);
		static Mat4 Scale(const EMBERS_REAL &s);
		static Mat4 Translate(const Vec3 &Vec);
		static Mat4 LookAt(Vec3 From, Vec3 At, Vec3 Up);
		static Mat4 Ortho(EMBERS_REAL Left,
						  EMBERS_REAL Right,
						  EMBERS_REAL Top,
						  EMBERS_REAL Bottom,
						  EMBERS_REAL Near,
						  EMBERS_REAL Far);

		static Mat4 Perspective(EMBERS_REAL FOV,
							    EMBERS_REAL AspectRatio,
							    EMBERS_REAL Near,
							    EMBERS_REAL Far);

        static Mat4 Invert(Mat4 Matrix);
        static Mat4 Transpose(Mat4 Matrix);
        static Mat4 Sqrt(Mat4 Matrix);
};

#endif /* MAT_4_H */
