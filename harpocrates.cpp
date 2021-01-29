/*
 * @Author: your name
 * @Date: 2020-12-08 10:10:54
 * @LastEditTime: 2021-01-12 16:37:35
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \harpocrates\harpocrates.cpp
 */

#include <regex>
#include <thread>
#include <fstream>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <path_walker.hpp>
#include <scope_guard.hpp>
#include <scoped_allocator>

#include <io.hpp>
#include <opengl/opengl.hpp>
#include <opengl/openglui.hpp>

#include <algorithm/algorithm.hpp>

using namespace glm;
using namespace std;
using namespace harpocrates;

const int width = 1280;
const int height = 720;

const std::string vertex_shader_path = { "../shader/base_vertex.glsl" };
const std::string fragment_shader_path = { "../shader/base_fragment.glsl" };

unsigned int indices[] = {
	0, 1, 2,
	0, 2, 3
};

float vertices[] = {
	1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
	-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	-1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f 
};

// https://stackoverflow.com/questions/25351176/drawing-text-with-opengl-glfw-freetype-produces-white-rectangle-instead-of-g

int main() {

	auto data = new unsigned char[4 * width * height]{ 0 };
	defer(delete[] data);
	auto alpha = new unsigned char[3 * width * height]{ 0 };
	defer(delete[] alpha);
	auto image = new unsigned char[3 * width * height]{ 0 };
	defer(delete[] image);

	std::memset(alpha, 0, 3 * width * height);
	std::memset(image, 255, 3 * width * height);

	auto camera = Camera::get_instance(width, height, 50.0f);
	camera->set_position({ -1, 1, 1, -1 });

	auto ui = UI();
	ui.create_window(width, height, "opengl ui");
	ui.make_current();

	ui.set_key_callback(
		[](GLFWwindow* window, int key, int scancode, int action, int mods) {
			auto camera = Camera::get_instance();
			camera->key_callback(window);
		}
	);
	ui.set_size_callback(
		[](GLFWwindow* window, int width, int height) {
			auto camera = Camera::get_instance();
			camera->resize_callback(width, height);
		}
	);
	ui.set_mouse_callback(
		[](GLFWwindow* window, int button, int action, int mode) {
			auto camera = Camera::get_instance();
			camera->mouse_callback(window, button, action, mode);
		}
	);
	ui.set_cursor_callback(
		[](GLFWwindow* window, double x, double y) {
			auto camera = Camera::get_instance();
			camera->cursor_callback(window, x, y);
		}
	);
	ui.set_scroll_callback(
		[](GLFWwindow* window, double x, double y) {
			auto camera = Camera::get_instance();
			camera->scroll_callback(y);
		}
	);
	ui.set_drop_callback(
		[](GLFWwindow* window, int count, const char** paths) {
			auto camera = Camera::get_instance();
			camera->drop_callback(count, paths);
		}
	);

	auto gl = OpenGL();
	Glad().apply();

	auto shader = Shader();
	shader.gen_shader();
	shader.add_shader(vertex_shader_path, fragment_shader_path);
	shader.attach(0);

	auto render = Render();
	render.gen_vao();
	render.bind_vao(0);
	render.gen_vbo();
	render.bind_vbo(0, 32 * 4, vertices, GL_STATIC_DRAW);
	render.gen_ebo();
	render.bind_ebo(0, sizeof(indices), indices, GL_STATIC_DRAW);

	render.set_attribution(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	render.set_attribution(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	render.set_attribution(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	auto texture = Texture();
	texture.gen_texture();
	texture.bind(0);
	texture.apply(0, width, height, false, alpha);

	texture.gen_texture();
	texture.bind(1);
	texture.apply(1, width, height, false, image);

	shader.use(0);

	shader.set_interger(0, "texture_1", 0);
	shader.set_interger(0, "texture_2", 1);

	render.gen_fbo();
	render.bind_fbo(0);
	render.gen_rbo();
	render.bind_rbo(0, width, height, 0);

	texture.gen_texture();
	texture.bind(2);
	texture.apply(2, width, height, true, nullptr);

	render.bind_texture_to_fbo(texture.get_id(2));
	render.unbind_fbo();

	thread(std::move(packaged_task<void()>{
		[&]() {
			auto path = std::string();
			auto image_list = PathWalker::get_instance()->walk_path(path);
			auto algorithm = Algorithm();
			for (auto& ref : image_list) {
				algorithm.apply();
			}
		}
	})).detach();

	while (!ui.close_window()) {

		render.bind_fbo(0);
		gl.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, 0.f, 1.f, 1.f, 1.f);
		glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
		render.unbind_fbo();

		gl.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		render.bind_vao(0);

		texture.active_texture(GL_TEXTURE0);
		texture.bind(0);

		texture.active_texture(GL_TEXTURE1);
		texture.bind(1);

		shader.set_float(0, "weight", float(camera->swap()));
		shader.use(0);

		// projection
		auto projection = camera->get_projection();
		shader.set_matrix4(0, "projection", 1, value_ptr(projection));
		// camera/view transformation
		auto view = camera->get_view();
		shader.set_matrix4(0, "view", 1, value_ptr(view));
		// model
		auto model = camera->get_model();
		shader.set_matrix4(0, "model", 1, value_ptr(model));

		render.rending(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		ui.loop();
	}
	return 0;
}
 