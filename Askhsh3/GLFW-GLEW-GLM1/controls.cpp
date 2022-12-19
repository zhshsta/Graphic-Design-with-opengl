// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
extern GLFWwindow* window; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include "controls.hpp"

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix() {
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix() {
	return ProjectionMatrix;
}


// Initial position : on +Z
glm::vec3 position = glm::vec3(0, 0, 1000);
// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;

float speed = 13.0f; // 3 units / second
float mouseSpeed = 0.005f;



void computeMatricesFromInputs() {

	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position
	double xpos, ypos;
	//glfwGetCursorPos(window, &xpos, &ypos);

	// Reset mouse position for next frame
	//glfwSetCursorPos(window, 1024/2, 768/2);

	// Compute new orientation
	//horizontalAngle += mouseSpeed * float(800/2 - xpos );
	//verticalAngle   += mouseSpeed * float( 800/2 - ypos );

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	);
	glm::vec3 panw = glm::vec3(
		0,
		sin(verticalAngle - 3.14f / 2.0f),
		cos(verticalAngle - 3.14f / 2.0f)
	);
	//glm::mat4 m_rotation = glm::mat4(1.0);
	//glm::vec3 left = glm::vec3(1, 0, 0);
	//glm::vec3 left_rotated = m_rotation * left;

	glm::vec3 kentro = glm::vec3(0, 0, 0);
	// Up vector
	glm::vec3 up = glm::cross(right, direction);
	glm::vec3 Xaxis = glm::vec3(
		horizontalAngle,
		cos(verticalAngle) - sin(initialFoV),
		sin(verticalAngle) - cos(initialFoV)
	);
	glm::vec3 Yaxis = glm::vec3(
		cos(horizontalAngle) + sin(initialFoV),
		verticalAngle,
		-sin(horizontalAngle) + cos(initialFoV)
	);

	// Move forward
	if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) {
		position += Yaxis * deltaTime * speed;
	}
	// Move backward
	if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) {
		position -= Yaxis * deltaTime * speed;
	}
	// Strafe right
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		//glm::mat4 Model(1.f);
		//Model = glm::translate(Model, glm::vec3(0.f, 0.f, 0.f));
		//Model = glm::rotate(Model, glm::radians(10.f), glm::vec3(1.f, 0.f, 0.f));
		//Model = glm::scale(Model, glm::vec3(2.f));
		position += right * deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		//glm::mat4 MVP = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 0.0f, 0.0f));
		//glm::vec4 position(10.0f,10.0f,10.0f,0.0f);
		//glm::vec4 transformedVector = MVP * position;
		//position = glm::rotate(position, deltaTime * speed, verticalAngle);
		position -= right * deltaTime * speed;
	}
	// Strafe PANW
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		position += panw * deltaTime * speed;
	}
	// Strafe KATW
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		position -= panw * deltaTime * speed;
	}

	float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	ViewMatrix = glm::lookAt(
		position,           // Camera is here
		vec3(0, 0, 0), // and looks here : at the same position, plus "direction"
		vec3(0, 0, 1)                  // Head is up (set to 0,-1,0 to look upside-down)
	);

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}