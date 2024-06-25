#include "Engine/Core/BufferUtils.hpp"


//
//buffer writer functions
//
void BufferWriter::SetEndianMode(BufferEndian endianMode)
{
	//compare incoming endian mode to platform native one; if not native then set isOpposite to true
	BufferEndian nativeEndian = GetPlatformNativeEndian();

	if (endianMode == BufferEndian::NATIVE)
	{
		m_endianMode = nativeEndian;
		m_isOppositeNativeEndian = false;
	}
	else
	{
		m_endianMode = endianMode;

		if (m_endianMode != nativeEndian)
		{
			m_isOppositeNativeEndian = true;
		}
		else
		{
			m_isOppositeNativeEndian = false;
		}
	}
}


BufferEndian BufferWriter::GetPlatformNativeEndian()
{
	//check first byte of 0x12345678 to determine if big or little
	uint32_t orderedInt = 0x12345678;
	unsigned char* intAsBytes = reinterpret_cast<unsigned char*>(&orderedInt);

	if (intAsBytes[0] == 0x12)
	{
		return BufferEndian::BIG;
	}
	else if (intAsBytes[0] == 0x78)
	{
		return BufferEndian::LITTLE;
	}
	else
	{
		ERROR_AND_DIE("Native endian mode was neither little nor big!");
	}
}


void BufferWriter::Reverse2Bytes(unsigned char* bytePointer)
{
	uint16_t originalBytes = *reinterpret_cast<uint16_t*>(bytePointer);

	*reinterpret_cast<uint16_t*>(bytePointer) = ((originalBytes & 0x00ff) << 8 | (originalBytes & 0xff00) >> 8);
}


void BufferWriter::Reverse4Bytes(unsigned char* bytePointer)
{
	uint32_t originalBytes = *reinterpret_cast<uint32_t*>(bytePointer);

	*reinterpret_cast<uint32_t*>(bytePointer) = ((originalBytes & 0x000000ff) << 24 | (originalBytes & 0x0000ff00) << 8 | (originalBytes & 0x00ff0000) >> 8 
		| (originalBytes & 0xff000000) >> 24);
}


void BufferWriter::Reverse8Bytes(unsigned char* bytePointer)
{
	uint64_t originalBytes = *reinterpret_cast<uint64_t*>(bytePointer);

	*reinterpret_cast<uint64_t*>(bytePointer) = ((originalBytes & 0x00000000000000ff) << 56 | (originalBytes & 0x000000000000ff00) << 40 | (originalBytes & 0x0000000000ff0000) << 24
		| (originalBytes & 0x00000000ff000000) << 8 | (originalBytes & 0x000000ff00000000) >> 8 | (originalBytes & 0x0000ff0000000000) >> 24 | (originalBytes & 0x00ff000000000000) >> 40
		| (originalBytes & 0xff00000000000000) >> 56);
}


void BufferWriter::AppendByte(unsigned char byte)
{
	m_buffer.emplace_back(byte);
}


void BufferWriter::AppendChar(char character)
{
	m_buffer.emplace_back(character);
}


void BufferWriter::AppendUShort(unsigned short uShort16)
{
	unsigned short* uShortPointer = &uShort16;
	unsigned char* bytePointer = reinterpret_cast<unsigned char*>(uShortPointer);
	if (m_isOppositeNativeEndian)
	{
		Reverse2Bytes(bytePointer);
	}
	AppendByte(bytePointer[0]);
	AppendByte(bytePointer[1]);
}


void BufferWriter::AppendShort(short short16)
{
	short* shortPointer = &short16;
	unsigned char* bytePointer = reinterpret_cast<unsigned char*>(shortPointer);
	if (m_isOppositeNativeEndian)
	{
		Reverse2Bytes(bytePointer);
	}
	AppendByte(bytePointer[0]);
	AppendByte(bytePointer[1]);
}


