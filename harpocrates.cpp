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

const std::string path = { "f:/share/asff/degraded_image/" };
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

auto forward_thread() {
	auto image_list = harpocrates::PathWalker::get_instance()->walk_path(path, ".*\.(jpg|JPG)");
	auto algorithm = Algorithm();
	for (auto &ref : image_list) {
		algorithm.apply();
	}
}

int main() {

	//auto alpha = imread("f:/share/base/red.jpg", 3);
	//auto image = imread("f:/share/base/image.jpg", 3);

	auto alpha = new unsigned char[3 * 256 * 256]{ 0 };
	auto image = new unsigned char[3 * 256 * 256]{ 0 };

	std::memset(alpha, 0, 3 * 256 * 256);
	std::memset(image, 255, 3 * 256 * 256);

	auto camera = Camera::get_instance(width, height, 50.0f);

	auto key = [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		auto camera = Camera::get_instance(0, 0, 0);
		camera->key_callback(window);
	};

	auto size = [](GLFWwindow* window, int width, int height) {
		auto camera = Camera::get_instance(0, 0, 0);
		camera->resize_callback(width, height);
	};

	auto scroll = [](GLFWwindow* window, double x, double y) {
		auto camera = Camera::get_instance(0, 0, 0);
		camera->scroll_callback(y);
	};

	auto cursor = [](GLFWwindow* window, double x, double y) {
		auto camera = Camera::get_instance(0, 0, 0);
		camera->cursor_callback(window, x, y);
	};

	auto mouse = [](GLFWwindow* window, int button, int action, int mode) {
		auto camera = Camera::get_instance(0, 0, 0);
		camera->mouse_callback(window, button, action, mode);
	};

	auto ui = UI();
	ui.create_window(width, height, "opengl ui");
	ui.make_current();

	ui.set_key_callback(key);
	ui.set_size_callback(size);
	ui.set_mouse_callback(mouse);
	ui.set_cursor_callback(cursor);
	ui.set_scroll_callback(scroll);

	auto gl = OpenGL();
	Glad().apply();

	auto shader = Shader();
	shader.add_shader(vertex_shader_path, fragment_shader_path);
	shader.attach(0);

	auto render = Render();
	render.set_vao();
	render.set_vbo(32 * 4, vertices, GL_STATIC_DRAW);
	render.set_ebo(sizeof(indices), indices, GL_STATIC_DRAW);

	render.set_attribution(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	render.set_attribution(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	render.set_attribution(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	auto texture_1 = Texture();
	texture_1.bind();
	texture_1.apply(256, 256, false, alpha);
	// texture_1.apply(alpha.cols, alpha.rows, false, alpha.data);

	auto texture_2 = Texture();
	texture_2.bind();
	texture_2.apply(256, 256, false, image);
	// texture_2.apply(image.cols, image.rows, false, image.data);

	shader.use();

	shader.set_interger("texture_1", 0);
	shader.set_interger("texture_2", 1);

	auto pt = packaged_task<void()>{ forward_thread };
	auto f = pt.get_future();
	auto t = thread(std::move(pt), image);
	t.detach();

	while (!ui.close_window()) {
		gl.clear();
		render.set_vao();

		texture_1.active(GL_TEXTURE0);
		texture_1.bind();

		texture_2.active(GL_TEXTURE1);
		texture_2.bind();

		shader.set_float("weight", float(camera->swap()));
		shader.use();

		// projection
		auto projection = camera->get_projection();
		shader.set_matrix4("projection", 1, &projection[0][0]);
		// camera/view transformation
		auto view = camera->get_view();
		shader.set_matrix4("view", 1, &view[0][0]);
		// model
		auto model = camera->get_model();
		shader.set_matrix4("model", 1, &model[0][0]);
        
		render.rending(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		ui.loop();
    }

	delete[] alpha;
	delete[] image;
	return 0;
}