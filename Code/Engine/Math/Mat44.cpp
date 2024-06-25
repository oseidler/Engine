#include "Engine/Math/Mat44.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/Vec4.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/Quaternion.hpp"
#include <math.h>


//
//constructors
//
Mat44::Mat44()
{
	//default constructor makes identity matrix
	m_values[Ix] = 1.0f;
	m_values[Iy] = 0.0f;
	m_values[Iz] = 0.0f;
	m_values[Iw] = 0.0f;

	m_values[Jx] = 0.0f;
	m_values[Jy] = 1.0f;
	m_values[Jz] = 0.0f;
	m_values[Jw] = 0.0f;

	m_values[Kx] = 0.0f;
	m_values[Ky] = 0.0f;
	m_values[Kz] = 1.0f;
	m_values[Kw] = 0.0f;

	m_values[Tx] = 0.0f;
	m_values[Ty] = 0.0f;
	m_values[Tz] = 0.0f;
	m_values[Tw] = 1.0f;
}


Mat44::Mat44(Vec2 const& iBasis2D, Vec2 const& jBasis2D, Vec2 const& translation2D)
{
	m_values[Ix] = iBasis2D.x;
	m_values[Iy] = iBasis2D.y;
	m_values[Iz] = 0.0f;
	m_values[Iw] = 0.0f;

	m_values[Jx] = jBasis2D.x;
	m_values[Jy] = jBasis2D.y;
	m_values[Jz] = 0.0f;
	m_values[Jw] = 0.0f;

	m_values[Kx] = 0.0f;
	m_values[Ky] = 0.0f;
	m_values[Kz] = 1.0f;
	m_values[Kw] = 0.0f;

	m_values[Tx] = translation2D.x;
	m_values[Ty] = translation2D.y;
	m_values[Tz] = 0.0f;
	m_values[Tw] = 1.0f;
}


Mat44::Mat44(Vec3 const& iBasis3D, Vec3 const& jBasis3D, Vec3 const& kBasis3D, Vec3 const& translation3D)
{
	m_values[Ix] = iBasis3D.x;
	m_values[Iy] = iBasis3D.y;
	m_values[Iz] = iBasis3D.z;
	m_values[Iw] = 0.0f;

	m_values[Jx] = jBasis3D.x;
	m_values[Jy] = jBasis3D.y;
	m_values[Jz] = jBasis3D.z;
	m_values[Jw] = 0.0f;

	m_values[Kx] = kBasis3D.x;
	m_values[Ky] = kBasis3D.y;
	m_values[Kz] = kBasis3D.z;
	m_values[Kw] = 0.0f;

	m_values[Tx] = translation3D.x;
	m_values[Ty] = translation3D.y;
	m_values[Tz] = translation3D.z;
	m_values[Tw] = 1.0f;
}


Mat44::Mat44(Vec4 const& iBasis4D, Vec4 const& jBasis4D, Vec4 const& kBasis4D, Vec4 const& translation4D)
{
	m_values[Ix] = iBasis4D.x;
	m_values[Iy] = iBasis4D.y;
	m_values[Iz] = iBasis4D.z;
	m_values[Iw] = iBasis4D.w;

	m_values[Jx] = jBasis4D.x;
	m_values[Jy] = jBasis4D.y;
	m_values[Jz] = jBasis4D.z;
	m_values[Jw] = jBasis4D.w;

	m_values[Kx] = kBasis4D.x;
	m_values[Ky] = kBasis4D.y;
	m_values[Kz] = kBasis4D.z;
	m_values[Kw] = kBasis4D.w;

	m_values[Tx] = translation4D.x;
	m_values[Ty] = translation4D.y;
	m_values[Tz] = translation4D.z;
	m_values[Tw] = translation4D.w;
}


