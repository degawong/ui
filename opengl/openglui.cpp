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
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <ft2build.h>
#include "freetype/freetype.h"

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
		__world_position = vec4{ 0.f };
		__cursor_position = vec2{ 0.f };
		__window_size = { width, height };
		__camera_up = vec3(0.0f, 1.0f, 0.0f);
		__camera_front = vec3(0.0f, 0.0f, -1.0f);
		__camera_position = vec3(0.0f, 0.0f, 2.5f);
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
			__camera_position -= vec3 { vec3 { vec2 { x, y } - __cursor_position, 0 } * vec3 {
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
		return mat4(1.0f);
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
		__camera_up = vec3(0.0f, 1.0f, 0.0f);
		__camera_front = vec3(0.0f, 0.0f, -1.0f);
		__camera_position = vec3(0.0f, 0.0f, 2.5f);
	}

	//Font::Font(std::string path) {
	//	__font_path = path;
	//	FT_Init_FreeType(&__freetype);
	//}

	//Font::~Font() {
	//	FT_Done_Face(__font_face);
	//	FT_Done_FreeType(__freetype);
	//}

	//int Font::create_face() {
	//	auto res = 0;
	//	do {
	//		res = FT_New_Face(__freetype, __font_path.c_str(), 0, &__font_face);
	//		if (res != 0) break;
	//	} while (false);
	//	return res;
	//}

	//int Font::create_font_set() {
	//	auto res = 0;
	//	do{
	//		for (GLubyte i = 0; i < 128; ++i){
	//			res = FT_Load_Char(__font_face, i, FT_LOAD_RENDER);
	//			if (res != 0) break;
	//			__texture.gen_texture();
	//			__texture.bind(i);
	//			__texture.apply(
	//				i,
	//				__font_face->glyph->bitmap.width,
	//				__font_face->glyph->bitmap.rows,
	//				GL_RED,
	//				GL_RED,
	//				__font_face->glyph->bitmap.buffer
	//			);
	//			__characters.push_back(Character {
	//				glm::ivec2(__font_face->glyph->bitmap.width, __font_face->glyph->bitmap.rows),
	//				glm::ivec2(__font_face->glyph->bitmap_left, __font_face->glyph->bitmap_top),
	//				__font_face->glyph->advance.x
	//			});
	//		}
	//	} while (false);
	//	return res;
	//}

	//int Font::set_font_size(int width, int height) {
	//	auto res = 0;
	//	do {
	//		res = FT_Set_Pixel_Sizes(__font_face, width, height);
	//		if (res != 0) break;
	//	} while (false);
	//	return res;
	//}

	// arcball
	const static vec1 fov = vec1(45.f);
	const static vec3 up = vec3(0.0f, 1.0f, 0.0f);
	const static vec3 front = vec3(0.0f, 0.0f, -1.0f);
	const static vec3 position = vec3(0.0f, 0.0f, 3.f);

	Arcball::Arcball(vec2 size) {
		__reset();
		__window_size = size;
	}

	void Arcball::scroll_callback(int offset) {
		__camera_fov -= (float)offset;
		if (__camera_fov[0] < 0.f) __camera_fov[0] = 0.f;
		if (__camera_fov[0] > 90.f) __camera_fov[0] = 90.f;
		// if u use quaternion, multiply scale as a factor

	}

	void Arcball::key_callback(GLFWwindow* window) {
		// action == press
		const float speed = 0.05f;
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			__swap = !__swap;
		}
		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
			__reset();
		}
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GL_TRUE);
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

	void Arcball::mouse_callback(GLFWwindow* window) {
		// can also get current mouse cursor
		//double position_x, position_y;
		//glfwGetCursorPos(window, &position_x, &position_y);
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			__left_button_down = true;
		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
			__left_button_down = false;
		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
			__right_button_down = true;
		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
			__right_button_down = false;
		}
	}

	void Arcball::resize_callback(int width, int height) {
		glViewport(0, 0, width, height);
		__window_size = { width, height };
	}

	void Arcball::drop_callback(int count, const char** paths) {
		// do not use reference parameter
		// after detach operation, we can not access the data ?
		// the paths is the temporary variable, do noe use it directly
		// even u use the local variable is still not safe
		// pass the parameter as a function parameter
	}

	void Arcball::cursor_callback(GLFWwindow* window, double x, double y) {
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
			// on move
			////auto lt_window = vec3(0.f, 0.f, 0.f);
			////auto rb_window = vec3(__window_size.x - 1.f, __window_size.y - 1.f, (float)0);
			//auto m = __get_model();
			//auto v = __get_view();
			//auto p = __get_projection();
			//auto viewport = vec4(0.0f, 0.0f, __window_size.x, __window_size.y);
			////auto lt_world = unProject(lt_window, v * m, p, viewport);
			////auto rb_world = unProject(rb_window, v * m, p, viewport);
			//auto lt_screen = project({ __world_position.x, __world_position.y, 0 }, v * m, p, viewport);
			//auto rb_screen = project({ __world_position.z, __world_position.w, 0 }, v * m, p, viewport);
			//__camera_position -= vec3 { vec3 { vec2 { x, y } - __cursor_position, 0 } * vec3 {
			//	fabs(__world_position.x - __world_position.z) / fabs(lt_screen.x - rb_screen.x),
			//	-fabs(__world_position.y - __world_position.w) / fabs(lt_screen.y - rb_screen.y),
			//	1.0f
			//}};
		}
		if (__right_button_down == true) {
			// on rotate
			__on_rotate(__cursor_position, vec2{ x, y });
		}
		__cursor_position = { x, y };
	}

	bool Arcball::swap() {
		return __swap;
	}

	mat4x4 Arcball::get_mvp() {
		return __get_projection() * __get_view() * __get_model();
	}

	void Arcball::__reset() {
		__swap = false;
		__camera_up = up;
		__camera_fov = fov;
		__camera_front = front;
		__left_button_down = false;
		__right_button_down = false;
		__camera_position = position;
		__cursor_position = vec2{ 0.f,0.f };
	}

	mat4 Arcball::__get_model() {
		return mat4(1.0f);
	}

	mat4x4 Arcball::__get_view() {
		return lookAt(__camera_position, __camera_position + __camera_front, __camera_up);
	}

	mat4x4 Arcball::__get_projection() {
		return perspective(radians(__camera_fov[0]), __window_size.x / __window_size.y, 0.1f, 100.0f);
	}

	vec2 Arcball::__to_ndc(vec2 point) {
		// use all window as reference
		return vec2 {
			(2.0 * point.x / (__window_size.x - 1.0) - 1.0),
			(-2.0 * point.y / (__window_size.y - 1.0) - 1.0)
		};
	}

	vec3 Arcball::__to_sphere(vec2 point) {
		auto sphere = vec3{ 0 };
		auto ndc = __to_ndc(point);
		auto square = dot(ndc, ndc);
		//auto square = ndc.x * ndc.x + ndc.y * ndc.y;
		if (square < 1.f) {
			sphere = {
				vec3{ point, sqrtf(1.0f - square) }
			};
		}
		else {
			sphere = {
				vec3{ normalize(point), 0.f }
				//vec3{ point * (1.0f / sqrtf(square)), 0.f }
			};
		}
		return sphere;
	}

	void Arcball::__on_drag(vec2 start, vec2 end) {
	}

	void Arcball::__on_rotate(vec2 start, vec2 end) {
		// use matrix
		__use_matrix(start, end);

		// use quaternion
		__use_quaternion(start, end);
	}

	void Arcball::__use_matrix(vec2 start, vec2 end) {
		auto sphere_end = __to_sphere(end);
		auto sphere_start = __to_sphere(start);
		auto to_view = inverse(__get_view() * __get_model());
		sphere_end = vec3{ to_view * vec4{sphere_end, 0.f} };
		sphere_start = vec3{ to_view * vec4{sphere_start, 0.f} };
		auto axis = normalize(cross(sphere_start, sphere_end));
		auto angle = std::acos(dot(sphere_start, sphere_end));
		__camera_up = __rodrigues_rotation(__camera_up, axis, angle);
		__camera_front = __rodrigues_rotation(__camera_front, axis, angle);
		__camera_position = __rodrigues_rotation(__camera_position, axis, angle);
	}

	void Arcball::__use_quaternion(vec2 start, vec2 end) {
		// https://zhuanlan.zhihu.com/p/79894982
		// https://openhome.cc/Gossip/WebGL/Quaternion.html
		// https://openhome.cc/Gossip/ComputerGraphics/QuaternionsRotate.htm
		auto sphere_end = __to_sphere(end);
		auto sphere_start = __to_sphere(start);
		auto perpendicular = cross(sphere_start, sphere_end);
		auto square = dot(perpendicular, perpendicular);
		if (square > std::numeric_limits<float>().epsilon()) {
			auto quater_value = vec4{ dot(sphere_start, sphere_end), perpendicular };
			auto quaternion = quat{ normalize(quater_value) };
			auto matrix = quaternion.operator glm::mat<4, 4, float, glm::packed_highp>();
			// how can i apply quaternion into my __camera_(up|front|position)?
			//quat orientation = conjugate(toQuat(lookAt(eye, o, up)));
		}
	}

	vec3 Arcball::__rodrigues_rotation(vec3 vec, vec3 axis, float angle) {
		// http://www.euclideanspace.com/maths/geometry/rotations/
		// https://en.wikipedia.org/wiki/Axis%E2%80%93angle_representation
		// https://stackoverflow.com/questions/4039664/trackball-rotation-in-opengl
		// https://stackoverflow.com/questions/1875780/how-to-rotate-about-the-center-of-screen-using-quaternions-in-opengl
		return vec * std::cos(angle) + axis * dot(axis, vec) * (1.0f - std::cos(angle)) + cross(axis, vec) * std::sin(angle);
	}

}
