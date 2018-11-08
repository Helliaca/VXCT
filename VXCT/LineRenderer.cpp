#include "LineRenderer.h"

#include "core\globals.h"


LineRenderer::LineRenderer() : IOobject("Unnamed LineRenderer")
{
	vertexData = {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f};
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertexData.size(), &vertexData[0], GL_STATIC_DRAW);

	LineShader = new Shader(LINESHADER_VS, LINESHADER_FS);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	checkErrors("Initialize");
}


LineRenderer::~LineRenderer()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}


void LineRenderer::drawLine() 
{
	
}

void LineRenderer::draw() {
	//LineShader->use();

	//LineShader->setMat4("view_u", G::SceneCamera->viewMatrix);
	//LineShader->setMat4("proj_u", G::SceneCamera->projMatrix);

	// render
	//glBindVertexArray(VAO);
	//glDrawArrays(GL_LINES, 0, 6);
	print(this, "smth");

	checkErrors("Draw");
}