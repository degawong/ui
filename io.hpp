/*
 * @Description: 
 * @Autor: degawong
 * @Date: 2020-03-23 16:41:29
 * @LastEditors: degawong
 * @LastEditTime: 2020-05-12 15:04:15
 * @FilePath: \harpocrates\io.hpp
 */
#pragma once

#include <string>

namespace harpocrates {
	struct IO {
		void imwrite(int width, int height, int channel, unsigned char * data, std::string name);
		void imwrite(int width, int height, int channel, int stride, unsigned char * data, std::string name);
	};
}
