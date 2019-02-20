#pragma once

#include "World/Model.h"

class VoxelMap : IOobject
{
public:
	//ID
	unsigned int textureID;

	//CPU/visualization related attributes
	GLubyte* data = nullptr;
	GLubyte* lod_data = nullptr;
	GLint width, height, depth, internalFormat, numBytes;
	GLint lod_width, lod_height, lod_depth, lod_internalFormat, lod_numBytes;
	int saved_lod_level;

	VoxelMap();
	~VoxelMap();

	//Methods
	void clear();
	void use(const int shaderProgramID, const std::string glSamplerName, const int textureUnit);
	void updateMemory(bool toCPU);
	void visualize(Model* voxelModel, glm::vec3* shaderReference, int lod_level);
	void retrieveData(int level);
private:
};

