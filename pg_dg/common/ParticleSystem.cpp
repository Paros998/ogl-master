#include "ParticleSystem.h"

#include "GL/glew.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

particle::particle(const vec3 pos, const float life_span, const float death_span)
{
	this->life_time_ = 0.f;
	this->death_time_ = 0.f;
	this->pos_ = pos;
	this->original_pos_ = pos;
	this->active_ = true;
	this->life_span_ = life_span;
	this->death_span_ = death_span;
	this->max_height_ = original_pos_.y;
	this->min_height_ = original_pos_.y;
}
particle::particle(const vec3 pos, const float life_span, const float death_span, const float initial_life_time)
{
	this->life_time_ = initial_life_time;
	this->death_time_ = 0.f;
	this->pos_ = pos;
	this->original_pos_ = pos;
	this->active_ = true;
	this->life_span_ = life_span;
	this->death_span_ = death_span;
	this->max_height_ = original_pos_.y;
	this->min_height_ = original_pos_.y;
}

particle::particle(const vec3 pos, const float life_span, const float death_span, const float initial_life_time, const float max_height) {
	this->life_time_ = initial_life_time;
	this->death_time_ = 0.f;
	this->pos_ = pos;
	this->original_pos_ = pos;
	this->active_ = true;
	this->life_span_ = life_span;
	this->death_span_ = death_span;
	this->max_height_ = max_height;
	this->min_height_ = original_pos_.y;
}

auto particle::update_life(const float raw_time) -> void
{
	this->life_time_ += raw_time;

	if (life_time_ >= life_span_){
		active_ = false;
		first_life_ = false;
		life_time_ = 0.f;
	}

	if(!first_life_  && max_height_ != original_pos_.y)
	{
		original_pos_.y =  min_height_ + static_cast <float> (rand( )) / (RAND_MAX / (max_height_ - min_height_));
	}
}

auto particle::update_death(const float raw_time) -> void
{
	this->death_time_ += raw_time;

	if (death_time_ >= death_span_){
		active_ = true;
		death_time_ = 0.f;
		pos_ = original_pos_;
	}
}

void particle::update(const float raw_time) {
	if (active_) {
		update_life(raw_time);
	}
	else update_death(raw_time);
}

bool particle::is_active( ) const
{
	return active_;
}

vec3 particle::get_current_pos( ) const
{
	return pos_;
}

auto particle::translate_position(const vec3 model) -> void
{
	this->pos_ += model;
}

ParticleSystem::ParticleSystem(const particle_type particle, global_data* data, const uint number_of_particles,
                               const uint particle_size, const vec3 original_point, const vec3 move_vector, const vec3 gravitation_vector,
                               const vec3 rotation_vector, const vec4 color_and_alpha, const vec2 plain, const float height, const float max_life_span,
                               const float res_time, const bool randomize_movement)
{
	this->number_of_particles_ = number_of_particles;
	this->original_point_ = original_point;
	this->particle_size_ = particle_size;

	this->move_ = move_vector;
	this->gravitation_ = gravitation_vector;
	this->rotation_ = rotation_vector;

	this->rgba_ = color_and_alpha;

	this->plain_ = plain;
	this->height_ = height;

	this->life_span_ = max_life_span;
	this->res_time_ = res_time;

	this->particle_ = particle;
	this->randomize_movement_ = randomize_movement;

	this->combined_movement_ = move_ + gravitation_ + rotation_;
	this->particle_shader_ = new shader("C:/Users/part4/Desktop/grafika_2.0/ogl-master/pg_dg/lab3/shaders/Particle.vert",
		"C:/Users/part4/Desktop/grafika_2.0/ogl-master/pg_dg/lab3/shaders/Particle.frag");

	this->plain_shader_ = new shader("C:/Users/part4/Desktop/grafika_2.0/ogl-master/pg_dg/lab3/shaders/Basic.vert",
		"C:/Users/part4/Desktop/grafika_2.0/ogl-master/pg_dg/lab3/shaders/Color.frag");

	this->data_ = data;

	projection_uniform_loc_ = glGetUniformLocation(particle_shader_->id, "projection");
	model_uniform_loc_ = glGetUniformLocation(particle_shader_->id, "model");
	view_uniform_loc_ = glGetUniformLocation(particle_shader_->id, "view");

	projection_uniform_loc_plain_ = glGetUniformLocation(plain_shader_->id, "projection");
	model_uniform_loc_plain_ = glGetUniformLocation(plain_shader_->id, "model");
	view_uniform_loc_plain_ = glGetUniformLocation(plain_shader_->id, "view");

	calculate_particles( );
}

