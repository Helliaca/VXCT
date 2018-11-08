#include "LineRenderer.h"



LineRenderer::LineRenderer() : IOobject("Unnamed LineRenderer")
{
	vertexData = {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertexData.size(), &vertexData[0], GL_STATIC_DRAW);

	shader = new Shader(LINESHADER_VS, LINESHADER_FS);

	//Position Attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


LineRenderer::~LineRenderer()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}


void LineRenderer::draw() 
{
	shader->use();
	shader->setMat4("view_u", G::SceneCamera->viewMatrix);
	shader->setMat4("proj_u", G::SceneCamera->projMatrix);

	// render
	glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, 6);

	checkErrors("Draw");
}

void LineRenderer::drawLine(glm::vec3 from, glm::vec3 to) {
	vertexData = { from.x, from.y, from.z, to.x, to.y, to.z };
	updateMemory();
	draw();
}

void LineRenderer::drawLine(glm::vec3 from, glm::vec3 dir, float length) {
	dir = (glm::normalize(dir) * length) + from;
	vertexData = { from.x, from.y, from.z, dir.x, dir.y, dir.z };
	updateMemory();
	draw();
}

void LineRenderer::updateMemory() {
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertexData.size(), &vertexData[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}