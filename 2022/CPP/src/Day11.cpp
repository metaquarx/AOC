// SPDX-FileCopyrightText: 2022 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "Day11.hpp"

#include "Utils.hpp"
#include <cassert>
#include <functional>
#include <iostream>
#include <numeric>
#include <stdexcept>

namespace {

struct Monkey {
	std::vector<unsigned long> worry_levels;
	std::function<unsigned long(unsigned long)> inspect;
	unsigned long divisor;
	std::pair<std::size_t, std::size_t> next; // true, false
};

}

Solution::Answer Day11::solve(std::string input) const {
	std::vector<Monkey> monkeys;
	for (auto & monkey : Utils::split(input, "\n\n")) {
		Monkey temp;
		auto lines = Utils::split(monkey, "\n");

		std::size_t monkey_n;
		assert(std::sscanf(lines[0].c_str(), "Monkey %zu", &monkey_n) == 1);
		assert(monkey_n == monkeys.size());

		auto items = Utils::split(lines[1], " ");
		for (std::size_t i = 4; i < items.size(); i++) {
			temp.worry_levels.push_back(std::stoul(items[i]));
		}

		auto tokens = Utils::split(lines[2], " ");
		assert(tokens[3] == "new");
		auto get_val = [](std::string str, unsigned long old) {
			if (str == "old") {
				return old;
			} else {
				return std::stoul(str);
			}
		};
		temp.inspect = [&get_val, tokens](unsigned long old) {
			if (tokens[6] == "+") {
				return get_val(tokens[5], old) + get_val(tokens[7], old);
			} else if (tokens[6] == "*") {
				return get_val(tokens[5], old) * get_val(tokens[7], old);
			} else {
				throw std::runtime_error("Unexpected operation " + tokens[6]);
			}
		};

		tokens = Utils::split(lines[3], " ");
		assert(tokens[3] == "divisible");
		temp.divisor = std::stoul(tokens[5]);

		temp.next.first = std::stoul(Utils::split(lines[4], " ").back());
		temp.next.second = std::stoul(Utils::split(lines[5], " ").back());

		monkeys.push_back(temp);
	}
	unsigned long modulus = 1;
	for (auto & monkey : monkeys) {
		modulus *= monkey.divisor;
	}
	auto monkey_backup = monkeys;

	std::vector<unsigned> inspect_count(monkeys.size(), 0);
	auto round = [&](bool part1) {
		for (std::size_t i = 0; i < monkeys.size(); i++) {
			for (auto item : monkeys[i].worry_levels) {
				inspect_count[i]++;
				item = monkeys[i].inspect(item);
				if (part1) {
					item /= 3;
				} else {
					item %= modulus;
				}

				monkeys[(item % monkeys[i].divisor == 0) ? monkeys[i].next.first : monkeys[i].next.second].worry_levels.push_back(item);
			}
			monkeys[i].worry_levels.clear();
		}
	};

	std::string p1;
	for (unsigned i = 0; i < 20; i++) {
		round(true);
	}
	std::sort(inspect_count.begin(), inspect_count.end(), std::greater<unsigned long>());
	p1 = std::to_string(inspect_count[0] * inspect_count[1]);

	monkeys = monkey_backup;
	std::fill(inspect_count.begin(), inspect_count.end(), 0);

	std::string p2;
	for (unsigned i = 0; i < 10'000; i++) {
		round(false);
	}
	std::sort(inspect_count.begin(), inspect_count.end(), std::greater<unsigned long>());
	p2 = std::to_string(static_cast<unsigned long>(inspect_count[0]) * static_cast<unsigned long>(inspect_count[1]));

	return {p1, p2};
}

std::vector<Solution::Test> Day11::get_tests() const {
	return {
		{
			"Monkey 0:\n  Starting items: 79, 98\n  Operation: new = old * 19\n  Test: divisible by 23\n    If true: throw to monkey 2\n    If false: throw to monkey 3\n\nMonkey 1:\n  Starting items: 54, 65, 75, 74\n  Operation: new = old + 6\n  Test: divisible by 19\n    If true: throw to monkey 2\n    If false: throw to monkey 0\n\nMonkey 2:\n  Starting items: 79, 60, 97\n  Operation: new = old * old\n  Test: divisible by 13\n    If true: throw to monkey 1\n    If false: throw to monkey 3\n\nMonkey 3:\n  Starting items: 74\n  Operation: new = old + 3\n  Test: divisible by 17\n    If true: throw to monkey 0\n    If false: throw to monkey 1",
			{"10605", "2713310158"}
		}
	};
}
