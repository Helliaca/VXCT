#include "Shader.h"

Shader::~Shader() {}


Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath, const char* geometryShaderPath) : IOobject("unnamedShader")
{
	std::string vertexShaderCode, fragmentShaderCode, geometryShaderCode;
	std::ifstream vShaderFile, fShaderFile, gShaderFile;
	// allow ifstream to throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vertexShaderPath);
		fShaderFile.open(fragmentShaderPath);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexShaderCode = vShaderStream.str();
		fragmentShaderCode = fShaderStream.str();
		// if geometry shader path is present, also load a geometry shader
		if (geometryShaderPath != nullptr)
		{
			gShaderFile.open(geometryShaderPath);
			std::stringstream gShaderStream;
			gShaderStream << gShaderFile.rdbuf();
			gShaderFile.close();
			geometryShaderCode = gShaderStream.str();
		}
	}
	catch (std::ifstream::failure e)
	{
		if (LOG_ERROR) print(this, "Could not read shader file");
		print(this, fragmentShaderPath);
		print(this, fragmentShaderCode.c_str());
	}

	//Files read. Compile them:

	const char* vShaderCode = vertexShaderCode.c_str();
	const char * fShaderCode = fragmentShaderCode.c_str();
	// 2. compile shaders
	unsigned int vertex, fragment;
	int success;
	char infoLog[512];
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");
	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");
	// if geometry shader is given, compile geometry shader
	unsigned int geometry;
	if (geometryShaderPath != nullptr)
	{
		const char * gShaderCode = geometryShaderCode.c_str();
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, NULL);
		glCompileShader(geometry);
		checkCompileErrors(geometry, "GEOMETRY");
	}
	// shader Program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	if (geometryShaderPath != nullptr)
		glAttachShader(ID, geometry);
	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (geometryShaderPath != nullptr)
		glDeleteShader(geometry);

}


void Shader::use()
{
	glUseProgram(ID);
}


void Shader::setBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}


void Shader::setInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}


void Shader::setFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}


void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
{
	glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}


void Shader::setVec2(const std::string &name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}


void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}


void Shader::setVec3(const std::string &name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}


void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
{
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}


void Shader::setVec4(const std::string &name, float x, float y, float z, float w)
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}


void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}


void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}


void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}


void Shader::setLighting(const std::string &name, Lighting &light_obj) const
{
	this->setInt(name + "_size", light_obj.pointLights.size()); //Note: size of lighting is defined as name_size (int) and must be defined so in all shaders.
	for (unsigned i = 0; i < light_obj.pointLights.size(); i++) {
		this->setPointLight(name + "[" + std::to_string(i) + "]", *light_obj.pointLights[i]);
	}
}


void Shader::setPointLight(const std::string &name, PointLight &light_obj) const
{
	const PointLight_struct light = light_obj.to_struct();
	this->setVec3(name + ".position", light.position);
	this->setVec3(name + ".color", light.color);
	this->setFloat(name + ".att_constant", light.att_constant);
	this->setFloat(name + ".att_linear", light.att_linear);
	this->setFloat(name + ".att_quadratic", light.att_quadratic);
}


void Shader::setMaterial(const std::string &name, Material &material_obj) const
{
	const Material_struct mat = material_obj.to_struct();
	this->setVec3(name + ".color", mat.color);
	this->setFloat(name + ".shininess", mat.shininess);
	this->setFloat(name + ".ambient_str", mat.ambient_str);
	this->setFloat(name + ".diffuse_str", mat.diffuse_str);
	this->setFloat(name + ".specular_str", mat.specular_str);
}


void Shader::setVsettings(const std::string &name, VoxSettings &vsettings_obj) const
{
	const VoxSettings_struct vs = vsettings_obj.to_struct();
	this->setFloat(name + ".diffuse_dist_factor", vs.diffuse_dist_factor);
	this->setFloat(name + ".diffuse_apperture", vs.diffuse_apperture);
	this->setFloat(name + ".diffuse_offset", vs.diffuse_offset);
	this->setFloat(name + ".diffuse_origin_offset", vs.diffuse_origin_offset);

	this->setFloat(name + ".occlusion_dist_factor", vs.occlusion_dist_factor);
	this->setFloat(name + ".occlusion_apperture", vs.occlusion_apperture);
	this->setFloat(name + ".occlusion_offset", vs.occlusion_offset);

	this->setFloat(name + ".specular_dist_factor", vs.specular_dist_factor);
	this->setFloat(name + ".specular_apperture", vs.specular_apperture);
	this->setFloat(name + ".specular_offset", vs.specular_offset);

	this->setFloat(name + ".shadow_str", vs.shadow_str);
	this->setFloat(name + ".shininess_falloff", vs.shininess_falloff);

	this->setBool(name + ".phong", vs.phong);
	this->setBool(name + ".phong_ambient", vs.phong_ambient);
	this->setBool(name + ".phong_diffuse", vs.phong_diffuse);
	this->setBool(name + ".phong_specular", vs.phong_specular);
	this->setBool(name + ".vox_diffuse", vs.vox_diffuse);
	this->setBool(name + ".vox_shadows", vs.vox_shadows);
	this->setBool(name + ".vox_specular", vs.vox_specular);

	this->setBool(name + ".front_cone", vs.front_cone);
	this->setBool(name + ".side_cones", vs.side_cones);
	this->setBool(name + ".intermediate_cones", vs.intermediate_cones);
}


void Shader::checkCompileErrors(GLuint shader, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}