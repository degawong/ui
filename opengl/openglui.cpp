/*
 * @Author: your name
 * @Date: 2021-01-18 15:34:30
 * @LastEditTime: 2021-01-18 15:49:45
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \harpocrates\opengl\openglui.cpp
 */

#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <opengl/openglui.hpp>

#include <singleton_pattern.hpp>

namespace harpocrates {

	using namespace std;

	int Glad::apply() {
		auto res = 0;
		do {
			auto r = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
			if (r == 0) { res = 1; break; }
		} while (false);
		return res;
	}

	UI::UI(int major, int minor) {
		glfwInit();
		__window = nullptr;
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	}
	UI::~UI() {
		glfwDestroyWindow(__window);
		glfwTerminate();
	}

	void UI::loop() {
		glfwSwapBuffers(__window);
		glfwPollEvents();
	}

	void UI::query() {
		//返回负责当前OpenGL实现厂商的名字
		cout << "verdor version : " << glGetString(GL_VENDOR) << endl;
		//返回一个渲染器标识符，通常是个硬件平台
		cout << "render version : " << glGetString(GL_RENDERER) << endl;
		//返回当前OpenGL实现的版本号
		cout << "opengl version : " << glGetString(GL_VERSION) << endl;
		//返回着色预压编译器版本号
		cout << "glsl version : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
	}

	void UI::enable_depth() {
		glEnable(GL_DEPTH_TEST);
	}

	void UI::make_current() {
		glfwMakeContextCurrent(__window);
	}

	bool UI::close_window() {
		return glfwWindowShouldClose(__window);
	}

	int UI::create_window(int width, int height, std::string name) {
		auto res = 0;
		do {
			__window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
			if (__window == nullptr) { res = 1; break; }
		} while (false);
		return res;
	}

	void UI::set_key_callback(GLFWkeyfun callback) {
		glfwSetKeyCallback(__window, callback);
	}

	void UI::set_scroll_callback(GLFWscrollfun callback) {
		// y > 0 ==> up
		// y < 0 ==> down
		glfwSetScrollCallback(__window, callback);
	}

	void UI::set_mouse_callback(GLFWmousebuttonfun callback) {
		glfwSetMouseButtonCallback(__window, callback);
	}

	void UI::set_cursor_callback(GLFWcursorposfun callback) {
		glfwSetCursorPosCallback(__window, callback);
	}

	void UI::set_cursor_callback(GLFWcursorenterfun callback) {
		glfwSetCursorEnterCallback(__window, callback);
	}

	void UI::set_size_callback(GLFWframebuffersizefun callback) {
		glfwSetFramebufferSizeCallback(__window, callback);
	}

	Camera::Camera(int width, int height, float fov) {
		__fov = fov;
		__swap = false;
		__fov_back = fov;
		__original_width = width;
		__original_height = height;
		__left_button_down = false;
		__current_position_x = 0;
		__current_position_y = 0;
		__privious_position_x = 0;
		__privious_position_y = 0;
		__camera_up = vec3(0.0f, 1.0f, 0.0f);
		__camera_front = vec3(0.0f, 0.0f, -1.0f);
		__camera_position = vec3(0.0f, 0.0f, 5.0f);
	}

	void Camera::scroll_callback(int offset) {
		__fov -= (float)offset;
		if (__fov < 0) __fov = 0;
		if (__fov > 90) __fov = 90;
	}

	void Camera::key_callback(GLFWwindow * window) {
		// action == press
		const float speed = 0.05f;
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			__swap = !__swap;
		}
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
			__reset_martrix();
		}
		if ((glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) || ((glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS))) {
			__camera_position -= speed * __camera_up;
		}
		if ((glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) || ((glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS))) {
			__camera_position += speed * __camera_up;
		}
		if ((glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) || ((glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS))) {
			__camera_position += normalize(cross(__camera_front, __camera_up)) * speed;
		}
		if ((glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) || ((glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS))) {
			__camera_position -= normalize(cross(__camera_front, __camera_up)) * speed;
		}
	}

	void Camera::resize_callback(int width, int height) {
		glViewport(0, 0, width, height);
	}

	void Camera::cursor_callback(GLFWwindow * window, double x, double y) {
		if (__left_button_down) {
			__current_position_x = x;
			__current_position_y = y;
		}
		else {
			__current_position_x = x;
			__current_position_y = y;
			__privious_position_x = x;
			__privious_position_y = y;			
		}
	}

	void Camera::mouse_callback(GLFWwindow * window, int button, int action, int mode) {
		const float speed = 0.02f;
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			__left_button_down = true;
		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
			__left_button_down = false;
			auto width = 0;
			auto height = 0;
			glfwGetWindowSize(window, &width, &height);
			__camera_position -= speed * ((float)__original_height / (float)height) * (__privious_position_y - __current_position_y) * __camera_up;
			__camera_position += normalize(cross(__camera_front, __camera_up)) * speed * ((float)__original_width / (float)width) * (__privious_position_x - __current_position_x);
		}
	}

	mat4x4 Camera::get_view() {
		return lookAt(__camera_position, __camera_position + __camera_front, __camera_up);
	}

	bool Camera::swap() {
		return __swap;
	}

	mat4 Camera::get_model() {
		return mat4(1.0f);
	}

	mat4x4 Camera::get_projection() {
		return perspective(radians(__fov), (float)__original_width / (float)__original_height, 0.1f, 100.0f);
	}

	void Camera::__reset_martrix() {
		__swap = false;
		__fov = __fov_back;
		__left_button_down = false;
		__camera_up = vec3(0.0f, 1.0f, 0.0f);
		__camera_front = vec3(0.0f, 0.0f, -1.0f);
		__camera_position = vec3(0.0f, 0.0f, 5.0f);
	}

 }
