#pragma once
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/Mat44.hpp"
#include "Engine/Math/EulerAngles.hpp"


class Camera
{
	enum Mode
	{
		MODE_ORTHOGRAPHIC,
		MODE_PERSPECTIVE,
		
		MODE_COUNT
	};

//public member functions
public:
	//accessors
	Vec2  GetOrthoBottomLeft() const;
	Vec2  GetOrthoTopRight() const;
	float GetPerspectiveAspect() const { return m_perspectiveAspect; }
	float GetPerspectiveFOV() const { return m_perspectiveFOVDegrees; }
	float GetPerspectiveNear() const { return m_perspectiveNear; }
	float GetPerspectiveFar() const { return m_perspectiveFar; }
	AABB2 GetBounds() const;
	Mat44 GetOrthoMatrix() const;
	Mat44 GetPerspectiveMatrix() const;
	Mat44 GetProjectionMatrix() const;
	Mat44 GetRenderMatrix() const;
	Mat44 GetViewMatrix() const;
	Vec2  GetViewportOrigin() const;
	Vec2  GetViewportWidthHeight() const;
	Vec3  GetCameraPosition() const;
	EulerAngles GetCameraOrientation() const;

	//mutators
	void SetOrthoView(Vec2 const& bottomLeft, Vec2 const& topRight, float near = 0.0f, float far = 1.0f);
	void SetPerspectiveView(float aspect, float FOVDegrees, float near, float far);
	void SetFieldOfViewDegrees(float FOVDegrees);
	void SetRenderBasis(Vec3 const& iBasis, Vec3 const& jBasis, Vec3 const& kBasis);
	void SetTransform(Vec3 const& position, EulerAngles const& orientation);
	void SetTransform(Vec3 const& position, Mat44 const& orientation);
	void Translate2D(Vec2 const& translation);
	void SetViewport(Vec2 const& viewportOrigin, Vec2 const& viewportWidthHeight);
	void SetUseMatrixOrientationMode(bool useMatrix);

//private member variables
private:
	Mode m_mode = MODE_ORTHOGRAPHIC;

	Vec2 m_orthoBottomLeft;
	Vec2 m_orthoTopRight;
	float m_orthoNear;
	float m_orthoFar;

	float m_perspectiveAspect;
	float m_perspectiveFOVDegrees;
	float m_perspectiveNear;
	float m_perspectiveFar;

	Vec3 m_renderIBasis = Vec3(1.0f, 0.0f, 0.0f);
	Vec3 m_renderJBasis = Vec3(0.0f, 1.0f, 0.0f);
	Vec3 m_renderKBasis = Vec3(0.0f, 0.0f, 1.0f);

	Vec3 m_position;
	EulerAngles m_orientation;

	Vec2 m_viewportOrigin = Vec2(-1.0f, -1.0f);
	Vec2 m_viewportWidthHeight = Vec2(-1.0f, -1.0f);

	Mat44 m_orientationAsMatrix = Mat44();
	bool m_useMatrixAsOrientation = false;
};
