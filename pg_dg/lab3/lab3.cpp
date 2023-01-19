// Include standard headers
#include "stdio.h"
#include <cstdlib>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
using namespace glm;

#include <pg_dg/common/GlobalData.h>
#include <pg_dg/common/Vector2i.h>
#include <pg_dg/common/Vector2f.h>
#include <pg_dg/common/Shader.h>
#include <pg_dg/common/ParticleSystem.h>

shader basic_shader;

unsigned int vao_1 {};

unsigned int vbo_1 {};

unsigned int ebo_1 {};

/// screen
Vector2i screen_size = Vector2i(1280, 720);

global_data* global;

ParticleSystem* particle_system;

GLFWwindow* initialize(void) {
	global = global_data::get_instance( );

	global->fullscreen = false;

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
	GLFWwindow* window = glfwCreateWindow(screen_size.getX(), screen_size.getY(), "Lab-2 Trying something :)", monitor,
	nullptr);
	if (window == nullptr) {
		fprintf(
			stderr,
			"Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
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
	basic_shader = shader("C:/Users/part4/Desktop/grafika_2.0/ogl-master/pg_dg/lab3/shaders/Basic.vert",
		"C:/Users/part4/Desktop/grafika_2.0/ogl-master/pg_dg/lab3/shaders/Color.frag");
}

int main(void)
{
	GLFWwindow* window = initialize();

	prepare_shaders();

	global->prepare( screen_size );
	global->bind_normal_shader_data( basic_shader );

	//constexpr particle_type particle = rain;
	//constexpr particle_type particle = smoke;
	constexpr particle_type particle = fog;

	switch (particle)
	{

	case rain:
		{
			particle_system = new ParticleSystem(rain, global, 15000, 3, vec3(1, -5, -20),
	                                     vec3(-0.1, 0, 0), vec3(0, -0.9, 0),
	                                     vec3(0, 0, 0), vec4(0.3, 0.8, 1, 1),
	                                     vec2(200, 200), 200, 2, 0, true);
			particle_system->enable_rendering_plain( );
			break;
		}

	case smoke:
		{
			particle_system = new ParticleSystem(smoke, global, 10000, 3.3f, vec3(1, 10, -10),
	                                     vec3(0.14, 0, -0.13), vec3(0, 0.06, 0),
	                                     vec3(0, 0, 0), vec4(0.91, 0.96, 0.89, 0.59),
	                                     vec2(20, 20), 20, 6, 0, true);

			break;
		}

	case fog:
		{
			particle_system = new ParticleSystem(fog, global, 20000, 12.f, vec3(1, -5, -20),
	                                     vec3(-0.01, 0, 0.02), vec3(0, 0.03, 0),
	                                     vec3(0, 0, 0), vec4(0.98, 0.96, 0.98, 0.89),
	                                     vec2(200, 200), 200, 20, 0.6, true);
			particle_system->enable_rendering_plain( );
			break;
		}
	}

	glClearColor(0, 0, 0, 1.f);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	do {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		const double raw_time = glfwGetTime( );
		global->update(raw_time);

		particle_system->update(raw_time);
		particle_system->render();

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

	// Cleanup VAO
	glDeleteVertexArrays(1, &vao_1);

	delete particle_system;
	delete global;

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}