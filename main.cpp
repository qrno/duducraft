#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "dbgmsg.h"
#include <iostream>
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

void check_shader_compilation(unsigned int sid) {
	int success; char infoLog[256];

	glGetShaderiv(sid, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(sid, 512, NULL, infoLog);
		print_failure("Shader compilation error (check infolog)");
		print_infolog(infoLog);
	} else print_success("Shader compiled succesfully");
}

void check_program_link(unsigned int pid) {
	int success; char infoLog[256];

	glGetProgramiv(pid, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(pid, 512, NULL, infoLog);
		print_failure("Shader Program creation error (check infolog)");
		print_infolog(infoLog);
	} else print_success("Shader Program created succesfully");
}

float vertices[] = {
	0.5f,   0.5f, 0.0f, // top right 
	0.5f,  -0.5f, 0.0f, // bottom right
	-0.5f, -0.5f, 0.0f, // bottom left
	-0.5f,  0.5f, 0.0f, // top left
};

unsigned int indices[] = {
	0, 1, 3,
	1, 2, 3
};

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main() {\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\n";

const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main() {\n"
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n";

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

	// vertex array object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	// vertex buffer object
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// element buffer object
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	// vertex shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// fragment shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// check for errors
	print_message("Checking Vertex and Fragment Shader Compilation");
	check_shader_compilation(vertexShader);
	check_shader_compilation(fragmentShader);

	// shader program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	check_program_link(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	if (wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//render loop
	while (!glfwWindowShouldClose(window)) {
		// input
		processInput(window);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// events and swap
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	print_success("glfw terminated");

	return 0;
}
