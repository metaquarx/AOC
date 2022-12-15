// SPDX-FileCopyrightText: 2022 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "Day14.hpp"

#include "Utils.hpp"
#include <array>
#include <cassert>
#include <algorithm>
#include <numeric>

namespace {

using Utils::Coord;

Coord parse_coord(const std::string & i) {
	Coord out;
	auto c = std::sscanf(i.c_str(), "%i, %i", &out.x, &out.y);
	assert(c == 2);
	return out;
}

Coord normalise(const Coord & c) {
	return {std::clamp(c.x, -1, 1), std::clamp(c.y, -1, 1)};
}

struct Cave {
	enum class Tile {Empty, Rock, Sand};
	std::array<std::array<Tile, 400>, 200> tiles;
	int highest_y;
	Cave() : tiles{{{Tile::Empty}}}, highest_y{} {}

	void add_line(Coord start, Coord end) {
		auto dir = normalise((end - start));
		for (auto p = start; p != end + dir; p = p + dir) {
			tiles[p.y][p.x - 300] = Tile::Rock;
			highest_y = std::max(highest_y, p.y);
		}
	}

	void run(bool until_full = false) {
		tiles[highest_y + 2].fill(Tile::Rock);

		while (!until_full || tiles[0][200] != Tile::Sand) {
			Coord f{200, 0};

			while (true) {
				f.y++;
				if (!until_full && f.y == highest_y + 1) {
					return;
				} else if (tiles[f.y][f.x] == Tile::Empty) {
				} else if (int c = (tiles[f.y][f.x - 1] == Tile::Empty ? -1 :
								   (tiles[f.y][f.x + 1] == Tile::Empty ?  1 :
								   	0))) {
					f.x += c;
				} else {
					tiles[f.y - 1][f.x] = Tile::Sand;
					break;
				}
			}
		}
	}

	unsigned get_rested_sand() {
		return std::accumulate(tiles.front().begin(), tiles.back().end(), 0u, [&](unsigned prev, Tile t) {
			return prev += t == Tile::Sand ? 1 : 0;
		});
	}
};

}

Solution::Answer Day14::solve(std::string input) const {
	Cave c;
	for (auto & path : Utils::split(input, "\n")) {
		auto lines = Utils::split(path, " -> ");
		for (std::size_t i = 0; i < lines.size() - 1; i++) {
			c.add_line(parse_coord(lines[i]), parse_coord(lines[i + 1]));
		}
	}

	c.run();
	unsigned p1 = c.get_rested_sand();

	c.run(true);
	unsigned p2 = c.get_rested_sand();

	return {std::to_string(p1), std::to_string(p2)};
}

std::vector<Solution::Test> Day14::get_tests() const {
	return {
		{
			"498,4 -> 498,6 -> 496,6\n503,4 -> 502,4 -> 502,9 -> 494,9",
			{"24", "93"}
		}
	};
}
