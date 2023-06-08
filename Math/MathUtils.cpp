#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/Vec4.hpp"
#include "Engine/Math/IntVec2.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/OBB2.hpp"
#include "Engine/Math/Mat44.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include <math.h>


//
//angle utilities
//
float ConvertDegreesToRadians(float degrees)
{
	return (degrees * (PI / 180.f));
}


float ConvertRadiansToDegrees(float radians)
{
	return (radians * (180.f / PI));
}


float CosDegrees(float degrees)
{
	return cosf(ConvertDegreesToRadians(degrees));
}


float SinDegrees(float degrees)
{
	return sinf(ConvertDegreesToRadians(degrees));
}


float TanDegrees(float degrees)
{
	return tanf(ConvertDegreesToRadians(degrees));
}


float Atan2Degrees(float y, float x)
{
	return ConvertRadiansToDegrees(atan2f(y, x));
}


float AcosDegrees(float cosine)
{
	return ConvertRadiansToDegrees(acosf(cosine));
}


float GetShortestAngularDispDegrees(float start, float end)
{
	float angularDisp = end - start;
	
	while (angularDisp > 180.f)
	{
		angularDisp -= 360.f;
	}
	while (angularDisp < -180.f)
	{
		angularDisp += 360.f;
	}
	
	return angularDisp;
}


float GetTurnedTowardDegrees(float currentDegrees, float endDegrees, float maxDeltaDegrees)
{
	float angularDisp = GetShortestAngularDispDegrees(currentDegrees, endDegrees);
	
	if (fabsf(angularDisp) <= maxDeltaDegrees)
	{
		return endDegrees;
	}
	else
	{
		if (angularDisp > 0)
		{
			return currentDegrees + maxDeltaDegrees;
		}
		else
		{
			return currentDegrees - maxDeltaDegrees;
		}
	}
}


float GetAngleDegreesBetweenVectors2D(Vec2 const& vectorA, Vec2 const& vectorB)
{
	float dotProduct = DotProduct2D(vectorA, vectorB);
	float magnitudeA = vectorA.GetLength();
	float magnitudeB = vectorB.GetLength();

	return AcosDegrees(dotProduct/(magnitudeA * magnitudeB));
}


// 
//basic 2d and 3d utilities
//
float GetDistance2D(Vec2 const& positionA, Vec2 const& positionB)
{
	float a = positionB.x - positionA.x;
	float b = positionB.y - positionA.y;
	return sqrtf((a * a) + (b * b));
}


float GetDistanceSquared2D(Vec2 const& positionA, Vec2 const& positionB)
{
	float a = positionB.x - positionA.x;
	float b = positionB.y - positionA.y;
	return (a * a) + (b * b);
}


float GetDistance3D(Vec3 const& positionA, Vec3 const& positionB)
{
	float a = positionB.x - positionA.x;
	float b = positionB.y - positionA.y;
	float c = positionB.z - positionA.z;
	return sqrtf((a * a) + (b * b) + (c * c));
}


float GetDistanceSquared3D(Vec3 const& positionA, Vec3 const& positionB)
{
	float a = positionB.x - positionA.x;
	float b = positionB.y - positionA.y;
	float c = positionB.z - positionA.z;
	return (a * a) + (b * b) + (c * c);
}


float GetDistanceXY3D(Vec3 const& positionA, Vec3 const& positionB)
{
	float a = positionB.x - positionA.x;
	float b = positionB.y - positionA.y;
	return sqrtf((a * a) + (b * b));
}


float GetDistanceXYSquared3D(Vec3 const& positionA, Vec3 const& positionB)
{
	float a = positionB.x - positionA.x;
	float b = positionB.y - positionA.y;
	return (a * a) + (b * b);
}


int GetTaxicabDistance2D(IntVec2 const& positionA, IntVec2 const& positionB)
{
	IntVec2 displacement = positionB - positionA;
	
	return displacement.GetTaxicabLength();
}


float GetProjectedLength2D(Vec2 const& projectedVector, Vec2 const& vectorProjectedOnto)
{
	Vec2 normalVectorProjectedOnto = vectorProjectedOnto.GetNormalized();
	
	return DotProduct2D(projectedVector, normalVectorProjectedOnto);
}


Vec2 const GetProjectedOnto2D(Vec2 const& projectedVector, Vec2 const& vectorProjectedOnto)
{
	Vec2 normalVectorProjectedOnto = vectorProjectedOnto.GetNormalized();
	
	return DotProduct2D(projectedVector, normalVectorProjectedOnto) * normalVectorProjectedOnto;
}


Vec3 const GetProjectedOnto3D(Vec3 const& projectedVector, Vec3 const& vectorProjectedOnto)
{
	Vec3 normalVectorProjectedOnto = vectorProjectedOnto.GetNormalized();

	return DotProduct3D(projectedVector, normalVectorProjectedOnto) * normalVectorProjectedOnto;
}


