#include "SceneParser.h"

#include <istream>
#include <iterator>
#include <map>

//Maps Node type to identifier that characterizes it in scene.txt
std::map<std::string, sp_nodetype> TT_ids{
	{ "scene", sp_nodetype::SCENE },
	{ "model", sp_nodetype::MODEL },
	{ "position", sp_nodetype::POSITION },
	{ "rotation", sp_nodetype::ROTATION },
	{ "x", sp_nodetype::X },
	{ "y", sp_nodetype::Y },
	{ "z", sp_nodetype::Z },
	{ "scale", sp_nodetype::SCALE },
	{ "name", sp_nodetype::NAME },
	{ "active", sp_nodetype::ACTIVE },
	{ "path", sp_nodetype::PATH },
	{ "material", sp_nodetype::MATERIAL },
	{ "shininess", sp_nodetype::SHININESS },
	{ "specular_str", sp_nodetype::SPECULAR_STR },
	{ "diffuse_str", sp_nodetype::DIFFUSE_STR },
	{ "ambient_str", sp_nodetype::AMBIENT_STR },
	{ "shader", sp_nodetype::SHADER },
	{ "color", sp_nodetype::COLOR },
	{ "r", sp_nodetype::R },
	{ "g", sp_nodetype::G },
	{ "b", sp_nodetype::B },
	{ "scale_x", sp_nodetype::SCALE_X },
	{ "scale_y", sp_nodetype::SCALE_Y },
	{ "scale_z", sp_nodetype::SCALE_Z },
	{ "primitive", sp_nodetype::PRIMITIVE },
	{ "static", sp_nodetype::STATIC },
	{ "light", sp_nodetype::LIGHT },
};

//Maps Node type to data type that is stored on this node.
std::map<sp_nodetype, sp_datatype> Datatypes_ids{
	{ sp_nodetype::MODEL, sp_datatype::NODE },
	{ sp_nodetype::SCENE, sp_datatype::NODE },
	{ sp_nodetype::POSITION, sp_datatype::NODE },
	{ sp_nodetype::ROTATION, sp_datatype::NODE },
	{ sp_nodetype::X, sp_datatype::FLOAT },
	{ sp_nodetype::Y, sp_datatype::FLOAT },
	{ sp_nodetype::Z, sp_datatype::FLOAT },
	{ sp_nodetype::SCALE, sp_datatype::FLOAT },
	{ sp_nodetype::NAME, sp_datatype::STRING },
	{ sp_nodetype::ACTIVE, sp_datatype::BOOL },
	{ sp_nodetype::PATH, sp_datatype::STRING },
	{ sp_nodetype::MATERIAL, sp_datatype::NODE },
	{ sp_nodetype::SHININESS, sp_datatype::FLOAT },
	{ sp_nodetype::SPECULAR_STR, sp_datatype::FLOAT },
	{ sp_nodetype::DIFFUSE_STR, sp_datatype::FLOAT },
	{ sp_nodetype::AMBIENT_STR, sp_datatype::FLOAT },
	{ sp_nodetype::SHADER, sp_datatype::STRING },
	{ sp_nodetype::COLOR, sp_datatype::NODE },
	{ sp_nodetype::R, sp_datatype::FLOAT },
	{ sp_nodetype::G, sp_datatype::FLOAT },
	{ sp_nodetype::B, sp_datatype::FLOAT },
	{ sp_nodetype::SCALE_X, sp_datatype::FLOAT },
	{ sp_nodetype::SCALE_Y, sp_datatype::FLOAT },
	{ sp_nodetype::SCALE_Z, sp_datatype::FLOAT },
	{ sp_nodetype::PRIMITIVE, sp_datatype::STRING },
	{ sp_nodetype::STATIC, sp_datatype::BOOL },
	{ sp_nodetype::LIGHT, sp_datatype::NODE },
};

sp_nodetype SceneParser::NodeTypes_lookup(std::string str) {
	std::map<std::string, sp_nodetype>::iterator it = TT_ids.find(str);
	if (it == TT_ids.end()) {
		print(this, "ERR: Invalid token not in lookup-table: " + str);
		return sp_nodetype::ERR;
	}
	else return it->second;
}

