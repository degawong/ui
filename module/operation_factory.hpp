#pragma once

#include <map>
#include <string>
#include <iostream>
#include <iterator>

#include <base_algorithm/base_algorithm.h>

namespace harpocrates {
	using namespace std;
	class operation_factory {
	public:
		template<typename _type>
		void register_factory(std::string key, _type*&& value) {
			auto deleter = [&](auto pointer) { delete reinterpret_cast<_type*>(value); };
			__operation[key] = std::shared_ptr<_type>(value, deleter);
		}
	public:
		decltype(auto) operator[](std::string key) {
			return __operation[key];
		}
	private:
		std::map<std::string, std::shared_ptr<base_algorithm>> __operation;
	};
}