#pragma once

#include "Model.h"

class VoxelMap : IOobject
{
public:
	unsigned int textureID;
	GLubyte* data = nullptr;
	GLint width, height, depth, internalFormat;
	GLint numBytes;
	VoxelMap(const std::vector<GLubyte> & textureBuffer, const std::vector<GLubyte> & lod_textureBuffer);
	~VoxelMap();
	void clear();
	void activate(const int shaderProgramID, const std::string glSamplerName, const int textureUnit);
	void updateMemory();
	void visualize(Model* voxelModel, glm::vec3* shaderReference);
	void visualizeLod(Model* voxelModel, glm::vec3* shaderReference, int LOD, bool scaleModel);
};

