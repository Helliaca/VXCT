#pragma once

#include "Scene.h"

enum class sp_nodetype { SCENE, MODEL, POSITION, ROTATION, X, Y, Z, NAME, SCALE, ACTIVE, PATH, MATERIAL, SHININESS, SPECULAR_STR, AMBIENT_STR, DIFFUSE_STR, SHADER, COLOR, R, G, B, SCALE_X, SCALE_Y, SCALE_Z, PRIMITIVE, STATIC, LIGHT, ERR };
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
	bool parse(std::string path);
	Scene* to_scene();
private:
	sp_nodetype NodeTypes_lookup(std::string str);
	sp_datatype DataTypes_lookup(sp_nodetype nt);
	std::vector<sp_node*> parse_iterate(std::vector<std::string> tokens);
	std::vector<std::string> getFirstComplexNode(std::vector<std::string> tokens, int offset);
	void processComplexNode_toscene(Scene* scene, sp_node* node);
	bool hasChildNodeOfType(sp_node* node, sp_nodetype type);
	sp_node* getChildOfType(sp_node* node, sp_nodetype type);
	std::string Node_to_string(sp_nodetype t);
	bool parsed;
};