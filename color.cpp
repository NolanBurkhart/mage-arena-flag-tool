#include <png.h>
#include <map>
#include "util.h"
#include "color.h"
#include "palette.h"


std::string match_color(const color_map_t color_map, const color_t& desired)
{
	static color_map_t cache;
	if (cache.contains(desired))
		return cache[desired];

	double best_distance = INT_MAX;
	const std::string* best = nullptr;
	for (const auto& pair : color_map) {
		double dist = desired.distance(pair.first);
		if (dist < best_distance) {
			best_distance = dist;
			best = &pair.second;
		}
	}
	cache[desired] = *best;
	return *best;
}

const color_map_t create_color_map() {
	color_map_t color_map = {};
	int width, height = 0;
	auto row_pointers = load_image("palette.png", width, height);

	//only can store 4 characters (i.e 0.11)
	double height_step = 0.0;
	for (int y = height - 1; y >= 0; y -= height * 0.01) {
		const auto row = row_pointers[y];
		double width_step = 0.0;
		for (int x = 0; x < width; x += width * 0.01) {
			color_t c = *(color_t*)(&(row[x * 4]));
			for (const auto& pair : color_map) {
				if (pair.first.distance(c) < 25)
					continue;
			}
			color_map[c] = std::to_string(width_step) + ":" + std::to_string(height_step);
			width_step += 0.01; //left to right
		}
		height_step += 0.01; //its flipped
	}
	return color_map;
}