Mat44::Mat44(float const* sixteenValuesBasisMajor)
{
	m_values[Ix] = sixteenValuesBasisMajor[0];
	m_values[Iy] = sixteenValuesBasisMajor[1];
	m_values[Iz] = sixteenValuesBasisMajor[2];
	m_values[Iw] = sixteenValuesBasisMajor[3];

	m_values[Jx] = sixteenValuesBasisMajor[4];
	m_values[Jy] = sixteenValuesBasisMajor[5];
	m_values[Jz] = sixteenValuesBasisMajor[6];
	m_values[Jw] = sixteenValuesBasisMajor[7];

	m_values[Kx] = sixteenValuesBasisMajor[8];
	m_values[Ky] = sixteenValuesBasisMajor[9];
	m_values[Kz] = sixteenValuesBasisMajor[10];
	m_values[Kw] = sixteenValuesBasisMajor[11];

	m_values[Tx] = sixteenValuesBasisMajor[12];
	m_values[Ty] = sixteenValuesBasisMajor[13];
	m_values[Tz] = sixteenValuesBasisMajor[14];
	m_values[Tw] = sixteenValuesBasisMajor[15];
}


//
//static creation functions
//
Mat44 const Mat44::CreateTranslation2D(Vec2 const& translationXY)
{
	Mat44 trans; //trans rights!
	trans.m_values[Tx] = translationXY.x;
	trans.m_values[Ty] = translationXY.y;
	return trans;
}


Mat44 const Mat44::CreateTranslation3D(Vec3 const& translationXYZ)
{
	Mat44 trans;
	trans.m_values[Tx] = translationXYZ.x;
	trans.m_values[Ty] = translationXYZ.y;
	trans.m_values[Tz] = translationXYZ.z;
	return trans;
}


Mat44 const Mat44::CreateUniformScale2D(float uniformScaleXY)
{
	Mat44 scale;
	scale.m_values[Ix] = uniformScaleXY;
	scale.m_values[Jy] = uniformScaleXY;
	return scale;
}


Mat44 const Mat44::CreateUniformScale3D(float uniformScaleXYZ)
{
	Mat44 scale;
	scale.m_values[Ix] = uniformScaleXYZ;
	scale.m_values[Jy] = uniformScaleXYZ;
	scale.m_values[Kz] = uniformScaleXYZ;
	return scale;
}


Mat44 const Mat44::CreateNonUniformScale2D(Vec2 const& nonUniformScaleXY)
{
	Mat44 scale;
	scale.m_values[Ix] = nonUniformScaleXY.x;
	scale.m_values[Jy] = nonUniformScaleXY.y;
	return scale;
}


Mat44 const Mat44::CreateNonUniformScale3D(Vec3 const& nonUniformScaleXYZ)
{
	Mat44 scale;
	scale.m_values[Ix] = nonUniformScaleXYZ.x;
	scale.m_values[Jy] = nonUniformScaleXYZ.y;
	scale.m_values[Kz] = nonUniformScaleXYZ.z;
	return scale;
}


Mat44 const Mat44::CreateZRotationDegrees(float rotationDegreesAboutZ)
{
	Mat44 rotate;
	float c = CosDegrees(rotationDegreesAboutZ);
	float s = SinDegrees(rotationDegreesAboutZ);
	rotate.m_values[Ix] = c;
	rotate.m_values[Iy] = s;
	rotate.m_values[Jx] = -s;
	rotate.m_values[Jy] = c;
	return rotate;
}


Mat44 const Mat44::CreateYRotationDegrees(float rotationDegreesAboutY)
{
	Mat44 rotate;
	float c = CosDegrees(rotationDegreesAboutY);
	float s = SinDegrees(rotationDegreesAboutY);
	rotate.m_values[Ix] = c;
	rotate.m_values[Iz] = -s;
	rotate.m_values[Kx] = s;
	rotate.m_values[Kz] = c;
	return rotate;
}


Mat44 const Mat44::CreateXRotationDegrees(float rotationDegreesAboutX)
{
	Mat44 rotate;
	float c = CosDegrees(rotationDegreesAboutX);
	float s = SinDegrees(rotationDegreesAboutX);
	rotate.m_values[Jy] = c;
	rotate.m_values[Jz] = s;
	rotate.m_values[Ky] = -s;
	rotate.m_values[Kz] = c;
	return rotate;
}


Mat44 const Mat44::CreateOrthoProjection(float left, float right, float bottom, float top, float zNear, float zFar)
{
	Mat44 orthoProjection;

	orthoProjection.m_values[Ix] = 2 / (right - left);
	orthoProjection.m_values[Tx] = (left + right) / (left - right);

	orthoProjection.m_values[Jy] = 2 / (top - bottom);
	orthoProjection.m_values[Ty] = (bottom + top) / (bottom - top);

	orthoProjection.m_values[Kz] = 1 / (zFar - zNear);
	orthoProjection.m_values[Tz] = -zNear / (zFar - zNear);

	return orthoProjection;
}


