// SPDX-FileCopyrightText: 2022 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "Day06.hpp"

#include "Utils.hpp"
#include <unordered_set>

Solution::Answer Day06::solve(std::string input) const {
	std::string p1, p2;

	auto repeats = [](const std::string & s) {
		return s.size() == std::unordered_set<char>{s.begin(), s.end()}.size();
	};

	for (std::size_t i = 4; i != input.size(); i++) {
		if (p1 == "" && repeats(input.substr(i - 4, 4))) {
			p1 = std::to_string(i);
		}
		if (p2 == "" && i >= 14 && repeats(input.substr(i - 14, 14))) {
			p2 = std::to_string(i);
		}
	}

	return {p1, p2};
}

std::vector<Solution::Test> Day06::get_tests() const {
	return {
		{
			"mjqjpqmgbljsphdztnvjfqwrcgsmlb",
			{"7", "19"}
		},
		{
			"bvwbjplbgvbhsrlpgdmjqwftvncz",
			{"5", "23"}
		},
		{
			"nppdvjthqldpwncqszvftbrmjlhg",
			{"6", "23"}
		},
		{
			"nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg",
			{"10", "29"}
		},
		{
			"zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw",
			{"11", "26"}
		}
	};
}
