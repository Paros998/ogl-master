//
// Created by part4 on 29.01.2022.
//
#define _USE_MATH_DEFINES
#include <math.h>

#include "Shader.h"
#include "Texture.h"
#include "glm/gtc/type_ptr.hpp"
#include "vector"

#include "glm/gtc/matrix_transform.hpp"


using namespace glm;

class sphere{
    int stack_count_;
    int sectors_count_;
    float diameter_;

    ///view
    GLint view_uniform_loc_{};

    GLint projection_uniform_loc_{};

    GLint model_uniform_loc_{};

    ///update of objects
    mat4 transform_ = mat4(1.f);
    GLint transform_uniform_loc_{};

    shader sphere_shader_;
    texture sphere_texture1_;
    texture sphere_texture2_;
    unsigned int vao_;
    unsigned int vbo_;
    unsigned int ebo_;

    double sector_angle_;
    double stack_angle_;

    vec3 position_;

    vector<float> vertices_and_tex_coords_;
    vector<int> indices_;
    unsigned int number_of_indices_;
public:
    void calculate_vertices_and_indices() {
        const float sector_step = 2 * M_PI / sectors_count_;
        const float stack_step = M_PI / stack_count_;
        sector_angle_ = 2 * M_PI * (sector_step/sectors_count_);
        stack_angle_ = (M_PI / 2) - (M_PI * (stack_step/stack_count_));

        float sector_angle, stack_angle;
        float x, y, z, xy;
        float s, t;

        for(int i = 0; i <= stack_count_; ++i)
        {
            stack_angle = M_PI / 2 - i * stack_step;        // starting from pi/2 to -pi/2
            xy = diameter_ / 2 * cosf(stack_angle);      // r * cos(u)
            z = diameter_ / 2 * sinf(stack_angle);       // r * sin(u)

            for(int j = 0; j <= sectors_count_; ++j)
            {
                sector_angle = j * sector_step;              // starting from 0 to 2pi

                // vertex position (x, y, z)
                x = xy * cosf(sector_angle);             // r * cos(u) * cos(v)
                y = xy * sinf(sector_angle);             // r * cos(u) * sin(v)
                vertices_and_tex_coords_.push_back(x + position_.x);
                vertices_and_tex_coords_.push_back(y + position_.y);
                vertices_and_tex_coords_.push_back(z + position_.z);

                // vertex tex coord (s, t) range between [0, 1]
                s = static_cast<float>(j) / sectors_count_;
                t = static_cast<float>(i) / stack_count_;
                vertices_and_tex_coords_.push_back(s);
                vertices_and_tex_coords_.push_back(t);
            }
        }

        int k1, k2;
        for(int i = 0; i < stack_count_; ++i)
        {
            k1 = i * (sectors_count_ + 1);     // beginning of current stack
            k2 = k1 + sectors_count_ + 1;      // beginning of next stack

            for(int j = 0; j < sectors_count_; ++j, ++k1, ++k2)
            {
                // 2 triangles per sector excluding first and last stacks
                // k1 => k2 => k1+1
                if(i != 0)
                {
                    indices_.push_back(k1);
                    indices_.push_back(k2);
                    indices_.push_back(k1 + 1);
                }

                // k1+1 => k2 => k2+1
                if(i != (stack_count_-1))
                {
                    indices_.push_back(k1 + 1);
                    indices_.push_back(k2);
                    indices_.push_back(k2 + 1);
                }

            }
        }
    }

    sphere()= default;;

    sphere(const int stack_count, const int sectors_count, const float diameter, const char* sphere_texture1_file, const char* sphere_texture2_file,
            const char* vertex_file, const char* frag_file, const vec3 position){
        this->stack_count_ = stack_count;
        this->sectors_count_ = sectors_count;
        this->diameter_ = diameter;
        this->position_ = position;

        sphere_shader_ = shader(vertex_file,frag_file);
        sphere_texture1_ = texture(sphere_texture1_file);
        sphere_texture2_ = texture(sphere_texture2_file);

        calculate_vertices_and_indices();

        number_of_indices_ = indices_.size();

        transform_uniform_loc_ = glGetUniformLocation(sphere_shader_.id, "transform");
        projection_uniform_loc_ = glGetUniformLocation(sphere_shader_.id, "projection");
        model_uniform_loc_ = glGetUniformLocation(sphere_shader_.id, "model");
        view_uniform_loc_ = glGetUniformLocation(sphere_shader_.id, "view");

        glGenVertexArrays(1,&vao_);
        glGenBuffers(1,&vbo_);
        glGenBuffers(1,&ebo_);


        const auto vertices = new float[vertices_and_tex_coords_.size()];
        int i = 0;
        for(const float vertex : vertices_and_tex_coords_){
            vertices[i++] = vertex;
        }

        const auto indices = new int[indices_.size()];
        i = 0;
        for(const int indice : indices_){
            indices[i++] = indice;
        }

        glBindVertexArray(vao_);

        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices_and_tex_coords_.size(), vertices_and_tex_coords_.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * indices_.size(), indices_.data(), GL_STATIC_DRAW);


        ///2-Setting vertex linking for openGL (call once in prepare)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), static_cast<void*>(0 * sizeof(float)));
        glEnableVertexAttribArray(0);

        /// texture attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);

    }

    void update_and_draw(mat4 projection, mat4 model, mat4 view){
        sphere_shader_.activate();
        glBindVertexArray(vao_);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, sphere_texture1_.get_texture());
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, sphere_texture2_.get_texture());

        sphere_shader_.set_int("texture_f", 2);
        sphere_shader_.set_int("texture_s", 3);

        transform_ = rotate(transform_, radians(0.05f), vec3(0.f, 0.f, -0.3f) );

        glUniformMatrix4fv(transform_uniform_loc_, 1, GL_FALSE, value_ptr(transform_));

        glUniformMatrix4fv(projection_uniform_loc_, 1, GL_FALSE, value_ptr(projection));

        glUniformMatrix4fv(model_uniform_loc_, 1, GL_FALSE, value_ptr(model));

        glUniformMatrix4fv(view_uniform_loc_, 1, GL_FALSE, value_ptr(view));

        glDrawElements(GL_TRIANGLES, number_of_indices_,GL_UNSIGNED_INT, nullptr);

    }

};

