/******************************************************************************\
*  mat4.cpp                                                                    *
*                                                                              * 
*  Implements a 4x4 matrix.                                                    *
*                                                                              *
*              Written by Abed Naran                          November 2022    *
*                                                                              *
\******************************************************************************/

#include "mat4.h"
#include "config.h"
#include "vec3.h"
#include <cmath>
#include <cstring>

/******************************************************************************\
* Mat4                                                                         *
*                                                                              *
*  The Mat4 copy constructor.                                                  *
*                                                                              *
* Parameters                                                                   *                                                   *
*                                                                              *
*  -Other: The matrix to copy from.                                            *
*                                                                              *
\******************************************************************************/
Mat4::Mat4(const Mat4 &Other) 
{
	memcpy(Inner, Other.Inner, sizeof(Inner[0][0]) * 16);
}

/******************************************************************************\
* operator[]                                                                   *
*                                                                              *
*  The Mat4 row indexing operator.                                             *
*                                                                              *
* Parameters                                                                   *                                                   *
*                                                                              *
*  -i: The index.                                                              *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  -Real*: The row at index.                                                   *
*                                                                              *
\******************************************************************************/
EMBERS_REAL *Mat4::operator[](int i)
{
	return Inner[i];
}

/******************************************************************************\
* operator[]                                                                   *
*                                                                              *
*  The Mat4 CONST row indexing operator.                                       *
*  This is used for R value indexing.                                          *
*                                                                              *
* Parameters                                                                   *                                                   *
*                                                                              *
*  -i: The index.                                                              *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  -Real*: The row at index.                                                   *
*                                                                              *
\******************************************************************************/
EMBERS_REAL const * const Mat4::operator[](int i) const
{
	return Inner[i];
}

/******************************************************************************\
* operator=                                                                    *
*                                                                              *
*  Matrix assignemnt operator.                                                 *
*                                                                              *
* Parameters                                                                   *                                                   *
*                                                                              *
*  -Other: The other matrix.                                                   *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  -Mat4: The matrix after assignment.                                         *
*                                                                              *
\******************************************************************************/
Mat4 Mat4::operator=(const Mat4 &Other) 
{
	memcpy(Inner, Other.Inner, sizeof(Inner[0][0]) * 16);
	return *this;
}

/******************************************************************************\
* operator*                                                                    *
*                                                                              *
*  Matrix matrix multiplication.                                               *
*                                                                              *
* Parameters                                                                   *                                                   *
*                                                                              *
*  -Other: The other matrix.                                                   *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  -Mat4: The resulting matrix.                                                *
*                                                                              *
\******************************************************************************/
Mat4 Mat4::operator*(const Mat4 &Other) const
{
	Mat4 
		Ret = Mat4::Zero();

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			for (int k = 0; k < 4; k++)
				Ret[i][j] += (*this)[i][k] * Other[k][j];	

	return Ret;
}

/******************************************************************************\
* operator*                                                                    *
*                                                                              *
*  Matrix vector multiplication and perspective division.                      *
*  Takes in a vector3 but treats it like a vector4 with an extra 1 depending   *
*  on the user's choice.                                                       *
*                                                                              *
* Parameters                                                                   *                                                   *
*                                                                              *
*  -Vec: The vector.                                                           *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  -Vec3: the resulting vector.                                                *
*                                                                              *
\******************************************************************************/
Vec3 Mat4::operator*(const Vec3 &Vec) const
{
	Vec3 Result;
	Result.x = (*this)[0][0] * Vec.x +
			   (*this)[0][1] * Vec.y +
			   (*this)[0][2] * Vec.z +
			   (*this)[0][3] * Vec.w;

	Result.y = (*this)[1][0] * Vec.x +
			   (*this)[1][1] * Vec.y +
			   (*this)[1][2] * Vec.z +
			   (*this)[1][3] * Vec.w;

	Result.z = (*this)[2][0] * Vec.x +
			   (*this)[2][1] * Vec.y +
			   (*this)[2][2] * Vec.z +
			   (*this)[2][3] * Vec.w;


    Result.w = (*this)[3][0] * Vec.x +
               (*this)[3][1] * Vec.y +
               (*this)[3][2] * Vec.z +
               (*this)[3][3] * Vec.w;

	return Result;
}

/******************************************************************************\
* Identity                                                                     *
*                                                                              *
*  Builds the identity matrix.                                                 *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  -Mat4: The resulting matrix.                                                *
*                                                                              *
\******************************************************************************/
Mat4 Mat4::Identity()
{
	Mat4 
		Ret = Mat4::Zero();

	for (int i = 0; i < 4; i++)
		Ret[i][i] = 1;

	return Ret;
}

