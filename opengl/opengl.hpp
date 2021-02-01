/*
 * @Author: your name
 * @Date: 2021-01-18 15:34:30
 * @LastEditTime: 2021-01-18 15:49:21
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \harpocrates\opengl\opengl.hpp
 */

#pragma once

#include <string>
#include <fstream>

#include <vector>

namespace harpocrates {

	class OpenGL {
	public:
		OpenGL() = default;
		~OpenGL() = default;
	public:
		void clear(unsigned int mask);
		void clear_color(float r = 1.f, float g = 1.f, float b = 0.f, float a = 1.f);
	};

	class Shader {
	public:
		Shader() = default;
		~Shader();
	public:
		int gen_shader();
		int use(int index);
		int attach(int index);
		int add_shader(std::string vertex_path, std::string fragment_path);
	public:
		void set_float(int index, std::string name, float data);
		void set_boolean(int index, std::string name, bool data);
		void set_interger(int index, std::string name, int data);
		void set_matrix2(int index, std::string name, int count, const float* data);
		void set_matrix3(int index, std::string name, int count, const float* data);
		void set_matrix4(int index, std::string name, int count, const float* data);
	private:
		std::vector<unsigned char> __id;
		std::vector<std::string> __vertex_path;
		std::vector<std::string> __fragment_path;
	};

	class Texture {
	public:
		Texture();
		~Texture();
	public:
		int gen_texture();
		unsigned int get_id(int index);
		int bind(int index);
		int active_texture(unsigned int texture_index);
		int apply(int index, int width, int height, bool is_bgr, unsigned char *data);
	private:
		std::vector<unsigned int> __id;
	};

	class Render {
		// only for one vao vbo ebo
	public:
		Render() = default;
		~Render();
	public:
		int gen_vao();
		int bind_vao(int index);
		int gen_vbo();
		int bind_vbo(int index, int length, void * data, unsigned int usage);
		int gen_ebo();
		int bind_ebo(int index, int length, void * data, unsigned int usage);
		int gen_fbo();
		int bind_fbo(int index);
		int bind_texture_to_fbo(unsigned int id);
		int check_frame_buffer_status();
		int unbind_fbo();
		int gen_cbo();
		int bind_cbo(int index, int width, int height, unsigned int attach_id);
		int gen_rbo();
		int bind_rbo(int index, int width, int height, unsigned int attach_id);
		int set_attribution(
			unsigned int index,
			int length,
			unsigned int data_type,
			unsigned char is_normalized,
			int stride,
			void* pointer
		);
		int rending(
			unsigned int mode,
			int count,
			unsigned int type,
			const void* indices
		);
	private:
		std::vector<unsigned int> __vao;
		std::vector<unsigned int> __vbo;
		std::vector<unsigned int> __ebo;
		std::vector<unsigned int> __fbo;
		std::vector<unsigned int> __rbo;
		std::vector<unsigned int> __cbo;
	};

}