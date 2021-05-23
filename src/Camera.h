#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/vector_angle.hpp"

#include "Renderer.h"


class Camera
{
private:
	// Stores the main vectors of the camera
	glm::vec3 Position;
	glm::vec3 Orientation;

	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

	// flag for the first click
	bool firstClick = true;

	// width and height of the window
	int width;
	int height;

	float speed = 1.0f;
	float sensitivity = 100.0f;

public:
	static constexpr glm::vec3 OrientationFrontal = glm::vec3(0.0f, 0.0f, -1.0f);
	static constexpr glm::vec3 OrientationTopRight = glm::vec3(0.445f, 0.225f, -0.855f);
	static constexpr glm::vec3 OrientationBottomLeft = glm::vec3(-0.445f, -0.225f, -0.855f);
	Camera(int width, int height, glm::vec3 position, glm::vec3 myOrientation = Camera::OrientationFrontal);

	// returns the camera matrix 
	glm::mat4 getCameraMatrix(float FOVdeg, float nearPlane, float farPlane);
	// user input
	void Inputs(GLFWwindow* window);
};


#endif // !CAMERA_CLASS_H
