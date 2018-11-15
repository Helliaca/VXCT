#include "globals.h"

namespace G {
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	float lastX = WIN_WIDTH / 2.0f;
	float lastY = WIN_HEIGHT / 2.0f;
	bool firstMouse = true;

	Camera* SceneCamera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
	PointLight* SceneLight = new PointLight(glm::vec3(0.0f, 0.4f, 0.4f), glm::vec3(1.0f, 1.0f, 1.0f));
	VoxSettings* VoxLightSettings = new VoxSettings();
}