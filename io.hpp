/*
 * @Description: 
 * @Autor: degawong
 * @Date: 2020-03-23 16:41:29
 * @LastEditors: degawong
 * @LastEditTime: 2020-05-12 15:04:15
 * @FilePath: \harpocrates\io.hpp
 */
#pragma once

#include <map>
#include <string>

namespace harpocrates {
	class image {
	public:
		image();
		~image();
	public:
		int get_width() const;
		int get_height() const;
		unsigned char* get_data();
		unsigned char* get_data() const;
		int imread(std::string path);
		int imwrite(std::string path);
	public:
		static int imwrite(std::string path, int width, int height, int channel, unsigned char* data);
		static int imread(std::string path, int& width, int& height, int& channel, unsigned char*& data);
	private:
		int __width;
		int __height;
		int __channel;
		unsigned char* __data;
		//std::map<std::string, std::function<int (const char*, int, int, int, void*)>> __write_list;
		//std::map<std::string, std::function<unsigned char* (const char*, int*, int*, int*, int)>> __read_list;
	};
}
