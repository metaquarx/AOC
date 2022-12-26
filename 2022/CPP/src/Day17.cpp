// SPDX-FileCopyrightText: 2022 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "Day17.hpp"

#include "Utils.hpp"
#include <array>
#include <map>

Solution::Answer Day17::solve(std::string input) const {
	if (input.back() == '\n') {
		input.pop_back();
	}

	std::array<std::vector<unsigned>, 5> rocks{{
		{0x1E},
		{0x08, 0x1C, 0x08},
		{0x1C, 0x04, 0x04},
		{0x10, 0x10, 0x10, 0x10},
		{0x18, 0x18}
	}};
	std::array<std::array<int, 2>, 5> sides{{
		{-2, 1}, {-2, 2}, {-2, 2}, {-2, 4}, {-2, 3}
	}};

	std::vector<unsigned> field;
	unsigned sp{};
	unsigned wp{};

	field.push_back(0x7F);

	auto fits = [&](unsigned stone, int shift, std::size_t bp) {
		for (std::size_t r = 0; r < rocks[stone].size(); r++) {
			unsigned sv = shift > 0 ? rocks[stone][r] >> shift : rocks[stone][r] << -shift;
			if (bp + r < field.size() && (field[bp + r] & sv))
				return false;
		}
		return true;
	};

	std::vector<std::size_t> heights;
	std::map<std::pair<unsigned, unsigned>, std::vector<unsigned>> drops;
	std::map<unsigned, unsigned> loopcount;

	while (true) {
		std::size_t bp = field.size() + 3;
		int shift = 0;
		bool drop = true;
		do {
			switch(input[wp]) {
				case '<': {
					if (shift != sides[sp % 5][0] && fits(sp % 5, shift - 1, bp))
						--shift;
				} break;
				case '>': {
					if (shift != sides[sp % 5][1] && fits(sp % 5, shift + 1, bp))
						++shift;
				} break;
			}

			wp = (wp+1) % input.size();
			drop = fits(sp % 5, shift, bp - 1);
			if (drop) --bp;
		} while(drop);

		std::size_t top = bp + rocks[sp % 5].size();
		if (field.size() < top) {
			field.resize(top);
		}
		for (std::size_t r = 0; r < rocks[sp % 5].size(); r++) {
			unsigned sv = shift > 0 ? rocks[sp % 5][r] >> shift : rocks[sp % 5][r] << -shift;
			field[bp+r] |= sv;
		}
		++sp;
		heights.push_back(field.size() - 1);

		auto check = std::make_pair((sp - 1) % 5, (wp - 1 + input.size()) % input.size());
		if (drops[check].size() != 0) {
			for (auto x : drops[check]) {
				unsigned d = (sp - 1) - x;
				loopcount[d]++;
				if (loopcount[d] >= 40) {
					std::size_t hd = heights[sp - 1] - heights[x];

					std::size_t loops = (2022 - x) / d;
					std::size_t inLoop = 2022 - loops * d;
					auto p1 = heights[inLoop] + hd * loops - 2;

					loops = (1'000'000'000'000 - x) / d;
					inLoop = 1'000'000'000'000 - loops * d;
					auto p2 = heights[inLoop] + hd * loops - 1;

					return {std::to_string(p1), std::to_string(p2)};
				}
			}
		}
		drops[check].push_back(sp-1);
	}
}

std::vector<Solution::Test> Day17::get_tests() const {
	return {
		{
			">>><<><>><<<>><>>><<<>>><<<><<<>><>><<>>",
			{"3068", "1514285714288"}
		}
	};
}
