#pragma once

#ifndef MODEL_H
#define MODEL_H

#include "core\shader.h"
#include "core\camera.h"


class Model
{
private:
	unsigned int VBO, VAO;
	Shader *shader;
public:
	Model();
	~Model();
	void load();
	void draw(Camera* camera);
};

#endif