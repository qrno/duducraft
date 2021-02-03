#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "dbgmsg.h"
#include "shader.h"
#include <iostream>
#include <cmath>
#include <string>

#define RESET "\033[0m"
#define BOLDRED "\033[1m\033[31m"
#define BOLDGREEN "\033[1m\033[32m"
#define BOLDYELLOW  "\033[1m\033[33m"

const bool wireframe = false;

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
	print_message("resizing window to " +  std::to_string(width) + " " + std::to_string(height));
}

void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		print_message("q detected, window should close!");
		glfwSetWindowShouldClose(window, true);
	}
}

float vertices[] = {
	0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
	0.0f,   0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
};

unsigned int indices[] = {
	0, 1, 2
};

int main() {

	// glfw init and version config (3.3 core)
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// creates window
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		print_failure("Failed to create GLFW window");
		glfwTerminate();
		return -1;
	} else {
		print_success("GLFW window created successfully!");
	}

	// assigns context
	glfwMakeContextCurrent(window);

	// initializes glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		print_failure("Failed to initialize GLAD");
		return -1;
	} else {
		print_success("GLAD initialized succesfully");
	}

	// viewport
	glViewport(0, 0, 800, 600);
	print_success("Viewport defined");

	// update viewport on window resize
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	/* ============================================= */

	// vertex array object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	// vertex buffer object
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// defines VAO layout
	// index, size, type, normalized, stride, offset

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// element buffer object
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// shader
	Shader shader("shader/shader.vert", "shader/shader.frag");

	if (wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//render loop
	while (!glfwWindowShouldClose(window)) {
		// input
		processInput(window);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.use();

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(float), GL_UNSIGNED_INT, (const void*)0);
		glBindVertexArray(0);

		// events and swap
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	print_success("glfw terminated");

	return 0;
}