Mat44 const Mat44::CreatePerspectiveProjection(float fovYDegrees, float aspect, float zNear, float zFar)
{
	Mat44 perspectiveProjection;

	float inverseTanFovDegrees = 1.0f / TanDegrees(fovYDegrees * 0.5f);

	perspectiveProjection.m_values[Ix] = inverseTanFovDegrees / aspect;
	perspectiveProjection.m_values[Jy] = inverseTanFovDegrees;
	perspectiveProjection.m_values[Kz] = zFar / (zFar - zNear);
	perspectiveProjection.m_values[Tz] = -(zFar * zNear) / (zFar - zNear);
	perspectiveProjection.m_values[Kw] = 1.0f;
	perspectiveProjection.m_values[Tw] = 0.0f;

	return perspectiveProjection;
}


//
//transform functions
//
Vec2 const Mat44::TransformVectorQuantity2D(Vec2 const& vectorQuantityXY) const
{
	float newX = m_values[Ix] * vectorQuantityXY.x + m_values[Jx] * vectorQuantityXY.y;
	float newY = m_values[Iy] * vectorQuantityXY.x + m_values[Jy] * vectorQuantityXY.y;

	return Vec2(newX, newY);
}


Vec3 const Mat44::TransformVectorQuantity3D(Vec3 const& vectorQuantityXYZ) const
{
	float newX = m_values[Ix] * vectorQuantityXYZ.x + m_values[Jx] * vectorQuantityXYZ.y + m_values[Kx] * vectorQuantityXYZ.z;
	float newY = m_values[Iy] * vectorQuantityXYZ.x + m_values[Jy] * vectorQuantityXYZ.y + m_values[Ky] * vectorQuantityXYZ.z;
	float newZ = m_values[Iz] * vectorQuantityXYZ.x + m_values[Jz] * vectorQuantityXYZ.y + m_values[Kz] * vectorQuantityXYZ.z;

	return Vec3(newX, newY, newZ);
}


Vec2 const Mat44::TransformPosition2D(Vec2 const& positionXY) const
{
	float newX = m_values[Ix] * positionXY.x + m_values[Jx] * positionXY.y + m_values[Tx];
	float newY = m_values[Iy] * positionXY.x + m_values[Jy] * positionXY.y + m_values[Ty];

	return Vec2(newX, newY);
}


Vec3 const Mat44::TransformPosition3D(Vec3 const& positionXYZ) const
{
	float newX = m_values[Ix] * positionXYZ.x + m_values[Jx] * positionXYZ.y + m_values[Kx] * positionXYZ.z + m_values[Tx];
	float newY = m_values[Iy] * positionXYZ.x + m_values[Jy] * positionXYZ.y + m_values[Ky] * positionXYZ.z + m_values[Ty];
	float newZ = m_values[Iz] * positionXYZ.x + m_values[Jz] * positionXYZ.y + m_values[Kz] * positionXYZ.z + m_values[Tz];

	return Vec3(newX, newY, newZ);
}


Vec4 const Mat44::TransformHomogeneous3D(Vec4 const& homogeneousPoint3D) const
{
	float newX = m_values[Ix] * homogeneousPoint3D.x + m_values[Jx] * homogeneousPoint3D.y + m_values[Kx] * homogeneousPoint3D.z + m_values[Tx] * homogeneousPoint3D.w;
	float newY = m_values[Iy] * homogeneousPoint3D.x + m_values[Jy] * homogeneousPoint3D.y + m_values[Ky] * homogeneousPoint3D.z + m_values[Ty] * homogeneousPoint3D.w;
	float newZ = m_values[Iz] * homogeneousPoint3D.x + m_values[Jz] * homogeneousPoint3D.y + m_values[Kz] * homogeneousPoint3D.z + m_values[Tz] * homogeneousPoint3D.w;
	float newW = m_values[Iw] * homogeneousPoint3D.x + m_values[Jw] * homogeneousPoint3D.y + m_values[Kw] * homogeneousPoint3D.z + m_values[Tw] * homogeneousPoint3D.w;

	return Vec4(newX, newY, newZ, newW);
}


