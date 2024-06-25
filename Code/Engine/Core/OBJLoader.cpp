#include "Engine/Core/OBJLoader.hpp"
#include "Engine/Core/FileUtils.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/Time.hpp"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


struct ModelFace
{
	std::vector<int> vertexIndexes;
	std::vector<int> uvIndexes;
	std::vector<int> normalIndexes;
};


void OBJLoader::LoadObjFile(std::string const& fileName, Mat44 const& transformMatrix, std::vector<Vertex_PCUTBN>& vertexes, std::vector<int>& indexes)
{
	float startParseTime = static_cast<float>(GetCurrentTimeSeconds());

	std::string objString = "";
	int fileReadSuccess = FileReadToString(objString, fileName);
	if (fileReadSuccess == 0)
	{
		return;
	}

	//create arrays for positions, normals, texture coordinates, and faces
	std::vector<Vec3> positions;
	std::vector<Vec3> normals;
	std::vector<Vec2> textureCoords;
	std::vector<ModelFace> faces;

	//parse string for vertexes, texture coordinates, normals, and faces
	Strings stringSplitOnNewline = SplitStringOnDelimiter(objString, '\n');

	for (int stringIndex = 0; stringIndex < stringSplitOnNewline.size(); stringIndex++)
	{
		std::string& substring = stringSplitOnNewline[stringIndex];
		
		if (substring.length() > 0 && substring[substring.length() - 1] == '\r')
		{
			substring.pop_back();
		}

		if (substring.size() > 2)	//no point in checking substrings without at least a starting character, a space, and something after
		{
			bool currentlyParsingNumber = false;
			int numStart = 0;
			int numEnd = 0;
			int currentNum = 0;

			if (substring[0] == 'v' && substring[1] == ' ')
			{
				Vec3 positionCoords = Vec3();

				for (int charIndex = 2; charIndex < substring.size(); charIndex++)
				{
					char c = substring[charIndex];

					if (c != ' ' && !currentlyParsingNumber)
					{
						currentlyParsingNumber = true;

						numStart = charIndex;
					}
					else if (c == ' ' && currentlyParsingNumber)
					{
						currentlyParsingNumber = false;

						numEnd = charIndex;

						if (currentNum == 0)
						{
							std::string xString = substring.substr(numStart, numEnd - numStart);
							positionCoords.x = static_cast<float>(atof(xString.c_str()));
						}
						else if (currentNum == 1)
						{
							std::string yString = substring.substr(numStart, numEnd - numStart);
							positionCoords.y = static_cast<float>(atof(yString.c_str()));
						}

						currentNum++;
					}
					else if (charIndex == substring.size() - 1)
					{
						if (currentNum == 2)
						{
							std::string zString = substring.substr(numStart, numEnd - numStart);
							positionCoords.z = static_cast<float>(atof(zString.c_str()));

							positions.emplace_back(positionCoords);
						}
					}
				}
			}
			else if (substring[0] == 'f' && substring[1] == ' ')
			{
				ModelFace tempFace;

				for (int charIndex = 2; charIndex < substring.size(); charIndex++)
				{
					char c = substring[charIndex];

					if (c != ' ' && !currentlyParsingNumber)
					{
						currentlyParsingNumber = true;

						numStart = charIndex;

						if (charIndex == substring.size() - 1)
						{
							std::string vertexIndexString = substring.substr(numStart, numEnd - numStart);
							tempFace.vertexIndexes.emplace_back(atoi(vertexIndexString.c_str()));

							faces.emplace_back(tempFace);
						}
					}
					else if (c == ' ' && currentlyParsingNumber && charIndex != substring.size() - 1)
					{
						currentlyParsingNumber = false;

						numEnd = charIndex;

						std::string vertexIndexString = substring.substr(numStart, numEnd - numStart);

						//parse out slashes
						Strings splitOnBackslash = SplitStringOnDelimiter(vertexIndexString, '/');

						//get vertex position
						tempFace.vertexIndexes.emplace_back((atoi(splitOnBackslash[0].c_str())));
						
						//get uv coords
						if (splitOnBackslash.size() > 1 && splitOnBackslash[1] != "")
						{
							tempFace.uvIndexes.emplace_back(atoi(splitOnBackslash[1].c_str()));
						}

						//get normal
						if (splitOnBackslash.size() > 2)
						{
							tempFace.normalIndexes.emplace_back(atoi(splitOnBackslash[2].c_str()));
						}

						currentNum++;
					}
					else if (charIndex == substring.size() - 1)
					{
						std::string vertexIndexString = substring.substr(numStart, numEnd - numStart);
						
						//parse out slashes
						Strings splitOnBackslash = SplitStringOnDelimiter(vertexIndexString, '/');

						//get vertex position
						tempFace.vertexIndexes.emplace_back(atoi(splitOnBackslash[0].c_str()));

						//get uv coords
						if (splitOnBackslash.size() > 1 && splitOnBackslash[1] != "")
						{
							tempFace.uvIndexes.emplace_back(atoi(splitOnBackslash[1].c_str()));
						}

						//get normal
						if (splitOnBackslash.size() > 2)
						{
							tempFace.normalIndexes.emplace_back(atoi(splitOnBackslash[2].c_str()));
						}

						faces.emplace_back(tempFace);
					}
				}
			}
			else if (substring[0] == 'v' && substring[1] == 'n' && substring[2] == ' ')
			{
				Vec3 normalCoords = Vec3();

				for (int charIndex = 3; charIndex < substring.size(); charIndex++)
				{
					char c = substring[charIndex];

					if (c != ' ' && !currentlyParsingNumber)
					{
						currentlyParsingNumber = true;

						numStart = charIndex;
					}
					else if (c == ' ' && currentlyParsingNumber)
					{
						currentlyParsingNumber = false;

						numEnd = charIndex;

						if (currentNum == 0)
						{
							std::string xString = substring.substr(numStart, numEnd - numStart);
							normalCoords.x = static_cast<float>(atof(xString.c_str()));
						}
						else if (currentNum == 1)
						{
							std::string yString = substring.substr(numStart, numEnd - numStart);
							normalCoords.y = static_cast<float>(atof(yString.c_str()));
						}

						currentNum++;
					}
					else if (charIndex == substring.size() - 1)
					{
						if (currentNum == 2)
						{
							std::string zString = substring.substr(numStart, numEnd - numStart);
							normalCoords.z = static_cast<float>(atof(zString.c_str()));

							normals.emplace_back(normalCoords);
						}
					}
				}
			}
			else if (substring[0] == 'v' && substring[1] == 't' && substring[2] == ' ')
			{
				Vec2 uvCoords = Vec2();

				for (int charIndex = 3; charIndex < substring.size(); charIndex++)
				{
					char c = substring[charIndex];

					if (c != ' ' && !currentlyParsingNumber)
					{
						currentlyParsingNumber = true;

						numStart = charIndex;
					}
					else if (c == ' ' && currentlyParsingNumber)
					{
						currentlyParsingNumber = false;

						numEnd = charIndex;

						if (currentNum == 0)
						{
							std::string xString = substring.substr(numStart, numEnd - numStart);
							uvCoords.x = static_cast<float>(atof(xString.c_str()));
						}
						if (currentNum == 1)
						{
							std::string yString = substring.substr(numStart, numEnd - numStart);
							uvCoords.y = static_cast<float>(atof(yString.c_str()));
						}

						currentNum++;
					}
					else if (charIndex == substring.size() - 1)
					{
						if (currentNum == 1)
						{
							std::string yString = substring.substr(numStart, numEnd - numStart);
							uvCoords.y = static_cast<float>(atof(yString.c_str()));
						}

						textureCoords.emplace_back(uvCoords);
					}
				}
			}
		}
	}

	float totalParseTime = static_cast<float>(GetCurrentTimeSeconds()) - startParseTime;

	float startCreateTime = static_cast<float>(GetCurrentTimeSeconds());

	//push all the data to the vertex and index arrays
	if (faces.size() == 0)
	{
		if (normals.size() == 0 && textureCoords.size() == 0)
		{
			for (int vertexIndex = 0; vertexIndex < positions.size(); vertexIndex += 3)
			{
				Vec3 vertexNormal = CrossProduct3D(positions[vertexIndex + 1] - positions[vertexIndex], positions[vertexIndex + 2] - positions[vertexIndex]).GetNormalized();

				vertexes.emplace_back(Vertex_PCUTBN(positions[vertexIndex], vertexNormal, Rgba8()));
				vertexes.emplace_back(Vertex_PCUTBN(positions[vertexIndex + 1], vertexNormal, Rgba8()));
				vertexes.emplace_back(Vertex_PCUTBN(positions[vertexIndex + 2], vertexNormal, Rgba8()));

				indexes.emplace_back(vertexIndex);
				indexes.emplace_back(vertexIndex + 1);
				indexes.emplace_back(vertexIndex + 2);
			}
		}
		else if(normals.size() > 0 && textureCoords.size() == 0)
		{
			for (int vertexIndex = 0; vertexIndex < positions.size(); vertexIndex++)
			{
				vertexes.emplace_back(Vertex_PCUTBN(positions[vertexIndex], normals[vertexIndex], Rgba8()));
				indexes.emplace_back(vertexIndex);
			}
		}
		else if (normals.size() == 0 && textureCoords.size() > 0)
		{
			for (int vertexIndex = 0; vertexIndex < positions.size(); vertexIndex += 3)
			{
				Vec3 vertexNormal = CrossProduct3D(positions[vertexIndex + 1] - positions[vertexIndex], positions[vertexIndex + 2] - positions[vertexIndex]).GetNormalized();

				vertexes.emplace_back(Vertex_PCUTBN(positions[vertexIndex], vertexNormal, Rgba8(), textureCoords[vertexIndex]));
				vertexes.emplace_back(Vertex_PCUTBN(positions[vertexIndex + 1], vertexNormal, Rgba8(), textureCoords[vertexIndex + 1]));
				vertexes.emplace_back(Vertex_PCUTBN(positions[vertexIndex + 2], vertexNormal, Rgba8(), textureCoords[vertexIndex + 2]));

				indexes.emplace_back(vertexIndex);
				indexes.emplace_back(vertexIndex + 1);
				indexes.emplace_back(vertexIndex + 2);
			}
		}
		else
		{
			for (int vertexIndex = 0; vertexIndex < positions.size(); vertexIndex++)
			{
				vertexes.emplace_back(Vertex_PCUTBN(positions[vertexIndex], normals[vertexIndex], Rgba8(), textureCoords[vertexIndex]));
				indexes.emplace_back(vertexIndex);
			}
		}
	}
	else
	{
		if (normals.size() == 0 && textureCoords.size() == 0)
		{
			int totalVertexNumber = 0;

			for (int faceIndex = 0; faceIndex < faces.size(); faceIndex++)
			{
				Vec3 vertexNormal = CrossProduct3D(positions[faces[faceIndex].vertexIndexes[1] - 1] - positions[faces[faceIndex].vertexIndexes[0] - 1],
					positions[faces[faceIndex].vertexIndexes[2] - 1] - positions[faces[faceIndex].vertexIndexes[0] - 1]).GetNormalized();
				
				for (int vertexIndex = 0; vertexIndex < faces[faceIndex].vertexIndexes.size(); vertexIndex++)
				{
					vertexes.emplace_back(Vertex_PCUTBN(positions[faces[faceIndex].vertexIndexes[vertexIndex] - 1], vertexNormal, Rgba8()));
				}

				for (int indexIndex = 2; indexIndex < faces[faceIndex].vertexIndexes.size(); indexIndex++)
				{
					indexes.emplace_back(totalVertexNumber);
					indexes.emplace_back(totalVertexNumber + indexIndex - 1);
					indexes.emplace_back(totalVertexNumber + indexIndex);
				}

				totalVertexNumber += static_cast<int>(faces[faceIndex].vertexIndexes.size());
			}
			
			/*int vertexIndex = 0;

			for (int faceIndex = 0; faceIndex < faces.size(); faceIndex++)
			{
				Vec3 vertexNormal = CrossProduct3D(positions[faces[faceIndex].vertexIndexes[1] - 1] - positions[faces[faceIndex].vertexIndexes[0] - 1],
					positions[faces[faceIndex].vertexIndexes[2] - 1] - positions[faces[faceIndex].vertexIndexes[0] - 1]).GetNormalized();

				vertexes.emplace_back(Vertex_PNCU(positions[faces[faceIndex].vertexIndexes[0] - 1], vertexNormal, Rgba8()));
				vertexes.emplace_back(Vertex_PNCU(positions[faces[faceIndex].vertexIndexes[1] - 1], vertexNormal, Rgba8()));
				vertexes.emplace_back(Vertex_PNCU(positions[faces[faceIndex].vertexIndexes[2] - 1], vertexNormal, Rgba8()));

				indexes.emplace_back(vertexIndex);
				indexes.emplace_back(vertexIndex + 1);
				indexes.emplace_back(vertexIndex + 2);

				vertexIndex += 3;
			}*/
		}
		else if (normals.size() > 0 && textureCoords.size() == 0)
		{
			int totalVertexNumber = 0;
			
			for (int faceIndex = 0; faceIndex < faces.size(); faceIndex++)
			{
				for (int vertexIndex = 0; vertexIndex < faces[faceIndex].vertexIndexes.size(); vertexIndex++)
				{
					vertexes.emplace_back(Vertex_PCUTBN(positions[faces[faceIndex].vertexIndexes[vertexIndex] - 1], normals[faces[faceIndex].normalIndexes[vertexIndex] - 1], Rgba8()));
				}

				for (int indexIndex = 2; indexIndex < faces[faceIndex].vertexIndexes.size(); indexIndex++)
				{
					indexes.emplace_back(totalVertexNumber);
					indexes.emplace_back(totalVertexNumber + indexIndex - 1);
					indexes.emplace_back(totalVertexNumber + indexIndex);
				}

				totalVertexNumber += static_cast<int>(faces[faceIndex].vertexIndexes.size());
			}
		}
		else if (normals.size() == 0 && textureCoords.size() > 0)
		{
			int totalVertexNumber = 0;

			for (int faceIndex = 0; faceIndex < faces.size(); faceIndex++)
			{
				Vec3 vertexNormal = CrossProduct3D(positions[faces[faceIndex].vertexIndexes[1] - 1] - positions[faces[faceIndex].vertexIndexes[0] - 1],
					positions[faces[faceIndex].vertexIndexes[2] - 1] - positions[faces[faceIndex].vertexIndexes[0] - 1]).GetNormalized();

				for (int vertexIndex = 0; vertexIndex < faces[faceIndex].vertexIndexes.size(); vertexIndex++)
				{
					vertexes.emplace_back(Vertex_PCUTBN(positions[faces[faceIndex].vertexIndexes[vertexIndex] - 1], vertexNormal, Rgba8(), 
						textureCoords[faces[faceIndex].uvIndexes[vertexIndex] - 1]));
				}

				for (int indexIndex = 2; indexIndex < faces[faceIndex].vertexIndexes.size(); indexIndex++)
				{
					indexes.emplace_back(totalVertexNumber);
					indexes.emplace_back(totalVertexNumber + indexIndex - 1);
					indexes.emplace_back(totalVertexNumber + indexIndex);
				}

				totalVertexNumber += static_cast<int>(faces[faceIndex].vertexIndexes.size());
			}
			
			/*int vertexIndex = 0;

			for (int faceIndex = 0; faceIndex < faces.size(); faceIndex++)
			{
				Vec3 vertexNormal = CrossProduct3D(positions[faces[faceIndex].vertexIndexes[1] - 1] - positions[faces[faceIndex].vertexIndexes[0] - 1],
					positions[faces[faceIndex].vertexIndexes[2] - 1] - positions[faces[faceIndex].vertexIndexes[0] - 1]).GetNormalized();

				vertexes.emplace_back(Vertex_PNCU(positions[faces[faceIndex].vertexIndexes[0] - 1], vertexNormal, Rgba8(), textureCoords[faces[faceIndex].uvIndexes[0] - 1]));
				vertexes.emplace_back(Vertex_PNCU(positions[faces[faceIndex].vertexIndexes[1] - 1], vertexNormal, Rgba8(), textureCoords[faces[faceIndex].uvIndexes[0] - 1]));
				vertexes.emplace_back(Vertex_PNCU(positions[faces[faceIndex].vertexIndexes[2] - 1], vertexNormal, Rgba8(), textureCoords[faces[faceIndex].uvIndexes[0] - 1]));

				indexes.emplace_back(vertexIndex);
				indexes.emplace_back(vertexIndex + 1);
				indexes.emplace_back(vertexIndex + 2);

				vertexIndex += 3;
			}*/
		}
		else
		{
			int totalVertexNumber = 0;

			for (int faceIndex = 0; faceIndex < faces.size(); faceIndex++)
			{
				for (int vertexIndex = 0; vertexIndex < faces[faceIndex].vertexIndexes.size(); vertexIndex++)
				{
					if (faces[faceIndex].uvIndexes.size() == 0)
					{
						vertexes.emplace_back(Vertex_PCUTBN(positions[faces[faceIndex].vertexIndexes[vertexIndex] - 1], normals[faces[faceIndex].normalIndexes[vertexIndex] - 1], Rgba8(),
							Vec2()));
					}
					else
					{
						vertexes.emplace_back(Vertex_PCUTBN(positions[faces[faceIndex].vertexIndexes[vertexIndex] - 1], normals[faces[faceIndex].normalIndexes[vertexIndex] - 1], Rgba8(),
							textureCoords[faces[faceIndex].uvIndexes[vertexIndex] - 1]));
					}
				}

				for (int indexIndex = 2; indexIndex < faces[faceIndex].vertexIndexes.size(); indexIndex++)
				{
					indexes.emplace_back(totalVertexNumber);
					indexes.emplace_back(totalVertexNumber + indexIndex - 1);
					indexes.emplace_back(totalVertexNumber + indexIndex);
				}

				totalVertexNumber += static_cast<int>(faces[faceIndex].vertexIndexes.size());
			}
			/*int vertexIndex = 0;

			for (int faceIndex = 0; faceIndex < faces.size(); faceIndex++)
			{
				vertexes.emplace_back(Vertex_PNCU(positions[faces[faceIndex].vertexIndexes[0] - 1], normals[faces[faceIndex].normalIndexes[0] - 1], Rgba8(), textureCoords[faces[faceIndex].uvIndexes[0] - 1]));
				vertexes.emplace_back(Vertex_PNCU(positions[faces[faceIndex].vertexIndexes[1] - 1], normals[faces[faceIndex].normalIndexes[1] - 1], Rgba8(), textureCoords[faces[faceIndex].uvIndexes[0] - 1]));
				vertexes.emplace_back(Vertex_PNCU(positions[faces[faceIndex].vertexIndexes[2] - 1], normals[faces[faceIndex].normalIndexes[2] - 1], Rgba8(), textureCoords[faces[faceIndex].uvIndexes[0] - 1]));

				indexes.emplace_back(vertexIndex);
				indexes.emplace_back(vertexIndex + 1);
				indexes.emplace_back(vertexIndex + 2);

				vertexIndex += 3;
			}*/
		}
	}

	//transform by transform matrix
	TransformVertexArray3D(vertexes, transformMatrix);

	CalculateTangentSpaceVectors(vertexes, indexes);

	float totalCreateTime = static_cast<float>(GetCurrentTimeSeconds()) - startCreateTime;

	//display results to console window
	OutputDebugStringA("\n");
	OutputDebugStringA("--------------------------------------------------\n");
	std::string const& fileLoadedString = Stringf("Loaded .obj file %s\n", fileName.c_str());
	OutputDebugStringA(fileLoadedString.c_str());

	int numFaces = static_cast<int>(faces.size());
	int numTriangles = static_cast<int>(vertexes.size()) / 3;
	if (numFaces == 0) numFaces = numTriangles;

	std::string const& fileDataString = Stringf("  [file data]   vertexes: %i  texture coordinates: %i  normals: %i  faces: %i  triangles: %i\n", 
		positions.size(), textureCoords.size(), normals.size(), numFaces, numTriangles);
	OutputDebugStringA(fileDataString.c_str());

	std::string const& loadedMeshString = Stringf("  [loaded mesh] vertexes: %i  indexes: %i\n", vertexes.size(), indexes.size());
	OutputDebugStringA(loadedMeshString.c_str());

	std::string const& timeString = Stringf("  [time]       parse:  %.6f s  create: %.6f s\n", totalParseTime, totalCreateTime);
	OutputDebugStringA(timeString.c_str());

	OutputDebugStringA("--------------------------------------------------\n");
	OutputDebugStringA("\n");
}
