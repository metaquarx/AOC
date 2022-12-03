// SPDX-FileCopyrightText: 2022 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "Day01.hpp"

#include "Utils.hpp"
#include <algorithm>
#include <numeric>

Solution::Answer Day01::solve(std::string input) const {
	std::vector<unsigned> elves;
	elves.reserve(1000);

	for (auto & elf : Utils::split(input, "\n\n")) {
		unsigned total{};
		for (auto & calories : Utils::split(elf, "\n")) {
			total += std::stoul(calories);
		}
		elves.push_back(total);
	}

	std::sort(elves.begin(), elves.end());
	return {std::to_string(elves.back()),
			std::to_string(std::reduce(elves.end() - 3, elves.end()))};
}

std::vector<Solution::Test> Day01::get_tests() const {
	return {
		{
			"1000\n2000\n3000\n\n4000\n\n5000\n6000\n\n7000\n8000\n9000\n\n10000",
			{"24000", "45000"}
		}
	};
}