/******************************************************************************\
* Zero                                                                         *
*                                                                              *
*  Builds the zero matrix.                                                     *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  -Mat4: The resulting matrix.                                                *
*                                                                              *
\******************************************************************************/
Mat4 Mat4::Zero()
{
	Mat4 Ret;
	memset(Ret.Inner, 0, sizeof(Ret.Inner[0][0]) * 16);
	return Ret;
}

/******************************************************************************\
* RotateX                                                                      *
*                                                                              *
*  Builds a rotation matrix around the x axis.                                 *
*                                                                              *
* Parameters                                                                   *
*                                                                              *
*  -a: The angle of rotation.                                                  *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  -Mat4: The resulting matrix.                                                *
*                                                                              *
\******************************************************************************/
Mat4 Mat4::RotateX(const EMBERS_REAL &a)
{
	Mat4 
		Ret = Mat4::Identity();

	Ret[1][1] = cos(a); Ret[1][2] = -sin(a);
	Ret[2][1] = sin(a); Ret[2][2] =  cos(a);
	return Ret;
}

/******************************************************************************\
* RotateY                                                                      *
*                                                                              *
*  Builds a rotation matrix around the y axis.                                 *
*                                                                              *
* Parameters                                                                   *
*                                                                              *
*  -a: The angle of rotation.                                                  *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  -Mat4: The resulting matrix.                                                *
*                                                                              *
\******************************************************************************/
Mat4 Mat4::RotateY(const EMBERS_REAL &a)
{
	Mat4 
		Ret = Mat4::Identity();

	Ret[0][0] =  cos(a); Ret[0][2] = sin(a);
	Ret[2][0] = -sin(a); Ret[2][2] = cos(a);
	return Ret;
}

/******************************************************************************\
* RotateZ                                                                      *
*                                                                              *
*  Builds a rotation matrix around the z axis.                                 *
*                                                                              *
* Parameters                                                                   *
*                                                                              *
*  -a: The angle of rotation.                                                  *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  -Mat4: The resulting matrix.                                                *
*                                                                              *
\******************************************************************************/
Mat4 Mat4::RotateZ(const EMBERS_REAL &a)
{
	Mat4 
		Ret = Mat4::Identity();

	Ret[0][0] = cos(a); Ret[0][1] = -sin(a);
	Ret[1][0] = sin(a); Ret[1][1] =  cos(a);
	return Ret;
}

/******************************************************************************\
* ScaleX                                                                       *
*                                                                              *
*  Builds an X axis scaling matrix.                                            *
*                                                                              *
* Parameters                                                                   *
*                                                                              *
*  -s: The scaling factor.                                                     *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  -Mat4: The resulting matrix.                                                *
*                                                                              *
\******************************************************************************/
Mat4 Mat4::ScaleX(const EMBERS_REAL &s)
{
	Mat4 
		Ret = Mat4::Identity();

	Ret[0][0] = s;
	return Ret;
}

/******************************************************************************\
* ScaleY                                                                       *
*                                                                              *
*  Builds a Y axis scaling matrix.                                             *
*                                                                              *
* Parameters                                                                   *
*                                                                              *
*  -s: The scaling factor.                                                     *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  -Mat4: The resulting matrix.                                                *
*                                                                              *
\******************************************************************************/
Mat4 Mat4::ScaleY(const EMBERS_REAL &s)
{
	Mat4 
		Ret = Mat4::Identity();

	Ret[1][1] = s;
	return Ret;
}

/******************************************************************************\
* ScaleZ                                                                       *
*                                                                              *
*  Builds a Z axis scaling matrix.                                             *
*                                                                              *
* Parameters                                                                   *
*                                                                              *
*  -s: The scaling factor.                                                     *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  -Mat4: The resulting matrix.                                                *
*                                                                              *
\******************************************************************************/
Mat4 Mat4::ScaleZ(const EMBERS_REAL &s)
{
	Mat4 
		Ret = Mat4::Identity();

	Ret[2][2] = s;
	return Ret;
}

/******************************************************************************\
* Scale                                                                        *
*                                                                              *
*  Builds a scaling matrix.                                                    *
*                                                                              *
* Parameters                                                                   *
*                                                                              *
*  -s: The scaling factor.                                                     *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  -Mat4: The resulting matrix.                                                *
*                                                                              *
\******************************************************************************/
Mat4 Mat4::Scale(const EMBERS_REAL &s)
{
	Mat4 
		Ret = Mat4::Identity();

	for (int i = 0; i < 3; i++)
		Ret[i][i] = s;

	return Ret;
}

