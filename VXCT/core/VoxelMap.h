#pragma once

//#include "globals.h"
//#include "base.h"
#include "../Model.h"

class VoxelMap : IOobject
{
public:
	unsigned int textureID;
	GLubyte* data = nullptr;
	GLint width, height, depth, internalFormat;
	GLint numBytes;
	VoxelMap(const std::vector<GLubyte> & textureBuffer);
	~VoxelMap();
	void clear();
	void activate(const int shaderProgramID, const std::string glSamplerName, const int textureUnit);
	void updateMemory();
	void visualize(Model* voxelModel, glm::vec3* shaderReference);
};

