#include "Window.h"

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

Window::Window()
{
	glfwInit(); //Initialize glfw
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //Set opengl version to 4.5 core
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //For MacOS

	//Create glfw window
	window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "WindowName", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);
	//Register framebuffer_size_callback as function that gets called on window resize
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//Register mouse_callback as mouse function stuff
	glfwSetCursorPosCallback(window, mouse_callback);
	//Register scroll_callback as scroll function
	glfwSetScrollCallback(window, scroll_callback);
}

Window::~Window()
{
}

void Window::setPolygonMode(PolygonMode polygonmode, bool doubleSidedFaces) {
	GLenum face, mode;
	face = doubleSidedFaces ? GL_FRONT_AND_BACK : GL_FRONT;
	if (polygonmode == W_VERTICES) mode = GL_POINT;
	else if (polygonmode == W_WIREFRAME) mode = GL_LINE;
	else mode = GL_FILL;
	glPolygonMode(face, mode);
}

bool Window::shouldClose() {
	return glfwWindowShouldClose(this->window);
}

GLFWwindow* Window::getGLFWwindow() {
	return this->window;
}


//Camera movement and stuff
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (G::firstMouse)
	{
		G::lastX = (float)xpos;
		G::lastY = (float)ypos;
		G::firstMouse = false;
	}

	float xoffset = (float)xpos - G::lastX;
	float yoffset = G::lastY - (float)ypos;
	G::lastX = (float)xpos;
	G::lastY = (float)ypos;

	G::SceneCamera->ProcessMouseMovement(xoffset, yoffset);
}

//Zoom on scroll wheel
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	G::SceneCamera->ProcessMouseScroll((float)yoffset);
}

//Function that gets calle when window is resized. Viewport should be adjusted accordingly
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height); //Tell OpenGL size of rendering window
}

//Function to handle all user input
void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		G::SceneCamera->ProcessKeyboard(FORWARD, G::deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		G::SceneCamera->ProcessKeyboard(BACKWARD, G::deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		G::SceneCamera->ProcessKeyboard(LEFT, G::deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		G::SceneCamera->ProcessKeyboard(RIGHT, G::deltaTime);
}