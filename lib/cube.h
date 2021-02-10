#ifndef CUBE_H
#define CUBE_H

#include <glm/glm.hpp>
#include "block.h"

class Cube {
public:
	glm::vec3 Position;
	Block B;

	Cube() {
		Position = {0, 0, 0};
		B.Color = {-1, -1, -1};
	}

	Cube(glm::vec3 position, glm::vec3 color={1.0f, 0.2f, 0.3f}) {
		SetPosition(position);
		SetColor(color);
	};

	Cube(glm::vec3 position, Block b) {
		SetPosition(position);
		SetBlock(b);
	}

	bool IsAir() { return B.IsAir(); }

	void SetPosition(glm::vec3 position) { Position = position; }
	void SetColor(glm::vec3 color) { B.Color = color; }
	void SetBlock(Block b) { B = b; }
};

#endif
