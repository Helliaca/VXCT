#pragma once

#include "../SVCall.h"

class VoxelLodMap : IOobject
{
public:
	VoxelLodMap(const std::vector<GLubyte> & textureBuffer, GLint LodLevel);
	~VoxelLodMap();
	unsigned int textureID;
	GLubyte* data = nullptr;

	GLint width, height, depth, internalFormat;
	GLint numBytes;
	void clear();
	GLint LodLevel;

	void activate(const int shaderProgramID, const std::string glSamplerName, const int textureUnit);
	void updateMemory();

	SVCall* getVoxelLodMap;

	void visualize(Model* voxelModel, glm::vec3* shaderReference);
	void visualizeLod(Model* voxelModel, glm::vec3* shaderReference, int LOD, bool scaleModel);
};

