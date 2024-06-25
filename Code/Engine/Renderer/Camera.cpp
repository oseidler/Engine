#include "Engine/Renderer/Camera.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"


//
//accessors
//
Vec2 Camera::GetOrthoBottomLeft() const
{
	return m_orthoBottomLeft;
}


Vec2 Camera::GetOrthoTopRight() const
{
	return m_orthoTopRight;
}


AABB2 Camera::GetBounds() const
{
	return AABB2(m_orthoBottomLeft, m_orthoTopRight);
}


Mat44 Camera::GetOrthoMatrix() const
{
	return Mat44::CreateOrthoProjection(m_orthoBottomLeft.x, m_orthoTopRight.x, m_orthoBottomLeft.y, m_orthoTopRight.y, m_orthoNear, m_orthoFar);
}


Mat44 Camera::GetPerspectiveMatrix() const
{
	return Mat44::CreatePerspectiveProjection(m_perspectiveFOVDegrees, m_perspectiveAspect, m_perspectiveNear, m_perspectiveFar);
}


Mat44 Camera::GetProjectionMatrix() const
{
	Mat44 projectionMatrix;
	
	if (m_mode == MODE_ORTHOGRAPHIC)
	{
		projectionMatrix.Append(Mat44::CreateOrthoProjection(m_orthoBottomLeft.x, m_orthoTopRight.x, m_orthoBottomLeft.y, m_orthoTopRight.y, m_orthoNear, m_orthoFar));
		//projectionMatrix = Mat44::CreateOrthoProjection(m_orthoBottomLeft.x, m_orthoTopRight.x, m_orthoBottomLeft.y, m_orthoTopRight.y, m_orthoNear, m_orthoFar);
	}
	else if (m_mode == MODE_PERSPECTIVE)
	{
		projectionMatrix.Append(Mat44::CreatePerspectiveProjection(m_perspectiveFOVDegrees, m_perspectiveAspect, m_perspectiveNear, m_perspectiveFar));
		//projectionMatrix = Mat44::CreatePerspectiveProjection(m_perspectiveFOVDegrees, m_perspectiveAspect, m_perspectiveNear, m_perspectiveFar);
	}
	else
	{
		ERROR_AND_DIE("Projection mode is not set to orthographic or perpsective!");
	}

	//append render matrix
	projectionMatrix.Append(GetRenderMatrix());
	
	return projectionMatrix;
}


Mat44 Camera::GetRenderMatrix() const
{
	Mat44 renderMatrix;
	renderMatrix.SetIJK3D(m_renderIBasis, m_renderJBasis, m_renderKBasis);
	return renderMatrix;
}


Mat44 Camera::GetViewMatrix() const
{
	Mat44 viewMatrix = Mat44();
	
	if (m_useMatrixAsOrientation)
	{
		viewMatrix = m_orientationAsMatrix;
	}
	else
	{
		viewMatrix = m_orientation.GetAsMatrix_XFwd_YLeft_ZUp();
	}

	viewMatrix.SetTranslation3D(m_position);

	viewMatrix = viewMatrix.GetOrthonormalInverse();

	return viewMatrix;
}


Vec2 Camera::GetViewportOrigin() const
{
	return m_viewportOrigin;
}


Vec2 Camera::GetViewportWidthHeight() const
{
	return m_viewportWidthHeight;
}


Vec3 Camera::GetCameraPosition() const
{
	return m_position;
}


EulerAngles Camera::GetCameraOrientation() const
{
	return m_orientation;
}


//
//mutators
//
void Camera::SetOrthoView(Vec2 const& bottomLeft, Vec2 const& topRight, float near, float far)
{
	m_mode = MODE_ORTHOGRAPHIC;

	m_orthoBottomLeft = bottomLeft;
	m_orthoTopRight = topRight;
	m_orthoNear = near;
	m_orthoFar = far;
}


void Camera::SetPerspectiveView(float aspect, float FOVDegrees, float near, float far)
{
	m_mode = MODE_PERSPECTIVE;

	m_perspectiveAspect = aspect;
	m_perspectiveFOVDegrees = FOVDegrees;
	m_perspectiveNear = near;
	m_perspectiveFar = far;
}


void Camera::SetFieldOfViewDegrees(float FOVDegrees)
{
	m_perspectiveFOVDegrees = FOVDegrees;
}


void Camera::SetRenderBasis(Vec3 const& iBasis, Vec3 const& jBasis, Vec3 const& kBasis)
{
	m_renderIBasis = iBasis;
	m_renderJBasis = jBasis;
	m_renderKBasis = kBasis;
}


void Camera::SetTransform(Vec3 const& position, EulerAngles const& orientation)
{
	m_position = position;
	m_orientation = orientation;
}


void Camera::SetTransform(Vec3 const& position, Mat44 const& orientation)
{
	m_position = position;
	m_orientationAsMatrix = orientation;
}


void Camera::Translate2D(Vec2 const& translation)
{
	SetOrthoView(m_orthoBottomLeft + translation, m_orthoTopRight + translation);
}


void Camera::SetViewport(Vec2 const& viewportOrigin, Vec2 const& viewportWidthHeight)
{
	m_viewportOrigin = viewportOrigin;
	m_viewportWidthHeight = viewportWidthHeight;
}


void Camera::SetUseMatrixOrientationMode(bool useMatrix)
{
	m_useMatrixAsOrientation = useMatrix;
}