//
//accessors
//
float* Mat44::GetAsFloatArray()
{
	return m_values;
}


float const* Mat44::GetAsFloatArray() const
{
	return m_values;
}


Quaternion const Mat44::GetAsQuaternion() const
{
	Quaternion quat = Quaternion();
	float t = 0.0f;

	if (m_values[Kz] < 0.0f)
	{
		if (m_values[Ix] > m_values[Jy])
		{
			t = 1 + m_values[Ix] - m_values[Jy] - m_values[Kz];
			quat = Quaternion(t, m_values[Iy] + m_values[Jx], m_values[Kx] + m_values[Iz], m_values[Jz] - m_values[Ky]);
		}
		else
		{
			t = 1 - m_values[Ix] + m_values[Jy] - m_values[Kz];
			quat = Quaternion(m_values[Iy] + m_values[Jx], t, m_values[Jz] + m_values[Ky], m_values[Kx] - m_values[Iz]);
		}
	}
	else
	{
		if (m_values[Ix] < -m_values[Jy])
		{
			t = 1 - m_values[Ix] - m_values[Jy] + m_values[Kz];
			quat = Quaternion(m_values[Kx] + m_values[Iz], m_values[Jz] + m_values[Ky], t, m_values[Iy] - m_values[Jx]);
		}
		else
		{
			t = 1 + m_values[Ix] + m_values[Jy] + m_values[Kz];
			quat = Quaternion(m_values[Jz] - m_values[Ky], m_values[Kx] - m_values[Iz], m_values[Iy] - m_values[Jx], t);
		}
	}

	quat *= (0.5f / sqrtf(t));

	return quat;
}


Vec2 const Mat44::GetIBasis2D() const
{
	return Vec2(m_values[Ix], m_values[Iy]);
}


Vec2 const Mat44::GetJBasis2D() const
{
	return Vec2(m_values[Jx], m_values[Jy]);
}


Vec2 const Mat44::GetTranslation2D() const
{
	return Vec2(m_values[Tx], m_values[Ty]);
}


Vec3 const Mat44::GetIBasis3D() const
{
	return Vec3(m_values[Ix], m_values[Iy], m_values[Iz]);
}


Vec3 const Mat44::GetJBasis3D() const
{
	return Vec3(m_values[Jx], m_values[Jy], m_values[Jz]);
}


Vec3 const Mat44::GetKBasis3D() const
{
	return Vec3(m_values[Kx], m_values[Ky], m_values[Kz]);
}


Vec3 const Mat44::GetTranslation3D() const
{
	return Vec3(m_values[Tx], m_values[Ty], m_values[Tz]);
}


Vec4 const Mat44::GetIBasis4D() const
{
	return Vec4(m_values[Ix], m_values[Iy], m_values[Iz], m_values[Iw]);
}


Vec4 const Mat44::GetJBasis4D() const
{
	return Vec4(m_values[Jx], m_values[Jy], m_values[Jz], m_values[Jw]);
}


Vec4 const Mat44::GetKBasis4D() const
{
	return Vec4(m_values[Kx], m_values[Ky], m_values[Kz], m_values[Kw]);
}


Vec4 const Mat44::GetTranslation4D() const
{
	return Vec4(m_values[Tx], m_values[Ty], m_values[Tz], m_values[Tw]);
}


Mat44 const Mat44::GetOrthonormalInverse() const
{
	Vec3 iBasis = GetIBasis3D();
	Vec3 jBasis = GetJBasis3D();
	Vec3 kBasis = GetKBasis3D();

	Mat44 rotationMatrix;
	rotationMatrix.SetIJK3D(iBasis, jBasis, kBasis);

	rotationMatrix.Transpose();

	Vec3 translation = GetTranslation3D();
	translation.x = -translation.x;
	translation.y = -translation.y;
	translation.z = -translation.z;

	Mat44 translationMatrix;
	translationMatrix.SetTranslation3D(translation);

	Mat44 inverseMatrix = rotationMatrix;
	inverseMatrix.Append(translationMatrix);

	return inverseMatrix;
}


//
//mutators
//
void Mat44::SetTranslation2D(Vec2 const& translationXY)
{
	m_values[Tx] = translationXY.x;
	m_values[Ty] = translationXY.y;
	m_values[Tz] = 0.0f;
	m_values[Tw] = 1.0f;
}


