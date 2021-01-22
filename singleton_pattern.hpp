/*
 * @Description: 
 * @Autor: degawong
 * @Date: 2020-03-23 16:41:29
 * @LastEditors: degawong
 * @LastEditTime: 2020-05-12 15:03:32
 * @FilePath: \harpocrates\singleton_pattern.hpp
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
	class SingletonPattern {
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
		SingletonPattern() = default;
		virtual ~SingletonPattern() {}
	protected:
		SingletonPattern(SingletonPattern<_derived>&& anther) = delete;
		SingletonPattern(const SingletonPattern<_derived>& anther) = delete;
		SingletonPattern<_derived> &operator=(SingletonPattern<_derived>&& anther) = delete;
		SingletonPattern<_derived> &operator=(const SingletonPattern<_derived>& anther) = delete;
	private:
		static std::mutex __mutex;
		static std::shared_ptr<_derived> __instance;
	};

	template<typename _derived>
	std::mutex SingletonPattern<_derived>::__mutex;

	template<typename _derived>
	std::shared_ptr<_derived> SingletonPattern<_derived>::__instance = nullptr;

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