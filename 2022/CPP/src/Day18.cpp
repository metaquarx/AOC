// SPDX-FileCopyrightText: 2022 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "Day18.hpp"

#include "Utils.hpp"
#include <limits>
#include <stdexcept>
#include <unordered_set>
#include <vector>

namespace {

struct Coord {
	int x, y, z;
	int & operator[](int i) {
		if (i == 0) {
			return x;
		} else if (i == 1) {
			return y;
		} else if (i == 2) {
			return z;
		}
		throw std::runtime_error("poopie");
	}
	bool operator==(Coord other) const {
		return x == other.x && y == other.y && z == other.z;
	}
	struct Hash {
		std::size_t operator()(Coord c) const {
			auto xh = std::hash<int>()(c.x);
			xh = std::hash<int>()(c.y) + 0x9e3779b9 + (xh << 6) + (xh >> 2);
			xh = std::hash<int>()(c.z) + 0x9e3779b9 + (xh << 6) + (xh >> 2);
			return xh;
		}
	};
};

}

Solution::Answer Day18::solve(std::string input) const {
	std::unordered_set<Coord, Coord::Hash> coords;
	for (auto & cube : Utils::split(input, "\n")) {
		Coord cub;
		std::sscanf(cube.c_str(), "%i,%i,%i", &cub.x, &cub.y, &cub.z);
		coords.emplace(cub);
	}

	auto adj = [&](Coord p) {
		std::vector<Coord> out;
		for (auto axis : {0, 1, 2}) {
			for (auto d : {-1, 1}) {
				auto q = p;
				q[axis] += d;
				out.emplace_back(q);
			}
		}
		return out;
	};

	std::size_t p1{};
	for (auto p : coords) {
		for (auto q : adj(p)) {
			if (coords.find(q) == coords.end()) {
				p1++;
			}
		}
	}

	Coord P_min{std::numeric_limits<int>::max(), std::numeric_limits<int>::max(), std::numeric_limits<int>::max()};
	Coord P_max{std::numeric_limits<int>::min(), std::numeric_limits<int>::min(), std::numeric_limits<int>::min()};
	for (auto axis : {0, 1, 2}) {
		for (auto p : coords) {
			P_min[axis] = std::min(P_min[axis], p[axis] - 1);
			P_max[axis] = std::max(P_max[axis], p[axis] + 1);
		}
	}

	std::vector<Coord> stack;
	stack.emplace_back(P_min);
	std::unordered_set<Coord, Coord::Hash> visited;
	std::size_t p2{};
	while (stack.size()) {
		auto p = stack.back();
		stack.pop_back();
		if (visited.find(p) != visited.end()) {
			continue;
		}
		visited.emplace(p);
		for (auto q : adj(p)) {
			if (coords.find(q) != coords.end()) {
				p2++;
			} else if (visited.find(q) == visited.end()) {
				if (P_min.x <= q.x && q.x <= P_max.x &&
					P_min.y <= q.y && q.y <= P_max.y &&
					P_min.z <= q.z && q.z <= P_max.z) {
					stack.emplace_back(q);
				}
			}
		}
	}

	return {std::to_string(p1),
			std::to_string(p2)};
}

std::vector<Solution::Test> Day18::get_tests() const {
	return {
		{
			"1,1,1\n2,1,1",
			{"10", "10"}
		}, {
			"2,2,2\n1,2,2\n3,2,2\n2,1,2\n2,3,2\n2,2,1\n2,2,3\n2,2,4\n2,2,6\n1,2,5\n3,2,5\n2,1,5\n2,3,5",
			{"64", "58"}
		}
	};
}
