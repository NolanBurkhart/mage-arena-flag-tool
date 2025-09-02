#pragma once

#include <string>
#include <map>

struct color_t {
	unsigned char r;
	unsigned char g;
	unsigned char b;

	const double distance(const color_t& x) const {
		return pow(pow(r - x.r, 2) + pow(g - x.g, 2) + pow(b - x.b, 2), 0.5);
	}
	const bool operator<(const color_t& x) const { //big meme
		double a = 255 * 255 * r + 255 * g + b;
		double b = 255 * 255 * x.r + 255 * x.g + x.b;
		return a < b;
	}
};

std::string match_color(const std::map<color_t, std::string> color_map, const color_t& desired);
std::string match_color_fast(const std::map<color_t, std::string> color_map, const color_t& desired);

typedef std::map<color_t, std::string> color_map_t;

const color_map_t create_color_map();