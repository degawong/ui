/*
 * @Author: your name
 * @Date: 2021-01-25 09:57:21
 * @LastEditTime: 2021-02-08 14:43:09
 * @LastEditors: your name
 * @Description: In User Settings Edit
 * @FilePath: \harpocrates\module\scope_guard.hpp
 */

#pragma once

namespace harpocrates {

// make the unique name of the defered operation
#define _make_name_(index, v) _##index##_##v
// replace variable value of their marco name
#define _forward_(_1, _2) _make_name_(_1, _2)
// define the auto variable of the defer operation
#define defer(operation) auto _forward_(__COUNTER__, scope_guard) { make_guard([&](){ operation; }) };

	class noncopyable {
	protected:
		noncopyable() = default;
		~noncopyable() = default;
		noncopyable(const noncopyable& _) = delete;
		noncopyable& operator=(const noncopyable& _) = delete;
	};

	template<typename _lambda>
	class scope_guard : public noncopyable {
	public:
		explicit scope_guard(_lambda&& exit_operation) :
			__dismissed(false),
			__exit_operation(std::forward<_lambda>(exit_operation)) {
		}

		//explicit scope_guard(scope_guard&& anther) :
		//	__dismissed(anther._dismissed)
		//	__exit_operation(std::move(anther.__exit_operation)) {
		//}

		~scope_guard() {
			if (!__dismissed) __exit_operation();
		}

	public:
		void dismiss() {
			__dismissed = true;
		}
	private:
		bool __dismissed;
		_lambda __exit_operation;
	};

	template<typename _lambda>
	auto make_guard(_lambda&& lambda) {
		return scope_guard<_lambda>(std::forward<_lambda>(lambda));
	}
}