#pragma once
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.hpp"


//forward declarations
struct Vec4;
struct IntVec2;
struct AABB2;
struct AABB3;
struct OBB2;
struct Mat44;
struct EulerAngles;
struct Quaternion;
struct Plane2D;
class  ConvexPoly2D;
class  ConvexHull2D;


//enums
enum class BillboardType
{
	NONE = -1,
	WORLD_UP_CAMERA_FACING,
	WORLD_UP_CAMERA_OPPOSING,
	FULL_CAMERA_FACING,
	FULL_CAMERA_OPPOSING,
	COUNT
};


//structs
struct RaycastResult2D
{
//public member functions
public:
	RaycastResult2D() {}

	RaycastResult2D(bool didImpact, float impactDist, Vec2 impactPos, Vec2 impactNormal)
		: m_didImpact(didImpact)
		, m_impactDist(impactDist)
		, m_impactPos(impactPos)
		, m_impactNormal(impactNormal)
	{
	}

//public member variables
public:
	bool	m_didImpact = false;
	float	m_impactDist = 0.f;
	Vec2	m_impactPos = Vec2(0.0f, 0.0f);
	Vec2	m_impactNormal = Vec2(0.0f, 0.0f);
};

struct RaycastResult3D
{
//public member variables
public:
	bool	m_didImpact = false;
	float	m_impactDist = 0.f;
	Vec3	m_impactPos = Vec3();
	Vec3	m_impactNormal = Vec3();
	Vec3	m_rayStartPosition = Vec3();
	Vec3	m_rayDirection = Vec3();
	float	m_rayLength = 0.0f;
};


//constants
constexpr float PI = 3.14159265358979f;

//angle utilities
float ConvertDegreesToRadians(float degrees);
float ConvertRadiansToDegrees(float radians);
float CosDegrees(float degrees);
float SinDegrees(float degrees);
float TanDegrees(float degrees);
float Atan2Degrees(float y, float x);
float AcosDegrees(float cosine);
float GetShortestAngularDispDegrees(float start, float end);
float GetTurnedTowardDegrees(float currentDegrees, float endDegrees, float maxDeltaDegrees);
float GetAngleDegreesBetweenVectors2D(Vec2 const& vectorA, Vec2 const& vectorB);
float GetAngleDegreesBetweenVectors3D(Vec3 const& vectorA, Vec3 const& vectorB);

//basic 2d and 3d utilities
float	    GetDistance2D(Vec2 const& positionA, Vec2 const& positionB);
float	    GetDistanceSquared2D(Vec2 const& positionA, Vec2 const& positionB);
float	    GetDistance3D(Vec3 const& positionA, Vec3 const& positionB);
float	    GetDistanceXY3D(Vec3 const& positionA, Vec3 const& positionB);
float	    GetDistanceSquared3D(Vec3 const& positionA, Vec3 const& positionB);
float	    GetDistanceXYSquared3D(Vec3 const& positionA, Vec3 const& positionB);
int	        GetTaxicabDistance2D(IntVec2 const& positionA, IntVec2 const& positionB);
float	    GetProjectedLength2D(Vec2 const& projectedVector, Vec2 const& vectorProjectedOnto);
Vec2 const  GetProjectedOnto2D(Vec2 const& projectedVector, Vec2 const& vectorProjectedOnto);
Vec3 const	GetProjectedOnto3D(Vec3 const& projectedVector, Vec3 const& vectorProjectedOnto);
Mat44 const GetBillboardMatrix(BillboardType billboardType, Mat44 const& cameraMatrix, Vec3 const& billboardPosition, Vec2 const& billboardScale = Vec2(1.0f, 1.0f));

