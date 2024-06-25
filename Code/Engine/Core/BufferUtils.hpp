#pragma once
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Math/Plane2D.hpp"
#include "Engine/Math/Plane3D.hpp"
#include "Engine/Math/Vec4.hpp"
#include "Engine/Math/IntVec3.hpp"
#include "Engine/Math/AABB3.hpp"
#include "Engine/Math/OBB2.hpp"
#include "Engine/Core/Vertex_PCUTBN.hpp"


enum class BufferEndian
{
	NATIVE,
	LITTLE,
	BIG
};


class BufferWriter
{
//public member functions
public:
	BufferWriter(std::vector<unsigned char>& buffer) 
		: m_buffer(buffer) {}

	void SetEndianMode(BufferEndian endianMode);
	BufferEndian GetEndianMode() { return m_endianMode; }
	BufferEndian GetPlatformNativeEndian();

	void Reverse2Bytes(unsigned char* bytePointer);
	void Reverse4Bytes(unsigned char* bytePointer);
	void Reverse8Bytes(unsigned char* bytePointer);

	void AppendByte(unsigned char byte);
	void AppendChar(char character);
	void AppendUShort(unsigned short uShort16);
	void AppendShort(short short16);
	void AppendUInt32(unsigned int uInt32);
	void AppendInt32(int int32);
	void AppendUInt64(uint64_t uInt64);
	void AppendInt64(int64_t int64);
	void AppendFloat(float float32);
	void AppendDouble(double double64);

	void AppendStringWithNullTerminator(std::string const& string);
	void AppendStringAfterLength(std::string const& string);
	
	void AppendVec2(Vec2 const& vec2);
	void AppendVec3(Vec3 const& vec3);
	void AppendVec4(Vec4 const& vec4);
	void AppendIntVec2(IntVec2 const& intVec2);
	void AppendIntVec3(IntVec3 const& intVec3);
	void AppendAABB2(AABB2 const& aabb2);
	void AppendAABB3(AABB3 const& aabb3);
	void AppendOBB2(OBB2 const& obb2);
	void AppendRgba8(Rgba8 const& color);
	void AppendRgb8(Rgba8 const& color);
	void AppendVertexPCU(Vertex_PCU const& vertex);
	void AppendVertexPCUTBN(Vertex_PCUTBN const& vertex);
	void AppendPlane2D(Plane2D const& plane2D);
	void AppendPlane3D(Plane3D const& plane3D);

	void OverwriteUInt32(size_t writeOffset, uint32_t uInt32);

//private member variables
private:
	std::vector<unsigned char>& m_buffer;
	BufferEndian m_endianMode = BufferEndian::NATIVE;
	bool m_isOppositeNativeEndian = false;
};


class BufferParser
{
//public member functions
public:
	BufferParser(uint8_t const* bufferPointer, size_t bufferSize)
		: m_bufferStart(bufferPointer), m_bufferSize(bufferSize) {}
	BufferParser(std::vector<unsigned char> const& buffer)
		:BufferParser(buffer.data(), buffer.size()) {}

	void SetEndianMode(BufferEndian endianMode);
	BufferEndian GetEndianMode() { return m_endianMode; }
	BufferEndian GetPlatformNativeEndian();

	void Reverse2Bytes(unsigned char* bytePointer);
	void Reverse4Bytes(unsigned char* bytePointer);
	void Reverse8Bytes(unsigned char* bytePointer);

	unsigned char  ParseByte();
	char		   ParseChar();
	unsigned short ParseUShort();
	short		   ParseShort();
	unsigned int   ParseUInt32();
	int			   ParseInt32();
	uint64_t	   ParseUInt64();
	int64_t		   ParseInt64();
	float		   ParseFloat();
	double		   ParseDouble();

	std::string const ParseStringWithNullTerminator();
	std::string const ParseStringAfterLength();

	Vec2 const			ParseVec2();
	Vec3 const			ParseVec3();
	Vec4 const			ParseVec4();
	IntVec2 const		ParseIntVec2();
	IntVec3 const		ParseIntVec3();
	AABB2 const			ParseAABB2();
	AABB3 const			ParseAABB3();
	OBB2 const			ParseOBB2();
	Rgba8 const			ParseRgba8();
	Rgba8 const			ParseRgb8();
	Vertex_PCU const	ParseVertexPCU();
	Vertex_PCUTBN const ParseVertexPCUTBN();
	Plane2D const		ParsePlane2D();
	Plane3D const		ParsePlane3D();

	void SetOffset(size_t newOffset);
	size_t GetOffset() { return m_currentOffset; }

//private member variables
private:
	unsigned char const* m_bufferStart = nullptr;
	size_t m_bufferSize = 0;
	size_t m_currentOffset = 0;
	BufferEndian m_endianMode = BufferEndian::NATIVE;
	bool m_isOppositeNativeEndian = false;
};
