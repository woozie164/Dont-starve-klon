#pragma once
#define GLM_MESSAGES
#define GLM_MESSAGE_CORE_INCLUDED_DISPLAYED
#include "glm\glm.hpp"

class Camera
{
	enum CAMERA_TYPE
	{
		FREE,
		ISOMETRIC,
		TOP_DOWN,
	};
public:
	glm::vec3 cameraPosition;
	glm::mat4 viewMat;
	glm::mat4 projMat;
	float mouseSpeed;
	float cameraSpeed;
	float horizontalAngle;
	float verticalAngle;
	CAMERA_TYPE type;

	Camera();
	~Camera();

	void Update(float dt);
	//glm::vec3 GetCameraPosition() { return glm::vec3(viewMat[3][0], viewMat[3][1], viewMat[3][2]); }
};