//geometric query utilities
bool	   IsPointInsideDisc2D(Vec2 const& point, Vec2 const& discCenter, float discRadius);
bool	   IsPointInsideOrientedSector2D(Vec2 const& point, Vec2 const& sectorTip, float sectorForwardDegrees, float sectorApertureDegrees, float sectorRadius);
bool	   IsPointInsideDirectedSector2D(Vec2 const& point, Vec2 const& sectorTip, Vec2 const& sectorForwardDirection, float sectorApertureDegrees, float sectorRadius);
bool	   IsPointInsideAABB2D(Vec2 const& point, AABB2 const& aabb);
bool	   IsPointInsideOBB2D(Vec2 const& point, OBB2 const& obb);
bool	   IsPointInsideCapsule2D(Vec2 const& point, Vec2 const& boneStart, Vec2 const& boneEnd, float radius);
bool	   IsPointInsideConvexPoly2D(Vec2 const& point, ConvexPoly2D const& poly);
bool	   IsPointInsideConvexHull2D(Vec2 const& point, ConvexHull2D const& hull);

bool	   IsPointInsideSphere3D(Vec3 const& point, Vec3 const& sphereCenter, float sphereRadius);
bool	   IsPointInsideAABB3D(Vec3 const& point, AABB3 const& aabb);

bool	   DoDiscsOverlap(Vec2 const& centerA, float radiusA, Vec2 const& centerB, float radiusB);
bool	   DoSpheresOverlap(Vec3 const& centerA, float radiusA, Vec3 const& centerB, float radiusB);

Vec2 const GetNearestPointOnDisc2D(Vec2 const& referencePoint, Vec2 const& discCenter, float discRadius);
Vec2 const GetNearestPointOnDiscEdge2D(Vec2 const& referencePoint, Vec2 const& discCenter, float discRadius);
Vec2 const GetNearestPointOnAABB2D(Vec2 const& referencePoint, AABB2 const& aabb);
Vec2 const GetNearestPointOnOBB2D(Vec2 const& referencePoint, OBB2 const& obb);
Vec2 const GetNearestPointOnLineSegment(Vec2 const& referencePoint, Vec2 const& segmentStart, Vec2 const& segmentEnd);
Vec2 const GetNearestPointOnInfiniteLine2D(Vec2 const& referencePoint, Vec2 const& pointOnLineA, Vec2 const& pointOnLineB);
Vec2 const GetNearestPointOnCapsule2D(Vec2 const& referencePoint, Vec2 const& boneStart, Vec2 const& boneEnd, float radius);
Vec2 const GetNearestPointOnOrientedSector2D(Vec2 const& point, Vec2 const& sectorTip, float sectorForwardDegrees, float sectorApertureDegrees, float sectorRadius);

Vec3 const GetNearestPointOnLineSegment3D(Vec3 const& referencePoint, Vec3 const& segmentStart, Vec3 const& segmentEnd);
Vec3 const GetNearestPointOnCapsule3D(Vec3 const& referencePoint, Vec3 const& boneStart, Vec3 const& boneEnd, float radius);
Vec3 const GetNearestPointOnPlane3D(Vec3 const& referencePoint, Vec3 const& planeCenter, EulerAngles const& planeOrientation, float planeHalfLength, float planeHalfWidth);
Vec3 const GetNearestPointOnSphere3D(Vec3 const& referencePoint, Vec3 const& sphereCenter, float sphereRadius);
Vec3 const GetNearestPointOnSphereEdge3D(Vec3 const& referencePoint, Vec3 const& sphereCenter, float sphereRadius);
Vec3 const GetNearestPointOnAABB3D(Vec3 const& referencePoint, AABB3 const& aabb);
Vec3 const GetNearestPointOnEllipsoid3D(Vec3 const& referencePoint, Vec3 const& ellipsoidCenter, float ellipsoidXRadius, float ellipsoidYRadius, float ellipsoidZRadius,
	EulerAngles const& ellipsoidOrientation);
Vec3 const GetNearestPointOnRoundedCube3D(Vec3 const& referencePoint, Vec3 const& cubeCenter, float cubeLength, float cubeWidth, float cubeHeight, float cubeRoundedness,
	EulerAngles const& cubeOrientation);
