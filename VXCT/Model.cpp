#include "Model.h"


#include "VertexData.h"
#include <GLFW\glfw3.h>


Model::Model(std::string name = "unnamedModel", RenderShader sh=RenderShader::EMIT) : IOobject(name)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vdata), Vdata, GL_STATIC_DRAW);

	if (sh == RenderShader::VOX) {
		shader = new Shader(VOXSHADER_VS, VOXSHADER_FS, VOXSHADER_GS);

		glBindVertexArray(VAO);
		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// normal attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}
	if (sh == RenderShader::COLOR) {
		shader = new Shader(COLORSHADER_VS, COLORSHADER_FS);

		glBindVertexArray(VAO);
		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// normal attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}
	else if (sh == RenderShader::EMIT) {
		shader = new Shader(EMITSHADER_VS, EMITSHADER_FS);

		glBindVertexArray(VAO);
		// note that we update the lamp's position attribute's stride to reflect the updated buffer data
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}
}


Model::~Model()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//The two lines below will be necessary to avoid memory leaks but will lead to problems when removing memory other Models are pointing at. (Like viewMatrix)
	//for (auto const& x : vec3Refs) if (x.second != NULL) { delete(x.second); x.second = 0; } //first is not a pointer so there is no need to call delete on it
	//for (auto const& x : mat4Refs) if (x.second != NULL) delete(x.second);
}

void Model::draw() {

	shader->use();

	//Set all references
	for (auto const& x : vec3Refs) shader->setVec3(x.first, *x.second);
	for (auto const& x : mat4Refs) shader->setMat4(x.first, *x.second);

	// render the cube
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Model::vox(GLint textureID) {
	//Set all references
	for (auto const& x : vec3Refs) shader->setVec3(x.first, *x.second);
	for (auto const& x : vec4Refs) shader->setVec4(x.first, *x.second);
	for (auto const& x : mat4Refs) shader->setMat4(x.first, *x.second);

	// render the cube
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Model::scale(float scale) {
	model = glm::scale(model, glm::vec3(scale));
}

void Model::translate(glm::vec3 vec) {
	model = glm::translate(model, vec);
}

void Model::translate(float x, float y, float z) {
	model = glm::translate(model, glm::vec3(x, y, z));
}

void Model::setPosition(float x, float y, float z) { //Set 3rd column of model matrix to xyz
	model[3].x = x;
	model[3].y = y;
	model[3].z = z;
}

void Model::setPosition(glm::vec3 pos) {
	setPosition(pos.x, pos.y, pos.z);
}