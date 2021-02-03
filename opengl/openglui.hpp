/*
 * @Author: your name
 * @Date: 2021-01-18 15:34:30
 * @LastEditTime: 2021-01-18 15:49:21
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \harpocrates\opengl\openglui.hpp
 */

#pragma once

#include <map>
#include <vector>
#include <string>
#include <fstream>

#include <GLFW/glfw3.h>

//#include <ft2build.h>
//#include "freetype/freetype.h"

#include <opengl/opengl.hpp>
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

	//class Font {
	//public:
	//	Font(std::string path);
	//	~Font();
	//public:
	//	int create_face();
	//	int create_font_set();
	//	int set_font_size(int width, int height);
	//private:
	//	struct Character {
	//		ivec2 __size;
	//		ivec2 __bearing;
	//		signed long __advance;
	//	};
	//private:
	//	Shader __shader;
	//	Texture __texture;
	//	FT_Face __font_face;
	//	FT_Library __freetype;
	//	std::string __font_path;
	//	std::vector<Character> __characters;
	//};

	class Arcball {
	// default is arcball unit
	// https://asliceofrendering.com/camera/2019/11/30/ArcballCamera/
	public:
		Arcball(vec2 size);
		virtual ~Arcball() = default;
	public:
		virtual void scroll_callback(int offset);
		virtual void key_callback(GLFWwindow* window);
		virtual void mouse_callback(GLFWwindow* window);
		virtual void resize_callback(int width, int height);
		virtual void drop_callback(int count, const char** paths);
		virtual void cursor_callback(GLFWwindow* window, double x, double y);
	public:
		bool swap();
		virtual mat4x4 get_mvp();
	private:
		//void set_range(vec4 range);
		virtual void __reset();
		virtual mat4 __get_model();
		virtual mat4x4 __get_view();
		virtual mat4x4 __get_projection();
		virtual vec2 __to_ndc(vec2 point);
		virtual vec3 __to_sphere(vec2 point);
		virtual void __on_drag(vec2 start, vec2 end);
		virtual void __on_rotate(vec2 start, vec2 end);
	private:
		bool __swap;
		vec2 __window_size;
		vec3 __camera_up;
		vec1 __camera_fov;
		vec3 __camera_front;
		vec3 __camera_position;
		vec2 __cursor_position;
		bool __left_button_down;
		bool __right_button_down;
	};

	//template<typename int = 0>
	// if i use the template signature, then the function implementation should be in the same file
	class Camera : public SingletonPattern<Camera> {
		// if the the window needs multi cameras, we can
		// use the interger template
		Camera(int width = 720, int height = 1080, float fov = 45.0f);
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
	public:
		bool swap();
		mat4 get_model();
		mat4x4 get_view();
		mat4x4 get_projection();
		void set_position(vec4 position);
	private:
		void __reset_martrix();
	private:
		bool __swap;
		vec2 __fov;
		vec3 __camera_up;
		vec2 __window_size;
		vec3 __camera_front;
		vec4 __world_position;
		vec3 __camera_position;
		vec2 __cursor_position;
		bool __left_button_down;
	private:
		friend SingletonPattern<Camera>;
	};

	struct CallBacks {
		// https://www.glfw.org/docs/3.3/input_guide.html
		// https://www.glfw.org/docs/latest/group__input.html
		// drop file through path
		// get windows position ==> glfwGetWindowUserPointer
		
		virtual void windows_position_callback(GLFWwindow);
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