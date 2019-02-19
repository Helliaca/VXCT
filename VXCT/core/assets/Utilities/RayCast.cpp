#include "RayCast.h"


RayCast::RayCast(Model* model, glm::vec3 origin, glm::vec3 dir) : IOobject("Unnamed Raycast")
{
	this->model = model;
	this->origin = origin;
	this->dir = dir;
}


RayCast::~RayCast()
{
}


bool RayCast::execute(glm::vec3& loc_out, glm::vec3& nrm_out)
{
	glm::vec3 closestPos;
	glm::vec3 closestPos_nrm;
	glm::vec3 pos;
	glm::vec3 nrm;
	bool hit = false;

	//NOTE: This will depend on the implementation of Model.cpp (*6 cause the stride is 6 (position=3, normal=3))
	for (int i = 0; i+2 < model->indices.size(); i+=3) {
		glm::vec3 vert0 = glm::vec3(model->vertexData[model->indices[i + 0] * 6 + 0], model->vertexData[model->indices[i + 0] * 6 + 1], model->vertexData[model->indices[i + 0] * 6 + 2]);
		glm::vec3 vert1 = glm::vec3(model->vertexData[model->indices[i + 1] * 6 + 0], model->vertexData[model->indices[i + 1] * 6 + 1], model->vertexData[model->indices[i + 1] * 6 + 2]);
		glm::vec3 vert2 = glm::vec3(model->vertexData[model->indices[i + 2] * 6 + 0], model->vertexData[model->indices[i + 2] * 6 + 1], model->vertexData[model->indices[i + 2] * 6 + 2]);
		glm::vec3 nrm0 = glm::vec3(model->vertexData[model->indices[i + 0] * 6 + 3], model->vertexData[model->indices[i + 0] * 6 + 4], model->vertexData[model->indices[i + 0] * 6 + 5]);
		glm::vec3 nrm1 = glm::vec3(model->vertexData[model->indices[i + 1] * 6 + 3], model->vertexData[model->indices[i + 1] * 6 + 4], model->vertexData[model->indices[i + 1] * 6 + 5]);
		glm::vec3 nrm2 = glm::vec3(model->vertexData[model->indices[i + 2] * 6 + 3], model->vertexData[model->indices[i + 2] * 6 + 4], model->vertexData[model->indices[i + 2] * 6 + 5]);

		if (TriIntersect(vert0, vert1, vert2, nrm0, nrm1, nrm2, pos, nrm)) {
			if (!hit || glm::distance(origin, pos) < glm::distance(origin, closestPos)) {
				closestPos = pos;
				closestPos_nrm = nrm;
			}
			hit = true;
		}
	}
	if (hit) {
		loc_out = closestPos;
		nrm_out = closestPos_nrm;
		return true;
	}
	return false;
}



//Möller-Trumbore intersection algorithm, See: https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
bool RayCast::TriIntersect(glm::vec3 vert0, glm::vec3 vert1, glm::vec3 vert2, glm::vec3 nrm0, glm::vec3 nrm1, glm::vec3 nrm2, glm::vec3& out_pos, glm::vec3& out_nrm) {
	dir = glm::normalize(dir);
	const float EPSILON = 0.0000001; //Approximation to 0

	glm::vec3 edge1 = vert1 - vert0;
	glm::vec3 edge2 = vert2 - vert0;

	glm::vec3 h = glm::cross(dir, edge2);
	float a = glm::dot(edge1, h);

	if (a > -EPSILON && a < EPSILON) return false; //Ray is parallel to this triangle.

	float f = 1.0f / a;
	glm::vec3 s = origin - vert0;
	float u = f * (glm::dot(s, h));
	if (u < 0.0f || u > 1.0f) return false;

	glm::vec3 q = glm::cross(s, edge1);
	float v = f * glm::dot(dir, q);
	if (v < 0.0f || u + v > 1.0f) return false;

	//Compute point of intersection
	float t = f * glm::dot(edge2, q);
	if (t > EPSILON) { //Ray intersection
		//calc face normal the same way as in vox.gs (!) NOTE: Fragment Normals *can* vary from geometry shader normals.
		glm::vec3 faceNormal = glm::normalize(nrm0 + nrm1 + nrm2);
		float angle = glm::acos(glm::dot(dir, faceNormal));
		if (angle < 1.5708f) return false; //if Angle is smaller than 90 deg we are hitting the back side of the triangle.
		//Set outputs
		out_pos = origin + dir * t;
		out_nrm = faceNormal;
		return true;
	}
	else return false; //Line intersection but no ray intersection.
}