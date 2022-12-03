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

	enum class RPS {Rock, Paper, Scissors};
	enum class Status {Lose, Draw, Win};

	std::string line;
	unsigned score_p1{};
	unsigned score_p2{};
	while (std::getline(file, line)) {
		auto input = RPS{line[0] - 'A'};
		auto output = RPS{line[2] - 'X'};

		auto calc_status = [](RPS a, RPS b) {
			switch (static_cast<int>(b) - static_cast<int>(a)) {
				case -2: [[fallthrough]];
				case  1: return Status::Win;
				case  0: return Status::Draw;
				default: return Status::Lose;
			}
		};

		auto calc_score = [](RPS b, Status s) {
			return 1u + static_cast<unsigned>(b) + static_cast<unsigned>(s) * 3;
		};

		score_p1 += calc_score(output, calc_status(input, output));

			for (auto response = 0; response != 3; response++) {
				if (calc_status(input, RPS{response}) == Status{line[2] - 'X'}) {
					score_p2 += calc_score(RPS{response}, Status{line[2] - 'X'});
				}
			}
	}
	std::cout << "P1: " << score_p1 << std::endl;
	std::cout << "P2: " << score_p2 << std::endl;
}
