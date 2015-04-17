#include "Camera.h"
#include "glfw3.h"
#include "glm\ext.hpp"

extern GLFWwindow * window;

Camera::Camera()
{
	mouseSpeed = 0.005f;
	cameraSpeed = 3.0f;
	horizontalAngle = 0.0f;
	verticalAngle = 0.0f;
}


Camera::~Camera()
{
}


void Camera::Update(float dt)
{
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// Reset mouse position for next frame
	glfwSetCursorPos(window, 640 / 2.0, 480 / 2.0);

	// Compute new orientation
	
	horizontalAngle += mouseSpeed * float(640 / 2 - xpos);
	verticalAngle += mouseSpeed * float(480 / 2 - ypos);

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction = glm::vec3(
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

	// Up vector
	glm::vec3 up = glm::cross(right, direction);
	
	// Move forward
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
		cameraPosition += direction * dt * cameraSpeed;
	}
	// Move backward
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
		cameraPosition -= direction * dt * cameraSpeed;
	}
	// Strafe right
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
		cameraPosition += right * dt * cameraSpeed;
	}
	// Strafe left
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
		cameraPosition -= right * dt * cameraSpeed;
	}

	projMat = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 150.0f);

	viewMat = glm::lookAt(
		cameraPosition,           // Camera is here
		cameraPosition + direction, // and looks here : at the same position, plus "direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
	);
}
