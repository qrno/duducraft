#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "lib/camera.h"
#include "lib/shader.h"
#include "lib/image.h"
#include "lib/chunk.h"
#include "lib/cube.h"

#include <iostream>
#include <cmath>
#include <vector>

const int WIN_WIDTH = 1000, WIN_HEIGHT = 1000;

void framebuffer_size_callback(GLFWwindow *window, int w, int h);
bool pressed(GLFWwindow* window, GLenum key);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

const glm::mat4 unit = glm::mat4(1.0f);

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX, lastY;
bool firstMouse = true;

int main() {
	
	// glfw setup
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "duducraft", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);

	// glad setup
	if (not gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// =============================================
	
	Texture dirt(GL_TEXTURE0, "textures/dirt.jpg");
	Texture smiley(GL_TEXTURE1, "textures/awesomeface.png", GL_RGBA);
	
	float vertices[] = {
		-0.5, 0.5, 0.5,0.0, 1.0,
		0.5, 0.5, 0.5,1.0, 1.0,
		0.5,-0.5, 0.5,1.0, 0.0,
		0.5,-0.5, 0.5,1.0, 0.0,
		-0.5,-0.5, 0.5,0.0, 0.0,
		-0.5, 0.5, 0.5,0.0, 1.0,
		0.5, 0.5,-0.5,0.0, 1.0,
		-0.5, 0.5,-0.5,1.0, 1.0,
		-0.5,-0.5,-0.5,1.0, 0.0,
		-0.5,-0.5,-0.5,1.0, 0.0,
		0.5,-0.5,-0.5,0.0, 0.0,
		0.5, 0.5,-0.5,0.0, 1.0,
		-0.5, 0.5,-0.5,0.0, 1.0,
		-0.5, 0.5, 0.5,1.0, 1.0,
		-0.5,-0.5, 0.5,1.0, 0.0,
		-0.5,-0.5, 0.5,1.0, 0.0,
		-0.5,-0.5,-0.5,0.0, 0.0,
		-0.5, 0.5,-0.5,0.0, 1.0,
		0.5, 0.5, 0.5,0.0, 1.0,
		0.5, 0.5,-0.5,1.0, 1.0,
		0.5,-0.5,-0.5,1.0, 0.0,
		0.5,-0.5,-0.5,1.0, 0.0,
		0.5,-0.5, 0.5,0.0, 0.0,
		0.5, 0.5, 0.5,0.0, 1.0,
		-0.5, 0.5,-0.5,0.0, 1.0,
		0.5, 0.5,-0.5,1.0, 1.0,
		0.5, 0.5, 0.5,1.0, 0.0,
		0.5, 0.5, 0.5,1.0, 0.0,
		-0.5, 0.5, 0.5,0.0, 0.0,
		-0.5, 0.5,-0.5,0.0, 1.0,
		0.5,-0.5,-0.5,0.0, 1.0,
		-0.5,-0.5,-0.5,1.0, 1.0,
		-0.5,-0.5, 0.5,1.0, 0.0,
		-0.5,-0.5, 0.5,1.0, 0.0,
		0.5,-0.5, 0.5,0.0, 0.0,
		0.5,-0.5,-0.5,0.0, 1.0,
	};


	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	Shader shader("shader/shader.vert", "shader/shader.frag");
	shader.use();

	shader.setInt("dirt", 0);
	shader.setInt("awesome", 1);

	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(camera.Fov), (float)WIN_HEIGHT/(float)WIN_HEIGHT, 0.1f, 100.0f);

	shader.setMat4("view", view);
	shader.setMat4("projection", projection);

	int height[16][16];
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 16; j++) {
			height[i][j] = 15 - abs(8-i) - abs(8-j);
		}
	}

	/* Chunk chunk; */
	Chunk chunk(height);

	// loop
	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.2f, 0.3f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();

		projection = glm::perspective(glm::radians(camera.Fov), (float)WIN_WIDTH/(float)WIN_HEIGHT, 0.1f, 100.0f);
		shader.setMat4("projection", projection);

		view = camera.GetViewMatrix();
		shader.setMat4("view", view);

		glBindVertexArray(VAO);
		for (int i = 0; i < 16; i++) {
			for (int j = 0; j < 16; j++) {
				for (int k = 0; k < 16; k++) {
					Cube cube = chunk.cubes[i][j][k];
					glm::mat4 model = glm::translate(unit, cube.Position);

					shader.setMat4("model", model);
					shader.setVec3("aColor", cube.B.Color);

					if (not cube.IsAir())
						glDrawArrays(GL_TRIANGLES, 0, 36);
				}
			}
		}
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int w, int h) {
	glViewport(0, 0, w, h);
	std::cout << "Window resized to " << w << " " << h << std::endl;
}

bool pressed(GLFWwindow* window, GLenum key) {
	return glfwGetKey(window, key) == GLFW_PRESS;
}

void processInput(GLFWwindow* window) {
	if (pressed(window, GLFW_KEY_Q)) {
		std::cout << "'q' detected, window should close" << std::endl;
		glfwSetWindowShouldClose(window, true);
	}
	
	if (pressed(window, GLFW_KEY_W))
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (pressed(window, GLFW_KEY_S))
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (pressed(window, GLFW_KEY_A))
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (pressed(window, GLFW_KEY_D))
		camera.ProcessKeyboard(RIGHT, deltaTime);

	if (pressed(window, GLFW_KEY_J))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (pressed(window, GLFW_KEY_K))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (pressed(window, GLFW_KEY_SPACE))
		camera.ProcessKeyboard(UP, deltaTime);
	if (pressed(window, GLFW_KEY_LEFT_SHIFT))
		camera.ProcessKeyboard(DOWN, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = ypos - lastY;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset);
}