Vec3 const GetNearestPointOnTorus3D(Vec3 const& referencePoint, Vec3 const& torusCenter, float torusTubeRadius, float torusHoleRadius, EulerAngles const& torusOrientation);
Vec3 const GetNearestPointOnCylinder3D(Vec3 const& referencePoint, Vec3 const& cylinderBase, Vec3 const& cylinderTop, float cylinderRadius);
Vec3 const GetNearestPointOnSector3D(Vec3 const& referencePoint, Vec3 const& sectorStart, Vec3 const& sectorEnd, float sectorRadius, float sectorForwardDegrees, float sectorApertureDegrees);
Vec3 const GetNearestPointOnTriangle3D(Vec3 const& referencePoint, Vec3 const& pointA, Vec3 const& pointB, Vec3 const& pointC);
//Vec3 const GetNearestPointOnMobiusStrip3D(Vec3 const& referencePoint, Vec3 const& stripCenter, float stripRadius, float stripHalfWidth, EulerAngles const& stripOrientation);

bool PushDiscOutOfFixedPoint2D(Vec2& discCenter, float discRadius, Vec2 const& fixedPoint);
bool PushDiscOutOfFixedPoint2D(Vec3& discCenter, float discRadius, Vec3 const& fixedPoint);
bool PushDiscOutOfFixedDisc2D(Vec2& mobileDiscCenter, float mobileDiscRadius, Vec2 const& fixedDiscCenter, float fixedDiscRadius);
bool PushDiscOutOfFixedDisc2D(Vec3& mobileDiscCenter, float mobileDiscRadius, Vec3 const& fixedDiscCenter, float fixedDiscRadius);
bool PushDiscOutOfFixedAABB2D(Vec2& discCenter, float discRadius, AABB2 const& fixedAABB2);
bool PushDiscOutOfFixedAABB2D(Vec3& discCenter, float discRadius, AABB2 const& fixedAABB2);
bool PushDiscsOutOfEachOther2D(Vec2& discCenterA, float discRadiusA, Vec2& discCenterB, float discRadiusB);
bool PushDiscsOutOfEachOther2D(Vec3& discCenterA, float discRadiusA, Vec3& discCenterB, float discRadiusB);
	 
bool BounceDiscsOffEachOther2D(Vec2& discCenterA, float discRadiusA, Vec2& discVelA, float discElasticA, Vec2& discCenterB, float discRadiusB, Vec2& discVelB, float discElasticB);
bool BounceDiscOffFixedDisc2D(Vec2& mobileDiscCenter, float mobileDiscRadius, Vec2& mobileDiscVel, float mobileDiscElastic,
		Vec2 const& fixedDiscCenter, float fixedDiscRadius, float fixedDiscElastic);
bool BounceDiscOffFixedCapsule2D(Vec2& discCenter, float discRadius, Vec2& discVel, float discElastic, Vec2 const& capsuleBoneStart, Vec2 const& capsuleBoneEnd, float capsuleRadius, 
	 	float capsuleElastic);
bool BounceDiscOffFixedOBB2D(Vec2& discCenter, float discRadius, Vec2& discVel, float discElastic, OBB2 const& obb, float obbElastic);
	 
bool PushSphereOutOfFixedPoint3D(Vec3& sphereCenter, float sphereRadius, Vec3 const& fixedPoint);
bool IsSphereInFixedPoint3D(Vec3 const& sphereCenter, float sphereRadius, Vec3 const& fixedPoint);
bool PushSphereOutOfPlane3D(Vec3& sphereCenter, float sphereRadius, Vec3 const& planeCenter, EulerAngles const& planeOrientation, float planeHalfLength, float planeHalfWidth);
bool PushSphereOutOfFixedSphere3D(Vec3& mobileSphereCenter, float mobileSphereRadius, Vec3 const& fixedSphereCenter, float fixedSphereRadius);
bool PushSphereOutOfFixedCapsule3D(Vec3& sphereCenter, float sphereRadius, Vec3 const& boneStart, Vec3 const& boneEnd, float capsuleRadius);
bool PushSphereOutOfFixedEllipsoid3D(Vec3& sphereCenter, float sphereRadius, Vec3 const& ellipsoidCenter, float ellipsoidXRadius, float ellipsoidYRadius, float ellipsoidZRadius, 
	EulerAngles const& ellipsoidOrientation);
bool PushSphereOutOfFixedRoundedCube3D(Vec3& sphereCenter, float sphereRadius, Vec3 const& cubeCenter, float cubeLength, float cubeWidth, float cubeHeight, float cubeRoundedness,
	EulerAngles const& cubeOrientation);