sp_datatype SceneParser::DataTypes_lookup(sp_nodetype nt) {
	std::map<sp_nodetype, sp_datatype>::iterator it = Datatypes_ids.find(nt);
	if (it == Datatypes_ids.end()) {
		print(this, "ERR: Invalid token type not in lookup-table.");
		return sp_datatype::NONE;
	}
	else return it->second;
}

void sp_node::setData(std::string str) { this->s = str; }
void sp_node::setData(float f) { this->f = f; }
void sp_node::setData(bool b) { this->b = b; }

sp_node::sp_node() {}
sp_node::~sp_node() {}

float sp_node::GetData_f() { return this->f; }
std::string sp_node::GetData_s() { return this->s; }
bool sp_node::GetData_b() { return this->b; }

SceneParser::SceneParser() : IOobject("SceneParser")
{
	parsed = false;
}


SceneParser::~SceneParser()
{
}

bool SceneParser::parse(std::string path) {
	std::ifstream inFile;

	//throw exception if failbit gets set
	std::ios_base::iostate exceptionMask = inFile.exceptions() | std::ios::failbit;
	inFile.exceptions(exceptionMask);

	//try opening file
	try {
		inFile.open(path); //open file
	}
	catch (std::ios_base::failure& e) {
		print(this, "ERR: Could not open file: " + path);
		return false;
	}

	std::stringstream strStream;
	strStream << inFile.rdbuf(); //get all text
	std::string input = strStream.str(); //copy into input

	//Separate into tokens
	std::istringstream iss(input);
	std::vector<std::string> tokens(std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>());

	root = new sp_node();
	root->children = parse_iterate(tokens);
	print(this, "Parsing Complete: " + path);
	return true;
}


std::vector<sp_node*> SceneParser::parse_iterate(std::vector<std::string> tokens) {
	std::vector<sp_node*> ret;

	int depth = 0;
	for (int i = 0; i < tokens.size(); i++) {
		if (tokens[i] == "=" && depth == 0) {
			std::string identifier = tokens[i - 1];

			sp_node* new_node = new sp_node();

			new_node->type = NodeTypes_lookup(identifier);
			new_node->datatype = DataTypes_lookup(new_node->type);

			switch (new_node->datatype)
			{
			case sp_datatype::NODE: {
				new_node->children = parse_iterate(getFirstComplexNode(tokens, i + 1));
				break;
			}
			case sp_datatype::FLOAT: {
				new_node->setData((float)strtof(tokens[i + 1].c_str(), 0));
				break;
			}
			case sp_datatype::STRING: {
				new_node->setData(tokens[i + 1]);
				break;
			}
			case sp_datatype::BOOL: {
				new_node->setData(tokens[i + 1] == "true");
				break;
			}
			default:
				print(this, "Invalid token: " + tokens[i]);
				break;
			}


			ret.push_back(new_node);
		}
		if (tokens[i] == "{") depth++;
		if (tokens[i] == "}") depth--;
	}
	return ret;
}

std::vector<std::string> SceneParser::getFirstComplexNode(std::vector<std::string> tokens, int offset) {
	int i_start = -1;
	int i_end = -1;
	int depth = 0;
	for (int i = offset; i < tokens.size(); i++) {
		if (tokens[i] == "{") {
			if (depth == 0 && i_start<0) {
				i_start = i;
			}
			depth++;
		}
		else if (tokens[i] == "}") {
			depth--;
			if (depth == 0 && i_end<0) {
				i_end = i;
				break;
			}
		}
	}
	if (i_start == -1) print(this, "ERR: Expected '{'");
	if (i_end == -1) std::cout << "ERR: Expected '}'" << std::endl;
	std::vector<std::string>::const_iterator first = tokens.begin() + i_start + 1;
	std::vector<std::string>::const_iterator last = tokens.begin() + i_end;
	std::vector<std::string> ret(first, last);
	return ret;
}

Scene* SceneParser::to_scene() {
	if (root->children.size() > 1) print(this, "WARN: More than one scene was declared, but only the first one will be used.");

	G::SceneLighting = new Lighting(); //flush old lightsources.
	Scene* ret = new Scene();
	sp_node *scene_node = root->children[0];
	processComplexNode_toscene(ret, scene_node);
	for (int i = 0; i < scene_node->children.size(); i++) processComplexNode_toscene(ret, scene_node->children[i]);
	return ret;
}

