#pragma once

#ifndef MODEL_H
#define MODEL_H

#include "core\shader.h"
#include "core\camera.h"
#include "core\globals.h"

enum RenderShader {VOX, EMIT, COLOR};

class Model : IOobject
{
private:

	//Shader Uniform References
	std::map<std::string, glm::vec3*> vec3Refs;
	typedef std::pair<std::string, glm::vec3*> Vec3Entry;

	std::map<std::string, glm::mat4*> mat4Refs;
	typedef std::pair<std::string, glm::mat4*> Mat4Entry;

public:

	void addVec3Reference(std::string name, glm::vec3* ref) {
		print(this, "Registered Reference: " + name);
		vec3Refs.insert(Vec3Entry(name, ref));
	}

	void addMat4Reference(std::string name, glm::mat4* ref) {
		print(this, "Registered Reference: " + name);
		mat4Refs.insert(Mat4Entry(name, ref));
	}

	glm::mat4 model; //Model Matrix
	void scale(float scale);
	void translate(glm::vec3 vec);
	void translate(float x, float y, float z);
	Shader* shader;
	unsigned int VBO, VAO;
	void draw();
	Model(std::string name, RenderShader sh);
	~Model();
	//void load(Shader* shader);
	//void draw(Camera* camera);
	//void bind();
};

#endif