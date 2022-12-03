// SPDX-FileCopyrightText: 2022 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include <iostream>
#include <fstream>
#include <unordered_set>
#include <set>
#include <vector>
#include <algorithm>
#include <cassert>

int main(int argc, char ** argv) {
	std::ifstream file(argv[1]);

	std::string line;
	unsigned long total_p1{};
	unsigned long total_p2{};

	auto value = [](char c){
		return 1 + (c < 'a' ? c - 'A' + 26 : c - 'a');
	};
	auto compare = [&](char l, char r) { return value(l) < value(r); };
	auto overlap = [&](auto b1, auto e1, auto b2, auto e2) {
		std::set<char, decltype(compare)> a(b1, e1, compare);
		std::set<char, decltype(compare)> b(b2, e2, compare);

		std::vector<char> output;
		std::set_intersection(a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(output), compare);
		return output;
	};

	std::vector<std::set<char, decltype(compare)>> group;
	while (std::getline(file, line)) {
		total_p1 += value(overlap(line.begin(), line.begin() + line.size() / 2,
								  line.begin() + line.size() / 2, line.end()).front());

		group.emplace_back(line.begin(), line.end(), compare);
		if (group.size() == 3) {
			auto first = overlap(group[0].begin(), group[0].end(),
								 group[1].begin(), group[1].end());
			auto second = overlap(first.begin(), first.end(),
								  group[2].begin(), group[2].end());
			total_p2 += value(second.front());
			group.clear();
		}
	}

	std::cout << "P1: " << total_p1 << std::endl;
	std::cout << "P2: " << total_p2 << std::endl;
}
