#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "dbgmsg.h"

void check_shader_compilation(unsigned int sid) {
	int success; char infoLog[256];

	glGetShaderiv(sid, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(sid, 256, NULL, infoLog);
		print_failure("Shader compilation error (check infolog)");
		print_infolog(infoLog);
	} else print_success("Shader compiled succesfully");
}

void check_program_link(unsigned int pid) {
	int success; char infoLog[256];

	glGetProgramiv(pid, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(pid, 256, NULL, infoLog);
		print_failure("Shader Program creation error (check infolog)");
		print_infolog(infoLog);
	} else print_success("Shader Program created succesfully");
}

class Shader {
public:
	unsigned int ID;

	Shader(const char* vertexPath, const char* fragmentPath);

	void use();

	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
};

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	print_message("creating shader from " + (std::string)vertexPath + " and " + (std::string)fragmentPath);

	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
	try {
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

	} catch (std::ifstream::failure e) {
		print_failure("shader: file not successfully read");
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	unsigned int vertex, fragment;

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	check_shader_compilation(vertex);

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	check_shader_compilation(fragment);

	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);

	check_program_link(ID);

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::use() {
	glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string &name, int value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) const {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

#endif