void Mat44::SetTranslation3D(Vec3 const& translationXYZ)
{
	m_values[Tx] = translationXYZ.x;
	m_values[Ty] = translationXYZ.y;
	m_values[Tz] = translationXYZ.z;
	m_values[Tw] = 1.0f;
}


void Mat44::SetIJ2D(Vec2 const& iBasis2D, Vec2 const& jBasis2D)
{
	m_values[Ix] = iBasis2D.x;
	m_values[Iy] = iBasis2D.y;
	m_values[Iz] = 0.0f;
	m_values[Iw] = 0.0f;

	m_values[Jx] = jBasis2D.x;
	m_values[Jy] = jBasis2D.y;
	m_values[Jz] = 0.0f;
	m_values[Jw] = 0.0f;
}


void Mat44::SetIJT2D(Vec2 const& iBasis2D, Vec2 const& jBasis2D, Vec2 const& translationXY)
{
	m_values[Ix] = iBasis2D.x;
	m_values[Iy] = iBasis2D.y;
	m_values[Iz] = 0.0f;
	m_values[Iw] = 0.0f;

	m_values[Jx] = jBasis2D.x;
	m_values[Jy] = jBasis2D.y;
	m_values[Jz] = 0.0f;
	m_values[Jw] = 0.0f;

	m_values[Tx] = translationXY.x;
	m_values[Ty] = translationXY.y;
	m_values[Tz] = 0.0f;
	m_values[Tw] = 1.0f;
}


void Mat44::SetIJK3D(Vec3 const& iBasis3D, Vec3 const& jBasis3D, Vec3 const& kBasis3D)
{
	m_values[Ix] = iBasis3D.x;
	m_values[Iy] = iBasis3D.y;
	m_values[Iz] = iBasis3D.z;
	m_values[Iw] = 0.0f;
						 
	m_values[Jx] = jBasis3D.x;
	m_values[Jy] = jBasis3D.y;
	m_values[Jz] = jBasis3D.z;
	m_values[Jw] = 0.0f;

	m_values[Kx] = kBasis3D.x;
	m_values[Ky] = kBasis3D.y;
	m_values[Kz] = kBasis3D.z;
	m_values[Kw] = 0.0f;
}

void Mat44::SetIJKT3D(Vec3 const& iBasis3D, Vec3 const& jBasis3D, Vec3 const& kBasis3D, Vec3 const& translationXYZ)
{
	m_values[Ix] = iBasis3D.x;
	m_values[Iy] = iBasis3D.y;
	m_values[Iz] = iBasis3D.z;
	m_values[Iw] = 0.0f;

	m_values[Jx] = jBasis3D.x;
	m_values[Jy] = jBasis3D.y;
	m_values[Jz] = jBasis3D.z;
	m_values[Jw] = 0.0f;

	m_values[Kx] = kBasis3D.x;
	m_values[Ky] = kBasis3D.y;
	m_values[Kz] = kBasis3D.z;
	m_values[Kw] = 0.0f;

	m_values[Tx] = translationXYZ.x;
	m_values[Ty] = translationXYZ.y;
	m_values[Tz] = translationXYZ.z;
	m_values[Tw] = 1.0f;
}


void Mat44::SetIJKT4D(Vec4 const& iBasis4D, Vec4 const& jBasis4D, Vec4 const& kBasis4D, Vec4 const& translation4D)
{
	m_values[Ix] = iBasis4D.x;
	m_values[Iy] = iBasis4D.y;
	m_values[Iz] = iBasis4D.z;
	m_values[Iw] = iBasis4D.w;

	m_values[Jx] = jBasis4D.x;
	m_values[Jy] = jBasis4D.y;
	m_values[Jz] = jBasis4D.z;
	m_values[Jw] = jBasis4D.w;

	m_values[Kx] = kBasis4D.x;
	m_values[Ky] = kBasis4D.y;
	m_values[Kz] = kBasis4D.z;
	m_values[Kw] = kBasis4D.w;

	m_values[Tx] = translation4D.x;
	m_values[Ty] = translation4D.y;
	m_values[Tz] = translation4D.z;
	m_values[Tw] = translation4D.w;
}


