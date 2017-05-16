#ifndef VBO_H
#define VBO_H

#include "GL/glut.h"
#include "glext.h"

class Vbo {
	const GLuint vboId;
	const int type;
	Vbo(GLuint, int);
public:
	Vbo create(GLuint);
	void bind(GLenum, GLuint);
	void unbind(GLenum);
	void storeData(const void*, int, GLenum, GLenum);
	void deleteBvo();
};

#endif