/******************************************************************************\
* Translate                                                                    *
*                                                                              *
*  Builds a translation matrix.                                                *
*                                                                              *
* Parameters                                                                   *
*                                                                              *
*  -Vec: The vector to translate to.                                           *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  -Mat4: The resulting matrix.                                                *
*                                                                              *
\******************************************************************************/
Mat4 Mat4::Translate(const Vec3 &Vec)
{
	Mat4 
		Ret = Mat4::Identity();

	Ret[0][3] = Vec.x;
	Ret[1][3] = Vec.y;
	Ret[2][3] = Vec.z;

	return Ret;
}

/******************************************************************************\
* LookAt                                                                       *
*                                                                              *
*  Builds the camera matrix.                                                   *
*                                                                              *
* Parameters                                                                   *
*                                                                              *
*  -From: The camera in world coordinates.                                     *
*  -At:   The camera's target in world coordinates.                            *
*  -Up:   The camera's up vector.                                              *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  -Mat4: The resulting matrix.                                                *
*                                                                              *
\******************************************************************************/
Mat4 Mat4::LookAt(Vec3 From, Vec3 At, Vec3 Up)
{
	Vec3 
		Z = (From - At).Normalize(),
		X = Vec3::Cross(Up, Z).Normalize(),
		Y = Vec3::Cross(Z, X).Normalize(),
		T = Vec3(-Vec3::Dot(X, From), -Vec3::Dot(Y, From), -Vec3::Dot(Z, From));

	Mat4 Ret;
	EMBERS_REAL 
		Mat[4][4] =  {{X.x, X.y, X.z, T.x},
					  {Y.x, Y.y, Y.z, T.y},
					  {Z.x, Z.y, Z.z, T.z},
					  {0.f, 0.f, 0.f, 1.f}};

	for (int i = 0; i < 4; i++) 
		for (int j = 0; j < 4; j++)
			Ret[i][j] = Mat[i][j];

	return Ret;
}

/******************************************************************************\
* Perspective                                                                  *
*                                                                              *
*  Builds the simple perspective warp matrix.                                  *
*                                                                              *
* Parameters                                                                   *
*                                                                              *
*  -FOV:   The field of view of the camera.                                   *
*  -AspectRatio: The aspect ratio of the near plane.                           *
*  -Near: The distance of the near plane of the frustum.                       *
*  -Far:  The distance of the far plane of the frustum.                        *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  -Mat4: The resulting matrix.                                                *
*                                                                              *
\******************************************************************************/
Mat4 Mat4::Perspective(EMBERS_REAL FOV,
					   EMBERS_REAL AspectRatio,
					   EMBERS_REAL Near,
					   EMBERS_REAL Far)
{
	Mat4 
		Ret = Mat4::Identity();

	EMBERS_REAL 
		Tan = tan(FOV / 2.f);

	Ret[0][0] = 1 / (AspectRatio * Tan);
	Ret[1][1] = 1 / (Tan);

	Ret[2][2] = -(Far + Near) / (Far - Near);
	Ret[2][3] = (2.f * Near * Far) / (Far - Near);

	Ret[3][2] = -1.f;

	return Ret;
}

/******************************************************************************\
* Ortho                                                                        *
*                                                                              *
*  Builds an orthographic projection matrix.                                   *
*                                                                              *
* Parameters                                                                   *
*                                                                              *
*  -Left:   The left coordinate of the projection box.                         *
*  -Right:  The Right coordinate of the projection box.                        *
*  -Top:    The top coordinate of the projection box.                          *
*  -Bottom: The bottom coordinate of the projection box.                       *
*  -Near:   The distance of the near face of the projection box.               *
*  -Far:    The distance of the far plane of the projection box.               *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  -Mat4: The resulting matrix.                                                *
*                                                                              *
\******************************************************************************/
Mat4 Mat4::Ortho(EMBERS_REAL Left,
				 EMBERS_REAL Right,
				 EMBERS_REAL Top,
				 EMBERS_REAL Bottom,
				 EMBERS_REAL Near,
				 EMBERS_REAL Far)
{
	Mat4
		Ret = Mat4::Identity();

	Ret[0][0] = 2 / (Right - Left);
	Ret[1][1] = 2 / (Top - Bottom);
	Ret[2][2] = -2 / (Far - Near);

	Ret[0][3] = -(Right + Left) / (Right - Left);
	Ret[1][3] = -(Top + Bottom) / (Top - Bottom);
	Ret[2][3] = -(Far + Near) / (Far - Near);
	return Ret;
}

Mat4 Mat4::Transpose(Mat4 Matrix)
{

    Mat4 
        Return = Mat4::Zero();
    
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            Return[j][i] = Matrix[i][j];

    return Return;
}

