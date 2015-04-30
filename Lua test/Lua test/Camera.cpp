#include "Camera.h"
#include "glfw3.h"
#include "glm\ext.hpp"
#include <iostream>

extern GLFWwindow * window;

Camera::Camera()
{
	mouseSpeed = 0.005f;
	cameraSpeed = 3.0f;
	horizontalAngle = 0.0f;
	verticalAngle = 0.0f;
	type = ISOMETRIC;
}


Camera::~Camera()
{
}


void Camera::Update(float dt)
{
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	glm::vec3 up;
	glm::vec3 right;
	if (type == FREE)
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
		 cameraSpeed = 3.0f;
	}
	else if (type == TOP_DOWN)
	{
		up = glm::vec3(0.0f, 0.0f, 1.0f);
		direction = glm::vec3(0.0f, -1.0f, 0.0f);
		right = glm::vec3(-1.0f, 0.0f, 0.0f);
		cameraPosition.y = 30.0f;
		cameraSpeed = 6.0f;
	}
	else
	{
		up = glm::vec3(0.0f, 1.0f, 0.0f);
		horizontalAngle = -728.854736f;
		verticalAngle = -120.005180f;
		direction = glm::vec3(
			cos(verticalAngle) * sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
		);
		right = glm::vec3(-1.0f, 0.0f, 0.0f);
		cameraPosition.y = 6.20885277f;		
		cameraSpeed = 3.0f;
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

glm::vec3 Camera::ScreenToWorldCoord(float x, float y)
{	
	glm::mat4 inverse = glm::inverse(projMat * viewMat);

	// Transform to a value between -1 and 1
	// 320.0f is half the screen width
	// 240 is half the screen height
	x -= 320.0f;
	x /= 320.0f;
	y -= 240.0f;
	y /= 240.0f;
	x = glm::clamp(x, -1.0f, 1.0f);
	y = glm::clamp(y, -1.0f, 1.0f);
	y *= -1.0f; // invert this axis

	glm::vec4 screenPos(x, y, 0.0f, 1.0f);
	glm::vec4 worldPos = inverse * screenPos;
	worldPos *= 1.0f / worldPos.w;

	return glm::vec3(worldPos);
}

