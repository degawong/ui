/*
 * @Author: your name
 * @Date: 2021-01-18 15:34:30
 * @LastEditTime: 2021-01-18 15:49:45
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \harpocrates\opengl\opengl.cpp
 */


#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>
#include <opengl/opengl.hpp>

namespace harpocrates {
	using namespace std;
	const unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	const float vertices[] = {
		// positions      // texture coords
		1.0f, 1.0f, 0.0f, 1.0f, 0.0f,      // top right
		1.0f, -1.0f, 0.0f, 1.0f, 1.0f,     // bottom right
		-1.0f, -1.0f, 0.0f, 0.0f, 1.0f,    // bottom left
		-1.0f, 1.0f, 0.0f, 0.0f, 0.0f      // top left 
	};

	void OpenGL::clear(
		float r,
		float g,
		float b,
		float a
	) {
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	Texture::Texture() {
		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &__id);
	}

	Texture::~Texture() {
		glDeleteTextures(1, &__id);
	}

	unsigned int Texture::get_id() {
		return __id;
	}

	int Texture::bind() {
		glBindTexture(GL_TEXTURE_2D, __id);
		return 0;
	}

	int Texture::active(unsigned int texture_index) {
		glActiveTexture(texture_index);
		return 0;
	}

	int Texture::apply(int width, int height, bool is_bgr, unsigned char *data) {
		/*
			format : GL_ALPHA, GL_RGB, GL_BGR, GL_RGBA, GL_LUMINANCE, GL_LUMINANCE_ALPHA
			type : GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT_5_6_5, GL_UNSIGNED_SHORT_4_4_4_4, GL_UNSIGNED_SHORT_5_5_5_1
		*/
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGB,
			width,
			height,
			0,
			(is_bgr ? GL_BGR : GL_RGB),
			GL_UNSIGNED_BYTE,
			data
		);
		glGenerateMipmap(GL_TEXTURE_2D);
		// set the texture wrapping parameters
		// set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		return 0;
	}

	Render::Render() {
		glGenBuffers(1, &__vbo);
		glGenBuffers(1, &__ebo);
		glGenVertexArrays(1, &__vao);
	}

	Render::~Render() {
		glDeleteBuffers(1, &__vbo);
		glDeleteBuffers(1, &__ebo);
		glDeleteVertexArrays(1, &__vao);
	}

	int Render::set_vao() {
		glBindVertexArray(__vao);
		return 0;
	}

	int Render::set_vbo(int length, void * data, unsigned int usage) {
		// usage = GL_STATIC_DRAW
		glBindBuffer(GL_ARRAY_BUFFER, __vbo);
		glBufferData(GL_ARRAY_BUFFER, length, data, usage);
		return 0;
	}

	int Render::set_ebo(int length, void * data, unsigned int usage) {
		// usage = GL_STATIC_DRAW
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, __ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, length, data, usage);
		return 0;
	}

	int Render::set_attribution(
		unsigned int index,
		int length,
		unsigned int data_type,
		unsigned char is_normalized,
		int stride,
		void* pointer) {
		// position color texture
		glVertexAttribPointer(index, length, data_type, is_normalized, stride, pointer);
		glEnableVertexAttribArray(index);
		return 0;
	}

	int Render::rending(
		unsigned int mode,
		int count,
		unsigned int type,
		const void* indices
	) {
		// glDrawArrays
		glDrawElements(mode, count, type, indices);
		return 0;
	}

	Shader::Shader() {
		__id = glCreateProgram();
	}

	Shader::~Shader() {
		glDeleteProgram(__id);
	}

	void Shader::use() {
		glUseProgram(__id);
	}

	int Shader::attach(int index) {
		auto res = 0;
		auto vertex_path = __vertex_path[index];
		auto fragment_path = __fragment_path[index];
		auto in_vertex_shader = ifstream();
		auto in_fragment_shader = ifstream();
		auto vertex_stream = stringstream();
		auto fragment_stream = stringstream();
		auto vertex = glCreateShader(GL_VERTEX_SHADER);
		auto fragment = glCreateShader(GL_FRAGMENT_SHADER);
		do {
			auto status = 0;
			in_vertex_shader.open(vertex_path);
			in_fragment_shader.open(fragment_path);
			vertex_stream << in_vertex_shader.rdbuf();
			fragment_stream << in_fragment_shader.rdbuf();
			// vertex shader
			auto vc_string = vertex_stream.str();
			auto vc = vc_string.c_str();
			glShaderSource(vertex, 1, &vc, nullptr);
			glCompileShader(vertex);
			glGetShaderiv(vertex, GL_COMPILE_STATUS, &status);
			if (status == GL_FALSE) { res = 1; break; }
			// fragment shader
			auto fc_string = fragment_stream.str();
			auto fc = fc_string.c_str();
			glShaderSource(fragment, 1, &fc, nullptr);
			glCompileShader(fragment);
			glGetShaderiv(fragment, GL_COMPILE_STATUS, &status);
			if (status == GL_FALSE) { res = 1; break; }
			// attach
			glAttachShader(__id, vertex);
			glAttachShader(__id, fragment);
			glLinkProgram(__id);
			glGetProgramiv(__id, GL_LINK_STATUS, &status);
			if (status == GL_FALSE) { res = 1; break; }
		} while (0);
		glDeleteShader(vertex);
		glDeleteShader(fragment);
		in_vertex_shader.close();
		in_fragment_shader.close();
		return res;
	}

	void Shader::add_shader(std::string vertex_path, std::string fragment_path) {
		__vertex_path.push_back(vertex_path);
		__fragment_path.push_back(fragment_path);
	}

	void Shader::set_float(std::string name, float data) {
		glUniform1f(glGetUniformLocation(__id, name.c_str()), data);
	}

	void Shader::set_boolean(std::string name, bool data) {
		glUniform1i(glGetUniformLocation(__id, name.c_str()), (int)data);
	}

	void Shader::set_interger(std::string name, int data) {
		glUniform1i(glGetUniformLocation(__id, name.c_str()), data);
	}

	void Shader::set_matrix2(std::string name, int count, const float * data) {
		glUniformMatrix2fv(glGetUniformLocation(__id, name.c_str()), count, false, data);
	}

	void Shader::set_matrix3(std::string name, int count, const float * data) {
		glUniformMatrix3fv(glGetUniformLocation(__id, name.c_str()), count, false, data);
	}

	void Shader::set_matrix4(std::string name, int count, const float * data) {
		glUniformMatrix4fv(glGetUniformLocation(__id, name.c_str()), count, false, data);
	}

}
