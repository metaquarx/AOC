// SPDX-FileCopyrightText: 2022 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "Solution.hpp"

struct Day11 : public Solution {
	virtual Answer solve(std::string input) const override;
	virtual std::vector<Test> get_tests() const override;
};
