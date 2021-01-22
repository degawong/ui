/*
 * @Description: 
 * @Autor: degawong
 * @Date: 2020-03-23 16:41:29
 * @LastEditors: degawong
 * @LastEditTime: 2020-05-12 15:04:15
 * @FilePath: \harpocrates\io.cpp
 */
#pragma once

#include <string>
#include <fstream>
#include <sstream>

#include <io.hpp>

namespace harpocrates {
	using namespace std;
	const char image_info[] = {
		0x42, 0x4D, 0x36, 0x00, 0x24, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00,
		0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x03,
		0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC4, 0x0E,
		0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	// not work well
	void IO::imwrite(int width, int height, int channel, unsigned char * data, std::string name) {
		auto out = ofstream();
		auto buffer = (char*)nullptr;
		const int size_of_header = 54;
		const auto data_size = channel * width * height;
		auto length = size_of_header + data_size;
		do {
			out.open(name, ios::out);
			buffer = new char[length] {0};
			if (buffer == nullptr) { break; }
			for (int i = 0; i < 54; i++) buffer[i] = image_info[i];
			*(int*)&(buffer[0x12]) = width;
			*(int*)&(buffer[0x16]) = height;
			*(int*)&(buffer[0x22]) = channel * width * height;
			std::copy((char*)data, (char*)(data + data_size), buffer + size_of_header);
			out.write(buffer, length);
		} while (false);
		out.close();
		delete[] buffer;
	}
	
	void IO::imwrite(int width, int height, int channel, int stride, unsigned char * data, std::string name) {
        //char * buffer = new char[54 + SCR_WIDTH * SCR_HEIGHT * csize];
        //for (int i = 0; i < 54; i++) buffer[i] = header[i];
        //*(int*)&(buffer[0x22]) = SCR_WIDTH * SCR_HEIGHT * csize;
        //*(int*)&(buffer[0x12]) = SCR_WIDTH;
        //*(int*)&(buffer[0x16]) = SCR_HEIGHT;
        //glReadPixels(0, 0, SCR_WIDTH, SCR_HEIGHT, GL_BGR, GL_UNSIGNED_BYTE, buffer + 54);
        //FILE * file = fopen("screenshot.bmp", "wb");
        //fwrite(buffer, 54 + SCR_WIDTH * SCR_HEIGHT * csize, 1, file);
        //fclose(file);
	}
}
