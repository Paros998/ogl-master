// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM

#include <pg_dg/common/GlobalData.h>
#include <pg_dg/common/Vector2i.h>
#include <pg_dg/common/Vector2f.h>
#include <pg_dg/common/Shader.h>
#include <pg_dg/common/Roller.h>

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

shader basic_shader;
shader texture_program;

Roller walec;

unsigned int vao_1 {};
unsigned int vao_2 {};

unsigned int VAO_3 {};
unsigned int VAO_4 {};

unsigned int vbo_1 {};
unsigned int VBO_2 {};

unsigned int VBO_3 {};
unsigned int VBO_4 {};

unsigned int ebo_1 {};
unsigned int EBO_2 {};

unsigned int EBO_3 {};
unsigned int EBO_4 {};

/// screen
Vector2i screen_size = Vector2i(1280, 720);

vec3 objects_pos[2] = {
		vec3(-3.f, 0.f, 0.f),
		vec3(1.f,  1.f,0.f)
};


vec3 doorSquaresPos[2] = {
		vec3(.8f, 0.04f, 1.22f),
		vec3(.8f, -0.04f, 1.22f)
};

global_data* global;

GLFWwindow* initialize(void) {
	global = global_data::get_instance( );

	global->fullscreen = false;

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

	GLFWmonitor* monitor = nullptr;
	if (global->fullscreen) {
		monitor = glfwGetPrimaryMonitor( );
	}
	else {
		glfwWindowHint(GLFW_DECORATED, true);
		glfwWindowHint(GLFW_RESIZABLE, true);
	}

	global->last_mouse_pos = Vector2f(screen_size.getX( ) / 2, screen_size.getY( ) / 2);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(screen_size.getX(), screen_size.getY( ), "Lab-2 Trying something :)", monitor, NULL);
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
	glfwSetKeyCallback(window, global_data::key_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, global_data::cursor_position_callback);
	glfwSetMouseButtonCallback(window, global_data::mouse_button_callback);

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	glViewport(0, 0, screen_size.getX( ), screen_size.getY( ));

	glfwWindowHint(GLFW_DOUBLEBUFFER, true);
	glfwWindowHint(GLFW_REFRESH_RATE, 60);

	glfwSwapInterval(1);

	return window;
}