void Mat44::Transpose()
{
	Mat44 copyOfThis = *this;
	float const* oldValues = copyOfThis.m_values;

	m_values[Iy] = oldValues[Jx];
	m_values[Iz] = oldValues[Kx];
	m_values[Iw] = oldValues[Tx];

	m_values[Jx] = oldValues[Iy];
	m_values[Jz] = oldValues[Ky];
	m_values[Jw] = oldValues[Ty];

	m_values[Kx] = oldValues[Iz];
	m_values[Ky] = oldValues[Jz];
	m_values[Kw] = oldValues[Tz];

	m_values[Tx] = oldValues[Iw];
	m_values[Ty] = oldValues[Jw];
	m_values[Tz] = oldValues[Kw];
}


void Mat44::Orthonormalize_XFwd_YLeft_ZUp()
{
	Vec3 iBasis = GetIBasis3D();
	Vec3 jBasis = GetJBasis3D();
	Vec3 kBasis = GetKBasis3D();

	iBasis.Normalize();
	
	float kDotI = DotProduct3D(kBasis, iBasis);
	Vec3 kProjectedOnI = iBasis * kDotI;
	kBasis -= kProjectedOnI;
	kBasis.Normalize();

	float jDotI = DotProduct3D(jBasis, iBasis);
	Vec3 jProjectedOnI = iBasis * jDotI;
	jBasis -= jProjectedOnI;
	float jDotK = DotProduct3D(jBasis, kBasis);
	Vec3 jProjectedOnK = kBasis * jDotK;
	jBasis -= jProjectedOnK;
	jBasis.Normalize();

	SetIJK3D(iBasis, jBasis, kBasis);
}


//
//append functions
//
void Mat44::Append(Mat44 const& matToAppend)
{
	Mat44 copyOfThis = *this;
	float const* oldValues = copyOfThis.m_values;
	float const* newValues = matToAppend.m_values;

	m_values[Ix] = oldValues[Ix] * newValues[Ix] + oldValues[Jx] * newValues[Iy] + oldValues[Kx] * newValues[Iz] + oldValues[Tx] * newValues[Iw];
	m_values[Iy] = oldValues[Iy] * newValues[Ix] + oldValues[Jy] * newValues[Iy] + oldValues[Ky] * newValues[Iz] + oldValues[Ty] * newValues[Iw];
	m_values[Iz] = oldValues[Iz] * newValues[Ix] + oldValues[Jz] * newValues[Iy] + oldValues[Kz] * newValues[Iz] + oldValues[Tz] * newValues[Iw];
	m_values[Iw] = oldValues[Iw] * newValues[Ix] + oldValues[Jw] * newValues[Iy] + oldValues[Kw] * newValues[Iz] + oldValues[Tw] * newValues[Iw];

	m_values[Jx] = oldValues[Ix] * newValues[Jx] + oldValues[Jx] * newValues[Jy] + oldValues[Kx] * newValues[Jz] + oldValues[Tx] * newValues[Jw];
	m_values[Jy] = oldValues[Iy] * newValues[Jx] + oldValues[Jy] * newValues[Jy] + oldValues[Ky] * newValues[Jz] + oldValues[Ty] * newValues[Jw];
	m_values[Jz] = oldValues[Iz] * newValues[Jx] + oldValues[Jz] * newValues[Jy] + oldValues[Kz] * newValues[Jz] + oldValues[Tz] * newValues[Jw];
	m_values[Jw] = oldValues[Iw] * newValues[Jx] + oldValues[Jw] * newValues[Jy] + oldValues[Kw] * newValues[Jz] + oldValues[Tw] * newValues[Jw];

	m_values[Kx] = oldValues[Ix] * newValues[Kx] + oldValues[Jx] * newValues[Ky] + oldValues[Kx] * newValues[Kz] + oldValues[Tx] * newValues[Kw];
	m_values[Ky] = oldValues[Iy] * newValues[Kx] + oldValues[Jy] * newValues[Ky] + oldValues[Ky] * newValues[Kz] + oldValues[Ty] * newValues[Kw];
	m_values[Kz] = oldValues[Iz] * newValues[Kx] + oldValues[Jz] * newValues[Ky] + oldValues[Kz] * newValues[Kz] + oldValues[Tz] * newValues[Kw];
	m_values[Kw] = oldValues[Iw] * newValues[Kx] + oldValues[Jw] * newValues[Ky] + oldValues[Kw] * newValues[Kz] + oldValues[Tw] * newValues[Kw];

	m_values[Tx] = oldValues[Ix] * newValues[Tx] + oldValues[Jx] * newValues[Ty] + oldValues[Kx] * newValues[Tz] + oldValues[Tx] * newValues[Tw];
	m_values[Ty] = oldValues[Iy] * newValues[Tx] + oldValues[Jy] * newValues[Ty] + oldValues[Ky] * newValues[Tz] + oldValues[Ty] * newValues[Tw];
	m_values[Tz] = oldValues[Iz] * newValues[Tx] + oldValues[Jz] * newValues[Ty] + oldValues[Kz] * newValues[Tz] + oldValues[Tz] * newValues[Tw];
	m_values[Tw] = oldValues[Iw] * newValues[Tx] + oldValues[Jw] * newValues[Ty] + oldValues[Kw] * newValues[Tz] + oldValues[Tw] * newValues[Tw];
}


