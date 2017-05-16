#ifndef VAO_H
#define VAO_H

#include "Vbo.h"
#include <vector>

class Vao {
	const int BYTES_PER_FLOAT = 4;
	const int BYTES_PER_INT = 4;
	std::vector<Vbo> dataVbos;
public:
	const int id;
} vao;

#endif