void BufferWriter::AppendUInt32(unsigned int uInt32)
{
	unsigned int* uIntPointer = &uInt32;
	unsigned char* bytePointer = reinterpret_cast<unsigned char*>(uIntPointer);
	if (m_isOppositeNativeEndian)
	{
		Reverse4Bytes(bytePointer);
	}
	AppendByte(bytePointer[0]);
	AppendByte(bytePointer[1]);
	AppendByte(bytePointer[2]);
	AppendByte(bytePointer[3]);
}


void BufferWriter::AppendInt32(int int32)
{
	int* intPointer = &int32;
	unsigned char* bytePointer = reinterpret_cast<unsigned char*>(intPointer);
	if (m_isOppositeNativeEndian)
	{
		Reverse4Bytes(bytePointer);
	}
	AppendByte(bytePointer[0]);
	AppendByte(bytePointer[1]);
	AppendByte(bytePointer[2]);
	AppendByte(bytePointer[3]);
}


void BufferWriter::AppendUInt64(uint64_t uInt64)
{
	uint64_t* uInt64Pointer = &uInt64;
	unsigned char* bytePointer = reinterpret_cast<unsigned char*>(uInt64Pointer);
	if (m_isOppositeNativeEndian)
	{
		Reverse8Bytes(bytePointer);
	}
	AppendByte(bytePointer[0]);
	AppendByte(bytePointer[1]);
	AppendByte(bytePointer[2]);
	AppendByte(bytePointer[3]);
	AppendByte(bytePointer[4]);
	AppendByte(bytePointer[5]);
	AppendByte(bytePointer[6]);
	AppendByte(bytePointer[7]);
}


void BufferWriter::AppendInt64(int64_t int64)
{
	int64_t* int64Pointer = &int64;
	unsigned char* bytePointer = reinterpret_cast<unsigned char*>(int64Pointer);
	if (m_isOppositeNativeEndian)
	{
		Reverse8Bytes(bytePointer);
	}
	AppendByte(bytePointer[0]);
	AppendByte(bytePointer[1]);
	AppendByte(bytePointer[2]);
	AppendByte(bytePointer[3]);
	AppendByte(bytePointer[4]);
	AppendByte(bytePointer[5]);
	AppendByte(bytePointer[6]);
	AppendByte(bytePointer[7]);
}


void BufferWriter::AppendFloat(float float32)
{
	float* floatPointer = &float32;
	unsigned char* bytePointer = reinterpret_cast<unsigned char*>(floatPointer);
	if (m_isOppositeNativeEndian)
	{
		Reverse4Bytes(bytePointer);
	}
	AppendByte(bytePointer[0]);
	AppendByte(bytePointer[1]);
	AppendByte(bytePointer[2]);
	AppendByte(bytePointer[3]);
}


void BufferWriter::AppendDouble(double double64)
{
	double* doublePointer = &double64;
	unsigned char* bytePointer = reinterpret_cast<unsigned char*>(doublePointer);
	if (m_isOppositeNativeEndian)
	{
		Reverse8Bytes(bytePointer);
	}
	AppendByte(bytePointer[0]);
	AppendByte(bytePointer[1]);
	AppendByte(bytePointer[2]);
	AppendByte(bytePointer[3]);
	AppendByte(bytePointer[4]);
	AppendByte(bytePointer[5]);
	AppendByte(bytePointer[6]);
	AppendByte(bytePointer[7]);
}


void BufferWriter::AppendStringWithNullTerminator(std::string const& string)
{
	for (int charIndex = 0; charIndex < string.length(); charIndex++)
	{
		AppendChar(string[charIndex]);
	}
	AppendChar('\0');
}


void BufferWriter::AppendStringAfterLength(std::string const& string)
{
	AppendUInt32(static_cast<uint32_t>(string.length()));
	for (int charIndex = 0; charIndex < string.length(); charIndex++)
	{
		AppendChar(string[charIndex]);
	}
}


