// SPDX-FileCopyrightText: 2022 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "Day10.hpp"

#include "Utils.hpp"
#include <regex>
#include <stdexcept>
#include <unordered_set>
#include <array>

Solution::Answer Day10::solve(std::string input) const {
	input = std::regex_replace(input, std::regex("addx"), "noop\naddx");

	int p1{};
	std::string display(40u * 6u, '.');

	int x = 1, cycles = 0;
	auto strength = [&]() {
		return x * cycles;
	};
	auto interesting = [&]() {
		return cycles == 20 || (cycles - 20) % 40 == 0;
	};
	auto draw = [&]() {
		auto drawing = (cycles - 1) % 40;
		if (drawing == x - 1 || drawing == x || drawing == x + 1) {
			display[static_cast<unsigned>(cycles - 1)] = '#';
		}
	};
	for (auto & ins : Utils::split(input, "\n")) {
		cycles++;

		draw();
		if (interesting()) {
			p1 += strength();
		}

		if (ins[0] == 'a') {
			x += std::stoi(Utils::split(ins, " ")[1]);
		} else if (ins[0] != 'n') {
			throw std::runtime_error("invalid input " + ins);
		}
	}

	std::string p2{};
	p2.reserve(41 * 6);
	for (std::size_t i = 0; i < display.size(); i++) {
		if (i % 40 == 0) {
			p2.push_back('\n');
		}
		p2.push_back(display[i]);
	}
	return {std::to_string(p1), p2};
}

std::vector<Solution::Test> Day10::get_tests() const {
	return {
		{
			"addx 15\naddx -11\naddx 6\naddx -3\naddx 5\naddx -1\naddx -8\naddx 13\naddx 4\nnoop\naddx -1\naddx 5\naddx -1\naddx 5\naddx -1\naddx 5\naddx -1\naddx 5\naddx -1\naddx -35\naddx 1\naddx 24\naddx -19\naddx 1\naddx 16\naddx -11\nnoop\nnoop\naddx 21\naddx -15\nnoop\nnoop\naddx -3\naddx 9\naddx 1\naddx -3\naddx 8\naddx 1\naddx 5\nnoop\nnoop\nnoop\nnoop\nnoop\naddx -36\nnoop\naddx 1\naddx 7\nnoop\nnoop\nnoop\naddx 2\naddx 6\nnoop\nnoop\nnoop\nnoop\nnoop\naddx 1\nnoop\nnoop\naddx 7\naddx 1\nnoop\naddx -13\naddx 13\naddx 7\nnoop\naddx 1\naddx -33\nnoop\nnoop\nnoop\naddx 2\nnoop\nnoop\nnoop\naddx 8\nnoop\naddx -1\naddx 2\naddx 1\nnoop\naddx 17\naddx -9\naddx 1\naddx 1\naddx -3\naddx 11\nnoop\nnoop\naddx 1\nnoop\naddx 1\nnoop\nnoop\naddx -13\naddx -19\naddx 1\naddx 3\naddx 26\naddx -30\naddx 12\naddx -1\naddx 3\naddx 1\nnoop\nnoop\nnoop\naddx -9\naddx 18\naddx 1\naddx 2\nnoop\nnoop\naddx 9\nnoop\nnoop\nnoop\naddx -1\naddx 2\naddx -37\naddx 1\naddx 3\nnoop\naddx 15\naddx -21\naddx 22\naddx -6\naddx 1\nnoop\naddx 2\naddx 1\nnoop\naddx -10\nnoop\nnoop\naddx 20\naddx 1\naddx 2\naddx 2\naddx -6\naddx -11\nnoop\nnoop\nnoop",
			{"13140", "\n##..##..##..##..##..##..##..##..##..##..\n###...###...###...###...###...###...###.\n####....####....####....####....####....\n#####.....#####.....#####.....#####.....\n######......######......######......####\n#######.......#######.......#######....."}
		}
	};
}
