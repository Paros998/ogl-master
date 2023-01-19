#ifndef GRAFIKA_PARTICLESYSTEM_H
#define GRAFIKA_PARTICLESYSTEM_H

#pragma once


// Include standard headers

#include <pg_dg/common/Shader.h>
#include <vector>
#include <pg_dg/common/GlobalData.h>

using namespace glm;

class particle {
	vec3 original_pos_;
	vec3 pos_;
	float life_time_;
	float death_time_;
	bool active_;
	bool first_life_ = true;
	float life_span_;
	float death_span_;

	float max_height_;
	float min_height_;

	void update_life(float raw_time);

	void update_death(float raw_time);
public:
	particle(vec3 pos, float life_span, float death_span);
	particle(vec3 pos, float life_span, float death_span, float initial_life_time);
	particle(vec3 pos, float life_span, float death_span, float initial_life_time, float max_height);

	void update(float raw_time);

	bool is_active( ) const;

	vec3 get_current_pos( ) const;

	void translate_position( vec3 model);
};

enum particle_type
{
	rain,
	fog,
	smoke
};

// ReSharper disable once CppInconsistentNaming
class ParticleSystem {
	uint number_of_particles_;
	shader* particle_shader_;
	shader* plain_shader_;
	vec3 original_point_;
	uint particle_size_;

	vec3 move_;
	vec3 gravitation_;
	vec3 rotation_;
	vec4 rgba_;

	vec2 plain_;
	float height_;
	float life_span_;
	float res_time_;
	bool randomize_movement_;

	vec3 combined_movement_;

	vector<particle*> particles_;

	bool render_plain_ = false;

	unsigned int vao_1_ {};
	unsigned int vbo_1_ {};
	unsigned int ebo_1_ {};

	unsigned int vao_2_ {};
	unsigned int vbo_2_ {};

	GLint model_uniform_loc_ {};
	GLint view_uniform_loc_ {};
	GLint projection_uniform_loc_ {};

	GLint model_uniform_loc_plain_ {};
	GLint view_uniform_loc_plain_ {};
	GLint projection_uniform_loc_plain_ {};

	mat4 current_model_;

	global_data* data_;

	particle_type particle_;

	double raw_time_ = 0.0;
    double current_time_ = 0.0;
public:
	static void draw_plain(const GLuint* vao, uint number_of_indices);

	ParticleSystem(particle_type particle, global_data* data, uint number_of_particles, uint particle_size, vec3 original_point,
		vec3 move_vector, vec3 gravitation_vector, vec3 rotation_vector, vec4 color_and_alpha,
		vec2 plain, float height, float max_life_span, float res_time, bool randomize_movement);

	void update(double new_time);

	void enable_rendering_plain( );

	void render( );

	void calculate_particles( );
};

#endif //GRAFIKA_GLOBALDATA_H