void prepare_shaders() {
	// Create and compile our GLSL program from the shaders
	basic_shader = shader("C:/Users/part4/Desktop/grafika_2.0/ogl-master/pg_dg/lab5/shaders/Basic.vert",
	                      "C:/Users/part4/Desktop/grafika_2.0/ogl-master/pg_dg/lab5/shaders/Color.frag");

	//textureProgram = Shader("C:/Users/part4/Desktop/grafika_2.0/ogl-master/pg_dg/lab2/shaders/Texture.vert",
	//	"C:/Users/part4/Desktop/grafika_2.0/ogl-master/pg_dg/lab2/shaders/Texture.frag");

	//textureProgram.activate( );
	//textureProgram.setInt("Texture", 0);

	//textureProgram.deactivate( );
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
	basic_shader.activate();

	GLfloat fstRectangle[] = {
		//near
		 1.0f,  1.0f, 1.0f,		 1.0f, .0f, .0f, 1.f,		//top right			- 0
		 1.0f, -1.0f, 1.0f,		 .0f, .0f, .0f, 1.f,		//bottom right		- 1
		-1.0f, -1.0f, 1.0f,		 .0f, .0f, .0f, 1.f,		//bottom left		- 2
		-1.0f,  1.0f, 1.0f,		 1.0f, .0f, .0f, 1.f,		//top left			- 3

		//far
		 1.0f,  1.0f, 3.0f,		 .0f, .0f, .0f, 1.f,		//top right			- 4
		 1.0f, -1.0f, 3.0f,		 1.0f, .0f, .0f, 1.f,		//bottom right		- 5
		-1.0f, -1.0f, 3.0f,		 1.0f, .0f, .0f, 1.f,		//bottom left		- 6
		-1.0f,  1.0f, 3.0f,		 .0f, .0f, .0f, 1.f,		//top left			- 7
	};

	GLfloat doors[] = {
		//near
		 1.0f,  2.0f, 1.0f,		 .43f, .25f, .19f, 1.f,		//top right			- 0
		 1.0f, -2.0f, 1.0f,		 .43f, .25f, .19f, 1.f,		//bottom right		- 1
		-1.0f, -2.0f, 1.0f,		 .43f, .25f, .19f, 1.f,		//bottom left		- 2
		-1.0f,  2.0f, 1.0f,		 .43f, .25f, .19f, 1.f,		//top left			- 3

		//far
		 1.0f,  2.0f, 1.20f,		 .43f, .25f, .19f, 1.f,		//top right			- 4
		 1.0f, -2.0f, 1.20f,		 .43f, .25f, .19f, 1.f,		//bottom right		- 5
		-1.0f, -2.0f, 1.20f,		 .43f, .25f, .19f, 1.f,		//bottom left		- 6
		-1.0f,  2.0f, 1.20f,		 .43f, .25f, .19f, 1.f,		//top left			- 7
	};

	GLfloat doorSquare[] = {
		//near
		 0.02f,    0.02f, 0.02f,			1.0f, 1.0f, 1.0f, 1.f,		//top right			- 0
		 0.02f,    -0.02f, 0.02f,			1.0f, 1.0f, 1.0f, 1.f,		//bottom right		- 1
		 -0.02f,   -0.02f, 0.02f,			1.0f, 1.0f, 1.0f, 1.f,		//bottom left		- 2
		 -0.02f,   0.02f, 0.02f,			1.0f, 1.0f, 1.0f, 1.f,		//top left			- 3

		//far
		 0.02f,    0.02f, -0.02f,		1.0f, 1.0f, 1.0f, 1.f,		//top right			- 4
		 0.02f,    -0.02f, -0.02f,		1.0f, 1.0f, 1.0f, 1.f,		//bottom right		- 5
		 -0.02f,   -0.02f, -0.02f,		1.0f, 1.0f, 1.0f, 1.f,		//bottom left		- 6
		 -0.02f,   0.02f, -0.02f,		1.0f, 1.0f, 1.0f, 1.f,		//top left			- 7
	};

	unsigned int indices[] = {

		//przednia sciana
		1, 2, 3,
		0, 1, 3,

		//prawo
		0, 4, 1,
		1, 5, 4,

		//lewa
		2, 3, 6,
		3, 6, 7,

		//gora
		0, 3, 4,
		3, 4, 7,

		//dol
		1, 2, 6,
		1, 5, 6,

		//tylna sciana
		5, 6, 7,
		4, 5, 7,
	};

	GLfloat sndRectangle[] = {
		 30.f,  10.f, .0f,		 0.0f, 1.0f, 1.0f, .5f,  //prawo góra
		 30.f, -10.f, .0f,		 0.0f, 1.0f, 1.0f, .5f,  //prawo dó³
		-30.f, -10.f, .0f,		 0.0f, 1.0f, 1.0f, .5f,  //lewo dó³
		-30.f,  10.f, .0f,		 0.0f, 1.0f, 1.0f, .5f,  //lewo góra
	};

	unsigned int sndIndices[] = {
		//przednia sciana
		1, 2, 3,
		0, 1, 3,
	};

	// Create VAO, VBO, EBO
	glGenVertexArrays(1, &vao_1);
	glGenBuffers(1, &vbo_1);
	glGenBuffers(1, &ebo_1);

	// Bind them
	glBindVertexArray(vao_1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fstRectangle), fstRectangle, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//set vertex position attrib
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (7 * sizeof(float)), (void*) (0 * sizeof(float)));
	glEnableVertexAttribArray(0);

	//set color attrib
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, (7 * sizeof(float)), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Create VAO, VBO, EBO
	glGenVertexArrays(1, &vao_2);
	glGenBuffers(1, &VBO_2);
	glGenBuffers(1, &EBO_2);

	// Bind them
	glBindVertexArray(vao_2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sndRectangle), sndRectangle, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sndIndices), sndIndices, GL_STATIC_DRAW);

	//set vertex position attrib
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (7 * sizeof(float)), (void*) (0 * sizeof(float)));
	glEnableVertexAttribArray(0);

	//set color attrib
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, (7 * sizeof(float)), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Create VAO, VBO, EBO of DOORS
	glGenVertexArrays(1, &VAO_3);
	glGenBuffers(1, &VBO_3);
	glGenBuffers(1, &EBO_3);

	// Bind them
	glBindVertexArray(VAO_3);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(doors), doors, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_3);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//set vertex position attrib
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (7 * sizeof(float)), (void*) (0 * sizeof(float)));
	glEnableVertexAttribArray(0);

	//set color attrib
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, (7 * sizeof(float)), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Create VAO, VBO, EBO of DOORS Knoobs
	glGenVertexArrays(1, &VAO_4);
	glGenBuffers(1, &VBO_4);
	glGenBuffers(1, &EBO_4);

	// Bind them
	glBindVertexArray(VAO_4);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_4);
	glBufferData(GL_ARRAY_BUFFER, sizeof(doorSquare), doorSquare, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_4);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//set vertex position attrib
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (7 * sizeof(float)), (void*) (0 * sizeof(float)));
	glEnableVertexAttribArray(0);

	//set color attrib
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, (7 * sizeof(float)), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	walec = Roller(20, 50, .2f, .25f, vec3(0.8f, .0f, 1.1f), global->model_uniform_loc, vec4(0.996f, 0.996f, 0.f, 1.f));

	glBindVertexArray(0);
}

