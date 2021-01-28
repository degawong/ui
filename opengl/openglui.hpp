/*
 * @Author: your name
 * @Date: 2021-01-18 15:34:30
 * @LastEditTime: 2021-01-18 15:49:21
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \harpocrates\opengl\openglui.hpp
 */

#pragma once

#include <string>
#include <fstream>

#include <GLFW/glfw3.h>

#include <singleton_pattern.hpp>

namespace harpocrates {
	using namespace glm;

	struct Glad {
		int apply();
	};

	class UI {
	public:
		UI(int major = 3, int minor = 3);
		~UI();
	public:
		void loop();
		void query();
		void enable_depth();
		void make_current();
		bool close_window();
		void window_hint(int hint, int value);
		int create_window(int width, int height, std::string name);
	public:
		void set_opacity(float intensity);
	public:
		void set_key_callback(GLFWkeyfun callback);
		void set_drop_callback(GLFWdropfun callback);
		void set_scroll_callback(GLFWscrollfun callback);
		void set_cursor_callback(GLFWcursorposfun callback);
		void set_mouse_callback(GLFWmousebuttonfun callback);
		void set_cursor_callback(GLFWcursorenterfun callback);
		void set_size_callback(GLFWframebuffersizefun callback);
		void set_window_position_callback(GLFWwindowposfun callback);
	private:
		GLFWwindow* __window;
	};

	//template<typename int = 0>
	// if i use the template signature, then the function implementation should be in the same file
	class Camera : public SingletonPattern<Camera> {
		// if the the window needs multi cameras, we can
		// use the interger template
		Camera(int width = 0, int height = 0, float fov = 50.0f);
	public:
		~Camera() = default;
	public:
		// callbacks
		void scroll_callback(int offset);
		void key_callback(GLFWwindow* window);
		void resize_callback(int width, int height);
		void drop_callback(int count, const char** paths);
		void cursor_callback(GLFWwindow* window, double x, double y);
		void mouse_callback(GLFWwindow* window, int button, int action, int mode);
		// TODO : add mouse drag function
		// matrixs
		bool swap();
		mat4 get_model();
		mat4x4 get_view();
		mat4x4 get_projection();
	private:
		void __reset_martrix();
	private:
		bool __swap;
		float __fov;
		float __fov_back;
		vec3 __camera_up;
		vec3 __camera_front;
		vec3 __camera_position;
		int __original_width;
		int __original_height;
		bool __left_button_down;
		float __current_position_x;
		float __current_position_y;
		float __privious_position_x;
		float __privious_position_y;
	private:
		friend SingletonPattern<Camera>;
	};

	// get windows position ==> glfwGetWindowUserPointer
	struct CallBacks {
		// https://www.glfw.org/docs/3.3/input_guide.html
		// https://www.glfw.org/docs/latest/group__input.html
		// drop file through path
		
		virtual void windows_position_callback(GLFWwindow)
		virtual void drop_callback(GLFWwindow* window, int count, const char** paths);
		virtual void charmods_callback(GLFWwindow* window, unsigned int codepoint, int mods);
		virtual void character_callback(GLFWwindow* window, unsigned int codepoint);
		virtual void window_refresh_callback(GLFWwindow* window);
		virtual void window_focus_callback(GLFWwindow* window, int focused);
		virtual void window_iconify_callback(GLFWwindow* window, int iconified);
		virtual void window_size_callback(GLFWwindow* window, int width, int height);
		virtual void window_pos_callback(GLFWwindow* window, int xpos, int ypos);
		virtual void error_callback(int error, const char* description);
		virtual void monitor_callback(GLFWmonitor* monitor, int event);
		virtual void window_close_callback(GLFWwindow* window);
		virtual void scroll_callback(GLFWwindow* window, double x, double y);
		virtual void framebuffer_size_callback(GLFWwindow* window, int w, int h);
		virtual void cursor_enter_callback(GLFWwindow* window, int entered);
		virtual void cursor_position_callback(GLFWwindow* window, double x, double y);
		virtual void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
		virtual void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	};
}