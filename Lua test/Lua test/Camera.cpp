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
	type = TOP_DOWN;
}


Camera::~Camera()
{
}


void Camera::Update(float dt)
{
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	glm::vec3 up;
	glm::vec3 direction;
	glm::vec3 right;
	if (type != TOP_DOWN)
	{
		// Reset mouse position for next frame
		glfwSetCursorPos(window, 640 / 2.0, 480 / 2.0);

		// Compute new orientation	
		horizontalAngle += mouseSpeed * float(640 / 2 - xpos);
		verticalAngle += mouseSpeed * float(480 / 2 - ypos);

		// Direction : Spherical coordinates to Cartesian coordinates conversion
		direction = glm::vec3(
			cos(verticalAngle) * sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
		);
	
		// Right vector
		right = glm::vec3(
			sin(horizontalAngle - 3.14f / 2.0f),
			0,
			cos(horizontalAngle - 3.14f / 2.0f)
		);

		// Up vector	 
		 up = glm::cross(right, direction);
	}
	else
	{
		up = glm::vec3(0.0f, 0.0f, 1.0f);
		direction = glm::vec3(0.0f, -1.0f, 0.0f);
		right = glm::vec3(-1.0f, 0.0f, 0.0f);
		cameraPosition.y = 30.0f;
		cameraSpeed = 6.0f;
	}
	
	// Move forward
	if (type == FREE)
	{
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
			cameraPosition += direction * dt * cameraSpeed;
		}

		// Move backward
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
			cameraPosition -= direction * dt * cameraSpeed;
		}
	}
	if (type == ISOMETRIC || type == TOP_DOWN) 
	{
		// Move forward along the z axis
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
			cameraPosition += glm::vec3(0.0f, 0.0f, 1.0f) * dt * cameraSpeed;
		}

		// Move backward along the z axis
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
			cameraPosition -= glm::vec3(0.0f, 0.0f, 1.0f) * dt * cameraSpeed;
		}
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
