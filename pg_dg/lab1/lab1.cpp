// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>


// Include GLM
#include <glm/glm.hpp>
using namespace glm;

#include <pg_dg/common/Shader.h>
shader basic_shader;

GLuint vao_1;
GLuint vbo_1;

GLuint vao_2;
GLuint VBO_2;

GLFWwindow* initialize(void) {
	GLFWwindow* window;
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		exit(-1);
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(1024, 768, "Lab-1 Trying something :)", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		exit(-1);
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(1, 0, 0.4f, 1.f);

	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// glBlendFunc( GL_SRC_COLOR, GL_DST_COLOR);

	return window;
}

void prepare_shaders() {
	// Create and compile our GLSL program from the shaders
	basic_shader = shader("C:/Users/part4/Desktop/grafika_2.0/ogl-master/pg_dg/lab1/shaders/Basic.vert",
	                      "C:/Users/part4/Desktop/grafika_2.0/ogl-master/pg_dg/lab1/shaders/Color.frag");
}

void bind_properties(GLuint *vao, GLuint *vbo, GLfloat data[]) {
	// Create VAO, VBO
	glGenVertexArrays(1, vao);
	glGenBuffers(1, vbo);

	// Bind them
	glBindVertexArray(*vao);

	glBindBuffer(GL_ARRAY_BUFFER, *vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

	//set vertex position attrib
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (7 * sizeof(float)), (void*) (0 * sizeof(float)));
	glEnableVertexAttribArray(0);

	//set color attrib
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, (7 * sizeof(float)), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void prepare_objects() {
	GLfloat fstTriangle[] = {
		-1.0f, -1.0f, 0.0f,		 1.0f, 1.0f, 1.0f, 1.f,
		 1.0f, -1.0f, 0.0f,		 1.0f, 1.0f, 0.0f, 1.f,
		 0.0f,  1.0f, 0.0f,		 0.0f, 1.0f, 1.0f, 1.f,
	};

	GLfloat sndTriangle[] = {
		-0.5f, -0.5f, 0.0f,		 1.0f, 0.0f, 1.0f, .5f,  //lewo dó³
		 0.5f, -0.5f, 0.0f,		 1.0f, 0.0f, 0.0f, .5f,  //prawo dó³
		-0.5f,  0.5f, 0.0f,		 0.0f, 1.0f, 1.0f, .5f,  //lewo góra

		 0.5f,  0.5f, 0.0f,		 1.0f, 1.0f, 1.0f, 1.f,  //prawo góra
		-0.5f,  0.5f, 0.0f,		 0.0f, 1.0f, 1.0f, .75f, //lewo góra
		 0.5f, -0.5f, 0.0f,		 1.0f, 0.0f, 0.0f, .75f, //prawo dó³
	};

	// firstTriangle
	// bindProperties(&VAO_1, &VBO_1, fstTriangle);

	// now second triangle
	// bindProperties(&VAO_2, &VBO_2, sndTriangle);

	// Create VAO, VBO
	glGenVertexArrays(1, &vao_1);
	glGenBuffers(1, &vbo_1);

	// Bind them
	glBindVertexArray(vao_1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fstTriangle), fstTriangle, GL_STATIC_DRAW);

	//set vertex position attrib
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (7 * sizeof(float)), (void*) (0 * sizeof(float)));
	glEnableVertexAttribArray(0);

	//set color attrib
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, (7 * sizeof(float)), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Create Second VAO, VBO
	glGenVertexArrays(1, &vao_2);
	glGenBuffers(1, &VBO_2);

	// Bind them
	glBindVertexArray(vao_2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sndTriangle), sndTriangle, GL_STATIC_DRAW);

	//set vertex position attrib
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (7 * sizeof(float)), (void*) (0 * sizeof(float)));
	glEnableVertexAttribArray(0);

	//set color attrib
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, (7 * sizeof(float)), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void drawTriangle(GLuint *VAO, uint numberOfVertices) {
	glBindVertexArray(*VAO);

	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, numberOfVertices); // 3 indices starting at 0 -> 1 triangle
}

void renderObjects( ) {
	// Use our shader
	basic_shader.activate();

	drawTriangle(&vao_1,3);
	drawTriangle(&vao_2,6);
}

int main(void)
{
	GLFWwindow* window = initialize();

	prepare_shaders();

	prepare_objects();

	do {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		renderObjects( );

		glBindVertexArray(0);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// deactivate shader (if only one)
	basic_shader.deactivate();

	// Cleanup VBO
	glDeleteBuffers(1, &vbo_1);
	glDeleteBuffers(1, &VBO_2);

	// Cleanup VAO
	glDeleteVertexArrays(1, &vao_1);
	glDeleteVertexArrays(1, &vao_2);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}