void Mat44::AppendZRotation(float degreesRotationAboutZ)
{
	Mat44 copyOfThis = *this;
	float const* oldValues = copyOfThis.m_values;

	float c = CosDegrees(degreesRotationAboutZ);
	float s = SinDegrees(degreesRotationAboutZ);

	m_values[Ix] = oldValues[Ix] * c + oldValues[Jx] * s;
	m_values[Iy] = oldValues[Iy] * c + oldValues[Jy] * s;
	m_values[Iz] = oldValues[Iz] * c + oldValues[Jz] * s;
	m_values[Iw] = oldValues[Iw] * c + oldValues[Jw] * s;

	m_values[Jx] = oldValues[Ix] * -s + oldValues[Jx] * c;
	m_values[Jy] = oldValues[Iy] * -s + oldValues[Jy] * c;
	m_values[Jz] = oldValues[Iz] * -s + oldValues[Jz] * c;
	m_values[Jw] = oldValues[Iw] * -s + oldValues[Jw] * c;
}


void Mat44::AppendYRotation(float degreesRotationAboutY)
{
	Mat44 copyOfThis = *this;
	float const* oldValues = copyOfThis.m_values;

	float c = CosDegrees(degreesRotationAboutY);
	float s = SinDegrees(degreesRotationAboutY);

	m_values[Ix] = oldValues[Ix] * c + oldValues[Kx] * -s;
	m_values[Iy] = oldValues[Iy] * c + oldValues[Ky] * -s;
	m_values[Iz] = oldValues[Iz] * c + oldValues[Kz] * -s;
	m_values[Iw] = oldValues[Iw] * c + oldValues[Kw] * -s;

	m_values[Kx] = oldValues[Ix] * s + oldValues[Kx] * c;
	m_values[Ky] = oldValues[Iy] * s + oldValues[Ky] * c;
	m_values[Kz] = oldValues[Iz] * s + oldValues[Kz] * c;
	m_values[Kw] = oldValues[Iw] * s + oldValues[Kw] * c;
}


void Mat44::AppendXRotation(float degreesRotationAboutX)
{
	Mat44 copyOfThis = *this;
	float const* oldValues = copyOfThis.m_values;

	float c = CosDegrees(degreesRotationAboutX);
	float s = SinDegrees(degreesRotationAboutX);

	m_values[Jx] = oldValues[Jx] * c + oldValues[Kx] * s;
	m_values[Jy] = oldValues[Jy] * c + oldValues[Ky] * s;
	m_values[Jz] = oldValues[Jz] * c + oldValues[Kz] * s;
	m_values[Jw] = oldValues[Jw] * c + oldValues[Kw] * s;

	m_values[Kx] = oldValues[Jx] * -s + oldValues[Kx] * c;
	m_values[Ky] = oldValues[Jy] * -s + oldValues[Ky] * c;
	m_values[Kz] = oldValues[Jz] * -s + oldValues[Kz] * c;
	m_values[Kw] = oldValues[Jw] * -s + oldValues[Kw] * c;
}