Mat44 const GetBillboardMatrix(BillboardType billboardType, Mat44 const& cameraMatrix, Vec3 const& billboardPosition, Vec2 const& billboardScale)
{
	//INCOMPLETE

	UNUSED(billboardScale);

	Vec3 billboardIBasis = Vec3(1.0f, 0.0f, 0.0f);
	Vec3 billboardJBasis = Vec3(0.0f, 1.0f, 0.0f);
	Vec3 billboardKBasis = Vec3(0.0f, 0.0f, 1.0f);

	Vec3 cameraIBasis = cameraMatrix.GetIBasis3D();
	Vec3 cameraJBasis = cameraMatrix.GetJBasis3D();
	Vec3 cameraKBasis = cameraMatrix.GetKBasis3D();
	Vec3 cameraPos = cameraMatrix.GetTranslation3D();

	if (billboardType == BillboardType::FULL_CAMERA_OPPOSING)
	{
		billboardIBasis = Vec3() - cameraIBasis;
		billboardJBasis = Vec3() - cameraJBasis;
		billboardKBasis = cameraKBasis;
	}
	else if (billboardType == BillboardType::FULL_CAMERA_FACING)
	{
		billboardIBasis = cameraPos - billboardPosition;
		billboardIBasis.Normalize();

		if (fabsf(DotProduct3D(billboardIBasis, Vec3(0.0f, 0.0f, 1.0f)) < 1.0f))
		{
			billboardJBasis = CrossProduct3D(Vec3(0.0f, 0.0f, 1.0f), billboardIBasis);
			billboardJBasis.Normalize();

			billboardKBasis = CrossProduct3D(billboardIBasis, billboardJBasis);
			billboardKBasis.Normalize();
		}
	}
	else if (billboardType == BillboardType::WORLD_UP_CAMERA_OPPOSING)
	{
		billboardIBasis = Vec3() - cameraIBasis;
		billboardIBasis.z = 0.0f;
		
		billboardJBasis = Vec3() - cameraJBasis;
		billboardJBasis.z = 0.0f;
	}
	else if (billboardType == BillboardType::WORLD_UP_CAMERA_FACING)
	{
		billboardIBasis = cameraPos - billboardPosition;
		billboardIBasis.z = 0.0f;
		billboardIBasis.Normalize();

		billboardJBasis = CrossProduct3D(billboardKBasis, billboardIBasis);
		billboardJBasis.Normalize();
	}

	return Mat44(billboardIBasis, billboardJBasis, billboardKBasis, Vec3());
}


//
//geometric query utilities
//
bool IsPointInsideDisc2D(Vec2 const& point, Vec2 const& discCenter, float discRadius)
{
	return GetDistanceSquared2D(point, discCenter) < (discRadius * discRadius);
}


bool IsPointInsideOrientedSector2D(Vec2 const& point, Vec2 const& sectorTip, float sectorForwardDegrees, float sectorApertureDegrees, float sectorRadius)
{
	if (!IsPointInsideDisc2D(point, sectorTip, sectorRadius))
	{
		return false;
	}

	Vec2 displacement = point - sectorTip;
	float displacementOrientation = displacement.GetOrientationDegrees();
	float angularDisplacement = GetShortestAngularDispDegrees(sectorForwardDegrees, displacementOrientation);

	return fabsf(angularDisplacement) < (sectorApertureDegrees * 0.5f);
}


bool IsPointInsideDirectedSector2D(Vec2 const& point, Vec2 const& sectorTip, Vec2 const& sectorForwardDirection, float sectorApertureDegrees, float sectorRadius)
{
	if (!IsPointInsideDisc2D(point, sectorTip, sectorRadius))
	{
		return false;
	}

	Vec2 displacement = point - sectorTip;
	float degreesBetween = GetAngleDegreesBetweenVectors2D(displacement, sectorForwardDirection);
	
	return degreesBetween < (sectorApertureDegrees * 0.5f);
}


