/*
 * @Description: 
 * @Autor: degawong
 * @Date: 2020-04-16 17:17:45
 * @LastEditors: degawong
 * @LastEditTime: 2020-05-12 15:04:30
 * @FilePath: \harpocrates\path_walker.hpp
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

#include <base.h>
#include <singleton_pattern.hpp>

namespace harpocrates {

	using namespace operator_reload;

	decltype(auto) make_directory(std::filesystem::path path) {
		if (!std::filesystem::is_directory(path.parent_path())) {
			std::filesystem::create_directory(path.parent_path());
		}
	}

	class PathWalker : public SingletonPattern<PathWalker> {
	public:
		~PathWalker() {
			std::vector<std::string>().swap(__file_list);
		};
	public:
		template<typename _iter_type = std::filesystem::directory_iterator>
		std::vector<std::string> walk_path(
			const std::string directory,
			const std::string parameter = ".*\.(bmp|BMP|jpg|JPG|png|PNG)$") {
			if (return_code::success != __parse_path(directory)) {
				return std::vector<std::string>();
			}
			(directory != __directory) | [&, this]() {
				__directory = directory;
				__parameter = parameter;
			};
			return __get_file_list<_iter_type>();
		}
	private:
		PathWalker() = default;
	private:
		template<typename _iter_type>
		auto __get_file_list() {
			std::regex regex(__parameter);
			std::vector<std::string>().swap(__file_list);
			if (std::filesystem::is_directory(__directory)) {
				for (auto ref : _iter_type(__directory)) {
					auto file_name(ref.path().generic_string());
					if (std::regex_match(file_name, regex)) {
						__file_list.push_back(file_name);
					}
				}
			}
			return __file_list;
		}
	private:
		return_code __parse_path(std::string path) {
			auto res = return_code::success;
			!std::filesystem::exists(path) | [&]() {
				res = return_code::not_exsit;
			};
			!std::filesystem::is_directory(path) | [&]() {
				res = return_code::not_directory;
			};
			return res;
		}
	private:
		std::string __directory;
		std::string __parameter;
		std::vector<std::string> __file_list;
	private:
		friend SingletonPattern<PathWalker>;
	};
}