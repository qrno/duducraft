#ifndef CHUNK_H
#define CHUNK_H

#include "cube.h"
#include "block.h"
#include <glm/glm.hpp>

class Chunk {
public:
	Cube cubes[16][16][16];
	
	Chunk() {
		for (int i = 0; i < 16; i++) {
			for (int j = 0; j < 16; j++) {
				for (int k = 0; k < 16; k++) {
					SetCube(i, j, k,
							Block({i/15.0f, j/15.0f, k/15.0f}));
				}
			}
		}
	}

	void SetCube(int x, int y, int z, Block b) {
		cubes[x][y][z] = Cube({x, 15-y, z}, b);
	}
};

#endif
