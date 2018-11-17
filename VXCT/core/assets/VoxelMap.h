#pragma once

#include "Model.h"

class VoxelMap : IOobject
{
public:
	unsigned int textureID;
	GLubyte* data = nullptr;
	GLubyte* lod_data = nullptr;
	GLint width, height, depth, internalFormat, numBytes;
	GLint lod_width, lod_height, lod_depth, lod_internalFormat, lod_numBytes;
	int saved_lod_level;
	VoxelMap(const std::vector<GLubyte> & textureBuffer, const std::vector<GLubyte> & lod_textureBuffer);
	~VoxelMap();
	void clear();
	void activate(const int shaderProgramID, const std::string glSamplerName, const int textureUnit);
	void updateMemory();
	void visualize(Model* voxelModel, glm::vec3* shaderReference, int lod_level);
	void retrieveData(int level);
private:
	void visualizeLod(Model* voxelModel, glm::vec3* shaderReference);
};

