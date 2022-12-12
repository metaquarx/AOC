// SPDX-FileCopyrightText: 2022 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "Day09.hpp"

#include "Utils.hpp"
#include <unordered_map>
#include <unordered_set>
#include <array>
#include <cmath>

struct Coord {
	int x, y;
	Coord(int x_ = 0, int y_ = 0) : x(x_), y(y_) {}

	bool operator==(const Coord & o) const {
		return x == o.x && y == o.y;
	}

	Coord & operator+=(const Coord & o) {
		x += o.x;
		y += o.y;
		return *this;
	}

	Coord operator-(const Coord & o) const {
		return {x - o.x, y - o.y};
	}

	bool over() const {
		return std::abs(x) >= 2 || std::abs(y) >= 2;
	}

	Coord norm() const {
		return { (x > 0) - (x < 0), (y > 0) - (y < 0) };
	}

	struct Hash {
		std::size_t operator()(const Coord & c) const {
			auto xh = std::hash<int>()(c.x);
			return std::hash<int>()(c.y) + 0x9e3779b9 + (xh << 6) + (xh >> 2);
		}
	};

	static std::unordered_map<char, Coord> dirs() {
		return {
			{'R', { 1,  0}},
			{'L', {-1,  0}},
			{'U', { 0,  1}},
			{'D', { 0, -1}}
		};
	}
};

Solution::Answer Day09::solve(std::string input) const {
	std::unordered_set<Coord, Coord::Hash> p1;
	Coord head{}, tail{};
	for (auto & move : Utils::split(input, "\n")) {
		auto dz = Coord::dirs()[move[0]];
		for (auto i = 0u; i < std::stoul(Utils::split(move, " ")[1]); i++) {
			head += dz;
			auto delta = head - tail;
			if (delta.over()) {
				tail += delta.norm();
			}
			p1.emplace(tail);
		}
	}

	std::unordered_set<Coord, Coord::Hash> p2;
	std::array<Coord, 10> parts;
	for (auto & move : Utils::split(input, "\n")) {
		auto dz = Coord::dirs()[move[0]];
		for (auto i = 0u; i < std::stoul(Utils::split(move, " ")[1]); i++) {
			parts[0] += dz;
			for (std::size_t j = 1; j < 10; j++) {
				auto delta = parts[j - 1] - parts[j];
				if (delta.over()) {
					parts[j] += delta.norm();
				}
			}
			p2.emplace(parts.back());
		}
	}

	return {std::to_string(p1.size()),
			std::to_string(p2.size())};
}

std::vector<Solution::Test> Day09::get_tests() const {
	return {
		{
			"R 4\nU 4\nL 3\nD 1\nR 4\nD 1\nL 5\nR 2",
			{"13", "1"}
		}, {
			"R 5\nU 8\nL 8\nD 3\nR 17\nD 10\nL 25\nU 20",
			{"88", "36"}
		}
	};
}
