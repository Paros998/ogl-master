#ifndef GRAFIKA_GLOBALDATA_H
#define GRAFIKA_GLOBALDATA_H

#pragma once

// Include standard headers

#include <glm/glm.hpp>

#include <pg_dg/common/Vector2f.h>
#include "Vector2i.h"
#include <pg_dg/common/Shader.h>
#include "GL/glew.h"
#include <GLFW/glfw3.h>

using namespace glm;

class global_data {

    static global_data* instance_;

    global_data( );

public:
    vec3 camera_pos = vec3(0.0f, 7.0f, 10.0f);
    vec3 camera_front = vec3(0.0f, 0.0f, -1.0f);
    vec3 camera_up = vec3(0.0f, 1.0f, 0.0f);

    Vector2f last_mouse_pos = Vector2f(0.f, 0.f);
    bool first_mouse_move = true;

    float my_yaw = -90.0f;
    float my_pitch = 0.0f;
    float base_cam_speed = 10.0f;

    double raw_time = 0;
    double current_time = 0;

    bool fullscreen = false;

    mat4 view = mat4(1.f);
    GLint view_uniform_loc {};
    GLint texture_view_uniform_loc {};

    mat4 projection = mat4(1.f);
    GLint projection_uniform_loc {};
    GLint texture_projection_uniform_loc {};

    mat4 model = mat4(1.f);
    GLint model_uniform_loc {};
    GLint texture_model_uniform_loc {};

    const mat4 const_model = mat4(1.f);

    void bind_normal_shader_data(shader shader);
    void bind_texture_shader_data(shader shader);

    void update(double new_time);

    static global_data* get_instance( );
    static global_data* get_instance( float cam_speed );

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

    static void cursor_position_callback(GLFWwindow* window, double x_pos, double y_pos);

    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

    void prepare(Vector2i screen_size);
};

#endif //GRAFIKA_GLOBALDATA_H