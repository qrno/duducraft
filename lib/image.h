#ifndef IMAGE_H
#define IMAGE_H

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include "dbgmsg.h"

class Texture {
public:
	Texture(GLenum texture, std::string path, GLenum color_encoding=GL_RGB) {
		glGenTextures(1, &ID);

		glActiveTexture(texture);
		glBindTexture(GL_TEXTURE_2D, ID);

		stbi_set_flip_vertically_on_load(true);
		
		int nrChannels;	
		unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
		if (data) {
			print_success("Loaded texture from " + path);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, color_encoding, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		} else {
			print_failure("Failed to load texture from " + path);
		}

		stbi_image_free(data);
	}

	unsigned int ID;
	int width, height;

	void bind() {
		glBindTexture(GL_TEXTURE_2D, ID);
	}
};
#endif
