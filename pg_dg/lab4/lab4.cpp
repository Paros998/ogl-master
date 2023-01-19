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
#include <pg_dg/common/Texture.h>
#include <pg_dg/common/stb_image.h>
#include <pg_dg/common/Sphere.cpp>

shader basic_shader;
texture sky_texture;
sphere earth;

unsigned int vao_1 {};

unsigned int vbo_1 {};

unsigned int ebo_1 {};

/// screen
Vector2i screen_size = Vector2i(1280, 720);

global_data* global;


GLFWwindow* initialize(void) {
	global = global_data::get_instance( 40.f );

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
	basic_shader = shader("C:/Users/part4/Desktop/grafika_2.0/ogl-master/pg_dg/lab4/shaders/simpleTexture/vertex.shader",
		"C:/Users/part4/Desktop/grafika_2.0/ogl-master/pg_dg/lab4/shaders/simpleTexture/frag.shader");
}

void prepare_sky()
{
	sky_texture = texture("C:/Users/part4/Desktop/grafika_2.0/ogl-master/pg_dg/lab4/assets/stars.jpg");

	constexpr float bg_square[] = {
                2500.f,2500.f,2500.f,       1.0f,1.0f,
                2500.f,-2500.f,2500.f,       1.0f,0.0f,
                -2500.f,-2500.f,2500.f,       0.0f,0.0f,
                -2500.f,2500.f,2500.f,       0.0f,1.0f,

                2500.f,2500.f,-2500.f,       0.0f,0.0f,
                2500.f,-2500.f,-2500.f,       0.0f,1.0f,
                -2500.f,-2500.f,-2500.f,       1.0f,1.0f,
                -2500.f,2500.f,-2500.f,       1.0f,0.0f,
        };

	const unsigned int indices[] = {
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
                //przednia sciana
                1, 2, 3,
                0, 1, 3,
                //tylna sciana
                5, 6, 7,
                4, 5, 7,
        };

	glGenVertexArrays(1, &vao_1);
	glGenBuffers(1, &vbo_1);
	glGenBuffers(1, &ebo_1);

	glBindVertexArray(vao_1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bg_square), bg_square, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), static_cast<void*>(0 * sizeof(float)));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void render()
{
	earth.update_and_draw(global->projection,global->model, global->view);

	basic_shader.activate();
	glBindVertexArray(vao_1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sky_texture.get_texture());

	basic_shader.set_int("s_texture",0);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

}

int main(void)
{
	GLFWwindow* window = initialize();
	stbi_set_flip_vertically_on_load(false);

	prepare_shaders();

	global->prepare( screen_size );
	global->bind_normal_shader_data( basic_shader );

	glClearColor(1, 0.5, 0, 1.f);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	prepare_sky();

	earth = sphere(18,36,500.f,"C:/Users/part4/Desktop/grafika_2.0/ogl-master/pg_dg/lab4/assets/earth_daymap.jpg",
		"C:/Users/part4/Desktop/grafika_2.0/ogl-master/pg_dg/lab4/assets/earth_clouds.jpg",
		"C:/Users/part4/Desktop/grafika_2.0/ogl-master/pg_dg/lab4/shaders/multiTexture/vertex.shader",
		"C:/Users/part4/Desktop/grafika_2.0/ogl-master/pg_dg/lab4/shaders/multiTexture/frag.shader",
		vec3(0.f,0.f,-550.f));

	do {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		const double raw_time = glfwGetTime( );
		global->update(raw_time);

		render();

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

	delete global;

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
