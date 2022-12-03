// SPDX-FileCopyrightText: 2022 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "Day03.hpp"

#include "Utils.hpp"

static int value(char c) {
	return c < 'a' ? c - 'A' + 27 : c - 'a' + 1; // flip A-Za-z ASCII into a-zA-Z
}

template <typename T1, typename T2>
static std::vector<char> overlap(T1 lhs_b, T1 lhs_e, T2 rhs_b, T2 rhs_e) {
	std::vector<char> out;
	for (auto outer = lhs_b; outer != lhs_e; outer++) {
		for (auto inner = rhs_b; inner != rhs_e; inner++) {
			if (value(*outer) == value(*inner)) { // extract intersections
				out.push_back(*outer);
			}
		}
	}
	return out;
}

Solution::Answer Day03::solve(std::string input) const {
	int p1{};
	int p2{};

	std::vector<std::string> prev;
	for (auto & backpack : Utils::split(input, "\n")) {
		p1 += value(overlap(backpack.begin(), backpack.begin() + backpack.size() / 2,
							backpack.rbegin(), backpack.rend() - backpack.size() / 2).front());

		prev.push_back(backpack);
		if (prev.size() == 3) {
			auto pair = overlap(prev[0].begin(), prev[0].end(), prev[1].begin(), prev[1].end());
				 pair = overlap(prev[2].begin(), prev[2].end(), pair.begin(), pair.end());
			p2 += value(pair.front());
			prev.clear();
		}
	}

	return {std::to_string(p1),
			std::to_string(p2)};
}

std::vector<Solution::Test> Day03::get_tests() const {
	return {
		{
			"vJrwpWtwJgWrhcsFMMfFFhFp\njqHRNqRjqzjGDLGLrsFMfFZSrLrFZsSL\nPmmdzqPrVvPwwTWBwg\nwMqvLMZHhHMvwLHjbvcjnnSBnvTQFn\nttgJtRGJQctTZtZT\nCrZsJsPPZsGzwwsLwLmpwMDw",
			{"157", "70"}
		}
	};
}
