#pragma once
#include <png.h> 
#include <stdexcept>

png_bytep* load_image(const char* path, int& width, int& height);