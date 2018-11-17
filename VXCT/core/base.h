#pragma once

#include <glad/glad.h>
#include <GLFW\glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <map>

#include "settings\defs.h"
#include "settings\VertexData.h"

class IOobject;

inline void print(IOobject* source, std::string out);

class IOobject {
public:
	std::string name="unnamed";
	IOobject(std::string name) { this->name = name; }

	void checkErrors(std::string context="Unknwon Context") {
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR)
		{
			print(this, "OpenGL Error:");
			print(this, "\t CONTEXT: " + context);
			if (err == GL_INVALID_ENUM) print(this, "\t TYPE: Invalid Enum");
			else if (err == GL_INVALID_VALUE) print(this, "\t TYPE: Invalid Value");
			else if (err == GL_INVALID_OPERATION) print(this, "\t TYPE: Invalid Operation");
			//else if (err == GL_STACK_OVERFLOW) print(this, "\t TYPE: Stack Overflow"); //not defined
			//else if (err == GL_STACK_UNDERFLOW) print(this, "\t TYPE: Stack Underflow"); //not defined
			else if (err == GL_OUT_OF_MEMORY) print(this, "\t TYPE: Out of Memory");
			else if (err == GL_INVALID_FRAMEBUFFER_OPERATION) print(this, "\t TYPE: Invalid Framebuffer Operation");
			else if (err == GL_CONTEXT_LOST) print(this, "\t TYPE: Context Lost");
			else print(this, "\t TYPE: Undefined Error");
		}
	}

};

inline void print(IOobject* source, std::string out) { std::cout << source->name << " >> " << out << std::endl; }

//Coordiante Systems:
// World Coordiante: From (MIN_X, MIN_Y, MIN_Z) to (MAX_X, MAX_Y, MAX_Z)
// Base Voxel Coordiante: From (0, 0, 0) to (1, 1, 1) on VoxelMap
// Voxel Coordiante: From (0, 0, 0) to (VOX_SIZE, VOX_SIZE, VOX_SIZE) on VoxelMap

//Voxel Coordiante to World Coordinate
inline glm::vec3 vC_to_wC(glm::vec3 pos) {
	float x = (pos.x / VOX_SIZE) * (MAX_X - MIN_X) + MIN_X;
	float y = (pos.y / VOX_SIZE) * (MAX_Y - MIN_Y) + MIN_Y;
	float z = (pos.z / VOX_SIZE) * (MAX_Z - MIN_Z) + MIN_Z;
	return glm::vec3(x, y, z);
}

//Voxel Coordiante to World Coordinate with custom voxelMap size (used for LOD)
inline glm::vec3 vC_to_wC(glm::vec3 pos, int voxelMap_size) {
	float x = (pos.x / voxelMap_size) * (MAX_X - MIN_X) + MIN_X;
	float y = (pos.y / voxelMap_size) * (MAX_Y - MIN_Y) + MIN_Y;
	float z = (pos.z / voxelMap_size) * (MAX_Z - MIN_Z) + MIN_Z;
	return glm::vec3(x, y, z);
}