void draw_3d_rectangle(const GLuint *VAO, uint number_of_indices) {
	glBindVertexArray(*VAO);

	glDrawElements(GL_TRIANGLES, number_of_indices, GL_UNSIGNED_INT, nullptr);
}

void render_blocks( ) {
	global->update( glfwGetTime());

	// Use our shader
	basic_shader.activate();
	global->model = mat4(1.f);
	mat4 matR = scale(global->model, vec3(1, 1, -1));

	glColorMask(1, 1, 1, 1);

	//drawTwoObjects
	for(auto pos : objects_pos)
	{
		mat4 currentModel = translate(global->model, pos);
		glUniformMatrix4fv(global->model_uniform_loc, 1, GL_FALSE, value_ptr(currentModel));
		draw_3d_rectangle(&vao_1,36);
	}

	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 1);

	// œciana
	global->model = mat4(1.f);
	glUniformMatrix4fv(global->model_uniform_loc, 1, GL_FALSE, value_ptr(global->model));
	draw_3d_rectangle(&vao_2, 6);

	glDisable(GL_DEPTH_TEST);
	glStencilFunc(GL_EQUAL, 1, 1);
	//drawTwoObjects that emulate reflection
	for (auto pos : objects_pos)
	{
		mat4 currentModel = translate(matR, pos);
		glUniformMatrix4fv(global->model_uniform_loc, 1, GL_FALSE, value_ptr(currentModel));
		draw_3d_rectangle(&vao_1, 36);
	}
	glDisable(GL_STENCIL_TEST);
	glEnable(GL_DEPTH_TEST);

}

void renderDoor( ) {
	global->update( glfwGetTime());

	// Use our shader
	basic_shader.activate();
	global->model = mat4(1.f);

	glColorMask(1, 1, 1, 1);

	glEnable(GL_STENCIL_TEST);

	glStencilFunc(GL_EQUAL, 0, 0x00);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	for (auto pos : doorSquaresPos) {
		mat4 currentModel = translate(global->model, pos);
		glUniformMatrix4fv(global->model_uniform_loc, 1, GL_FALSE, value_ptr(currentModel));
		draw_3d_rectangle(&VAO_4, 36);
	}

	global->model = mat4(1.f);
	glUniformMatrix4fv(global->model_uniform_loc, 1, GL_FALSE, value_ptr(global->model));

	glStencilFunc(GL_EQUAL, 0, 0x00);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	draw_3d_rectangle(&VAO_3, 36);
	walec.render( );
}

int main(void)
{
	bool showMirrorScene = true;
	GLFWwindow* window = initialize();

	prepare_shaders();
	global->prepare( screen_size );
	global->bind_normal_shader_data(basic_shader);
	//global->bindTextureShaderData( textureProgram );

	prepare_objects();

	glClearColor(1, 0, 1, 1.f);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	if (!showMirrorScene)
	{
		global->camera_pos = vec3(1.5f, 0.0f, 6.0f);
	}

	do {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		showMirrorScene ? render_blocks( ) : renderDoor();

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