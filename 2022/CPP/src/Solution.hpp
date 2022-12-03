// SPDX-FileCopyrightText: 2022 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <string>
#include <utility>
#include <vector>

struct Solution {
	virtual ~Solution();

	struct Answer {
		std::string p1;
		std::string p2;
	};

	struct Test {
		std::string input;
		Answer expected_output;
	};

	virtual Answer solve(std::string input) const = 0;
	virtual std::vector<Test> get_tests() const = 0;
};
