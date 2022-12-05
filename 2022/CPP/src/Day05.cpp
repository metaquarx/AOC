// SPDX-FileCopyrightText: 2022 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "Day05.hpp"

#include "Utils.hpp"
#include <algorithm>

using Crate = char;
using Stack = std::vector<Crate>;
using Ship = std::vector<Stack>;

static Ship parse_ship(const std::string & input) {
	Ship output;
	output.resize(20);

	auto rows = Utils::split(input, "\n");
	rows.pop_back(); // remove crate positions entry
	for (auto & row : rows) {
		std::size_t stack_idx{};
		do {
			if (char c; std::sscanf(row.c_str(), "[%c]", &c) == 1) { // crate
				output[stack_idx].push_back(c);
			}
			row.erase(0, 4); // go to next column
		} while (++stack_idx && row.size());
	}

	for (auto & stack : output) { // reverse, since current order is top-down
		std::reverse(stack.begin(), stack.end());
	}

	return output;
}

struct Move {
	std::size_t count;
	std::size_t from;
	std::size_t to;
};
using Moves = std::vector<Move>;

static Moves parse_moves(const std::string & input) {
	Moves output;

	auto moves = Utils::split(input, "\n");
	output.reserve(moves.size());
	for (auto move : moves) {
		Move temp;
		if (std::sscanf(move.c_str(), "move %zu from %zu to %zu", &temp.count, &temp.from, &temp.to) == 3) {
			output.push_back({temp.count, temp.from - 1, temp.to -1}); // -1 to get indices
		}
	}

	return output;
}

Solution::Answer Day05::solve(std::string input) const {
	auto parts = Utils::split(input, "\n\n");

	auto ship = parse_ship(parts[0]);
	auto moves = parse_moves(parts[1]);

	std::string p1;
	auto stacks = ship;
	for (auto & move : moves) {
		auto & from = stacks[move.from];
		auto & to = stacks[move.to];

		for (auto _ = move.count; _--; ) {
			to.push_back(from.back());
			from.pop_back();
		}
	}
	for (auto & stack : stacks) {
		if (stack.size())
		p1 += stack.back();
	}

	std::string p2{};
	stacks = ship;
	for (auto & move : moves) {
		auto & from = stacks[move.from];
		auto & to = stacks[move.to];

		to.insert(to.end(), from.end() - static_cast<long>(move.count), from.end());
		from.erase(from.end() - static_cast<long>(move.count), from.end());
	}
	for (auto & stack : stacks) {
		if (stack.size()) {
			p2 += stack.back();
		}
	}

	return {p1,
			p2};
}

std::vector<Solution::Test> Day05::get_tests() const {
	return {
		{
			"    [D]    \n[N] [C]    \n[Z] [M] [P]\n 1   2   3 \n\nmove 1 from 2 to 1\nmove 3 from 1 to 3\nmove 2 from 2 to 1\nmove 1 from 1 to 2\n",
			{"CMZ", "MCD"}
		}
	};
}