void BufferWriter::AppendVec2(Vec2 const& vec2)
{
	AppendFloat(vec2.x);
	AppendFloat(vec2.y);
}


void BufferWriter::AppendVec3(Vec3 const& vec3)
{
	AppendFloat(vec3.x);
	AppendFloat(vec3.y);
	AppendFloat(vec3.z);
}


void BufferWriter::AppendVec4(Vec4 const& vec4)
{
	AppendFloat(vec4.x);
	AppendFloat(vec4.y);
	AppendFloat(vec4.z);
	AppendFloat(vec4.w);
}


void BufferWriter::AppendIntVec2(IntVec2 const& intVec2)
{
	AppendInt32(intVec2.x);
	AppendInt32(intVec2.y);
}


void BufferWriter::AppendIntVec3(IntVec3 const& intVec3)
{
	AppendInt32(intVec3.x);
	AppendInt32(intVec3.y);
	AppendInt32(intVec3.z);
}


void BufferWriter::AppendAABB2(AABB2 const& aabb2)
{
	AppendVec2(aabb2.m_mins);
	AppendVec2(aabb2.m_maxs);
}


void BufferWriter::AppendAABB3(AABB3 const& aabb3)
{
	AppendVec3(aabb3.m_mins);
	AppendVec3(aabb3.m_maxs);
}


void BufferWriter::AppendOBB2(OBB2 const& obb2)
{
	AppendVec2(obb2.m_center);
	AppendVec2(obb2.m_iBasisNormal);
	AppendVec2(obb2.m_halfDimensions);
}


void BufferWriter::AppendRgba8(Rgba8 const& color)
{
	AppendByte(color.r);
	AppendByte(color.g);
	AppendByte(color.b);
	AppendByte(color.a);
}


void BufferWriter::AppendRgb8(Rgba8 const& color)
{
	AppendByte(color.r);
	AppendByte(color.g);
	AppendByte(color.b);
}


void BufferWriter::AppendVertexPCU(Vertex_PCU const& vertex)
{
	AppendVec3(vertex.m_position);
	AppendRgba8(vertex.m_color);
	AppendVec2(vertex.m_uvTexCoords);
}


void BufferWriter::AppendVertexPCUTBN(Vertex_PCUTBN const& vertex)
{
	AppendVec3(vertex.m_position);
	AppendRgba8(vertex.m_color);
	AppendVec2(vertex.m_uvTexCoords);
	AppendVec3(vertex.m_tangent);
	AppendVec3(vertex.m_bitangent);
	AppendVec3(vertex.m_normal);
}


void BufferWriter::AppendPlane2D(Plane2D const& plane2D)
{
	AppendVec2(plane2D.m_normal);
	AppendFloat(plane2D.m_distFromOrigin);
}


void BufferWriter::AppendPlane3D(Plane3D const& plane3D)
{
	AppendVec3(plane3D.m_normal);
	AppendFloat(plane3D.m_distFromOrigin);
}


void BufferWriter::OverwriteUInt32(size_t writeOffset, uint32_t uInt32)
{
	uint32_t* dataToOverwrite = reinterpret_cast<uint32_t*>(&m_buffer[writeOffset]);
	*dataToOverwrite = uInt32;
}


//
//buffer parser functions
//
void BufferParser::SetEndianMode(BufferEndian endianMode)
{
	//compare incoming endian mode to platform native one; if not native then set isOpposite to true
	BufferEndian nativeEndian = GetPlatformNativeEndian();

	if (endianMode == BufferEndian::NATIVE)
	{
		m_endianMode = nativeEndian;
		m_isOppositeNativeEndian = false;
	}
	else
	{
		m_endianMode = endianMode;

		if (m_endianMode != nativeEndian)
		{
			m_isOppositeNativeEndian = true;
		}
		else
		{
			m_isOppositeNativeEndian = false;
		}
	}
}