#define M(a, b) M[a][b]
#define Inverse(a, b) Inverse[a][b]
Mat4 Mat4::Invert(Mat4 M)
{


    Mat4 
        Inverse;
    EMBERS_REAL 
        D2233 = M(2, 2) * M(3, 3) - M(2, 3) * M(3, 2),
        D2133 = M(2, 1) * M(3, 3) - M(2, 3) * M(3, 1),
        D2132 = M(2, 1) * M(3, 2) - M(2, 2) * M(3, 1),
        D2033 = M(2, 0) * M(3, 3) - M(2, 3) * M(3, 0),
        D2032 = M(2, 0) * M(3, 2) - M(2, 2) * M(3, 0),
        D2031 = M(2, 0) * M(3, 1) - M(2, 1) * M(3, 0),
        D00 = M(1, 1) * D2233 - M(1, 2) * D2133 + M(1, 3) * D2132,
        D01 = -(M(1, 0) * D2233 - M(1, 2) * D2033 + M(1, 3) * D2032),
        D02 = M(1, 0) * D2133 - M(1, 1) * D2033 + M(1, 3) * D2031,
        D03 = -(M(1, 0) * D2132 - M(1, 1) * D2032 + M(1, 2) * D2031),
        InvDet = 1.0f / (M(0, 0) * D00 + M(0, 1) * D01 + M(0, 2) * D02 + M(0, 3) * D03);

    Inverse(0, 0) = D00 * InvDet;
    Inverse(1, 0) = D01 * InvDet;
    Inverse(2, 0) = D02 * InvDet;
    Inverse(3, 0) = D03 * InvDet;
    Inverse(0, 1) = -(M(0, 1) * D2233 - M(0, 2) * D2133 + M(0, 3) * D2132) * InvDet;
    Inverse(1, 1) = (M(0, 0) * D2233 - M(0, 2) * D2033 + M(0, 3) * D2032) * InvDet;
    Inverse(2, 1) = -(M(0, 0) * D2133 - M(0, 1) * D2033 + M(0, 3) * D2031) * InvDet;
    Inverse(3, 1) = (M(0, 0) * D2132 - M(0, 1) * D2032 + M(0, 2) * D2031) * InvDet;

    EMBERS_REAL 
        D1233 = M(1, 2) * M(3, 3) - M(3, 1) * M(3, 2),
        D1133 = M(1, 1) * M(3, 3) - M(1, 3) * M(3, 1),
        D1132 = M(1, 1) * M(3, 2) - M(1, 2) * M(3, 1),
        D1033 = M(1, 0) * M(3, 3) - M(1, 3) * M(3, 0),
        D1032 = M(1, 0) * M(3, 2) - M(1, 2) * M(3, 0),
        D1031 = M(1, 0) * M(3, 1) - M(1, 1) * M(3, 0);

    Inverse(0, 2) = (M(0, 1) * D1233 - M(0, 2) * D1133 + M(0, 3) * D1132) * InvDet;
    Inverse(1, 2) = -(M(0, 0) * D1233 - M(0, 2) * D1033 + M(0, 3) * D1032) * InvDet;
    Inverse(2, 2) = (M(0, 0) * D1133 - M(0, 1) * D1033 + M(0, 3) * D1031) * InvDet;
    Inverse(3, 2) = -(M(0, 0) * D1132 - M(0, 1) * D1032 + M(0, 2) * D1031) * InvDet;

    EMBERS_REAL 
        D1223 = M(1, 2) * M(2, 3) - M(1, 3) * M(2, 2),
        D1123 = M(0, 1) * M(2, 3) - M(1, 3) * M(2, 1),
        D1122 = M(1, 1) * M(1, 2) - M(1, 2) * M(2, 1),
        D1023 = M(1, 0) * M(2, 3) - M(1, 3) * M(2, 0),
        D1022 = M(1, 0) * M(2, 2) - M(1, 2) * M(2, 0),
        D1021 = M(1, 0) * M(2, 1) - M(1, 1) * M(2, 0);

    Inverse(0, 3) = -(M(0, 1) * D1223 - M(0, 2) * D1123 + M(0, 3) * D1122) * InvDet;
    Inverse(1, 3) = (M(0, 0) * D1223 - M(0, 2) * D1023 + M(0, 3) * D1022) * InvDet;
    Inverse(2, 3) = -(M(0, 0) * D1123 - M(0, 1) * D1023 + M(0, 1) * D1021) * InvDet;
    Inverse(3, 3) = (M(0, 0) * D1122 - M(0, 1) * D1022 + M(0, 2) * D1021) * InvDet;
    return Inverse;
}

Mat4 Mat4::Sqrt(Mat4 Matrix)
{
    Mat4 Result = Mat4::Identity();

    for (int i = 0; i < 4; i++)
        Result[i][i] = sqrt(Matrix[i][i]);

    return Result;
}
