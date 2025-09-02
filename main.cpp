#include <Windows.h>
#include <iostream>
#include <winreg.h>
#include <string>
#include <map>
#include <png.h> 
#include <fstream>
#include <filesystem>

#include "util.h"
#include "color.h"
#include "registry.h"


int main(int args_c, char* args_v[]) {
	if (args_c != 2)
	{
		printf("Usage: %s <path to flag as a .png>", args_v[0]);
		return -1;
	}

	printf("=== Mage Arena Flag Importer - By Nolan Burkhart (Github: @NolanBurkhart) ===\n");
	color_map_t color_map;

	try {
		color_map = create_color_map();
	}
	catch (...) {
		printf("[-] Failed to load colormap, make sure that palette.png is in same directory as executable.");
		return -1;
	}
	
	const std::string flag_path = args_v[1];
	const auto object = std::filesystem::path(flag_path);

	if (!std::filesystem::exists(object))
		throw std::runtime_error("Image not found!");

	printf("[+] Loading image\n");
	int width = 100, height = 66;
	png_bytep* row_pointers;
	try {
		row_pointers = load_image(flag_path.c_str(), width, height);
	}
	catch (std::runtime_error err) {
		printf("Runtime Error (%s): You most likely chose an image that is either not a png or not 100x66px", err.what());
		return -1;
	}

	printf("[+] Translating textures \n");
	std::string flag_data;
	for (int x = 0; x < width; x++) { //flag is stored by columns
		for (int y = height - 1; y >= 0; y--) {
			const auto row = row_pointers[y];
			color_t c = *(color_t*)(&(row[x * 4]));
			flag_data += match_color(color_map, c) + ",";
		}
	}

	printf("[+] Writing to registry\n");
	const auto error = RegSetKeyValue(HKEY_CURRENT_USER, MAGEARENA_SUBKEY, MAGEARENA_FLAG_KEY, REG_BINARY, flag_data.c_str(), flag_data.size() - 1); 

	printf("[+] Done, enjoy!\n");
	return error;
}
