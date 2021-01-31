/*
 * @Description: 
 * @Autor: degawong
 * @Date: 2020-03-23 16:41:29
 * @LastEditors: degawong
 * @LastEditTime: 2020-05-12 15:04:15
 * @FilePath: \harpocrates\io.cpp
 */
#pragma once

#include <regex>
#include <string>
#include <fstream>
#include <sstream>

#include <stb_image_aug.h>

#include <io.hpp>

namespace harpocrates {
	using namespace std;
	const int header_size = 54;
	const char image_info[] = {
		0x42, 0x4D, 0x36, 0x00, 0x24, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00,
		0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x03,
		0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC4, 0x0E,
		0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	// https://blog.csdn.net/yyfzy/article/details/785945
	// https://stackoverflow.com/questions/9296059/read-pixel-value-in-bmp-file
	void imread(std::string path, int& width, int& height, int& channel, unsigned char*& data) {
		auto in = ifstream();
		auto buffer = (char *)nullptr;
		do {
			in.open(path, ios::in);
			if (!in) break;
			in.seekg(0, ios::end);
			auto length = in.tellg();
			in.seekg(0, ios::beg);
			buffer = new char[length] {0};
			in.read(buffer, length);
			in.close();
			width = *(int*)&(buffer[0x12]);
			height = *(int*)&(buffer[0x16]);
			channel = (*(int*)&(buffer[0x22])) / (width * height);
			auto size = channel * width * height;
			data = new unsigned char[size] {0};
			if (data == nullptr) break;
			for (int i = 0; i < height; ++i) {
				for (int j = 0; j < width; ++j) {
					for (int k = 0; k < channel; ++k) {
						data[(height - 1 - i) * channel * width + (j) * channel + k] = buffer[header_size + i * channel * width + j * channel + k];
					}
				}
			}
		} while (0);
		delete[] buffer;
	}
	// recommand to this function
	void imwrite(int width, int height, int channel, int stride, unsigned char* data, std::string path) {
		auto out = ofstream();
		auto buffer = (char*)nullptr;
		const auto data_size = channel * width * height;
		auto length = header_size + data_size;
		do {
			out.open(path, ios::out);
			buffer = new char[length] {0};
			if (buffer == nullptr) { break; }
			for (int i = 0; i < header_size; ++i) buffer[i] = (char)image_info[i];
			*(int*)&(buffer[0x12]) = width;
			*(int*)&(buffer[0x16]) = height;
			*(int*)&(buffer[0x22]) = channel * width * height;
			// std::copy((char*)data, ((char*)(data) + data_size), buffer + header_size);
			for (int i = 0; i < height; ++i) {
				for (int j = 0; j < width; ++j) {
					for (int k = 0; k < channel; ++k) {
						buffer[header_size + (height - 1 - i) * channel * width + (j) * channel + k] = data[i * stride + j * channel + k];
					}
				}
			}
			out.write(buffer, length);
			out.close();
		} while (false);
		delete[] buffer;
	}

	image::image() {
		__data = nullptr;

	}
	image::~image() {
		stbi_image_free(__data);
	}
	int image::get_width() const {
		return __width;
	}

	int image::get_height() const {
		return __height;
	}

	unsigned char* image::get_data() const {
		return __data;
	}

	int image::imread(std::string path) {
		auto res = 0;
		do {
			if (__data != nullptr) stbi_image_free(__data);
			__data = nullptr;
			__data = stbi_load(path.c_str(), &__width, &__height, &__channel, 0);
			if (__data == nullptr) { res = 1; break; }
		} while (false);
		return res;
	}
	int image::imwrite(std::string path) {
		auto res = 0;
		do {
			res = stbi_write_bmp(path.c_str(), __width, __height, __channel, __data);
			if (res == 0) { res = 1; break; }
		} while (false);
		return res;
	}
}
