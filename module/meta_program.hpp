/*
 * @Author: your name
 * @Date: 2021-02-08 14:37:30
 * @LastEditTime: 2021-02-08 14:42:51
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \harpocrates\module\meta_program.hpp
 */


#pragma once

#include <type_traits>

namespace harpocrates {
	template<char...>
	struct _signature_type {
	};

	template<typename>
	struct _signature {
		using type = std::void_t<>;
		static const int value = -1;
	};

	template<>
	struct _signature<_signature_type<>> {
		using type = std::void_t<>;
		static const int value = 0;
	};

	template<char _>
	struct _signature<_signature_type<_>> {
		using type = _signature_type<_>;
		static const int value = int(_);
	};

	template<char _1st, char _2nd>
	struct _signature<_signature_type<_1st, _2nd>> {
		using type = _signature_type<_1st, _2nd>;
		static const int value = int(((int)_1st << 1) + (int)_2nd);
	};

	template<char _1st, char _2nd, char... _>
	struct _signature<_signature_type<_1st, _2nd, _...>> {
		using type = _signature_type<_1st, _2nd, _...>;
		static const int value = _signature<_signature_type<_1st, _2nd>>::value + _signature<_signature_type<_...>>::value;
	};

	template<char... _>
	auto signature_v = _signature<_signature_type<_...>>::value;

	template<char... _>
	using signature_t = _signature<_signature_type<_...>>::type;
	
	template<typename _type, typename = void>
	struct _parallelable : std::false_type {
	};

	template<typename _type>
	struct _parallelable<
		_type,
		std::void_t<
			decltype(std::declval<_type&>().end()),
			decltype(std::declval<_type&>().begin()),
			decltype(std::declval<_type&>().signature())
		>
	> : std::true_type {
	};
	
	template<typename _type>
	auto parallelable_v = _parallelable<_type>::value;

	template<typename _type>
	using parallelable_t = _parallelable<_type>::type;

	template<typename _type>
	constexpr auto _instantance = std::void_t<_type>{};

	template<typename _type, typename... _args>
	std::false_type _is_implemented(...);
	
	template<
		typename _type,
		typename... _args,
		typename = std::void_t<
			decltype(std::declval<_type>()(std::declval<_args&&>()...))
		>
	>
	std::true_type _is_implemented();

	constexpr auto  _is_implemention_accessable = [](auto instance) {
		return [](auto&&... args) {
			return decltype(_is_implemented<decltype(instance), decltype(args)&&...>()){};
		};
	};

	constexpr auto  _is_default_constructor_accessable = [](auto instance) -> decltype(decltype(instance)()) {
	};
}