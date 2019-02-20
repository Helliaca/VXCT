#include "Model.h"



#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

Model::Model(std::string name, RenderShader sh=RenderShader::EMIT, std::string inputfile="") : IOobject(name)
{
	if (inputfile!="") { this->fromFile(inputfile); }
	initialize(sh);
	this->material = new Material();
	is_static = false;
}

Model::Model(std::string name = "unnamedModel", RenderShader sh = RenderShader::EMIT, std::vector<int>indices = {}, std::vector<float>vertexData = {}) : IOobject(name)
{
	this->vertexData = vertexData;
	this->indices = indices;
	initialize(sh);
	this->material = new Material();
	is_static = false;
}

void Model::initialize(RenderShader sh) {
	//Generate VAO, VBO and EBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertexData.size(), &vertexData[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*indices.size(), &indices[0], GL_STATIC_DRAW);

	// Set attributes in accordance to the shader
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
	for (auto const& x : lightingRefs) shader->setLighting(x.first, *x.second);
	for (auto const& x : materialRefs) shader->setMaterial(x.first, *x.second);
	for (auto const& x : vsettingsRefs) shader->setVsettings(x.first, *x.second);

	// render
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, vertexData.size(), GL_UNSIGNED_INT, 0);

	checkErrors("Draw");
}

void Model::draw(Shader* customShader) {
	Shader* tmp = this->shader;
	this->shader = customShader;
	this->draw();
	this->shader = tmp;
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

void Model::rotate(glm::vec3 euler_angles) {
	model = glm::rotate(model, glm::radians(euler_angles.x), glm::vec3(1.0, 0.0, 0.0));
	model = glm::rotate(model, glm::radians(euler_angles.y), glm::vec3(0.0, 1.0, 0.0));
	model = glm::rotate(model, glm::radians(euler_angles.z), glm::vec3(0.0, 0.0, 1.0));
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
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(inputfile, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}
	//directory = path.substr(0, path.find_last_of('/'));
	aiMesh* mesh = scene->mMeshes[0];
	for (int i = 0; i < mesh->mNumVertices; i++) {
		vertexData.push_back(mesh->mVertices[i].x);
		vertexData.push_back(mesh->mVertices[i].y);
		vertexData.push_back(mesh->mVertices[i].z);

		vertexData.push_back(mesh->mNormals[i].x);
		vertexData.push_back(mesh->mNormals[i].y);
		vertexData.push_back(mesh->mNormals[i].z);
	}
	for (int i = 0; i < mesh->mNumFaces; i++) {
		for (int j = 0; j < mesh->mFaces[i].mNumIndices; j++) {
			indices.push_back(mesh->mFaces[i].mIndices[j]);
		}
	}
	

}