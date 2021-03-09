/*
 * @Author: your name
 * @Date: 2021-02-08 14:37:30
 * @LastEditTime: 2021-02-08 14:42:51
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \harpocrates\module\meta_program.hpp
 */


#pragma once

#include <tuple>
#include <utility>
#include <iterator>
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

	//template<char... _>
	//using signature_t = _signature<_signature_type<_...>>::type;
	
	template<typename _type, typename = void>
	struct _is_parallelable : std::false_type {
	};

	template<typename _type>
	struct _is_parallelable <
		_type,
		std::void_t<
			decltype(std::declval<_type&>().end()),
			decltype(std::declval<_type&>().begin()),
			decltype(std::declval<_type&>().signature())
		>
	> : std::true_type {
	};
	
	template<typename _type>
	auto is_parallelable_v = _is_parallelable<_type>::value;

	//template<typename _type>
	//using is_parallelable_t = _is_parallelable<_type>::type;

	// https://github.com/cshelton/zipfor
	// https://scicomp.stackexchange.com/questions/25738/two-level-iterators-c
	// https://stackoverflow.com/questions/8511035/sequence-zip-function-for-c11
	// https://www.ibm.com/support/knowledgecenter/SSLTBW_2.3.0/com.ibm.zos.v2r3.cbclx01/template_template_arguments.htm
	template <typename _iter>
	decltype(auto) _advance_iterator(_iter&& iter) {
		return iter++;
	}

	template <typename _iter_head, typename... _iter_tails>
	decltype(auto) _advance_iterator(_iter_head&& iter_head, _iter_tails&&... iter_tails) {
		return { iter_head++, advance_iterator(iter_tails...) };
	}

	//template <
	//	template<typename _iter_data_type_head>
	//	class _iter_head,
	//	template<typename... _iter_data_type_tails>
	//	class... _iter_tails
	//>

	template <typename _iter_head, typename... _iter_tails>
	class zzip {
	public:
		 zzip() = delete;
		~zzip() = default;
	public:
		zzip(_iter_head&& head, _iter_tails&&... tails) {
			__element = std::make_tuple(head, tails...);
			__end = std::make_tuple(head.end(), tails.end()...);
			__iterator = __begin = std::make_tuple(head.begin(), tails.begin()...);
		}
		zzip(const _iter_head& head, const _iter_tails&... tails) {
			__element = std::make_tuple(head, tails...);
			__end = std::make_tuple(head.end(), tails.end()...);
			__iterator = __begin = std::make_tuple(head.begin(), tails.begin()...);
		}
	public:
		decltype(auto) begin() {
			return __begin;
		}
		decltype(auto) end() {
			return __end;
		}
	private:
		std::tuple<_iter_head, _iter_tails...> __element;
		std::tuple<
			decltype(std::declval<_iter_head&>().begin()),
			decltype(std::declval<_iter_tails&>().begin())...
		> __iterator;
		std::tuple<
			decltype(std::declval<_iter_head&>().begin()),
			decltype(std::declval<_iter_tails&>().begin())...
		> __begin;
		std::tuple<
			decltype(std::declval<_iter_head&>().begin()),
			decltype(std::declval<_iter_tails&>().begin())...
		> __end;
	};

	//template <typename _iter_head, typename... _iter_tails>
	//decltype(auto) zip(_iter_head&& iter_head, _iter_tails&&... iter_tails) {
	//	return std::tuple<_iter_head, _iter_tails...>(iter_head, iter_tails...);
	//}

	//template <typename Function, typename _iter, typename ... _iters>
	//Function zip(Function func, _iter begin,
	//	_iter end,
	//	_iters ... iterators)
	//{
	//	for (; begin != end; ++begin, advance_all(iterators...))
	//		func(*begin, *(iterators)...);
	//	//could also make this a tuple
	//	return func;
	//}
	// https://github.com/CommitThis/zip-iterator
	// https://committhis.github.io/2020/10/14/zip-iterator.html
	// https://codereview.stackexchange.com/questions/231352/c17-zip-iterator-compatible-with-stdsort
	template <typename... _args, std::size_t... _index>
	auto any_equal_impl(
		std::tuple<_args...> const& left,
		std::tuple<_args...> const& right,
		std::index_sequence<_index...>) -> bool {
		return (... | (std::get<_index>(left) == std::get<_index>(right)));
	}
	template <typename ... _args>
	auto any_equal(std::tuple<_args...> const& left, std::tuple<_args...> const& right) -> bool {
		return any_equal_impl(left, right, std::index_sequence_for<_args...>{});
	}
	template <typename _iter>
	using access_t = std::conditional_t<
		std::is_same_v<_iter, std::vector<bool>::iterator> ||
		std::is_same_v<_iter, std::vector<bool>::const_iterator>,
		typename _iter::value_type,
		typename _iter::reference
	>;
	template <typename... _iters>
	class zip_iterator {
		using value_type = std::tuple<access_t<_iters>...>;
	public:
		zip_iterator() = delete;
		zip_iterator(_iters&&... iters)
			: __iters{ std::forward<_iters>(iters)... } {
		}
		auto operator++() -> zip_iterator& {
			std::apply(
				[](auto&&... args) { ((args++), ...); },
				__iters
			);
			return *this;
		}
		auto operator++(int) -> zip_iterator {
			auto ret = *this;
			++(*this);
			return ret;
		}
		auto operator!=(zip_iterator const& anther) {
			return !(*this == anther);
		}
		auto operator==(zip_iterator const& anther) {
			return any_equal(__iters, anther.__iters);
		}
		auto operator*() -> value_type {
			return std::apply(
				[](auto && ... args) {
					return value_type(*args...);
				},
				__iters
			);
		}
	private:
		std::tuple<_iters...> __iters;
	};
	// std::decay needed because T is a reference, and is not a complete type */
	template <typename _type>
	using iterator_t = std::conditional_t<
		std::is_const_v<std::remove_reference_t<_type>>,
		typename std::decay_t<_type>::const_iterator,
		typename std::decay_t<_type>::iterator
	>;
	template <typename... _type>
	class zipper {
		using iter_type = zip_iterator<iterator_t<_type>...>;
	public:
		template <typename... _args>
		zipper(_args&&... args) : __args { std::forward<_args&&>(args)... } {
		}
		auto begin() -> iter_type {
			return std::apply(
				[](auto&& ... args) {
					return iter_type(std::begin(args)...);
				},
				__args
			);
		}
		auto end() -> iter_type {
			return std::apply(
				[](auto&& ... args) {
					return iter_type(std::end(args)...);
				},
				__args
			);
		}
	private:
		std::tuple<_type...> __args;
	};

	template <typename... _args>
	auto zip(_args&& ... args) {
		return zipper<_args...> { std::forward<_args&&>(args)... };
	}
	//template<typename _type>
	//constexpr auto _instantance = std::void_t<_type>{};

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