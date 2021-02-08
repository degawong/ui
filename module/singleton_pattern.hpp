/*
 * @Author: your name
 * @Date: 2021-01-22 17:37:11
 * @LastEditTime: 2021-02-08 14:43:24
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \harpocrates\module\singleton_pattern.hpp
 */

#pragma once

#include <deque>
#include <regex>
#include <mutex>
#include <queue>
#include <atomic>
#include <future>
#include <thread>
#include <vector>
#include <numeric>
#include <iostream>
#include <filesystem>
#include <functional>
#include <condition_variable>

namespace harpocrates {

	template<typename _derived>
	class singleton_pattern {
	public:
		template<typename ..._args>
		static std::shared_ptr<_derived> get_instance(_args&&... args) {
			std::lock_guard<std::mutex> lock(__mutex);
			if (nullptr == __instance) {
				auto ptr = new(std::nothrow) _derived(std::forward<_args>(args)...);
				__instance = std::shared_ptr<_derived>(ptr);
				// still i don't know why, is there any other class needed to construct the instance
				//__instance = std::make_shared<_derived>(std::forward<_args>(args)...);
			}
			return __instance;
		}
	protected:
		singleton_pattern() = default;
		virtual ~singleton_pattern() {}
	protected:
		singleton_pattern(singleton_pattern<_derived>&& anther) = delete;
		singleton_pattern(const singleton_pattern<_derived>& anther) = delete;
		singleton_pattern<_derived> &operator=(singleton_pattern<_derived>&& anther) = delete;
		singleton_pattern<_derived> &operator=(const singleton_pattern<_derived>& anther) = delete;
	private:
		static std::mutex __mutex;
		static std::shared_ptr<_derived> __instance;
	};

	template<typename _derived>
	std::mutex singleton_pattern<_derived>::__mutex;

	template<typename _derived>
	std::shared_ptr<_derived> singleton_pattern<_derived>::__instance = nullptr;

	class SingletonMY {
	public:
		static SingletonMY& get_instance() {
			static SingletonMY __instance;
			return __instance;
		}
	private:
		SingletonMY();
		SingletonMY(const SingletonMY& other);
	};

	template<typename _derived>
	class SingletonMeyers {
	public:
		~SingletonMeyers() = default;
		static _derived& getInstance() {
			static _derived __instance;
			return __instance;
		}
	private:
		SingletonMeyers(const SingletonMeyers&) = delete;
		SingletonMeyers& operator = (const SingletonMeyers&) = delete;
	private:
		SingletonMeyers() = default;
	private:
		friend typename _derived;
	};

}