void Mat44::AppendTranslation2D(Vec2 const& translationXY)
{
	Mat44 copyOfThis = *this;

	float const* oldValues = copyOfThis.m_values;

	m_values[Tx] = oldValues[Ix] * translationXY.x + oldValues[Jx] * translationXY.y + oldValues[Tx];
	m_values[Ty] = oldValues[Iy] * translationXY.x + oldValues[Jy] * translationXY.y + oldValues[Ty];
	m_values[Tz] = oldValues[Iz] * translationXY.x + oldValues[Jz] * translationXY.y + oldValues[Tz];
	m_values[Tw] = oldValues[Iw] * translationXY.x + oldValues[Jw] * translationXY.y + oldValues[Tw];
}


void Mat44::AppendTranslation3D(Vec3 const& translationXYZ)
{
	Mat44 copyOfThis = *this;

	float const* oldValues = copyOfThis.m_values;
	
	m_values[Tx] = oldValues[Ix] * translationXYZ.x + oldValues[Jx] * translationXYZ.y + oldValues[Kx] * translationXYZ.z + oldValues[Tx];
	m_values[Ty] = oldValues[Iy] * translationXYZ.x + oldValues[Jy] * translationXYZ.y + oldValues[Ky] * translationXYZ.z + oldValues[Ty];
	m_values[Tz] = oldValues[Iz] * translationXYZ.x + oldValues[Jz] * translationXYZ.y + oldValues[Kz] * translationXYZ.z + oldValues[Tz];
	m_values[Tw] = oldValues[Iw] * translationXYZ.x + oldValues[Jw] * translationXYZ.y + oldValues[Kw] * translationXYZ.z + oldValues[Tw];
}


void Mat44::AppendScaleUniform2D(float uniformScaleXY)
{
	m_values[Ix] *= uniformScaleXY;
	m_values[Iy] *= uniformScaleXY;
	m_values[Iz] *= uniformScaleXY;
	m_values[Iw] *= uniformScaleXY;
				    
	m_values[Jx] *= uniformScaleXY;
	m_values[Jy] *= uniformScaleXY;
	m_values[Jz] *= uniformScaleXY;
	m_values[Jw] *= uniformScaleXY;
}


void Mat44::AppendScaleUniform3D(float uniformScaleXYZ)
{
	m_values[Ix] *= uniformScaleXYZ;
	m_values[Iy] *= uniformScaleXYZ;
	m_values[Iz] *= uniformScaleXYZ;
	m_values[Iw] *= uniformScaleXYZ;

	m_values[Jx] *= uniformScaleXYZ;
	m_values[Jy] *= uniformScaleXYZ;
	m_values[Jz] *= uniformScaleXYZ;
	m_values[Jw] *= uniformScaleXYZ;

	m_values[Kx] *= uniformScaleXYZ;
	m_values[Ky] *= uniformScaleXYZ;
	m_values[Kz] *= uniformScaleXYZ;
	m_values[Kw] *= uniformScaleXYZ;
}


void Mat44::AppendScaleNonUniform2D(Vec2 const& nonUniformScaleXY)
{
	m_values[Ix] *= nonUniformScaleXY.x;
	m_values[Iy] *= nonUniformScaleXY.x;
	m_values[Iz] *= nonUniformScaleXY.x;
	m_values[Iw] *= nonUniformScaleXY.x;

	m_values[Jx] *= nonUniformScaleXY.y;
	m_values[Jy] *= nonUniformScaleXY.y;
	m_values[Jz] *= nonUniformScaleXY.y;
	m_values[Jw] *= nonUniformScaleXY.y;
}


void Mat44::AppendScaleNonUniform3D(Vec3 const& nonUniformScaleXYZ)
{
	m_values[Ix] *= nonUniformScaleXYZ.x;
	m_values[Iy] *= nonUniformScaleXYZ.x;
	m_values[Iz] *= nonUniformScaleXYZ.x;
	m_values[Iw] *= nonUniformScaleXYZ.x;

	m_values[Jx] *= nonUniformScaleXYZ.y;
	m_values[Jy] *= nonUniformScaleXYZ.y;
	m_values[Jz] *= nonUniformScaleXYZ.y;
	m_values[Jw] *= nonUniformScaleXYZ.y;

	m_values[Kx] *= nonUniformScaleXYZ.z;
	m_values[Ky] *= nonUniformScaleXYZ.z;
	m_values[Kz] *= nonUniformScaleXYZ.z;
	m_values[Kw] *= nonUniformScaleXYZ.z;
}
