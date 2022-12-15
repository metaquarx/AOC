// SPDX-FileCopyrightText: 2022 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "Day02.hpp"

#include "Utils.hpp"
#include <algorithm>
#include <numeric>

namespace {

enum State : int {
	Rock     = 0,
	Paper    = 1,
	Scissors = 2,
	Lose     = 0,
	Draw     = 1,
	Win      = 2
};

State status(State lhs, State rhs) {
	switch (rhs - lhs) {
		case -2: [[fallthrough]];
		case  1: return State::Win;
		case  0: return State::Draw;
		default: return State::Lose;
	}
}

int score(State state, State outcome) {
	return 1 + state + outcome * 3;
}

}

Solution::Answer Day02::solve(std::string input) const {
	int p1{};
	int p2{};
	for (auto & game : Utils::split(input, "\n")) {
		auto othr = State{game[0] - 'A'};
		auto self = State{game[2] - 'X'};

		p1 += score(self, status(othr, self));

		for (auto response = 0; response < 3; response++) {
			if (status(othr, State{response}) == self) {
				p2 += score(State{response}, self);
			}
		}
	}

	return {std::to_string(p1),
			std::to_string(p2)};
}

std::vector<Solution::Test> Day02::get_tests() const {
	return {
		{
			"A Y\nB X\nC Z",
			{"15", "12"}
		}
	};
}
