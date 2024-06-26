#pragma once


//forward declarations
struct Vec2;
struct Vec3;
struct Vec4;
struct Quaternion;


struct Mat44
{
	enum {Ix, Iy, Iz, Iw,   Jx, Jy, Jz, Jw,   Kx, Ky, Kz, Kw,   Tx, Ty, Tz, Tw}; //enum to associate idex with notation
	
//public member variables
public:
	float m_values[16];	//stored in column major notation (Ix, Iy, Iz, Iw, Jx, Jy...)

//public member functions
public:
	//constructors
	Mat44();
	explicit Mat44(Vec2 const& iBasis2D, Vec2 const& jBasis2D, Vec2 const& translation2D);
	explicit Mat44(Vec3 const& iBasis3D, Vec3 const& jBasis3D, Vec3 const& kBasis3D, Vec3 const& translation3D);
	explicit Mat44(Vec4 const& iBasis4D, Vec4 const& jBasis4D, Vec4 const& kBasis4D, Vec4 const& translation4D);
	explicit Mat44(float const* sixteenValuesBasisMajor);

	//static creation functions
	static Mat44 const CreateTranslation2D(Vec2 const& translationXY);
	static Mat44 const CreateTranslation3D(Vec3 const& translationXYZ);
	static Mat44 const CreateUniformScale2D(float uniformScaleXY);
	static Mat44 const CreateUniformScale3D(float uniformScaleXYZ);
	static Mat44 const CreateNonUniformScale2D(Vec2 const& nonUniformScaleXY);
	static Mat44 const CreateNonUniformScale3D(Vec3 const& nonUniformScaleXYZ);
	static Mat44 const CreateZRotationDegrees(float rotationDegreesAboutZ);
	static Mat44 const CreateYRotationDegrees(float rotationDegreesAboutY);
	static Mat44 const CreateXRotationDegrees(float rotationDegreesAboutX);
	static Mat44 const CreateOrthoProjection(float left, float right, float bottom, float top, float zNear, float zFar);
	static Mat44 const CreatePerspectiveProjection(float fovYDegrees, float aspect, float zNear, float zFar);

	//transform functions
	Vec2 const TransformVectorQuantity2D(Vec2 const& vectorQuantityXY) const;
	Vec3 const TransformVectorQuantity3D(Vec3 const& vectorQuantityXYZ) const;
	Vec2 const TransformPosition2D(Vec2 const& positionXY) const;
	Vec3 const TransformPosition3D(Vec3 const& positionXYZ) const;
	Vec4 const TransformHomogeneous3D(Vec4 const& homogeneousPoint3D) const;

	//get functions (accessors)
	float*		 GetAsFloatArray();
	float const* GetAsFloatArray() const;
	Quaternion const GetAsQuaternion() const;
	Vec2 const	 GetIBasis2D() const;
	Vec2 const	 GetJBasis2D() const;
	Vec2 const	 GetTranslation2D() const;
	Vec3 const	 GetIBasis3D() const;
	Vec3 const	 GetJBasis3D() const;
	Vec3 const	 GetKBasis3D() const;
	Vec3 const	 GetTranslation3D() const;
	Vec4 const	 GetIBasis4D() const;
	Vec4 const	 GetJBasis4D() const;
	Vec4 const	 GetKBasis4D() const;
	Vec4 const	 GetTranslation4D() const;
	Mat44 const  GetOrthonormalInverse() const;

	//set functions (mutators)
	void SetTranslation2D(Vec2 const& translationXY);
	void SetTranslation3D(Vec3 const& translationXYZ);
	void SetIJ2D(Vec2 const& iBasis2D, Vec2 const& jBasis2D);
	void SetIJT2D(Vec2 const& iBasis2D, Vec2 const& jBasis2D, Vec2 const& translationXY);
	void SetIJK3D(Vec3 const& iBasis3D, Vec3 const& jBasis3D, Vec3 const& kBasis3D);
	void SetIJKT3D(Vec3 const& iBasis3D, Vec3 const& jBasis3D, Vec3 const& kBasis3D, Vec3 const& translationXYZ);
	void SetIJKT4D(Vec4 const& iBasis4D, Vec4 const& jBasis4D, Vec4 const& kBasis4D, Vec4 const& translation4D);
	void Transpose();
	void Orthonormalize_XFwd_YLeft_ZUp();
	
	//append functions
	void Append(Mat44 const& matToAppend);
	void AppendZRotation(float degreesRotationAboutZ);
	void AppendYRotation(float degreesRotationAboutY);
	void AppendXRotation(float degreesRotationAboutX);
	void AppendTranslation2D(Vec2 const& translationXY);
	void AppendTranslation3D(Vec3 const& translationXYZ);
	void AppendScaleUniform2D(float uniformScaleXY);
	void AppendScaleUniform3D(float uniformScaleXYZ);
	void AppendScaleNonUniform2D(Vec2 const& nonUniformScaleXY);
	void AppendScaleNonUniform3D(Vec3 const& nonUniformScaleXYZ);
};