void SceneParser::processComplexNode_toscene(Scene* scene, sp_node* node) {
	switch (node->type)
	{
	case sp_nodetype::SCENE: {
		if (hasChildNodeOfType(node, sp_nodetype::NAME)) scene->name = getChildOfType(node, sp_nodetype::NAME)->GetData_s();
		break; }

	case sp_nodetype::LIGHT: {
		PointLight* l = new PointLight();
		if (hasChildNodeOfType(node, sp_nodetype::POSITION)) {
			sp_node* pos_node = getChildOfType(node, sp_nodetype::POSITION);
			glm::vec3 offset;
			offset.x = getChildOfType(pos_node, sp_nodetype::X)->GetData_f();
			offset.y = getChildOfType(pos_node, sp_nodetype::Y)->GetData_f();
			offset.z = getChildOfType(pos_node, sp_nodetype::Z)->GetData_f();
			l->position = offset;
		}
		if (hasChildNodeOfType(node, sp_nodetype::COLOR)) {
			sp_node* pos_node = getChildOfType(node, sp_nodetype::COLOR);
			glm::vec3 c;
			c.x = getChildOfType(pos_node, sp_nodetype::R)->GetData_f();
			c.y = getChildOfType(pos_node, sp_nodetype::G)->GetData_f();
			c.z = getChildOfType(pos_node, sp_nodetype::B)->GetData_f();
			l->color = c;
		}
		G::SceneLighting->add_pointLight(l);
		break; }

	case sp_nodetype::MODEL: {
		//path, shader, name
		std::string path = OBJ_SCENE_CUBE1;
		std::string tmp_shader="COLOR";
		RenderShader shader = RenderShader::COLOR;
		std::string name = "UnnamedModel";
		if (hasChildNodeOfType(node, sp_nodetype::NAME)) name = getChildOfType(node, sp_nodetype::NAME)->GetData_s();
		if (hasChildNodeOfType(node, sp_nodetype::SHADER)) tmp_shader = getChildOfType(node, sp_nodetype::SHADER)->GetData_s();
		if (tmp_shader == "COLOR") shader = RenderShader::COLOR;
		if (tmp_shader == "EMIT") shader = RenderShader::EMIT;
		if (tmp_shader == "EMITRGBA") shader = RenderShader::EMITRGBA;
		if (tmp_shader == "VOX") shader = RenderShader::VOX;

		Model* new_model;
		if (hasChildNodeOfType(node, sp_nodetype::PATH)) {
			path = getChildOfType(node, sp_nodetype::PATH)->GetData_s();
			new_model = new Model(name, shader, path);
		}
		else if (hasChildNodeOfType(node, sp_nodetype::PRIMITIVE)) {
			std::string primitive_type = getChildOfType(node, sp_nodetype::PRIMITIVE)->GetData_s();
			if (primitive_type == "cube") new_model = new Model(name, shader, defaultModels::cube_indices, defaultModels::cube_vertexData);
			else { print(this, "Primitive type not recognized: " + primitive_type); return; }
		}
		else { print(this, "Model has neither path nor primitive: " + name); return; }

		//active
		if (hasChildNodeOfType(node, sp_nodetype::ACTIVE) && !getChildOfType(node, sp_nodetype::ACTIVE)->GetData_b()) new_model->active = false;

		//set shader unifrom references
		new_model->addMat4Reference("model_u", &new_model->model);
		new_model->addMat4Reference("proj_u", &G::SceneCamera->projMatrix);
		new_model->addMat4Reference("view_u", &G::SceneCamera->viewMatrix);
		new_model->addVec3Reference("viewPos", &G::SceneCamera->Position);
		new_model->addlightingReference("lighting", G::SceneLighting);

		//Material + color
		if (hasChildNodeOfType(node, sp_nodetype::MATERIAL)) {
			sp_node* material_node = getChildOfType(node, sp_nodetype::MATERIAL);
			if (hasChildNodeOfType(material_node, sp_nodetype::AMBIENT_STR)) new_model->material->ambient_str = getChildOfType(material_node, sp_nodetype::AMBIENT_STR)->GetData_f();
			if (hasChildNodeOfType(material_node, sp_nodetype::DIFFUSE_STR)) new_model->material->diffuse_str = getChildOfType(material_node, sp_nodetype::DIFFUSE_STR)->GetData_f();
			if (hasChildNodeOfType(material_node, sp_nodetype::SPECULAR_STR)) new_model->material->specular_str = getChildOfType(material_node, sp_nodetype::SPECULAR_STR)->GetData_f();
			if (hasChildNodeOfType(material_node, sp_nodetype::SHININESS)) new_model->material->shininess = getChildOfType(material_node, sp_nodetype::SHININESS)->GetData_f();
			if (hasChildNodeOfType(material_node, sp_nodetype::COLOR)) {
				sp_node* color_node = getChildOfType(material_node, sp_nodetype::COLOR);
				new_model->material->color.r = getChildOfType(color_node, sp_nodetype::R)->GetData_f();
				new_model->material->color.g = getChildOfType(color_node, sp_nodetype::G)->GetData_f();
				new_model->material->color.b = getChildOfType(color_node, sp_nodetype::B)->GetData_f();
			}
		}
		new_model->addMaterialReference("material", new_model->material);
		new_model->addVsettingsReference("settings", G::VoxLightSettings);

		//Position
		if (hasChildNodeOfType(node, sp_nodetype::POSITION)) {
			sp_node* pos_node = getChildOfType(node, sp_nodetype::POSITION);
			glm::vec3 offset;
			offset.x = getChildOfType(pos_node, sp_nodetype::X)->GetData_f();
			offset.y = getChildOfType(pos_node, sp_nodetype::Y)->GetData_f();
			offset.z = getChildOfType(pos_node, sp_nodetype::Z)->GetData_f();
			new_model->setPosition(offset);
		}

		//Position
		if (hasChildNodeOfType(node, sp_nodetype::ROTATION)) {
			sp_node* pos_node = getChildOfType(node, sp_nodetype::ROTATION);
			glm::vec3 rot;
			rot.x = getChildOfType(pos_node, sp_nodetype::X)->GetData_f();
			rot.y = getChildOfType(pos_node, sp_nodetype::Y)->GetData_f();
			rot.z = getChildOfType(pos_node, sp_nodetype::Z)->GetData_f();
			new_model->rotate(rot);
		}

		//Scale
		if (hasChildNodeOfType(node, sp_nodetype::SCALE)) new_model->scale(getChildOfType(node, sp_nodetype::SCALE)->GetData_f());
		glm::vec3 localScale = glm::vec3(1.0f);
		if (hasChildNodeOfType(node, sp_nodetype::SCALE_X)) localScale.x = getChildOfType(node, sp_nodetype::SCALE_X)->GetData_f();
		if (hasChildNodeOfType(node, sp_nodetype::SCALE_Y)) localScale.y = getChildOfType(node, sp_nodetype::SCALE_Y)->GetData_f();
		if (hasChildNodeOfType(node, sp_nodetype::SCALE_Z)) localScale.z = getChildOfType(node, sp_nodetype::SCALE_Z)->GetData_f();
		new_model->scale(localScale);

		//Static
		if (hasChildNodeOfType(node, sp_nodetype::STATIC)) new_model->is_static = getChildOfType(node, sp_nodetype::STATIC)->GetData_b();

		scene->addObject(new_model);
		break; }
	default:
		break;
	}
}

bool SceneParser::hasChildNodeOfType(sp_node* node, sp_nodetype type) {
	for (int i = 0; i < node->children.size(); i++) if (node->children[i]->type == type) return true;
	return false;
}

sp_node* SceneParser::getChildOfType(sp_node* node, sp_nodetype type) {
	if (node->children.size() <= 0) return NULL;
	for (int i = 0; i < node->children.size(); i++) if (node->children[i]->type == type) return node->children[i];
	print(this, "ERR: Expected token: " + Node_to_string(type));
	return node->children[0];
}

std::string SceneParser::Node_to_string(sp_nodetype t) {
	for (std::map<std::string, sp_nodetype>::iterator it = TT_ids.begin(); it != TT_ids.end(); it++) {
		if (it->second == t) return it->first;
	}
}