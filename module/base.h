/*
 * @Author: your name
 * @Date: 2021-01-22 17:37:11
 * @LastEditTime: 2021-02-08 14:43:03
 * @LastEditors: your name
 * @Description: In User Settings Edit
 * @FilePath: \harpocrates\module\base.h
 */


#pragma once

#include <string>
#include <cstdarg>

namespace harpocrates {

	enum class return_code {
		success,
		io_error,
		null_ptr,
		unsupport,
		not_exsit,
		not_match,
		not_directory,
		out_of_memory,
		opengl_shader_vertex,
		opengl_shader_fragment,
		opengl_glad_load_error,
		opengl_glfw_no_memory,
	};

	enum class algorithm_code {
		base_algorithm,
		derived_algorithm,
	};

	template <typename _type>
	_type min(_type value) {
		return value;
	};

	template <typename _type_1, typename _type_2>
	_type_1 min(_type_1 first, _type_2 second) {
		return (_type_1)(first > second ? second : first);
	};

	template <typename _type, typename... _type_rest>
	_type min(_type head, _type_rest... rest) {
		return min(head, min(rest...));
	};

	template <typename _type>
	_type max(_type value) {
		return value;
	};

	template <typename _type_1, typename _type_2>
	_type_1 max(_type_1 first, _type_2 second) {
		return (_type_1)(first < second ? second : first);
	};

	template <typename _type, typename... _type_rest>
	_type max(_type head, _type_rest... rest) {
		return max(head, max(rest...));
	};

	template <typename _type, typename... _args>
	_type sum(_args&&... args) {
		return (_type)(args + ...);
	};

	template<typename _type_1, typename _type_2>
	_type_1 clamp( _type_1 value, _type_2 low_limit, _type_2 up_limit) {
		return min(max(low_limit, value), up_limit);
	};

	template<typename _type>
	// rounds x upward, returning the smallest integral x that is not less than x.
	inline int fast_ceil(_type x) {
		static_assert(!std::numeric_limits<_type>::is_integer, "fast_ceil can only handle float point number");
		if (x < 0) return 0;
		const int i = static_cast<int>(x);
		return i + (i - x < 0);
	}

	template<typename _type>
	inline int fast_round(_type x) {
		static_assert(!std::numeric_limits<_type>::is_integer, "fast_round can only handle float point number");
		const int i = static_cast<int>(x);
		return i + (i - x < 0);
	}

	template<typename _type>
	inline int fast_floor(_type x) {
		static_assert(!std::numeric_limits<_type>::is_integer, "fast_floor can only handle float point number");
		return x > 0 ? static_cast<int>(x) : 0;
	}

	template<class _type>
	_type fast_abs(_type x) {
		return x;
	}

	template<>
	inline char fast_abs(char x) {
		return (x + (x >> 8)) ^ (x >> 8); 
	}

	template<>
	inline signed char fast_abs(signed char x) {
		return (x + (x >> 8)) ^ (x >> 8);
	}

	template<>
	inline short fast_abs(short x) {
		return (x + (x >> 16)) ^ (x >> 16);
	}
	//template<> inline int fast_abs(int x) { return (x + (x>>31)) ^ (x>>31); }
	template<>
	inline int fast_abs(int x) {
		return x >= 0 ? x : -x;
	}

	template<>
	inline long fast_abs(long x) {
		return x >= 0 ? x : -x;
	}

	//template<>
	//inline float fast_abs(float x) {
	//	return ::fabsf(x);
	//}

	//template<>
	//inline double fast_abs(double x) {
	//	return ::fabs(x);
	//}

	//auto any_equel = [](auto&& input, auto&&... args) -> bool {
	//	return ((args == input) || ...);
	//};
	//auto any_lower = [](auto&& input, auto&&... args) -> bool {
	//	return ((args > input) && ...);
	//};
	//auto any_larger = [](auto&& input, auto&&... args) -> bool {
	//	return ((args < input) && ...);
	//};
	//auto every_eque = [](auto&& input, auto&&... args) -> bool {
	//	return ((args == input) && ...);
	//};
	//auto every_lower = [](auto&& input, auto&&... args) -> bool {
	//	return ((args > input) && ...);
	//};
	//auto every_larger = [](auto&& input, auto&&... args) -> bool {
	//	return ((args < input) && ...);
	//};
	//auto any_not_equel = [](auto&& input, auto&&... args) -> bool {
	//	return ((args != input) || ...);
	//};
	//auto every_not_eque = [](auto&& input, auto&&... args) -> bool {
	//	return ((args != input) && ...);
	//};

	template<typename _type>
	struct _bgra {
		_type b;
		_type g;
		_type r;
		_type a;
	};

	template<typename _type>
	struct _xyzw {
		_type x;
		_type y;
		_type z;
		_type w;
	};

	template<typename _type>
	struct _yuva {
		_type y;
		_type u;
		_type v;
		_type a;
	};

	template<typename _type>
	struct _rect {
		_type left;
		_type top;
		_type right;
		_type bottom;
	};

	template<typename _type>
	struct _region {
		_type x;
		_type y;
		_type width;
		_type height;
	};

