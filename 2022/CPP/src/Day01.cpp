// SPDX-FileCopyrightText: 2022 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>

int main(int argc, char ** argv) {
	if (argc < 2) {
		std::cout << "Not enough arguments: expected input filename as argument 1" << std::endl;
		return 1;
	}

	std::ifstream file(argv[1]);
	if (!file) {
		std::cout << "Couldn't open input file " << argv[1] << std::endl;
		return 1;
	}

	std::vector<unsigned> elves;
	elves.reserve(1000);

	unsigned counter{};
	std::string line;
	while (std::getline(file, line)) {
		if (line.empty()) {
		  	elves.emplace_back(counter);
		  	counter = 0;
		} else {
		  	counter += std::stoul(line);
		}
	}
	elves.push_back(counter);

	std::sort(elves.begin(), elves.end());

	std::cout << "P1: " << elves.back() << std::endl;
	std::cout << "P2: " << std::reduce(elves.end() - 3, elves.end()) << std::endl;
}