BufferEndian BufferParser::GetPlatformNativeEndian()
{
	//check first byte of 0x12345678 to determine if big or little
	uint32_t orderedInt = 0x12345678;
	unsigned char* intAsBytes = reinterpret_cast<unsigned char*>(&orderedInt);

	if (intAsBytes[0] == 0x12)
	{
		return BufferEndian::BIG;
	}
	else if (intAsBytes[0] == 0x78)
	{
		return BufferEndian::LITTLE;
	}
	else
	{
		ERROR_AND_DIE("Native endian mode was neither little nor big!");
	}
}


void BufferParser::Reverse2Bytes(unsigned char* bytePointer)
{
	uint16_t originalBytes = *reinterpret_cast<uint16_t*>(bytePointer);

	*reinterpret_cast<uint16_t*>(bytePointer) = ((originalBytes & 0x00ff) << 8 | (originalBytes & 0xff00) >> 8);
}


void BufferParser::Reverse4Bytes(unsigned char* bytePointer)
{
	uint32_t originalBytes = *reinterpret_cast<uint32_t*>(bytePointer);

	*reinterpret_cast<uint32_t*>(bytePointer) = ((originalBytes & 0x000000ff) << 24 | (originalBytes & 0x0000ff00) << 8 | (originalBytes & 0x00ff0000) >> 8 
		| (originalBytes & 0xff000000) >> 24);
}


void BufferParser::Reverse8Bytes(unsigned char* bytePointer)
{
	uint64_t originalBytes = *reinterpret_cast<uint64_t*>(bytePointer);

	*reinterpret_cast<uint64_t*>(bytePointer) = ((originalBytes & 0x00000000000000ff) << 56 | (originalBytes & 0x000000000000ff00) << 40 | (originalBytes & 0x0000000000ff0000) << 24
		| (originalBytes & 0x00000000ff000000) << 8 | (originalBytes & 0x000000ff00000000) >> 8 | (originalBytes & 0x0000ff0000000000) >> 24 | (originalBytes & 0x00ff000000000000) >> 40
		| (originalBytes & 0xff00000000000000) >> 56);
}


unsigned char BufferParser::ParseByte()
{
	if (m_currentOffset >= m_bufferSize) ERROR_AND_DIE("Attempting to read past size of buffer!");

	return m_bufferStart[m_currentOffset++];
}


char BufferParser::ParseChar()
{
	if (m_currentOffset >= m_bufferSize) ERROR_AND_DIE("Attempting to read past size of buffer!");
	
	return static_cast<char>(m_bufferStart[m_currentOffset++]);
}


unsigned short BufferParser::ParseUShort()
{
	if (m_currentOffset + sizeof(unsigned short) - 1 >= m_bufferSize) ERROR_AND_DIE("Attempting to read past size of buffer!");

	unsigned char const* bytePointer = &m_bufferStart[m_currentOffset];
	unsigned short const* uShortPointer = reinterpret_cast<unsigned short const*>(bytePointer);
	m_currentOffset += sizeof(unsigned short);

	unsigned short parsedUShort = *uShortPointer;
	if (m_isOppositeNativeEndian)
	{
		Reverse2Bytes(reinterpret_cast<unsigned char*>(&parsedUShort));
	}

	return parsedUShort;
}


short BufferParser::ParseShort()
{
	if (m_currentOffset + sizeof(short) - 1 >= m_bufferSize) ERROR_AND_DIE("Attempting to read past size of buffer!");

	unsigned char const* bytePointer = &m_bufferStart[m_currentOffset];
	short const* shortPointer = reinterpret_cast<short const*>(bytePointer);
	m_currentOffset += sizeof(short);

	short parsedShort = *shortPointer;
	if (m_isOppositeNativeEndian)
	{
		Reverse2Bytes(reinterpret_cast<unsigned char*>(&parsedShort));
	}

	return parsedShort;
}


