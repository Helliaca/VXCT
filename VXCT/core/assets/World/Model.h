#pragma once

#ifndef MODEL_H
#define MODEL_H

#include "../Graphics/Shader.h"

enum RenderShader {VOX, EMIT, COLOR, EMITRGBA};

class Model : public IOobject
{
private:

	//Shader Uniform References
	std::map<std::string, glm::vec3*> vec3Refs;
	typedef std::pair<std::string, glm::vec3*> Vec3Entry;

	std::map<std::string, glm::vec4*> vec4Refs;
	typedef std::pair<std::string, glm::vec4*> Vec4Entry;

	std::map<std::string, glm::mat4*> mat4Refs;
	typedef std::pair<std::string, glm::mat4*> Mat4Entry;

	std::map<std::string, Lighting*> lightingRefs;
	typedef std::pair<std::string, Lighting*> LightingEntry;

	std::map<std::string, Material*> materialRefs;
	typedef std::pair<std::string, Material*> MaterialEntry;

	std::map<std::string, VoxSettings*> vsettingsRefs;
	typedef std::pair<std::string, VoxSettings*> VsettingsEntry;

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

	void addlightingReference(std::string name, Lighting* ref) {
		print(this, "Registered Reference: " + name);
		lightingRefs.insert(LightingEntry(name, ref));
	}

	void addMaterialReference(std::string name, Material* ref) {
		print(this, "Registered Reference: " + name);
		materialRefs.insert(MaterialEntry(name, ref));
	}

	void addVsettingsReference(std::string name,VoxSettings* ref) {
		print(this, "Registered Reference: " + name);
		vsettingsRefs.insert(VsettingsEntry(name, ref));
	}

	bool is_static; //Determines whether if the model is tied to the given shader, or other shaders can be used (eg. voxelization shader)

	glm::mat4 model; //Model Matrix
	void scale(float scale);
	void scale(glm::vec3 scale);
	void translate(glm::vec3 vec);
	void translate(float x, float y, float z);
	void lookAt(glm::vec3 target);
	void setPosition(float x, float y, float z);
	void setPosition(glm::vec3 pos);
	void rotate(glm::vec3 euler_angles);
	void resetPSR(); //Reset position scale and rotation
	glm::vec3 getPosition();
	std::vector<float> vertexData; //Includes vertices and Normals
	std::vector<int> indices;
	Shader* shader;
	Material* material;
	unsigned int VBO, VAO, EBO;
	void draw();
	void draw(Shader* customShader);
	Model(std::string name, RenderShader sh, std::string inputfile);
	Model(std::string name, RenderShader sh, std::vector<int>indices, std::vector<float>vertexData);
	~Model();
	void fromFile(std::string inputfile);
	void initialize(RenderShader sh);
};

#endif