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
		void clear(float r = 0.f, float g = .0f, float b = 1.f, float a = 1.f);
	};

	class Shader {
	public:
		Shader();
		~Shader();
	public:
		void use();
		int attach(int index);
		void add_shader(std::string vertex_path, std::string fragment_path);
	public:
		void set_float(std::string name, float data);
		void set_boolean(std::string name, bool data);
		void set_interger(std::string name, int data);
		void set_matrix2(std::string name, int count, const float* data);
		void set_matrix3(std::string name, int count, const float* data);
		void set_matrix4(std::string name, int count, const float* data);
	private:
		unsigned char __id;
		std::vector<std::string> __vertex_path;
		std::vector<std::string> __fragment_path;
	};

	class Texture {
	public:
		Texture();
		~Texture();
	public:
		int bind();
		unsigned int get_id();
		int active(unsigned int texture_index);
		int apply(int width, int height, bool is_bgr, unsigned char *data);
	private:
		unsigned int __id;
	};

	class Render {
		// only for one vao vbo ebo
	public:
		Render();
		~Render();
	public:
		int set_vao();
		int set_vbo(int length, void* data, unsigned int usage);
		int set_ebo(int length, void* data, unsigned int usage);
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
	private:
		unsigned int __vao;
		unsigned int __vbo;
		unsigned int __ebo;
	};

}