unsigned int BufferParser::ParseUInt32()
{
	if (m_currentOffset + sizeof(unsigned int) - 1 >= m_bufferSize) ERROR_AND_DIE("Attempting to read past size of buffer!");

	unsigned char const* bytePointer = &m_bufferStart[m_currentOffset];
	unsigned int const* uIntPointer = reinterpret_cast<unsigned int const*>(bytePointer);
	m_currentOffset += sizeof(unsigned int);

	unsigned int parsedUInt = *uIntPointer;
	if (m_isOppositeNativeEndian)
	{
		Reverse4Bytes(reinterpret_cast<unsigned char*>(&parsedUInt));
	}

	return parsedUInt;
}


int BufferParser::ParseInt32()
{
	if (m_currentOffset + sizeof(int) - 1 >= m_bufferSize) ERROR_AND_DIE("Attempting to read past size of buffer!");

	unsigned char const* bytePointer = &m_bufferStart[m_currentOffset];
	int const* intPointer = reinterpret_cast<int const*>(bytePointer);
	m_currentOffset += sizeof(int);

	int parsedInt = *intPointer;
	if (m_isOppositeNativeEndian)
	{
		Reverse4Bytes(reinterpret_cast<unsigned char*>(&parsedInt));
	}

	return parsedInt;
}


uint64_t BufferParser::ParseUInt64()
{
	if (m_currentOffset + sizeof(uint64_t) - 1 >= m_bufferSize) ERROR_AND_DIE("Attempting to read past size of buffer!");

	unsigned char const* bytePointer = &m_bufferStart[m_currentOffset];
	uint64_t const* uInt64Pointer = reinterpret_cast<uint64_t const*>(bytePointer);
	m_currentOffset += sizeof(uint64_t);

	uint64_t parsedUInt64 = *uInt64Pointer;
	if (m_isOppositeNativeEndian)
	{
		Reverse8Bytes(reinterpret_cast<unsigned char*>(&parsedUInt64));
	}

	return parsedUInt64;
}


int64_t BufferParser::ParseInt64()
{
	if (m_currentOffset + sizeof(int64_t) - 1 >= m_bufferSize) ERROR_AND_DIE("Attempting to read past size of buffer!");

	unsigned char const* bytePointer = &m_bufferStart[m_currentOffset];
	int64_t const* int64Pointer = reinterpret_cast<int64_t const*>(bytePointer);
	m_currentOffset += sizeof(int64_t);

	int64_t parsedInt64 = *int64Pointer;
	if (m_isOppositeNativeEndian)
	{
		Reverse8Bytes(reinterpret_cast<unsigned char*>(&parsedInt64));
	}

	return parsedInt64;
}


float BufferParser::ParseFloat()
{
	if (m_currentOffset + sizeof(float)-1 >= m_bufferSize) ERROR_AND_DIE("Attempting to read past size of buffer!");

	unsigned char const* bytePointer = &m_bufferStart[m_currentOffset];
	float const* floatPointer = reinterpret_cast<float const*>(bytePointer);
	m_currentOffset += sizeof(float);

	float parsedFloat = *floatPointer;
	if (m_isOppositeNativeEndian)
	{
		Reverse4Bytes(reinterpret_cast<unsigned char*>(&parsedFloat));
	}

	return parsedFloat;
}


double BufferParser::ParseDouble()
{
	if (m_currentOffset + sizeof(double) - 1 >= m_bufferSize) ERROR_AND_DIE("Attempting to read past size of buffer!");

	unsigned char const* bytePointer = &m_bufferStart[m_currentOffset];
	double const* doublePointer = reinterpret_cast<double const*>(bytePointer);
	m_currentOffset += sizeof(double);

	double parsedDouble = *doublePointer;
	if (m_isOppositeNativeEndian)
	{
		Reverse8Bytes(reinterpret_cast<unsigned char*>(&parsedDouble));
	}

	return parsedDouble;
}


