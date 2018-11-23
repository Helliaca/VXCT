#pragma once

#include "Scene.h"

enum class sp_nodetype { SCENE, MODEL, POSITION, X, Y, Z, NAME, SCALE, ACTIVE, PATH, MATERIAL, SHININESS, SPECULAR_STR, AMBIENT_STR, DIFFUSE_STR, SHADER, COLOR, R, G, B, ERR };
enum class sp_datatype { NODE, FLOAT, STRING, BOOL, NONE };

class sp_node
{
public:
	sp_nodetype type;
	sp_datatype datatype;
	void setData(std::string str);
	void setData(float f);
	void setData(bool b);
	std::vector<sp_node*> children;
	sp_node();
	~sp_node();
	float GetData_f();
	std::string GetData_s();
	bool GetData_b();

private:
	std::string s;
	float f;
	bool b;
};

class SceneParser : IOobject
{
public:
	SceneParser();
	~SceneParser();
	sp_node* root;
	void parse(std::string path);
	Scene* to_scene();
private:
};