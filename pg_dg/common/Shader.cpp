//
// Created by part4 on 28.01.2022.
//

#include "Shader.h"

#include <GL/glew.h>
#include <iostream>
#include <fstream>


shader::shader(const char* vertex_path, const char* fragment_path)
{
	// 1. retrieve the vertex/fragment source code from filePath
	string vertex_code;
	string fragment_code;
	ifstream v_shader_file;
	ifstream f_shader_file;
	// ensure ifstream objects can throw exceptions:

	v_shader_file.exceptions(ifstream::failbit | ifstream::badbit);
	f_shader_file.exceptions(ifstream::failbit | ifstream::badbit);
	try
	{
		// open files
		v_shader_file = ifstream(vertex_path, ios::in);
		f_shader_file = ifstream(fragment_path, ios::in);
		stringstream v_shader_stream, f_shader_stream;
		// read file's buffer contents into streams
		v_shader_stream << v_shader_file.rdbuf();
		f_shader_stream << f_shader_file.rdbuf();
		// close file handlers
		v_shader_file.close();
		f_shader_file.close();
		// convert stream into string
		vertex_code = v_shader_stream.str();
		fragment_code = f_shader_stream.str();
	}
	catch (ifstream::failure e)
	{
		cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << endl;
	}
	const char* v_shader_code = vertex_code.c_str();
	const char* f_shader_code = fragment_code.c_str();


	// 2. compile shaders
	unsigned int vertex, fragment;
	int success;
	char info_log[512];

	// vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &v_shader_code, nullptr);
	glCompileShader(vertex);
	// print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, nullptr, info_log);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
	}

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &f_shader_code, nullptr);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, nullptr, info_log);
		cout << "ERROR::SHADER::FRAG::COMPILATION_FAILED\n" << info_log << endl;
	}

	// shader Program
	id = glCreateProgram();
	glAttachShader(id, vertex);
	glAttachShader(id, fragment);
	glLinkProgram(id);

	// print linking errors if any
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(id, 512, nullptr, info_log);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
	}

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void shader::activate() const
{
	glUseProgram(id);
}

void shader::deactivate() const
{
	glDeleteProgram(id);
}

void shader::set_bool(const std::string& name, const bool value) const
{
	glUniform1i(glGetUniformLocation(id, name.c_str()), static_cast<int>(value));
}

void shader::set_int(const std::string& name, const int value) const
{
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void shader::set_float(const std::string& name, const float value) const
{
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

shader::shader() = default;