auto ParticleSystem::enable_rendering_plain() -> void
{
	render_plain_ = true;
}

auto ParticleSystem::update(const double new_time) -> void
{
	raw_time_ = new_time - current_time_;
    current_time_ = new_time;

	vec3 movement_vector;

	if(randomize_movement_)
	{
		const float is_negative = 0 + rand() % (1 - 0 + 1) == 1;
		movement_vector = static_cast<float>(raw_time_  * (is_negative ? -1.0 : 1.0)) + combined_movement_;
		if(particle_ != fog && is_negative)
		{
			movement_vector.y *= 1.3f;
		}
	}else
	{
		movement_vector = combined_movement_;
	}

	for (const auto p : particles_) {
		p->update(raw_time_);
		p->translate_position(movement_vector);
	}
}


auto ParticleSystem::calculate_particles() -> void
{
	const float min_x = original_point_.x - plain_.x;
	const float max_x = original_point_.x + plain_.x;

	const float min_y = original_point_.y - height_;
	const float max_y = original_point_.y + height_;

	const float min_z = original_point_.z - plain_.y;
	const float max_z = original_point_.z + plain_.y;

	

	const GLfloat plain_vertices[] = {
		//upper
		 max_x,  min_y, min_z,		 .5f, 1.0f, .85f, 1.f,		//top right			- 0
		 max_x,  min_y, min_z,		 .5f, 1.0f, .85f, 1.f,		//bottom right		- 1
		 min_x,  min_y, min_z,		 .5f, 1.0f, .85f, 1.f,		//bottom left		- 2
		 min_x,  min_y, min_z,		 .5f, 1.0f, .85f, 1.f,		//top left			- 3
		 
		 //lower
		 max_x,  min_y + 2.f, max_z,		 .0f, 1.0f, .0f, 1.f,		//top right			- 0
		 max_x,  min_y + 2.f, max_z,		 .0f, 1.0f, .0f, 1.f,		//bottom right		- 1
		 min_x,  min_y + 2.f, max_z,		 .0f, 1.0f, .0f, 1.f,		//bottom left		- 2
		 min_x,  min_y + 2.f, max_z,		 .0f, 1.0f, .0f, 1.f,		//top left			- 3
	};

	const unsigned int indices[] = {
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

	glGenVertexArrays(1, &vao_1_);
	glGenBuffers(1, &vbo_1_);
	glGenBuffers(1, &ebo_1_);

	glBindVertexArray(vao_1_);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_1_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(plain_vertices), plain_vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_1_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	//set vertex position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (7 * sizeof(float)), static_cast<void*>(nullptr));
	glEnableVertexAttribArray(0);

	//set color attribute
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, (7 * sizeof(float)), reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	glGenVertexArrays(1, &vao_2_);
	glGenBuffers(1, &vbo_2_);

	glBindVertexArray(vao_2_);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_2_);

	switch (particle_)
	{
	case rain:
		{
			const GLfloat rain_vertices[] = {
				original_point_.x, original_point_.y, original_point_.z, rgba_.r, rgba_.g, rgba_.b, rgba_.a,
				original_point_.x, original_point_.y - static_cast<float>( this->particle_size_), original_point_.z, rgba_.r, rgba_.g, rgba_.b, rgba_.a,
			};
			glBufferData(GL_ARRAY_BUFFER, sizeof(rain_vertices), rain_vertices, GL_STATIC_DRAW);
			break;
		}
	case smoke:
		{
			const GLfloat smoke_vertices[] = {
				original_point_.x, original_point_.y, original_point_.z, rgba_.r, rgba_.g, rgba_.b, rgba_.a,

				//
				original_point_.x  - static_cast<float>( this->particle_size_), original_point_.y, original_point_.z, rgba_.r + 0.02f, rgba_.g + 0.02f, rgba_.b + 0.02f, rgba_.a - 0.1f,
				original_point_.x + static_cast<float>( this->particle_size_), original_point_.y, original_point_.z, rgba_.r + 0.02f, rgba_.g + 0.02f, rgba_.b + 0.02f, rgba_.a - 0.1f,

				original_point_.x, original_point_.y - static_cast<float>( this->particle_size_), original_point_.z, rgba_.r + 0.02f, rgba_.g + 0.02f, rgba_.b + 0.02f, rgba_.a - 0.1f,
				original_point_.x, original_point_.y + static_cast<float>( this->particle_size_), original_point_.z, rgba_.r + 0.02f, rgba_.g + 0.02f, rgba_.b + 0.02f, rgba_.a - 0.1f,

				original_point_.x, original_point_.y, original_point_.z - static_cast<float>( this->particle_size_), rgba_.r + 0.02f, rgba_.g + 0.02f, rgba_.b + 0.02f, rgba_.a - 0.1f,
				original_point_.x, original_point_.y, original_point_.z + static_cast<float>( this->particle_size_), rgba_.r + 0.02f, rgba_.g + 0.02f, rgba_.b + 0.02f, rgba_.a - 0.1f,

				original_point_.x - static_cast<float>( this->particle_size_), original_point_.y - static_cast<float>( this->particle_size_), original_point_.z, rgba_.r + 0.02f, rgba_.g + 0.02f, rgba_.b + 0.02f, rgba_.a - 0.1f,
				original_point_.x + static_cast<float>( this->particle_size_), original_point_.y + static_cast<float>( this->particle_size_), original_point_.z, rgba_.r + 0.02f, rgba_.g + 0.02f, rgba_.b + 0.02f, rgba_.a - 0.1f,

				original_point_.x + static_cast<float>( this->particle_size_), original_point_.y - static_cast<float>( this->particle_size_), original_point_.z, rgba_.r + 0.02f, rgba_.g + 0.02f, rgba_.b + 0.02f, rgba_.a - 0.1f,
				original_point_.x - static_cast<float>( this->particle_size_), original_point_.y + static_cast<float>( this->particle_size_), original_point_.z, rgba_.r + 0.02f, rgba_.g + 0.02f, rgba_.b + 0.02f, rgba_.a - 0.1f,

				original_point_.x + static_cast<float>( this->particle_size_), original_point_.y, original_point_.z - static_cast<float>( this->particle_size_), rgba_.r + 0.02f, rgba_.g + 0.02f, rgba_.b + 0.02f, rgba_.a - 0.1f,
				original_point_.x - static_cast<float>( this->particle_size_), original_point_.y, original_point_.z + static_cast<float>( this->particle_size_), rgba_.r + 0.02f, rgba_.g + 0.02f, rgba_.b + 0.02f, rgba_.a - 0.1f,

				original_point_.x, original_point_.y + static_cast<float>( this->particle_size_), original_point_.z - static_cast<float>( this->particle_size_), rgba_.r + 0.02f, rgba_.g + 0.02f, rgba_.b + 0.02f, rgba_.a - 0.1f,
				original_point_.x, original_point_.y - static_cast<float>( this->particle_size_), original_point_.z + static_cast<float>( this->particle_size_), rgba_.r + 0.02f, rgba_.g + 0.02f, rgba_.b + 0.02f, rgba_.a - 0.1f,

				original_point_.x + static_cast<float>( this->particle_size_), original_point_.y + static_cast<float>( this->particle_size_), original_point_.z + static_cast<float>( this->particle_size_), rgba_.r + 0.02f, rgba_.g + 0.02f, rgba_.b + 0.02f, rgba_.a - 0.1f,
				original_point_.x + static_cast<float>( this->particle_size_), original_point_.y + static_cast<float>( this->particle_size_), original_point_.z - static_cast<float>( this->particle_size_), rgba_.r + 0.02f, rgba_.g + 0.02f, rgba_.b + 0.02f, rgba_.a - 0.1f,
				original_point_.x + static_cast<float>( this->particle_size_), original_point_.y - static_cast<float>( this->particle_size_), original_point_.z - static_cast<float>( this->particle_size_), rgba_.r + 0.02f, rgba_.g + 0.02f, rgba_.b + 0.02f, rgba_.a - 0.1f,

				original_point_.x - static_cast<float>( this->particle_size_), original_point_.y - static_cast<float>( this->particle_size_), original_point_.z - static_cast<float>( this->particle_size_), rgba_.r + 0.02f, rgba_.g + 0.02f, rgba_.b + 0.02f, rgba_.a - 0.1f,
				original_point_.x - static_cast<float>( this->particle_size_), original_point_.y - static_cast<float>( this->particle_size_), original_point_.z + static_cast<float>( this->particle_size_), rgba_.r + 0.02f, rgba_.g + 0.02f, rgba_.b + 0.02f, rgba_.a - 0.1f,
				original_point_.x - static_cast<float>( this->particle_size_), original_point_.y + static_cast<float>( this->particle_size_), original_point_.z + static_cast<float>( this->particle_size_), rgba_.r + 0.02f, rgba_.g + 0.02f, rgba_.b + 0.02f, rgba_.a - 0.1f,
				};
			glBufferData(GL_ARRAY_BUFFER, sizeof(smoke_vertices), smoke_vertices, GL_STATIC_DRAW);
			break;
		}
	case fog:
		{
			const GLfloat smoke_vertices[] = {
				original_point_.x, original_point_.y, original_point_.z, rgba_.r, rgba_.g, rgba_.b, rgba_.a,

				//
				original_point_.x  - static_cast<float>( this->particle_size_), original_point_.y, original_point_.z, rgba_.r + 0.02f, rgba_.g + 0.02f, rgba_.b + 0.02f, rgba_.a - 0.1f,
				original_point_.x + static_cast<float>( this->particle_size_), original_point_.y, original_point_.z, rgba_.r + 0.02f, rgba_.g + 0.02f, rgba_.b + 0.02f, rgba_.a - 0.1f,

				original_point_.x, original_point_.y - static_cast<float>( this->particle_size_), original_point_.z, rgba_.r + 0.02f, rgba_.g + 0.02f, rgba_.b + 0.02f, rgba_.a - 0.1f,
				original_point_.x, original_point_.y + static_cast<float>( this->particle_size_), original_point_.z, rgba_.r + 0.02f, rgba_.g + 0.02f, rgba_.b + 0.02f, rgba_.a - 0.1f,

				original_point_.x, original_point_.y, original_point_.z - static_cast<float>( this->particle_size_), rgba_.r + 0.02f, rgba_.g + 0.02f, rgba_.b + 0.02f, rgba_.a - 0.1f,
				original_point_.x, original_point_.y, original_point_.z + static_cast<float>( this->particle_size_), rgba_.r + 0.02f, rgba_.g + 0.02f, rgba_.b + 0.02f, rgba_.a - 0.1f,

				original_point_.x - static_cast<float>( this->particle_size_), original_point_.y - static_cast<float>( this->particle_size_), original_point_.z, rgba_.r + 0.02f, rgba_.g + 0.02f, rgba_.b + 0.02f, rgba_.a - 0.1f,
				original_point_.x + static_cast<float>( this->particle_size_), original_point_.y + static_cast<float>( this->particle_size_), original_point_.z, rgba_.r + 0.02f, rgba_.g + 0.02f, rgba_.b + 0.02f, rgba_.a - 0.1f,

				original_point_.x + static_cast<float>( this->particle_size_), original_point_.y - static_cast<float>( this->particle_size_), original_point_.z, rgba_.r + 0.02f, rgba_.g + 0.02f, rgba_.b + 0.02f, rgba_.a - 0.1f,
				original_point_.x - static_cast<float>( this->particle_size_), original_point_.y + static_cast<float>( this->particle_size_), original_point_.z, rgba_.r + 0.02f, rgba_.g + 0.02f, rgba_.b + 0.02f, rgba_.a - 0.1f,

				original_point_.x + static_cast<float>( this->particle_size_), original_point_.y, original_point_.z - static_cast<float>( this->particle_size_), rgba_.r + 0.02f, rgba_.g + 0.02f, rgba_.b + 0.02f, rgba_.a - 0.1f,
				original_point_.x - static_cast<float>( this->particle_size_), original_point_.y, original_point_.z + static_cast<float>( this->particle_size_), rgba_.r + 0.02f, rgba_.g + 0.02f, rgba_.b + 0.02f, rgba_.a - 0.1f,

				original_point_.x, original_point_.y + static_cast<float>( this->particle_size_), original_point_.z - static_cast<float>( this->particle_size_), rgba_.r + 0.02f, rgba_.g + 0.02f, rgba_.b + 0.02f, rgba_.a - 0.1f,
				original_point_.x, original_point_.y - static_cast<float>( this->particle_size_), original_point_.z + static_cast<float>( this->particle_size_), rgba_.r + 0.02f, rgba_.g + 0.02f, rgba_.b + 0.02f, rgba_.a - 0.1f,

				original_point_.x + static_cast<float>( this->particle_size_), original_point_.y + static_cast<float>( this->particle_size_), original_point_.z + static_cast<float>( this->particle_size_), rgba_.r + 0.02f, rgba_.g + 0.02f, rgba_.b + 0.02f, rgba_.a - 0.1f,
				original_point_.x + static_cast<float>( this->particle_size_), original_point_.y + static_cast<float>( this->particle_size_), original_point_.z - static_cast<float>( this->particle_size_), rgba_.r + 0.02f, rgba_.g + 0.02f, rgba_.b + 0.02f, rgba_.a - 0.1f,
				original_point_.x + static_cast<float>( this->particle_size_), original_point_.y - static_cast<float>( this->particle_size_), original_point_.z - static_cast<float>( this->particle_size_), rgba_.r + 0.02f, rgba_.g + 0.02f, rgba_.b + 0.02f, rgba_.a - 0.1f,

				original_point_.x - static_cast<float>( this->particle_size_), original_point_.y - static_cast<float>( this->particle_size_), original_point_.z - static_cast<float>( this->particle_size_), rgba_.r + 0.02f, rgba_.g + 0.02f, rgba_.b + 0.02f, rgba_.a - 0.1f,
				original_point_.x - static_cast<float>( this->particle_size_), original_point_.y - static_cast<float>( this->particle_size_), original_point_.z + static_cast<float>( this->particle_size_), rgba_.r + 0.02f, rgba_.g + 0.02f, rgba_.b + 0.02f, rgba_.a - 0.1f,
				original_point_.x - static_cast<float>( this->particle_size_), original_point_.y + static_cast<float>( this->particle_size_), original_point_.z + static_cast<float>( this->particle_size_), rgba_.r + 0.02f, rgba_.g + 0.02f, rgba_.b + 0.02f, rgba_.a - 0.1f,
				};
			glBufferData(GL_ARRAY_BUFFER, sizeof(smoke_vertices), smoke_vertices, GL_STATIC_DRAW);
			break;
		}
	}

	//set vertex position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (7 * sizeof(float)), static_cast<void*>(nullptr));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, (7 * sizeof(float)), reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	srand(static_cast <unsigned> (time(nullptr)));

	for (uint i = 0; i < number_of_particles_; i++){
		float x,y,z;

		vec3 particle_pos;

		particle* p;

		switch (this->particle_)
		{
			case rain :
				{
					x = min_x + static_cast <float> (rand( )) / (RAND_MAX / (max_x - min_x));
					y = min_y + static_cast <float> (rand( )) / (RAND_MAX / (max_y - min_y));
					z = min_z + static_cast <float> (rand( )) / (RAND_MAX / (max_z - min_z));

					particle_pos = vec3(x, y, z);

					const float current_life_time = res_time_ + static_cast <float> (rand( )) / (RAND_MAX / (life_span_ - res_time_));

					p = new particle(particle_pos, life_span_, res_time_, current_life_time, max_y);
					break;
				}
			case smoke :
				{
					const float smoke_max_y = min_y  * 2.0f;
					x = (min_x / 1.6f) + static_cast <float> (rand( )) / (RAND_MAX / ((max_x/ 1.6f) - (min_x/ 1.6f)));
					y = min_y + static_cast <float> (rand( )) / (RAND_MAX / (smoke_max_y + min_y)) + min_y;
					z = (min_z / 1.6f) + static_cast <float> (rand( )) / (RAND_MAX / ((max_z/ 1.6f) - (min_z/ 1.6f)));

					y /= 1.4f;

					particle_pos = vec3(x, y, z);

					const float current_life_time = res_time_ + static_cast <float> (rand( )) / (RAND_MAX / (life_span_ - res_time_));
					p = new particle(particle_pos, life_span_, res_time_, current_life_time, smoke_max_y);
					break;
				}
			case fog :
				{
					x = min_x + static_cast <float> (rand( )) / (RAND_MAX / (max_x - min_x));
					y = min_y + static_cast <float> (rand( )) / (RAND_MAX / (max_y - min_y));
					z = min_z + static_cast <float> (rand( )) / (RAND_MAX / (max_z - min_z));

					particle_pos = vec3(x, y, z);

					const float current_life_time = res_time_ + static_cast <float> (rand( )) / (RAND_MAX / (life_span_ - res_time_));
					p = new particle(particle_pos, life_span_, res_time_, current_life_time, max_y);
					break;
				}
		}

		particles_.push_back(p);
	}
}

