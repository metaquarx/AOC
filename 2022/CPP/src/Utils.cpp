// SPDX-FileCopyrightText: 2022 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "Utils.hpp"

namespace Utils {

std::vector<std::string> split(const std::string & input, const std::string & delimiter) {
	std::vector<std::string> out;

	std::size_t start{};
	std::size_t end = input.find(delimiter);
	while (end != std::string::npos) {
		out.push_back(input.substr(start, end - start));
		start = end + delimiter.size();
		end = input.find(delimiter, start);
	}
	out.push_back(input.substr(start, end));

	if (out.size() && out.back().size() == 0) {
		out.pop_back();
	}

	return out;
}

Coord::Coord(int x_, int y_) : x(x_), y(y_) {}

int Coord::manhattan(const Coord & to) const {
	return std::abs(x - to.x) + std::abs(y - to.y);
}

Coord Coord::operator+(const Coord & o) const {
	return {x + o.x, y + o.y};
}

Coord & Coord::operator+=(const Coord &other) {
	*this = *this + other;
	return *this;
}

Coord Coord::operator-(const Coord & o) const {
	return {x - o.x, y - o.y};
}

bool Coord::operator==(const Coord & other) const {
	return x == other.x && y == other.y;
}

bool Coord::operator!=(const Coord & other) const {
	return !(*this == other);
}

std::size_t Coord::Hash::operator()(const Coord & c) const {
	auto xh = std::hash<int>()(c.x);
	return std::hash<int>()(c.y) + 0x9e3779b9 + (xh << 6) + (xh >> 2);
}

}