bool PushSphereOutOfFixedTorus3D(Vec3& sphereCenter, float sphereRadius, Vec3 const& torusCenter, float torusTubeRadius, float torusHoleRadius, EulerAngles const& torusOrientation);
bool PushSphereOutOfFixedSector3D(Vec3& sphereCenter, float sphereRadius, Vec3 const& sectorStart, Vec3 const& sectorEnd, float sectorRadius, float sectorForwardDegrees, float sectorApertureDegrees);
//bool PushSphereOutOfFixedMobiusStrip3D(Vec3& sphereCenter, float sphereRadius, Vec3 const& stripCenter, float stripRadius, float stripHalfWidth, EulerAngles const& stripOrientation);

//transform utilities
void TransformPosition2D(Vec2& posToTransform, float uniformScale, float rotationDegrees, Vec2 const& translation);
void TransformPosition2D(Vec2& posToTransform, Vec2 const& iBasis, Vec2 const& jBasis, Vec2 const& translation);
void TransformPositionXY3D(Vec3& posToTransform, float uniformScale, float rotationDegrees, Vec2 const& translation);
void TransformPositionXY3D(Vec3& posToTransform, Vec2 const& iBasis, Vec2 const& jBasis, Vec2 const& translation);

//clamp and lerp
float Interpolate(float start, float end, float fraction);
float GetFractionWithinRange(float value, float start, float end);
float RangeMap(float value, float inStart, float inEnd, float outStart, float outEnd);
float RangeMapClamped(float value, float inStart, float inEnd, float outStart, float outEnd);
float GetClamped(float value, float min, float max);
int	  GetClamped(int value, int min, int max);
float GetClampedZeroToOne(float value);
int	  RoundDownToInt(float value);
Quaternion Slerp(Quaternion start, Quaternion end, float fraction);

//dot and cross product
float DotProduct2D(Vec2 const& a, Vec2 const& b);
float DotProduct3D(Vec3 const& a, Vec3 const& b);
float DotProduct4D(Vec4 const& a, Vec4 const& b);
float CrossProduct2D(Vec2 const& a, Vec2 const& b);
Vec3  CrossProduct3D(Vec3 const& a, Vec3 const& b);

//raycasting
RaycastResult2D RaycastVsDisc2D(Vec2 const& startPosition, Vec2 const& directionNormal, float maxDistance, Vec2 const& discCenter, float discRadius);
RaycastResult2D RaycastVsLineSegment2D(Vec2 const& startPosition, Vec2 const& directionNormal, float maxDistance, Vec2 const& pointA, Vec2 const& pointB);
RaycastResult2D RaycastVsAABB2D(Vec2 const& startPosition, Vec2 const& directionNormal, float maxDistance, AABB2 const& aabb2);
RaycastResult2D RaycastVsPlane2D(Vec2 const& startPosition, Vec2 const& directionNormal, float maxDistance, Plane2D const& plane);
RaycastResult2D RaycastVsConvexHull2D(Vec2 const& startPosition, Vec2 const& directionNormal, float maxDistance, ConvexHull2D const& convexHull);
RaycastResult3D RaycastVsZCylinder3D(Vec3 const& startPosition, Vec3 const& directionNormal, float maxDistance, Vec3 const& cylinderCenter, float cylinderMinZ, float cylinderMaxZ, float cylinderRadius);

//conversion functions
float		  NormalizeByte(unsigned char byteValue);
unsigned char DenormalizeByte(float normalizedByte);

//bezier curve functions
float ComputeCubicBezier1D(float a, float b, float c, float d, float t);
float ComputeQuinticBezier1D(float a, float b, float c, float d, float e, float f, float t);

//easing functions
float SmoothStart2(float t);
float SmoothStart3(float t);
float SmoothStart4(float t);
float SmoothStart5(float t);
float SmoothStart6(float t);
float SmoothStop2(float t);
float SmoothStop3(float t);
float SmoothStop4(float t);
float SmoothStop5(float t);
float SmoothStop6(float t);
float SmoothStep3(float t);
float SmoothStep5(float t);
float Hesitate3(float t);
float Hesitate5(float t);
