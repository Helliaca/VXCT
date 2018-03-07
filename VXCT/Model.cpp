#include "Model.h"


#include "VertexData.h"
#include <glad\glad.h>
#include <GLFW\glfw3.h>


Model::Model()
{
	shader = new Shader("shaders/emit.vs", "shaders/emit.fs");
}


Model::~Model()
{
}


void Model::load()
{
	glGenVertexArrays(1, &VAO); //generate VAO
	glGenBuffers(1, &VBO);		//generate VBO

	glBindBuffer(GL_ARRAY_BUFFER, VBO); //bind VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vdata), Vdata, GL_STATIC_DRAW); //create buffer objects data store

	glBindVertexArray(VAO);

	//set vertex attributes
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void Model::draw(Camera* camera)
{
	glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera->GetViewMatrix();
	glm::mat4 model;
	shader->use();
	shader->setMat4("projection", projection);
	shader->setMat4("view", view);
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(0));
	model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
	shader->setMat4("model", model);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}