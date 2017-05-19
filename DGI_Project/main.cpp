#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <chrono>

// Shader sources
const GLchar* vertexSource = R"glsl(
    #version 150 core
    in vec2 position;
    in float color;
    out float Color;
    void main() {
        Color = color;
        gl_Position = vec4(position, 0.0, 1.0);
    }
)glsl";
const GLchar* fragmentSource = R"glsl(
    #version 150 core
    in float Color;
    out vec4 outColor;
    void main() {
        outColor = vec4(Color, Color, Color, 1.0);
    }
)glsl";

int shaderCheck(GLuint, std::string);

int main() {

	glfwInit();
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr);

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();

	GLfloat floorArr[] = {
		-1.0f, -0.7f, 0.3f, // Top-left
		 1.0f, -0.7f, 0.3f, // Top-right
		 1.0f, -1.0f, 0.3f, // Bottom-right
		-1.0f, -1.0f, 0.3f, // Bottom-left
	};

	GLfloat boxManArr[] = {
		-0.3f,  0.5f, 0.1f, // Top-left
		 0.3f,  0.5f, 0.1f, // Top-right
		 0.3f, -0.7f, 0.1f, // Bottom-right
		-0.3f, -0.7f, 0.1f, // Bottom-left
	};

	GLuint floorElements[] = {
		0, 1, 2,
		2, 3, 0,
	};

	GLuint boxElements[] = {
		0, 1, 2,
		2, 3, 0,
	};

	// Create two Vertex Array Objects, Vertex Buffer Objects and Element Buffer Objects, and the shader objects. 
	GLuint vao[2], vbo[2], ebo[2], shaderProgram, fragmentShader, vertexShader;
	glGenVertexArrays(2, vao);
	glGenBuffers(2, vbo);
	glGenBuffers(2, ebo);

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	if (shaderCheck(vertexShader, "vertexShader") == 0)
		return 0;

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	if (shaderCheck(fragmentShader, "fragmentShader") == 0)
		return 0;

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	GLint posAttrib = glGetAttribLocation(shaderProgram, "position"), colAttrib = glGetAttribLocation(shaderProgram, "color");

	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floorArr), floorArr, GL_STATIC_DRAW);
	
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glVertexAttribPointer(colAttrib, 1, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(2 * sizeof(float)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(floorElements), floorElements, GL_STATIC_DRAW);
	glEnableVertexAttribArray(posAttrib);
	glEnableVertexAttribArray(colAttrib);

	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(boxManArr), boxManArr, GL_STATIC_DRAW);

	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glVertexAttribPointer(colAttrib, 1, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(2 * sizeof(float)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(boxElements), boxElements, GL_STATIC_DRAW);

	glEnableVertexAttribArray(posAttrib);
	glEnableVertexAttribArray(colAttrib);

	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);

		// Clear the screen to black
		glClearColor(0.0f, 0.8f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(vao[0]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(vao[1]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteProgram(shaderProgram);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	glDeleteBuffers(1, vbo);

	glDeleteVertexArrays(1, vao);

	glfwTerminate();
	return 0;
}

int shaderCheck(GLuint shader, std::string id) {
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status != GL_TRUE) {
		char buffer[512];
		std::cout << id << " failed to compile." << std::endl;
		glGetShaderInfoLog(shader, 512, NULL, buffer);
		for each (char var in buffer) {
			if (var == NULL)
				break;
			std::cout << var;
		}
		return 0;
	}

	return 1;
}