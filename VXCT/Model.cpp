#include "Model.h"



#define TINYOBJLOADER_IMPLEMENTATION
#include "lib\tiny_obj_loader.h"

Model::Model(std::string name, RenderShader sh=RenderShader::EMIT, std::string inputfile="") : IOobject(name)
{
	if (inputfile!="") { this->fromFile(inputfile); }
	initialize(sh);
}

Model::Model(std::string name = "unnamedModel", RenderShader sh = RenderShader::EMIT, std::vector<int>indices = {}, std::vector<float>vertexData = {}) : IOobject(name)
{
	this->vertexData = vertexData;
	this->indices = indices;
	initialize(sh);
}

void Model::initialize(RenderShader sh) {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertexData.size(), &vertexData[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*indices.size(), &indices[0], GL_STATIC_DRAW);

	if (sh == RenderShader::VOX) {
		shader = new Shader(VOXSHADER_VS, VOXSHADER_FS, VOXSHADER_GS);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// normal attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}
	if (sh == RenderShader::COLOR) {
		shader = new Shader(COLORSHADER_VS, COLORSHADER_FS);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// normal attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}
	else if (sh == RenderShader::EMIT) {
		shader = new Shader(EMITSHADER_VS, EMITSHADER_FS);

		// note that we update the lamp's position attribute's stride to reflect the updated buffer data
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}
	else if (sh == RenderShader::EMITRGBA) {
		shader = new Shader(EMITRGBASHADER_VS, EMITRGBASHADER_FS);

		// note that we update the lamp's position attribute's stride to reflect the updated buffer data
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
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
	for (auto const& x : vec4Refs) shader->setVec4(x.first, *x.second);
	for (auto const& x : mat4Refs) shader->setMat4(x.first, *x.second);


	// render
	glBindVertexArray(VAO);
	//glDrawArrays(GL_TRIANGLES, 0, 36);
	glDrawElements(GL_TRIANGLES, vertexData.size(), GL_UNSIGNED_INT, 0);

	checkErrors("Draw");
}

void Model::draw(Shader* customShader) {

	customShader->use();

	//Set all references
	for (auto const& x : vec3Refs) customShader->setVec3(x.first, *x.second);
	for (auto const& x : vec4Refs) shader->setVec4(x.first, *x.second);
	for (auto const& x : mat4Refs) customShader->setMat4(x.first, *x.second);

	// render
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, vertexData.size(), GL_UNSIGNED_INT, 0);

	checkErrors("Draw(CustomShader)");
}

void Model::scale(float scale) {
	model = glm::scale(model, glm::vec3(scale));
}

void Model::scale(glm::vec3 scale) {
	model = glm::scale(model, scale);
}

void Model::resetPSR(){ //Reset position scale and rotation
	this->model = glm::mat4();
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

glm::vec3 Model::getPosition() {
	return glm::vec3(model[3].x, model[3].y, model[3].z);
}

//Note that this will change scale of object
//Note: assumes that y is forward.
void Model::lookAt(glm::vec3 target) {
	glm::vec3 front = glm::normalize(target - this->getPosition());
	glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), front));
	glm::vec3 up = glm::cross(front, right);

	model[0].x = right.x;
	model[0].y = right.y;
	model[0].z = right.z;

	model[2].x = up.x;
	model[2].y = up.y;
	model[2].z = up.z;

	model[1].x = front.x;
	model[1].y = front.y;
	model[1].z = front.z;
}

void Model::fromFile(std::string inputfile) {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile.c_str());

	if (!err.empty()) print(this, "Error reading File: " + err);

	if (!ret) { exit(1); }

	//Format conversion:

	for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
		indices.push_back(shapes[0].mesh.indices[i].vertex_index);
	}

	for (int i = 0; i+2 < attrib.vertices.size(); i+=3) {
		int current_vindex = i / 3;
		//Get vertex
		float x, y, z;
		float nx, ny, nz;
		x = attrib.vertices[i + 0];
		y = attrib.vertices[i + 1];
		z = attrib.vertices[i + 2];

		//Push vertex onto vector
		vertexData.push_back(x);
		vertexData.push_back(y);
		vertexData.push_back(z);

		//Get respective normal index (Needs to be done like this because tinyobjloader gives us a separate normals_indices and we want them all in one array)
		for (int j = 0; j < shapes[0].mesh.indices.size(); j++) {
			if (shapes[0].mesh.indices[j].vertex_index == current_vindex) {
				int normalindex = shapes[0].mesh.indices[j].normal_index;

				//Push normal onto vector
				nx = attrib.normals[3 * normalindex + 0];
				ny = attrib.normals[3 * normalindex + 1];
				nz = attrib.normals[3 * normalindex + 2];
				vertexData.push_back(nx);
				vertexData.push_back(ny);
				vertexData.push_back(nz);
				break;
			}
		}
	}

	//Format conversion done

	print(this, "File loaded. Size of vertexData: " + vertexData.size());
}