#ifndef SHADER_H
#define SHADER_H

#include "../../globals.h"

class Shader : IOobject
{
public:
	unsigned int ID;

	~Shader();


	Shader(const char* vertexShaderPath, const char* fragmentShaderPath, const char* geometryShaderPath = nullptr);

	void use();
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	void setVec2(const std::string &name, const glm::vec2 &value) const;
	void setVec2(const std::string &name, float x, float y) const;
	void setVec3(const std::string &name, const glm::vec3 &value) const;
	void setVec3(const std::string &name, float x, float y, float z) const;
	void setVec4(const std::string &name, const glm::vec4 &value) const;
	void setVec4(const std::string &name, float x, float y, float z, float w);
	void setMat2(const std::string &name, const glm::mat2 &mat) const;
	void setMat3(const std::string &name, const glm::mat3 &mat) const;
	void setMat4(const std::string &name, const glm::mat4 &mat) const;
	void setLighting(const std::string &name, Lighting &light_obj) const;
	void setPointLight(const std::string &name, PointLight &light_obj) const;
	void setMaterial(const std::string &name, Material &material_obj) const;
	void setVsettings(const std::string &name, VoxSettings &vsettings_obj) const;
private:
	void checkCompileErrors(GLuint shader, std::string type);
};
#endif