std::string const BufferParser::ParseStringWithNullTerminator()
{
	std::string parsedString = "";
	char parsedChar = ParseChar();
	while (parsedChar != '\0')
	{
		parsedString.push_back(parsedChar);
		parsedChar = ParseChar();
	}

	return parsedString;
}


std::string const BufferParser::ParseStringAfterLength()
{
	int length = ParseUInt32();

	std::string parsedString = "";
	for (int charIndex = 0; charIndex < length; charIndex++)
	{
		parsedString.push_back(ParseChar());
	}

	return parsedString;
}


Vec2 const BufferParser::ParseVec2()
{
	float x = ParseFloat();
	float y = ParseFloat();
	return Vec2(x, y);
}


Vec3 const BufferParser::ParseVec3()
{
	float x = ParseFloat();
	float y = ParseFloat();
	float z = ParseFloat();
	return Vec3(x, y, z);
}


Vec4 const BufferParser::ParseVec4()
{
	float x = ParseFloat();
	float y = ParseFloat();
	float z = ParseFloat();
	float w = ParseFloat();
	return Vec4(x, y, z, w);
}


IntVec2 const BufferParser::ParseIntVec2()
{
	int x = ParseInt32();
	int y = ParseInt32();
	return IntVec2(x, y);
}


IntVec3 const BufferParser::ParseIntVec3()
{
	int x = ParseInt32();
	int y = ParseInt32();
	int z = ParseInt32();
	return IntVec3(x, y, z);
}


AABB2 const BufferParser::ParseAABB2()
{
	Vec2 mins = ParseVec2();
	Vec2 maxs = ParseVec2();
	return AABB2(mins, maxs);
}


AABB3 const BufferParser::ParseAABB3()
{
	Vec3 mins = ParseVec3();
	Vec3 maxs = ParseVec3();
	return AABB3(mins, maxs);
}


OBB2 const BufferParser::ParseOBB2()
{
	Vec2 center = ParseVec2();
	Vec2 iBasis = ParseVec2();
	Vec2 halfDim = ParseVec2();
	return OBB2(center, iBasis, halfDim);
}


Rgba8 const BufferParser::ParseRgba8()
{
	unsigned char r = ParseByte();
	unsigned char g = ParseByte();
	unsigned char b = ParseByte();
	unsigned char a = ParseByte();
	return Rgba8(r, g, b, a);
}


Rgba8 const BufferParser::ParseRgb8()
{
	unsigned char r = ParseByte();
	unsigned char g = ParseByte();
	unsigned char b = ParseByte();
	return Rgba8(r, g, b);
}


Vertex_PCU const BufferParser::ParseVertexPCU()
{
	Vec3 pos = ParseVec3();
	Rgba8 color = ParseRgba8();
	Vec2 uv = ParseVec2();
	return Vertex_PCU(pos, color, uv);
}


Vertex_PCUTBN const BufferParser::ParseVertexPCUTBN()
{
	Vec3 pos = ParseVec3();
	Rgba8 color = ParseRgba8();
	Vec2 uv = ParseVec2();
	Vec3 tangent = ParseVec3();
	Vec3 bitangent = ParseVec3();
	Vec3 normal = ParseVec3();
	return Vertex_PCUTBN(pos, color, uv, tangent, bitangent, normal);
}


Plane2D const BufferParser::ParsePlane2D()
{
	Vec2 normal = ParseVec2();
	float dist = ParseFloat();
	return Plane2D(normal, dist);
}


Plane3D const BufferParser::ParsePlane3D()
{
	Vec3 normal = ParseVec3();
	float dist = ParseFloat();
	return Plane3D(normal, dist);
}


void BufferParser::SetOffset(size_t newOffset)
{
	m_currentOffset = newOffset;

	if (m_currentOffset >= m_bufferSize) ERROR_AND_DIE("Offset set past size of buffer!");
}