bool IsPointInsideAABB2D(Vec2 const& point, AABB2 const& aabb)
{
	if ((point.x > aabb.m_mins.x && point.x < aabb.m_maxs.x) && (point.y > aabb.m_mins.y && point.y < aabb.m_maxs.y))
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool IsPointInsideOBB2D(Vec2 const& point, OBB2 const& obb)
{
	Vec2 displacement = point - obb.m_center;

	Vec2 jBasisNormal = obb.m_iBasisNormal.GetRotated90Degrees();

	float displacementOnI = DotProduct2D(displacement, obb.m_iBasisNormal);
	float displacementOnJ = DotProduct2D(displacement, jBasisNormal);

	if (fabsf(displacementOnI) >= obb.m_halfDimensions.x || fabsf(displacementOnJ) >= obb.m_halfDimensions.y)
	{
		return false;
	}
	else
	{
		return true;
	}
}


bool IsPointInsideCapsule2D(Vec2 const& point, Vec2 const& boneStart, Vec2 const& boneEnd, float radius)
{
	Vec2 pointOnBone = GetNearestPointOnLineSegment(point, boneStart, boneEnd);

	if (GetDistance2D(point, pointOnBone) < radius)
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool DoDiscsOverlap(Vec2 const& centerA, float radiusA, Vec2 const& centerB, float radiusB)
{
	if (GetDistanceSquared2D(centerA, centerB) < (radiusA + radiusB) * (radiusA + radiusB)) return true;
	else return false;
}


bool DoSpheresOverlap(Vec3 const& centerA, float radiusA, Vec3 const& centerB, float radiusB)
{
	if (GetDistance3D(centerA, centerB) < (radiusA + radiusB)) return true;
	return false;
}


Vec2 const GetNearestPointOnDisc2D(Vec2 const& referencePoint, Vec2 const& discCenter, float discRadius)
{
	Vec2 displacement = referencePoint - discCenter;
	displacement.ClampLength(discRadius);
	
	return displacement + discCenter;
}


Vec2 const GetNearestPointOnAABB2D(Vec2 const& referencePoint, AABB2 const& aabb)
{
	float newX = GetClamped(referencePoint.x, aabb.m_mins.x, aabb.m_maxs.x);
	float newY = GetClamped(referencePoint.y, aabb.m_mins.y, aabb.m_maxs.y);
	
	return Vec2(newX, newY);
}


Vec2 const GetNearestPointOnOBB2D(Vec2 const& referencePoint, OBB2 const& obb)
{
	Vec2 displacement = referencePoint - obb.m_center;

	Vec2 jBasisNormal = obb.m_iBasisNormal.GetRotated90Degrees();

	float displacementOnI = DotProduct2D(displacement, obb.m_iBasisNormal);
	float displacementOnJ = DotProduct2D(displacement, jBasisNormal);

	float clampedDisplacementI = GetClamped(displacementOnI, -obb.m_halfDimensions.x, obb.m_halfDimensions.x);
	float clampedDisplacementJ = GetClamped(displacementOnJ, -obb.m_halfDimensions.y, obb.m_halfDimensions.y);

	return obb.m_center + (obb.m_iBasisNormal * clampedDisplacementI) + (jBasisNormal * clampedDisplacementJ);
}


Vec2 const GetNearestPointOnLineSegment(Vec2 const& referencePoint, Vec2 const& segmentStart, Vec2 const& segmentEnd)
{
	Vec2 dispFromStart = referencePoint - segmentStart;
	Vec2 dispFromEnd = referencePoint - segmentEnd;
	Vec2 lineVector = segmentEnd - segmentStart;

	if (DotProduct2D(lineVector, dispFromStart) < 0)
	{
		return segmentStart;
	}
	else if (DotProduct2D(lineVector, dispFromEnd) > 0)
	{
		return segmentEnd;
	}
	
	Vec2 referencePointOnLine = GetProjectedOnto2D(dispFromStart, lineVector);

	return referencePointOnLine + segmentStart;
}


Vec2 const GetNearestPointOnInfiniteLine2D(Vec2 const& referencePoint, Vec2 const& pointOnLineA, Vec2 const& pointOnLineB)
{
	Vec2 dispFromPointA = referencePoint - pointOnLineA;
	Vec2 lineVector = pointOnLineB - pointOnLineA;

	Vec2 referencePointOnLine = GetProjectedOnto2D(dispFromPointA, lineVector);

	return referencePointOnLine + pointOnLineA;
}


Vec2 const GetNearestPointOnCapsule2D(Vec2 const& referencePoint, Vec2 const& boneStart, Vec2 const& boneEnd, float radius)
{
	Vec2 pointOnBone = GetNearestPointOnLineSegment(referencePoint, boneStart, boneEnd);

	Vec2 displacement = referencePoint - pointOnBone;

	displacement.ClampLength(radius);

	return displacement + pointOnBone;
}


bool PushDiscOutOfFixedPoint2D(Vec2& discCenter, float discRadius, Vec2 const& fixedPoint)
{
	Vec2 displacement = discCenter - fixedPoint;
	float displacementLength = displacement.GetLength();

	if (displacementLength > discRadius)
	{
		return false;
	}
	
	float overlapDistance = discRadius - displacementLength;

	displacement.SetLength(overlapDistance);

	discCenter += displacement;

	return true;
}


bool PushDiscOutOfFixedPoint2D(Vec3& discCenter, float discRadius, Vec3 const& fixedPoint)
{
	Vec2 displacement = Vec2(discCenter.x, discCenter.y) - Vec2(fixedPoint.x, fixedPoint.y);
	float displacementLength = displacement.GetLength();

	if (displacementLength > discRadius)
	{
		return false;
	}

	float overlapDistance = discRadius - displacementLength;

	displacement.SetLength(overlapDistance);

	discCenter += Vec3(displacement.x, displacement.y, 0.0f);

	return true;
}


bool PushDiscOutOfFixedDisc2D(Vec2& mobileDiscCenter, float mobileDiscRadius, Vec2 const& fixedDiscCenter, float fixedDiscRadius)
{
	float sumOfRadii = mobileDiscRadius + fixedDiscRadius;
	
	return PushDiscOutOfFixedPoint2D(mobileDiscCenter, sumOfRadii, fixedDiscCenter);
}


bool PushDiscOutOfFixedDisc2D(Vec3& mobileDiscCenter, float mobileDiscRadius, Vec3 const& fixedDiscCenter, float fixedDiscRadius)
{
	float sumOfRadii = mobileDiscRadius + fixedDiscRadius;

	return PushDiscOutOfFixedPoint2D(mobileDiscCenter, sumOfRadii, fixedDiscCenter);
}


bool PushDiscOutOfFixedAABB2D(Vec2& discCenter, float discRadius, AABB2 const& fixedAABB2)
{
	Vec2 nearestPoint = GetNearestPointOnAABB2D(discCenter, fixedAABB2);
	
	return PushDiscOutOfFixedPoint2D(discCenter, discRadius, nearestPoint);
}


bool PushDiscOutOfFixedAABB2D(Vec3& discCenter, float discRadius, AABB2 const& fixedAABB2)
{
	Vec2 nearestPoint = GetNearestPointOnAABB2D(Vec2(discCenter.x, discCenter.y), fixedAABB2);
	Vec3 nearestPoint3D = Vec3(nearestPoint.x, nearestPoint.y, discCenter.z);

	return PushDiscOutOfFixedPoint2D(discCenter, discRadius, nearestPoint3D);
}


bool PushDiscsOutOfEachOther2D(Vec2& discCenterA, float discRadiusA, Vec2& discCenterB, float discRadiusB)
{
	Vec2 displacement = discCenterB - discCenterA;
	float displacementLength = displacement.GetLength();
	float sumOfRadii = discRadiusA + discRadiusB;

	if (sumOfRadii < displacementLength)
	{
		return false;
	}

	float overlapDistance = sumOfRadii - displacementLength;
	overlapDistance *= 0.5f;

	displacement.SetLength(overlapDistance);
	
	discCenterB += displacement;
	discCenterA -= displacement;

	return true;
}


bool PushDiscsOutOfEachOther2D(Vec3& discCenterA, float discRadiusA, Vec3& discCenterB, float discRadiusB)
{
	Vec2 displacement = Vec2(discCenterB.x, discCenterB.y) - Vec2(discCenterA.x, discCenterA.y);
	float displacementLength = displacement.GetLength();
	float sumOfRadii = discRadiusA + discRadiusB;

	if (sumOfRadii < displacementLength)
	{
		return false;
	}

	float overlapDistance = sumOfRadii - displacementLength;
	overlapDistance *= 0.5f;

	displacement.SetLength(overlapDistance);

	discCenterB += Vec3(displacement.x, displacement.y, 0.0f);
	discCenterA -= Vec3(displacement.x, displacement.y, 0.0f);

	return true;
}


bool BounceDiscsOffEachOther2D(Vec2& discCenterA, float discRadiusA, Vec2& discVelA, float discElasticA, Vec2& discCenterB, float discRadiusB, Vec2& discVelB, float discElasticB)
{
	bool pushed = PushDiscsOutOfEachOther2D(discCenterA, discRadiusA, discCenterB, discRadiusB);

	if (!pushed)
	{
		return false;
	}
	
	float elasticity = discElasticA * discElasticB;
	
	Vec2 impactNormal = discCenterB - discCenterA;

	Vec2 discAVelN = GetProjectedOnto2D(discVelA, impactNormal);
	Vec2 discAVelT = discVelA - discAVelN;

	Vec2 discBVelN = GetProjectedOnto2D(discVelB, impactNormal);
	Vec2 discBVelT = discVelB - discBVelN;

	discAVelN *= elasticity;
	discBVelN *= elasticity;

	//if distance of positions after velocity adjustment of centers is greater than current distance, velocities are already diverging
	if (GetDistance2D((discCenterA + discVelA), (discCenterB + discVelB)) > GetDistance2D(discCenterA, discCenterB))
	{
		discVelA = discAVelT + discAVelN;
		discVelB = discBVelT + discBVelN;

		return true;
	}

	discVelA = discAVelT + discBVelN;
	discVelB = discBVelT + discAVelN;

	return true;
}


bool BounceDiscOffFixedDisc2D(Vec2& mobileDiscCenter, float mobileDiscRadius, Vec2& mobileDiscVel, float mobileDiscElastic, Vec2 const& fixedDiscCenter, float fixedDiscRadius, float fixedDiscElastic)
{
	bool pushed = PushDiscOutOfFixedDisc2D(mobileDiscCenter, mobileDiscRadius, fixedDiscCenter, fixedDiscRadius);

	if (!pushed)
	{
		return false;
	}
	
	float elasticity = mobileDiscElastic * fixedDiscElastic;

	Vec2 impactNormal = fixedDiscCenter - mobileDiscCenter;

	Vec2 mobileDiscVelN = GetProjectedOnto2D(mobileDiscVel, impactNormal);
	Vec2 mobileDiscVelT = mobileDiscVel - mobileDiscVelN;
	
	mobileDiscVelN *= elasticity;

	//only reverse velocity if disc isn't already moving away from fixed object
	if (DotProduct2D(mobileDiscVel, impactNormal) >= 0.0f)
	{
		mobileDiscVelN = -mobileDiscVelN;
	}

	mobileDiscVel = mobileDiscVelT + mobileDiscVelN;

	return true;
}


bool BounceDiscOffFixedCapsule2D(Vec2& discCenter, float discRadius, Vec2& discVel, float discElastic, Vec2 const& capsuleBoneStart, Vec2 const& capsuleBoneEnd, float capsuleRadius, float capsuleElastic)
{
	Vec2 nearestPoint = GetNearestPointOnCapsule2D(discCenter, capsuleBoneStart, capsuleBoneEnd, capsuleRadius);
	bool pushed = PushDiscOutOfFixedPoint2D(discCenter, discRadius, nearestPoint);

	if (!pushed)
	{
		return false;
	}

	float elasticity = discElastic * capsuleElastic;

	Vec2 impactNormal = nearestPoint - discCenter;

	Vec2 discVelN = GetProjectedOnto2D(discVel, impactNormal) * elasticity;
	Vec2 discVelT = discVel - discVelN;

	//only reverse velocity if disc isn't already moving away from fixed object
	if (DotProduct2D(discVel, impactNormal) >= 0.0f)
	{
		discVelN = -discVelN;
	}

	discVel = discVelN + discVelT;

	return true;
}


bool BounceDiscOffFixedOBB2D(Vec2& discCenter, float discRadius, Vec2& discVel, float discElastic, OBB2 const& obb, float obbElastic)
{
	Vec2 nearestPoint = GetNearestPointOnOBB2D(discCenter, obb);
	bool pushed = PushDiscOutOfFixedPoint2D(discCenter, discRadius, nearestPoint);

	if (!pushed)
	{
		return false;
	}

	float elasticity = discElastic * obbElastic;

	Vec2 impactNormal = nearestPoint - discCenter;

	Vec2 discVelN = GetProjectedOnto2D(discVel, impactNormal) * elasticity;
	Vec2 discVelT = discVel - discVelN;

	//only reverse velocity if disc isn't already moving away from fixed object
	if (DotProduct2D(discVel, impactNormal) >= 0.0f)
	{
		discVelN = -discVelN;
	}

	discVel = discVelN + discVelT;

	return true;
}


//
//transform utilities
//
void TransformPosition2D(Vec2& posToTransform, float uniformScale, float rotationDegrees, Vec2 const& translation)
{
	posToTransform *= uniformScale;

	posToTransform.RotateDegrees(rotationDegrees);

	posToTransform += translation;
}


void TransformPosition2D(Vec2& posToTransform, Vec2 const& iBasis, Vec2 const& jBasis, Vec2 const& translation)
{
	posToTransform = (posToTransform.x * iBasis) + (posToTransform.y * jBasis) + translation;
}


void TransformPositionXY3D(Vec3& posToTransform, float uniformScale, float rotationDegrees, Vec2 const& translation)
{
	Vec2 pos2D = Vec2(posToTransform.x, posToTransform.y);
	
	pos2D *= uniformScale;

	pos2D.RotateDegrees(rotationDegrees);

	pos2D += Vec2(translation.x, translation.y);

	posToTransform.x = pos2D.x;
	posToTransform.y = pos2D.y;
}


void TransformPositionXY3D(Vec3& posToTransform, Vec2 const& iBasis, Vec2 const& jBasis, Vec2 const& translation)
{
	posToTransform = Vec3((posToTransform.x * iBasis) + (posToTransform.y * jBasis) + translation, posToTransform.z);
}


//
//clamp and lerp
//
float Interpolate(float start, float end, float fraction)
{
	return (end - start) * fraction + start;
}


float GetFractionWithinRange(float value, float start, float end)
{
	if (start == end)
	{
		return 0.5f;
	}
	else
	{
		return (value - start) / (end - start);
	}
}


float RangeMap(float value, float inStart, float inEnd, float outStart, float outEnd)
{
	float fraction = GetFractionWithinRange(value, inStart, inEnd);
	
	return Interpolate(outStart, outEnd, fraction);
}


float RangeMapClamped(float value, float inStart, float inEnd, float outStart, float outEnd)
{
	float valueClamped = GetClamped(value, inStart, inEnd);
	
	return RangeMap(valueClamped, inStart, inEnd, outStart, outEnd);
}


float GetClamped(float value, float min, float max)
{
	if (value < min)
	{
		return min;
	}
	else if (value > max)
	{
		return max;
	}
	else
	{
		return value;
	}
}


int GetClamped(int value, int min, int max)
{
	if (value < min)
	{
		return min;
	}
	else if (value > max)
	{
		return max;
	}
	else
	{
		return value;
	}
}


float GetClampedZeroToOne(float value)
{
	if (value < 0.f)
	{
		return 0.f;
	}
	else if (value > 1.f)
	{
		return 1.f;
	}
	else
	{
		return value;
	}
}


int RoundDownToInt(float value)
{
	return static_cast<int>(floorf(value));
}


//
//dot and cross product
//
float DotProduct2D(Vec2 const& a, Vec2 const& b)
{
	return (a.x * b.x) + (a.y * b.y);
}


float DotProduct3D(Vec3 const& a, Vec3 const& b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}


float DotProduct4D(Vec4 const& a, Vec4 const& b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
}


float CrossProduct2D(Vec2 const& a, Vec2 const& b)
{
	return (a.x * b.y) - (a.y * b.x);
}


Vec3 CrossProduct3D(Vec3 const& a, Vec3 const& b)
{
	return Vec3((a.y * b.z) - (a.z * b.y), (a.z * b.x) - (a.x * b.z), (a.x * b.y) - (a.y * b.x));
}


//
//raycast
//
RaycastResult2D RaycastVsDisc2D(Vec2 const& startPosition, Vec2 const& directionNormal, float maxDistance, Vec2 const& discCenter, float discRadius)
{
	Vec2 const& iBasis = directionNormal;
	Vec2 const jBasis = iBasis.GetRotated90Degrees();
	Vec2 displacement = discCenter - startPosition;

	//get length of displacement in i and j directions
	float dispOnI = DotProduct2D(displacement, iBasis);
	float dispOnJ = DotProduct2D(displacement, jBasis);

	//get end position (for position of raycast end on miss)
	Vec2 raycastEndPos = startPosition + (directionNormal * maxDistance);

	//miss if disc is too far to the side
	if (dispOnJ >= discRadius || dispOnJ <= -discRadius)
	{
		return RaycastResult2D(false, maxDistance, raycastEndPos, Vec2(0.0f, 0.0f));
	}

	//miss if the disc is too far behind the start
	if (dispOnI <= -discRadius)
	{
		return RaycastResult2D(false, maxDistance, raycastEndPos, Vec2(0.0f, 0.0f));
	}

	//miss if the disc is too far away
	if (dispOnI >= maxDistance + discRadius)
	{
		return RaycastResult2D(false, maxDistance, raycastEndPos, Vec2(0.0f, 0.0f));
	}

	//now actually check for hit
	float a = sqrtf((discRadius * discRadius) - (dispOnJ * dispOnJ));
	float hitDistance = dispOnI - a;

	//miss if hit distance is too far away
	if (hitDistance >= maxDistance || hitDistance <= 0)
	{
		return RaycastResult2D(false, maxDistance, raycastEndPos, Vec2(0.0f, 0.0f));
	}
	else
	{
		Vec2 hitPosition = startPosition + (iBasis * hitDistance);
		Vec2 impactNormal = (hitPosition - discCenter).GetNormalized();
		return RaycastResult2D(true, hitDistance, hitPosition, impactNormal);
	}
}


RaycastResult2D RaycastVsLineSegment2D(Vec2 const& startPosition, Vec2 const& directionNormal, float maxDistance, Vec2 const& pointA, Vec2 const& pointB)
{
	//get end position (for position of raycast end on miss)
	Vec2 raycastEndPos = startPosition + (directionNormal * maxDistance);
	
	Vec2 jBasis = directionNormal.GetRotated90Degrees();
	Vec2 dispA = pointA - startPosition;
	Vec2 dispB = pointB - startPosition;
	float dispAOnJ = DotProduct2D(dispA, jBasis);
	float dispBOnJ = DotProduct2D(dispB, jBasis);

	//straddle test
	if (dispAOnJ * dispBOnJ >= 0.0f)
	{
		return RaycastResult2D(false, maxDistance, raycastEndPos, Vec2(0.0f, 0.0f));
	}

	float tOnLine = -dispAOnJ / (dispBOnJ - dispAOnJ);
	Vec2 impactPos = pointA + tOnLine * (pointB - pointA);

	Vec2 impactDisp = impactPos - startPosition;
	float impactDist = DotProduct2D(impactDisp, directionNormal);
	if (impactDist >= maxDistance || impactDist <= 0.0f)
	{
		return RaycastResult2D(false, maxDistance, raycastEndPos, Vec2(0.0f, 0.0f));
	}

	Vec2 lineSegmentNorm = (pointB - pointA).GetNormalized();
	Vec2 impactNormal = lineSegmentNorm.GetRotated90Degrees();
	if (DotProduct2D(impactNormal, directionNormal) > 0.0f)
	{
		impactNormal *= -1.0f;
	}

	return RaycastResult2D(true, impactDist, impactPos, impactNormal);
}


RaycastResult2D RaycastVsAABB2D(Vec2 const& startPosition, Vec2 const& directionNormal, float maxDistance, AABB2 const& aabb2)
{
	//get end position (for position of raycast end on miss)
	Vec2 raycastEndPos = startPosition + (directionNormal * maxDistance);

	float tMinX = (aabb2.m_mins.x - startPosition.x) / directionNormal.x;
	float tMaxX = (aabb2.m_maxs.x - startPosition.x) / directionNormal.x;
	float tMinY = (aabb2.m_mins.y - startPosition.y) / directionNormal.y;
	float tMaxY = (aabb2.m_maxs.y - startPosition.y) / directionNormal.y;

	FloatRange tXRange;
	FloatRange tYRange;
	if (tMinX < tMaxX)
	{
		tXRange = FloatRange(tMinX, tMaxX);
	}
	else
	{
		tXRange = FloatRange(tMaxX, tMinX);
	}
	if (tMinY < tMaxY)
	{
		tYRange = FloatRange(tMinY, tMaxY);
	}
	else
	{
		tYRange = FloatRange(tMaxY, tMinY);
	}
	
	if (tXRange.IsOverlappingWith(tYRange))
	{
		Vec2 impactNormal = Vec2();

		float t = FLT_MAX;
		if (tYRange.IsOnRange(tMinX) && tMinX < t)
		{
			t = tMinX;
			impactNormal = Vec2(-1.0f, 0.0f);
		}
		if (tYRange.IsOnRange(tMaxX) && tMaxX < t)
		{
			t = tMaxX;
			impactNormal = Vec2(1.0f, 0.0f);
		}
		if (tXRange.IsOnRange(tMinY) && tMinY < t)
		{
			t = tMinY;
			impactNormal = Vec2(0.0f, -1.0f);
		}
		if (tXRange.IsOnRange(tMaxY) && tMaxY < t)
		{
			t = tMaxY;
			impactNormal = Vec2(0.0f, 1.0f);
		}

		Vec2 impactPos = startPosition + (directionNormal * t);
		Vec2 impactDisp = impactPos - startPosition;
		float impactDist = DotProduct2D(impactDisp, directionNormal);
		
		if (impactDist >= maxDistance || impactDist <= 0.0f)
		{
			return RaycastResult2D(false, maxDistance, raycastEndPos, Vec2(0.0f, 0.0f));
		}

		return RaycastResult2D(true, impactDist, impactPos, impactNormal);
	}

	return RaycastResult2D(false, maxDistance, raycastEndPos, Vec2(0.0f, 0.0f));
}


RaycastResult3D RaycastVsZCylinder3D(Vec3 const& startPosition, Vec3 const& directionNormal, float maxDistance, Vec3 const& cylinderCenter, float cylinderMinZ, float cylinderMaxZ, float cylinderRadius)
{
	RaycastResult3D raycastResult;

	Vec3 raycastVector = directionNormal * maxDistance;
	
	//if inside cylinder
	if (IsPointInsideDisc2D(Vec2(startPosition.x, startPosition.y), Vec2(cylinderCenter.x, cylinderCenter.y), cylinderRadius) && startPosition.z > cylinderMinZ && startPosition.z < cylinderMaxZ)
	{
		raycastResult.m_didImpact = true;
		raycastResult.m_impactDist = 0.0f;
		raycastResult.m_impactPos = startPosition;
		raycastResult.m_impactNormal = Vec3() - directionNormal;
	}
	else
	{
		//check against cylinder min z
		float tMinZ = (cylinderMinZ - startPosition.z) / raycastVector.z;
		Vec3 posAtTMinZ = Vec3(startPosition.x + (raycastVector.x * tMinZ), startPosition.y + (raycastVector.y * tMinZ), cylinderMinZ);
		if (IsPointInsideDisc2D(Vec2(posAtTMinZ.x, posAtTMinZ.y), Vec2(cylinderCenter.x, cylinderCenter.y), cylinderRadius) && tMinZ > 0.0f && tMinZ < 1.0f)
		{
			raycastResult.m_didImpact = true;
			raycastResult.m_impactDist = GetDistance3D(posAtTMinZ, startPosition);
			raycastResult.m_impactPos = posAtTMinZ;
			raycastResult.m_impactNormal = Vec3(0.00001f, 0.0f, -1.0f);	//for some reason, the arrow doesn't render when facing purely -z and I don't know why
		}
		else
		{
			tMinZ = 1.1f;
		}

		//check against cylinder max z
		float tMaxZ = (cylinderMaxZ - startPosition.z) / raycastVector.z;
		Vec3 posAtTMaxZ = Vec3(startPosition.x + (raycastVector.x * tMaxZ), startPosition.y + (raycastVector.y * tMaxZ), cylinderMaxZ);
		if (IsPointInsideDisc2D(Vec2(posAtTMaxZ.x, posAtTMaxZ.y), Vec2(cylinderCenter.x, cylinderCenter.y), cylinderRadius) && tMaxZ < tMinZ && tMaxZ > 0.0f && tMaxZ < 1.0f)
		{
			raycastResult.m_didImpact = true;
			raycastResult.m_impactDist = GetDistance3D(posAtTMaxZ, startPosition);
			raycastResult.m_impactPos = posAtTMaxZ;
			raycastResult.m_impactNormal = Vec3(0.0f, 0.0f, 1.0f);
		}
		else
		{
			tMaxZ = 1.1f;
		}

		//check against cylinder disc
		Vec3 raycastVectorXY = GetProjectedOnto3D(raycastVector, Vec3(raycastVector.x, raycastVector.y, 0.0f));
		RaycastResult2D raycastResultDisc = RaycastVsDisc2D(Vec2(startPosition.x, startPosition.y), Vec2(directionNormal.x, directionNormal.y).GetNormalized(),
			raycastVectorXY.GetLength(), Vec2(cylinderCenter.x, cylinderCenter.y), cylinderRadius);
		if (raycastResultDisc.m_didImpact)
		{
			float tXY = (raycastResultDisc.m_impactPos.x - startPosition.x) / raycastVectorXY.x;
			Vec3 posAtTXY = startPosition + (raycastVector * tXY);
			if (posAtTXY.z > cylinderMinZ && posAtTXY.z < cylinderMaxZ && tXY < tMaxZ && tXY < tMinZ && tXY > 0.0f && tXY < 1.0f)
			{
				raycastResult.m_didImpact = true;
				raycastResult.m_impactDist = GetDistance3D(posAtTXY, startPosition);
				raycastResult.m_impactPos = posAtTXY;
				raycastResult.m_impactNormal = (Vec3(posAtTXY.x, posAtTXY.y, cylinderMinZ) - cylinderCenter).GetNormalized();
			}
		}
	}

	return raycastResult;
}


//
//conversion functions
//
float NormalizeByte(unsigned char byteValue)
{
	return RangeMap(static_cast<float>(byteValue), 0.0f, 255.0f, 0.0f, 1.0f);
}


unsigned char DenormalizeByte(float normalizedByte)
{
	float unnormalizedByte = normalizedByte * 256.0f;

	unnormalizedByte = GetClamped(unnormalizedByte, 0.0f, 255.0f);
	
	return static_cast<unsigned char>(unnormalizedByte);
}


//
//bezier curve functions
//
float ComputeCubicBezier1D(float a, float b, float c, float d, float t)
{
	float s = 1.0f - t;

	float ab = s * a + t * b;
	float bc = s * b + t * c;
	float cd = s * c + t * d;

	float abc = s * ab + t * bc;
	float bcd = s * bc + t * cd;

	return s * abc + t * bcd;
}


float ComputeQuinticBezier1D(float a, float b, float c, float d, float e, float f, float t)
{
	float s = 1.0f - t;

	float ab = s * a + t * b;
	float bc = s * b + t * c;
	float cd = s * c + t * d;
	float de = s * d + t * e;
	float ef = s * e + t * f;

	float abc = s * ab + t * bc;
	float bcd = s * bc + t * cd;
	float cde = s * cd + t * de;
	float def = s * de + t * ef;

	float abcd = s * abc + t * bcd;
	float bcde = s * bcd + t * cde;
	float cdef = s * cde + t * def;

	float abcde = s * abcd + t * bcde;
	float bcdef = s * bcde + t * cdef;

	return s * abcde + t * bcdef;
}


//
//easing functions;
//
float SmoothStart2(float t)
{
	return t * t;
}


float SmoothStart3(float t)
{
	return t * t * t;
}


float SmoothStart4(float t)
{
	return t * t * t * t;
}


float SmoothStart5(float t)
{
	return t * t * t * t * t;
}


float SmoothStart6(float t)
{
	return t * t * t * t * t * t;
}


float SmoothStop2(float t)
{
	float s = 1.0f - t;
	return 1.0f - s * s;
}


float SmoothStop3(float t)
{
	float s = 1.0f - t;
	return 1.0f - s * s * s;
}


float SmoothStop4(float t)
{
	float s = 1.0f - t;
	return 1.0f - s * s * s * s;
}


float SmoothStop5(float t)
{
	float s = 1.0f - t;
	return 1.0f - s * s * s * s * s;
}


float SmoothStop6(float t)
{
	float s = 1.0f - t;
	return 1.0f - s * s * s * s * s * s;
}


float SmoothStep3(float t)
{
	float s = 1.0f - t;

	return (s * t * t) + (t * (s * t + t * (s + t)));
}


float SmoothStep5(float t)
{
	float s = 1.0f - t;

	return s * (s * (t * t * t) + t * (s * t * t + t * (s * t + t * (s + t)))) + t * (s * (s * t * t + t * (s * t + t * (s + t))) + t * (s * (s * t + t * (s + t)) + t * (s * (s + t) + t * (s + t))));
}


float Hesitate3(float t)
{
	float s = 1.0f - t;

	return s * (s * t + t * s) + t * (s * s + t * t);
}


float Hesitate5(float t)
{
	float s = 1.0f - t;

	return s * (s * (s * (s * t + t * s) + t * (s * s + t * t)) + t * (s * (s * s + t * t) + t * (s * t + t * s))) + t * (s * (s * (s * s + t * t) + t * (s * t + t * s)) + t * (s * (s * t + t * s) + t * (s * s + t * t)));
}
