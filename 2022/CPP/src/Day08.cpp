// SPDX-FileCopyrightText: 2022 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "Day08.hpp"

#include "Utils.hpp"

namespace {

using ipair = std::pair<int, int>;

}

Solution::Answer Day08::solve(std::string input) const {
	auto grid = Utils::split(input, "\n");
	auto height = static_cast<int>(grid.size());
	auto width = static_cast<int>(grid.front().size());

	auto search = [&](auto & self, ipair dir, int x, int y, char max, bool skip = true) {
		if (x == -1 || y == -1 || x == width || y == height) {
			return true;
		}

		if (!skip && grid[static_cast<std::size_t>(y)][static_cast<std::size_t>(x)] >= max) {
			return false;
		}

		return self(self, dir, x + dir.first, y + dir.second, max, false);
	};

	auto get_score = [&](auto & self, ipair dir, int x, int y, char max, bool skip = true) {
		if (x == 0 || y == 0 || x == width - 1 || y == height - 1 ||
			(!skip && grid[static_cast<std::size_t>(y)][static_cast<std::size_t>(x)] >= max)) {
			return 0;
		}

		return self(self, dir, x + dir.first, y + dir.second, max, false) + 1;
	};

	int visible = (width + height) * 2 - 4;
	int score{};
	for (int y = 1; y < height - 1; y++) {
		for (int x = 1; x < width - 1; x++) {
			auto current = grid[static_cast<std::size_t>(y)][static_cast<std::size_t>(x)];
			visible +=
				search(search, {-1,  0}, x, y, current) ||
				search(search, { 1,  0}, x, y, current) ||
				search(search, { 0, -1}, x, y, current) ||
				search(search, { 0,  1}, x, y, current);
			score = std::max(score,
				get_score(get_score, {-1,  0}, x, y, current) *
				get_score(get_score, { 1,  0}, x, y, current) *
				get_score(get_score, { 0, -1}, x, y, current) *
				get_score(get_score, { 0,  1}, x, y, current));
		}
	}

	return {std::to_string(visible),
			std::to_string(score)};
}

std::vector<Solution::Test> Day08::get_tests() const {
	return {
		{
			"30373\n25512\n65332\n33549\n35390",
			{"21", "8"}
		}
	};
}