auto ParticleSystem::draw_plain(const GLuint* vao, const uint number_of_indices) -> void
{
	glBindVertexArray(*vao);

	glDrawElements(GL_TRIANGLES, number_of_indices, GL_UNSIGNED_INT, nullptr);
}

void ParticleSystem::render( ) {
	mat4 projection = data_->projection;
	mat4 view = data_->view;
	mat4 model = data_->model;

	plain_shader_->activate( );
	
	if (render_plain_){
		glBindVertexArray(vao_1_);
		glUniformMatrix4fv(projection_uniform_loc_plain_, 1, GL_FALSE, value_ptr(projection));
		glUniformMatrix4fv(view_uniform_loc_plain_, 1, GL_FALSE, value_ptr(view));
		glUniformMatrix4fv(model_uniform_loc_plain_, 1, GL_FALSE, value_ptr(model));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
	}

	glUniformMatrix4fv(projection_uniform_loc_, 1, GL_FALSE, value_ptr(projection));
	glUniformMatrix4fv(view_uniform_loc_, 1, GL_FALSE, value_ptr(view));

	for (const auto p : particles_) {
		if (p->is_active( )){
			glBindVertexArray(vao_2_);
			current_model_ = translate(model, p->get_current_pos( ));
			glUniformMatrix4fv(model_uniform_loc_, 1, GL_FALSE, value_ptr(current_model_));

			switch (particle_)
			{
			case rain:
				{
					glDrawArrays(GL_LINES,0,2);
					break;
				}
			case smoke:
				{
					glPointSize(particle_size_ * 2.3f);
					glDrawArrays(GL_POINTS, 0, 24);
					break;
				}
			case fog:
				{
					glPointSize(particle_size_ * 2.3f);
					glDrawArrays(GL_POINTS,0,24);
					break;
				}
			}
		}
	}

}