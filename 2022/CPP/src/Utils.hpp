// SPDX-FileCopyrightText: 2022 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <string>
#include <vector>

namespace Utils {

std::vector<std::string> split(const std::string & input, const std::string & delimiter);

struct Coord {
	int x, y;
	Coord(int x_ = 0, int y_ = 0);

	int manhattan(const Coord & to) const;

	Coord operator+(const Coord & other) const;
	Coord & operator+=(const Coord & other);
	Coord operator-(const Coord & other) const;
	bool operator==(const Coord & other) const;
	bool operator!=(const Coord & o) const;

	struct Hash {
		std::size_t operator()(const Coord & c) const;
	};
};

}