	template<typename _type>
	struct view {
		union {
			_bgra<_type> color;
			_xyzw<_type> axis;
			_yuva<_type> yuv;
			_rect<_type> rect;
			_region<_type> region;
		};
		view(_type _ = 0) {
			color.b = color.g = color.r = color.a = _;
		}
		view(view&& other) {
			color.b = other.color.b;
			color.g = other.color.g;
			color.r = other.color.r;
			color.a = other.color.a;
		}
		view(const view& other) {
			color.b = other.color.b;
			color.g = other.color.g;
			color.r = other.color.r;
			color.a = other.color.a;
		}
		view(_type _1, _type _2, _type _3, _type _4) {
			color.b = _1;
			color.g = _2;
			color.r = _3;
			color.a = _4;
		}
		view& operator= (view&& other) {
			color.b = other.color.b;
			color.g = other.color.g;
			color.r = other.color.r;
			color.a = other.color.a;
			return *this;
		}
		view& operator= (const view& other) {
			color.b = other.color.b;
			color.g = other.color.g;
			color.r = other.color.r;
			color.a = other.color.a;
			return *this;
		}
		_type& operator[] (int index) {
			if (0 == index) {
				return color.b;
			}
			else if (1 == index) {
				return color.g;
			}
			else if (2 == index) {
				return color.r;
			}
			else if (3 == index) {
				return color.a;
			}
		}
		const _type& operator[] (int index) const {
			if (0 == index) {
				return color.b;
			}
			else if (1 == index) {
				return color.g;
			}
			else if (2 == index) {
				return color.r;
			}
			else if (3 == index) {
				return color.a;
			}
		}
	};

	using Rect = view<int>;
	using Point = view<int>;
	using Scalar = view<int>;
	using Region = view<int>;

	// downcase word class here for specific useage
	class uncopyable {
	protected:
		uncopyable() = default;
		~uncopyable() = default;
	private:
		uncopyable(const uncopyable&) = delete;
		const uncopyable& operator=(const uncopyable&) = delete;
	};

	template<typename _type>
	// downcase word class only for private use
	struct base_type {
		using value_type = _type;
		using size_type = size_t;
		using pointer = value_type *;
		using difference_type = ptrdiff_t;
		using const_pointer = const value_type *;
		using reference = value_type &;
		using const_reference = const value_type &;
	};

	template<>
	// downcase word class only for private use
	struct base_type<void> {
		using value_type = void;
		using size_type = size_t;
		using pointer = value_type *;
		using difference_type = ptrdiff_t;
		using const_pointer = const value_type *;
	};

	namespace type {
		using handle = void *;

		using uchar = unsigned char;
		using ushort = unsigned short;

		using int8_t = signed char;
		using int16_t = short;
		using int32_t = int;
		using int64_t = long long;
		using uint8_t = unsigned char;
		using uint16_t = unsigned short;
		using uint32_t = unsigned int;
		using uint64_t = unsigned long long;

		using int_least8_t = signed char;
		using int_least16_t = short;
		using int_least32_t = int;
		using int_least64_t = long long;
		using uint_least8_t = unsigned char;
		using uint_least16_t = unsigned short;
		using uint_least32_t = unsigned int;
		using uint_least64_t = unsigned long long;

		using int_fast8_t = signed char;
		using int_fast16_t = int;
		using int_fast32_t = int;
		using int_fast64_t = long long;
		using uint_fast8_t = unsigned char;
		using uint_fast16_t = unsigned int;
		using uint_fast32_t = unsigned int;
		using uint_fast64_t = unsigned long long;

		using intmax_t = long long;
		using uintmax_t = unsigned long long;
	}

	namespace engine {
		struct BaseEngine {
			virtual void __regist_engine() = 0;
			virtual void __regist_sse_engine() = 0;
			virtual void __regist_base_engine() = 0;
			virtual void __regist_neon_engine() = 0;
			virtual void __regist_opencl_engine() = 0;
		};
	}

	namespace format {
		template<typename>
		std::string formation(const char* format, ...) {
			const int const_size = 512;
			char description[const_size]{ "\0" };
			if (format) {
				va_list vl;
				va_start(vl, format);
				vsnprintf(description, sizeof(description), format, vl);
				va_end(vl);
				description[sizeof(description) - 1] = '\0';
			}
			return std::string(description);
		}
	}

	namespace operator_reload {
		//auto operator<<(std::ostream &, const std::string &) {
		//	return 0;
		//}

		template<typename _function>
		auto operator| (bool condition, _function&& function) {
			if (true == condition) {
				return std::forward<_function>(function)();
			}
		};
		
		template<typename _args, typename _function>
		auto operator| (_args&& args, _function&& function) {
			return std::forward<_function>(function)(std::forward<_args>(args));
		};

		//template<typename... _args, typename _function>
		//auto operator| (_args&&... args, const _function& function) {
		//	return function(std::forward<_args>(args)...);
		//};

		//template<typename _function, typename... _args>
		//auto operator| (_function& function, _args&&... args) {
		//	//return std::forward<_function>(function)(std::forward<_args>(args)...);
		//};

		//template<typename _function, typename... _args>
		//auto operator| (bool condition, _function& function, _args&&... args) {
		//	if (true == condition) {
		//		return std::forward<_function>(function)(std::forward<_args>(args)...);
		//	}
		//};

	};
};