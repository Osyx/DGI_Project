#include "Vbo.h"

Vbo::Vbo(GLuint _vboId, int _type) : 
	vboId(_vboId), type(_type) {
}

Vbo Vbo::create(GLuint vboId) {
	glGenBuffersARB(1, vboId);
}

void Vbo::bind(GLenum type, GLuint vboId) {
	glBindBufferARB(type, vboId);
}

void Vbo::unbind(GLenum type) {
	glBindBufferARB(type, 0);
}

void Vbo::storeData(const void* data, int dataSize, GLenum target, GLenum usage) {
	glBufferDataARB(target, dataSize, data, usage);
}

void Vbo::deleteBvo() {
	glDeleteBuffersARB(1, vboId);
}