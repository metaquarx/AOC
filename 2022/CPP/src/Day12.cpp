// SPDX-FileCopyrightText: 2022 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "Day12.hpp"

#include "Utils.hpp"
#include <algorithm>
#include <limits>
#include <unordered_map>
#include <optional>

namespace {

struct Coord {
	int x, y;
	Coord(int x_ = 0, int y_ = 0) : x(x_), y(y_) {}

	bool operator==(const Coord & o) const {
		return x == o.x && y == o.y;
	}

	bool operator!=(const Coord & o) const {
		return !(*this == o);
	}

	Coord operator+(const Coord & o) const {
		return {x + o.x, y + o.y};
	}

	struct Hash {
		std::size_t operator()(const Coord & c) const {
			auto xh = std::hash<int>()(c.x);
			return std::hash<int>()(c.y) + 0x9e3779b9 + (xh << 6) + (xh >> 2);
		}
	};
};

}

Solution::Answer Day12::solve(std::string input) const {
	std::vector<std::vector<int>> height_map;
	Coord start, end;
	for (auto & row : Utils::split(input, "\n")) {
		std::vector<int> new_row;
		new_row.reserve(row.size());

		for (auto height : row) {
			auto new_height = height - 'a';

			if (height == 'S') {
				new_height = 0;
				start.y = static_cast<int>(height_map.size());
				start.x = static_cast<int>(new_row.size());
			} else if (height == 'E') {
				new_height = 25;
				end.y = static_cast<int>(height_map.size());
				end.x = static_cast<int>(new_row.size());
			}

			new_row.emplace_back(new_height);
		}
		height_map.emplace_back(std::move(new_row));
	}

	const int height = static_cast<int>(height_map.size());
	const int width = static_cast<int>(height_map.front().size());

	std::unordered_map<Coord, unsigned, Coord::Hash> distance;
	std::vector<Coord> queue;
	queue.push_back(end);
	distance[end] = 0;

	std::optional<unsigned> p1, p2;

	while (queue.size()) {
		auto U = queue.back(); queue.pop_back();

		if (!p2 && height_map[U.y][U.x] == 0)
			p2.emplace(distance[U]);
		if (!p1 && U == start)
			p1.emplace(distance[U]);
		if (p1 && p2)
			return {std::to_string(*p1), std::to_string(*p2)};

		std::vector<Coord> opts{{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
		for (auto dir : opts) {
			auto V = U + dir;
			if (distance.count(V) == 1 ||
				V.x < 0 || V.y < 0 || V.x >= width || V.y >= height ||
				height_map[V.y][V.x] < height_map[U.y][U.x] - 1) {
				continue;
			}
			distance[V] = distance[U] + 1;
			queue.push_back(V);
			std::sort(queue.begin(), queue.end(), [&](const Coord & lhs, const Coord & rhs) {
				return distance[lhs] > distance[rhs];
			});
		}
	}

	return {"", ""};
}

std::vector<Solution::Test> Day12::get_tests() const {
	return {
		{
			"Sabqponm\nabcryxxl\naccszExk\nacctuvwj\nabdefghi",
			{"31", "29"}
		}
	};
}
