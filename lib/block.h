#ifndef BLOCK_H
#define BLOCK_H

#include <glm/glm.hpp>

class Block {
public:
	glm::vec3 Color;

	Block() {
		setAir();
	}

	Block(glm::vec3 color) {
		Color = color;
	}

	void setAir()  { Color = {-1.0f, -1.0f, -1.0f}; }
	void setFull() { Color = {1.0f, 1.0f, 1.0f};    }

	bool IsAir() {
		return Color == glm::vec3(-1.0f, -1.0f, -1.0f);
	}
};

#endif
