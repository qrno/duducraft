#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// default values
const float YAW			= -90.0f;
const float PITCH		=   0.0f;
const float SPEED		=   2.5f;
const float SENSITIVITY =   0.1f;
const float FOV			=  45.0f;

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN,
};

class Camera {
public:
	glm::vec3 Position;
	glm::vec3 Up;
	glm::vec3 Front;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	float Fov;
	float Yaw;
	float Pitch;

	float MovementSpeed;
	float MouseSensitivity;

	Camera(glm::vec3 position = glm::vec3(0.0f),
		   glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f),
		   float yaw = YAW, float pitch = PITCH) :
				Front(glm::vec3(0.0f, 0.0f, -1.0f)),
				MovementSpeed(SPEED),
				MouseSensitivity(SENSITIVITY),
				Fov(FOV) {
		Position = position;
		WorldUp = worldUp;
		Yaw = yaw;
		Pitch = pitch;

		updateCameraVectors();
	}

	glm::mat4 GetViewMatrix() {
		return glm::lookAt(Position, Position + Front, Up);
	}

	void ProcessKeyboard(Camera_Movement direction, float deltaTime) {
		float velocity = MovementSpeed * deltaTime;

		if (direction == FORWARD)
			Position += noHeight(Front) * velocity;
		if (direction == BACKWARD)
			Position -= noHeight(Front) * velocity;
		if (direction == RIGHT)
			Position -= noHeight(Right) * velocity;
		if (direction == LEFT)
			Position += noHeight(Right) * velocity;
		if (direction == UP)
			Position -= WorldUp * velocity;
		if (direction == DOWN)
			Position += WorldUp * velocity;
	}

	void ProcessMouseMovement(float xoffset, float yoffset) {
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw	  -= xoffset;
		Pitch += yoffset;

		const float MAX_PITCH =	 89.5f;
		const float MIN_PITCH =	-89.5f;

		if (Pitch > MAX_PITCH)
			Pitch = MAX_PITCH;
		if (Pitch < MIN_PITCH)
			Pitch = MIN_PITCH;

		updateCameraVectors();
	}

	void ProcessMouseScroll(float yoffset) {
		Fov -= (float) yoffset;

		const float MAX_FOV = 115.0f;
		const float MIN_FOV =  20.0f;

		if (Fov > MAX_FOV)
			Fov = MAX_FOV;
		if (Fov < MIN_FOV)
			Fov = MIN_FOV;
	}

private:
	// New values for Front, Right, Up based on Yaw and Pitch
	void updateCameraVectors() {
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);

		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up    = glm::normalize(glm::cross(Front, Right));
	}

	glm::vec3 noHeight(glm::vec3 v) {
		return glm::normalize(glm::vec3(v.x, 0, v.z));
	}
};

#endif
