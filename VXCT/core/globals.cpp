#include "globals.h"

#include "settings\defs.h"

namespace G {
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	float lastX = WIN_WIDTH / 2.0f;
	float lastY = WIN_HEIGHT / 2.0f;
	bool firstMouse = true;

	Camera* SceneCamera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
	LineRenderer* DebugLine;

	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
	glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
}