#pragma once

#ifndef MODEL_H
#define MODEL_H

#include "core\shader.h"

enum RenderShader {VOX, EMIT, COLOR};

class Model : IOobject
{
private:

	//Shader Uniform References
	std::map<std::string, glm::vec3*> vec3Refs;
	typedef std::pair<std::string, glm::vec3*> Vec3Entry;

	std::map<std::string, glm::vec4*> vec4Refs;
	typedef std::pair<std::string, glm::vec4*> Vec4Entry;

	std::map<std::string, glm::mat4*> mat4Refs;
	typedef std::pair<std::string, glm::mat4*> Mat4Entry;

public:

	void addVec3Reference(std::string name, glm::vec3* ref) {
		print(this, "Registered Reference: " + name);
		vec3Refs.insert(Vec3Entry(name, ref));
	}

	void addVec4Reference(std::string name, glm::vec4* ref) {
		print(this, "Registered Reference: " + name);
		vec4Refs.insert(Vec4Entry(name, ref));
	}

	void addMat4Reference(std::string name, glm::mat4* ref) {
		print(this, "Registered Reference: " + name);
		mat4Refs.insert(Mat4Entry(name, ref));
	}

	glm::mat4 model; //Model Matrix
	void scale(float scale);
	void translate(glm::vec3 vec);
	void translate(float x, float y, float z);
	void setPosition(float x, float y, float z);
	void setPosition(glm::vec3 pos);
	std::vector<float> vertexData; //Includes vertices and Normals
	std::vector<int> indices;
	Shader* shader;
	unsigned int VBO, VAO, EBO;
	void draw();
	void vox(GLint textureID);
	Model(std::string name, RenderShader sh, std::string inputfile);
	Model(std::string name, RenderShader sh, std::vector<int>indices, std::vector<float>vertexData);
	~Model();
private:
	void fromFile(std::string inputfile);
	void initialize(RenderShader sh);
};

#endif