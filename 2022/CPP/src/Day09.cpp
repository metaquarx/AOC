// SPDX-FileCopyrightText: 2022 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "Day09.hpp"

#include "Utils.hpp"
#include <unordered_map>
#include <unordered_set>
#include <array>
#include <cmath>

namespace {

using Utils::Coord;

bool over(const Coord & c) {
	return std::abs(c.x) >= 2 || std::abs(c.y) >= 2;
}

Coord norm(const Coord & c) {
	return { (c.x > 0) - (c.x < 0), (c.y > 0) - (c.y < 0) };
}

std::unordered_map<char, Coord> dirs() {
	return {
		{'R', { 1,  0}},
		{'L', {-1,  0}},
		{'U', { 0,  1}},
		{'D', { 0, -1}}
	};
}

}

Solution::Answer Day09::solve(std::string input) const {
	std::unordered_set<Coord, Coord::Hash> p1;
	Coord head{}, tail{};
	for (auto & move : Utils::split(input, "\n")) {
		auto dz = dirs()[move[0]];
		for (auto i = 0u; i < std::stoul(Utils::split(move, " ")[1]); i++) {
			head += dz;
			auto delta = head - tail;
			if (over(delta)) {
				tail += norm(delta);
			}
			p1.emplace(tail);
		}
	}

	std::unordered_set<Coord, Coord::Hash> p2;
	std::array<Coord, 10> parts;
	for (auto & move : Utils::split(input, "\n")) {
		auto dz = dirs()[move[0]];
		for (auto i = 0u; i < std::stoul(Utils::split(move, " ")[1]); i++) {
			parts[0] += dz;
			for (std::size_t j = 1; j < 10; j++) {
				auto delta = parts[j - 1] - parts[j];
				if (over(delta)) {
					parts[j] += norm(delta);
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
