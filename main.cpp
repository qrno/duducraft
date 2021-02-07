// OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// mine
#include "dbgmsg.h"
#include "shader.h"

// stb_image
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// stlib
#include <iostream>
#include <cmath>
#include <string>

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
	// coords				color				texcoords
   -0.5f,  -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	0.0f, 0.0f, //bl
    0.5f,  -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f, //br
   -0.5f,   0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 1.0f, //tl
	0.5f,   0.5f, 0.0f,		0.0f, 1.0f, 1.0f,	1.0f, 1.0f, //tr
};

unsigned int indices[] = {
	0, 1, 2,
	1, 2, 3
};

float texCoords[] = {
	0.0f, 0.0f, // lower left
	1.0f, 0.0f, // lower right
	0.5f, 1.0f	// top center
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

	// wrapping and filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load image and create texture
	unsigned int texture1, texture2;
	glGenTextures(1, &texture1);
	glGenTextures(1, &texture2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);


	//load image 
	stbi_set_flip_vertically_on_load(true);

	int width, height, nrChannels;
	unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (data) {
		print_success("loaded image successfully");
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		print_failure("failed to load image");
	}

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);

	data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	if (data) {
		print_success("loaded image successfully");
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		print_failure("failed to load image");
	}

	stbi_image_free(data);


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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// element buffer object
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	if (wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// shader
	Shader shader("shader/shader.vert", "shader/shader.frag");

	shader.use();

	/* ==== */
	/* 3d   */
	/* ==== */

	// vclip = mproj * mview * mmodel * vlocal
	
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f));

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), 800.0f/600.0f, 0.1f, 100.0f);

	int modelLoc = glGetUniformLocation(shader.ID, "model");
	int viewLoc = glGetUniformLocation(shader.ID, "view");
	int projectionLoc = glGetUniformLocation(shader.ID, "projection");

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	//render loop
	while (!glfwWindowShouldClose(window)) {
		// input
		processInput(window);

		// render
		glClearColor(0.2f, 0.3f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.use();
		shader.setInt("texture1", 0);
		shader.setInt("texture2", 1);

		/*
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians((float)(25.0f * sin(glfwGetTime()*9))), glm::vec3(0.0f, 1.0f, 0.3f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		view = glm::translate(view, glm::vec3(0.0, 0.0, 0.02f));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		*/

		float sinT = sin(glfwGetTime());

		float deg = (180.0f) * (sinT+1)/2;

		projection = glm::perspective(glm::radians(deg), 800.0f/600.0f, 0.1f, 100.0f);
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(VAO);
		glBindTexture(GL_TEXTURE_2D, texture2);
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
