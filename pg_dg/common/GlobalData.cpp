#include "GlobalData.h"

#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>


void global_data::prepare(const Vector2i screen_size ) {
    view = lookAt(camera_pos, camera_pos + camera_front, camera_up);

    projection = perspective(radians(75.f), static_cast<float>(screen_size.getX()) / static_cast<float>(screen_size.getY()), 0.1f, 10000.f);
};

void global_data::bind_normal_shader_data(const shader shader) {
    projection_uniform_loc = glGetUniformLocation(shader.id, "projection");
    model_uniform_loc = glGetUniformLocation(shader.id, "model");
    view_uniform_loc = glGetUniformLocation(shader.id, "view");
};

auto global_data::bind_texture_shader_data(const shader shader) -> void
{
    texture_projection_uniform_loc = glGetUniformLocation(shader.id, "projection");
    texture_model_uniform_loc = glGetUniformLocation(shader.id, "model");
    texture_view_uniform_loc = glGetUniformLocation(shader.id, "view");
};

void global_data::update(const double new_time) {
    model = mat4(1.f);
    raw_time = new_time - current_time;
    current_time = new_time;

    view = lookAt(camera_pos, camera_pos + camera_front, camera_up);

    glUniformMatrix4fv(projection_uniform_loc, 1, GL_FALSE, value_ptr(projection));
    glUniformMatrix4fv(model_uniform_loc, 1, GL_FALSE, value_ptr(model));
    glUniformMatrix4fv(view_uniform_loc, 1, GL_FALSE, value_ptr(view));
};

global_data* global_data::instance_ = nullptr;

global_data* global_data::get_instance( ) {
    if (instance_ == nullptr)
        instance_ = new global_data( );

    return instance_;
};

global_data* global_data::get_instance(const float cam_speed) {
    if (instance_ == nullptr)
    {
        instance_ = new global_data( );
        instance_->base_cam_speed = cam_speed;
    }

    return instance_;
};

global_data::global_data( ) = default;

void global_data::key_callback(GLFWwindow* window, const int key, int scancode, const int action, int mods) {

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwTerminate( );
        exit(0);
    }

    float camera_speed = instance_->base_cam_speed * instance_->raw_time;
    if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_REPEAT)
        camera_speed = instance_->base_cam_speed * 5.0 * instance_->raw_time;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        instance_->camera_pos += camera_speed * instance_->camera_front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        instance_->camera_pos -= camera_speed * instance_->camera_front;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        instance_->camera_pos -= normalize(cross(instance_->camera_front, instance_->camera_up)) * camera_speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        instance_->camera_pos += normalize(cross(instance_->camera_front, instance_->camera_up)) * camera_speed;
};

void global_data::cursor_position_callback(GLFWwindow* window, const double x_pos, const double y_pos) {
    const auto current_mouse_pos = Vector2f(static_cast<float>(x_pos), static_cast<float>(y_pos));

    if (instance_->first_mouse_move) {
        instance_->last_mouse_pos.setX(current_mouse_pos.getX( ));
        instance_->last_mouse_pos.setY(current_mouse_pos.getY( ));
        instance_->first_mouse_move = false;
    }

    constexpr float sensitivity = 0.1f;

    auto offset = Vector2f(
        current_mouse_pos.getX( ) - instance_->last_mouse_pos.getX( ),
        instance_->last_mouse_pos.getY( ) - current_mouse_pos.getY( )
        );
    instance_->last_mouse_pos.setX(current_mouse_pos.getX( ));
    instance_->last_mouse_pos.setY(current_mouse_pos.getY( ));

    offset = Vector2f((offset.getX( ) * sensitivity), (offset.getY( ) * sensitivity));

    instance_->my_yaw += offset.getX( );
    instance_->my_pitch += offset.getY( );

    if (instance_->my_pitch > 89.0f)
        instance_->my_pitch = 89.0f;
    if (instance_->my_pitch < -89.0f)
        instance_->my_pitch = -89.0f;

    vec3 direction;
    direction.x = cos(radians(instance_->my_yaw)) * cos(radians(instance_->my_pitch));
    direction.y = sin(radians(instance_->my_pitch));
    direction.z = sin(radians(instance_->my_yaw)) * cos(radians(instance_->my_pitch));
    instance_->camera_front = normalize(direction);

};

void global_data::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {};