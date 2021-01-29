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

	void UI::window_hint(int hint, int value) {
		glfwWindowHint(hint, value);
		// glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
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

	void UI::set_drop_callback(GLFWdropfun callback) {
		glfwSetDropCallback(__window, callback);
	}

	void UI::set_opacity(float intensity) {
		glfwSetWindowOpacity(__window, intensity);
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

	void UI::set_window_position_callback(GLFWwindowposfun callback) {
		glfwSetWindowPosCallback(__window, callback);
	}

	Camera::Camera(int width, int height, float fov) {
		__swap = false;
		__fov = { fov, fov };
		__left_button_down = false;
		__drag_position = vec3{ 0.f };
		__cursor_position = vec2{ 0.f };
		__window_size = { width, height };
		__camera_up = vec3(0.0f, 1.0f, 0.0f);
		__camera_front = vec3(0.0f, 0.0f, -1.0f);
		__camera_position = vec3(0.0f, 0.0f, 5.0f);
	}

	void Camera::scroll_callback(int offset) {
		__fov[0] -= (float)offset;
		if (__fov[0] < 0.f) __fov[0] = 0.f;
		if (__fov[0] > 90.f) __fov[0] = 90.f;
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
		__window_size = { width, height };
	}

	void Camera::drop_callback(int count, const char** paths) {
		// do not use reference parameter
		// after detach operation, we can not access the data ?
		// the paths is the temporary variable, do noe use it directly
		// even u use the local variable is still not safe
		// pass the parameter as a function parameter
	}

	void Camera::cursor_callback(GLFWwindow * window, double x, double y) {
		// screen coordinates to world coordinates
		// https://www.codenong.com/24127926/
		// https://www.bfilipek.com/2012/06/select-mouse-opengl.html#code
		// https://www.bfilipek.com/2012/06/select-mouse-opengl.html#drag
		// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-6-keyboard-and-mouse/
		// https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Arcball
		// https://gamedev.stackexchange.com/questions/77381/opengl-have-object-follow-mouse
		// https://stackoverflow.com/questions/45796287/screen-coordinates-to-world-coordinates
		// https://itqna.net/questions/17485/how-move-rotate-or-scale-object-its-local-axis-opengl		
		if (__left_button_down == true) {
			//auto lt_window = vec3(0.f, 0.f, 0.f);
			//auto rb_window = vec3(__window_size.x - 1.f, __window_size.y - 1.f, (float)0);
			auto m = get_model();
			auto v = get_view();
			auto p = get_projection();
			auto viewport = vec4(0.0f, 0.0f, __window_size.x, __window_size.y);
			//auto lt_world = unProject(lt_window, v * m, p, viewport);
			//auto rb_world = unProject(rb_window, v * m, p, viewport);
			auto lt_screen = project({ __world_position.x, __world_position.y, 0 }, v * m, p, viewport);
			auto rb_screen = project({ __world_position.z, __world_position.w, 0 }, v * m, p, viewport);
			__drag_position += vec3 { vec3 { vec2 { x, y } - __cursor_position, 0 } * vec3 {
				fabs(__world_position.x - __world_position.z) / fabs(lt_screen.x - rb_screen.x),
				-fabs(__world_position.y - __world_position.w) / fabs(lt_screen.y - rb_screen.y),
				1.0f
			}};
		}
		__cursor_position = { x, y };
	}

	void Camera::mouse_callback(GLFWwindow * window, int button, int action, int mode) {
		// can also get current mouse cursor
		//double position_x, position_y;
		//glfwGetCursorPos(window, &position_x, &position_y);
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			__left_button_down = true;
		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
			__left_button_down = false;
		}
	}

	mat4x4 Camera::get_view() {
		return lookAt(__camera_position, __camera_position + __camera_front, __camera_up);
	}

	bool Camera::swap() {
		return __swap;
	}

	mat4 Camera::get_model() {
		return translate(mat4(1.0f), __drag_position);
	}

	mat4x4 Camera::get_projection() {
		return perspective(radians(__fov[0]), __window_size.x / __window_size.y, 0.1f, 100.0f);
	}

	void Camera::set_position(vec4 position) {
		__world_position = position;
	}

	void Camera::__reset_martrix() {
		__swap = false;
		__fov[0] = __fov[1];
		__left_button_down = false;
		__drag_position = vec3{ 0.f };
		__camera_up = vec3(0.0f, 1.0f, 0.0f);
		__camera_front = vec3(0.0f, 0.0f, -1.0f);
		__camera_position = vec3(0.0f, 0.0f, 5.0f);
	}

 }
