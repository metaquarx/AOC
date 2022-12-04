// SPDX-FileCopyrightText: 2022 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "Day04.hpp"

#include "Utils.hpp"
#include <stdexcept>

Solution::Answer Day04::solve(std::string input) const {
	int p1{};
	int p2{};

	for (auto & pairs : Utils::split(input, "\n")) {
		int ls, le, rs, re;
		if (std::sscanf(pairs.c_str(), "%i-%i,%i-%i", &ls, &le, &rs, &re) != 4) {
			throw std::runtime_error("Invalid input: " + pairs);
		}

		auto contains = [](int lhs_start, int lhs_end, int rhs_start, int rhs_end) {
			return lhs_start >= rhs_start && lhs_end <= rhs_end;
		};
		p1 += contains(ls, le, rs, re) || contains(rs, re, ls, le);

		auto overlap = [](int lhs_end, int rhs_start, int rhs_end) {
			return lhs_end >= rhs_start && lhs_end <= rhs_end;
		};
		p2 += overlap(le, rs, re) || overlap(re, ls, le);
	}

	return {std::to_string(p1),
			std::to_string(p2)};
}

std::vector<Solution::Test> Day04::get_tests() const {
	return {
		{
			"2-4,6-8\n2-3,4-5\n5-7,7-9\n2-8,3-7\n6-6,4-6\n2-6,4-8",
			{"2", "4"}
		}
	};
}
