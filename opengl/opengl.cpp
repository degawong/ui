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

	void OpenGL::clear(unsigned int mask) {
		glClear(mask);
	}

	void OpenGL::clear_color(float r, float g, float b, float a) {
		glClearColor(r, g, b, a);
	}

	Texture::Texture() {
		glEnable(GL_TEXTURE_2D);
	}

	Texture::~Texture() {
		for (auto ref : __id) {
			glDeleteTextures(1, &ref);
		}
	}

	int Texture::gen_texture() {
		unsigned int index = 0;
		glGenTextures(1, &index);
		__id.push_back(index);
		return 0;
	}

	unsigned int Texture::get_id(int index) {
		return __id[index];
	}

	int Texture::bind(int index) {
		glBindTexture(GL_TEXTURE_2D, __id[index]);
		return 0;
	}

	int Texture::active_texture(unsigned int texture_index) {
		glActiveTexture(texture_index);
		return 0;
	}

	int Texture::apply(int index, int width, int height, bool is_bgr, unsigned char *data) {
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

		// need segment
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

	Render::~Render() {
		for (auto ref : __vbo) {
			glDeleteBuffers(1, &ref);
		}
		for (auto ref : __ebo) {
			glDeleteBuffers(1, &ref);
		}
		for (auto ref : __fbo) {
			glDeleteFramebuffers(1, &ref);
		}
		for (auto ref : __vao) {
			glDeleteVertexArrays(1, &ref);
		}
	}

	int Render::gen_vao() {
		unsigned int index = 0;
		glGenVertexArrays(1, &index);
		__vao.push_back(index);
		return 0;
	}

	int Render::bind_vao(int index) {
		glBindVertexArray(__vao[index]);
		return 0;
	}

	int Render::gen_vbo() {
		unsigned int index = 0;
		glGenBuffers(1, &index);
		__vbo.push_back(index);
		return 0;
	}

	int Render::bind_vbo(int index, int length, void * data, unsigned int usage) {
		// usage = GL_STATIC_DRAW
		glBindBuffer(GL_ARRAY_BUFFER, __vbo[index]);
		glBufferData(GL_ARRAY_BUFFER, length, data, usage);
		return 0;
	}

	int Render::gen_ebo() {
		unsigned int index = 0;
		glGenBuffers(1, &index);
		__ebo.push_back(index);
		return 0;
	}

	int Render::bind_ebo(int index, int length, void* data, unsigned int usage) {
		// usage = GL_STATIC_DRAW
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, __ebo[index]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, length, data, usage);
		return 0;
	}

	int Render::gen_fbo() {
		unsigned int index = 0;
		glGenFramebuffers(1, &index);
		__fbo.push_back(index);
		return 0;
	}

	int Render::bind_fbo(int index){
		glBindFramebuffer(GL_FRAMEBUFFER, __fbo[index]);
		return 0;
	}

	int Render::bind_texture_to_fbo(unsigned int id) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, id, 0);
		return 0;
	}

	int Render::check_frame_buffer_status() {
		// call when we have created the framebuffer and added all attachments
		return (int)(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE);
	}

	int Render::unbind_fbo() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return 0;
	}

	int Render::gen_cbo() {
		unsigned int index = 0;
		glGenTextures(1, &index);
		__cbo.push_back(index);
		return 0;
	}

	int Render::bind_cbo(int index, int width, int height, unsigned int attach_id) {
		// attach_id = GL_COLOR_ATTACHMENT0
		glBindTexture(GL_TEXTURE_2D, __cbo[index]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, attach_id, GL_TEXTURE_2D, __cbo[index], 0);
		return 0;
	}

	int Render::gen_rbo() {
		unsigned int index = 0;
		glGenRenderbuffers(1, &index);
		__rbo.push_back(index);
		return 0;
	}

	int Render::bind_rbo(int index, int width, int height, unsigned int attach_id) {
		glBindRenderbuffer(GL_RENDERBUFFER, __rbo[index]);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, __rbo[index]);
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

	Shader::~Shader() {
		for (auto ref : __id) {
			glDeleteProgram(ref);
		}
	}

	int Shader::gen_shader() {
		unsigned int index = 0;
		index = glCreateProgram();
		__id.push_back(index);
		return 0;
	}

	int Shader::use(int index) {
		glUseProgram(__id[index]);
		return 0;
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
			glAttachShader(__id[index], vertex);
			glAttachShader(__id[index], fragment);
			glLinkProgram(__id[index]);
			glGetProgramiv(__id[index], GL_LINK_STATUS, &status);
			if (status == GL_FALSE) { res = 1; break; }
		} while (0);
		glDeleteShader(vertex);
		glDeleteShader(fragment);
		in_vertex_shader.close();
		in_fragment_shader.close();
		return res;
	}

	int Shader::add_shader(std::string vertex_path, std::string fragment_path) {
		__vertex_path.push_back(vertex_path);
		__fragment_path.push_back(fragment_path);
		return 0;
	}

	void Shader::set_float(int index, std::string name, float data) {
		glUniform1f(glGetUniformLocation(__id[index], name.c_str()), data);
	}

	void Shader::set_boolean(int index, std::string name, bool data) {
		glUniform1i(glGetUniformLocation(__id[index], name.c_str()), (int)data);
	}

	void Shader::set_interger(int index, std::string name, int data) {
		glUniform1i(glGetUniformLocation(__id[index], name.c_str()), data);
	}

	void Shader::set_matrix2(int index, std::string name, int count, const float * data) {
		glUniformMatrix2fv(glGetUniformLocation(__id[index], name.c_str()), count, false, data);
	}

	void Shader::set_matrix3(int index, std::string name, int count, const float * data) {
		glUniformMatrix3fv(glGetUniformLocation(__id[index], name.c_str()), count, false, data);
	}

	void Shader::set_matrix4(int index, std::string name, int count, const float * data) {
		glUniformMatrix4fv(glGetUniformLocation(__id[index], name.c_str()), count, false, data);
	}

}
