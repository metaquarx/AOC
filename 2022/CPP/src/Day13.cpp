// SPDX-FileCopyrightText: 2022 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "Day13.hpp"

#include "Utils.hpp"
#include <algorithm>
#include <variant>

struct Packet {
	using PT = std::variant<int, std::vector<Packet>>;
	PT value;

	Packet(const std::string & input)
	: value(input[0] != '[' ? PT{std::stoi(input)} : [&](){
		std::vector<Packet> out;

		std::string temp;
		auto looped = input.substr(1, input.size() - 2);
		for (std::size_t depth = 0, i = 0; i < looped.size(); i++) {
			depth += looped[i] == '[' ? 1 : 0;
			depth -= looped[i] == ']' ? 1 : 0;

			if (looped[i] == ',' && depth == 0) {
				out.emplace_back(temp);
				temp.clear();
			} else {
				temp.push_back(looped[i]);
				if (i + 1 == looped.size()) {
					out.emplace_back(temp);
				}
			}
		}

		return out;
	}()) {}

	Packet(int i) : Packet("[" + std::to_string(i) + "]") {}

	int compare(const Packet & o) const {
		if (std::holds_alternative<std::vector<Packet>>(value)) {
			if (std::holds_alternative<std::vector<Packet>>(o.value)) {
				auto & left = std::get<std::vector<Packet>>(value);
				auto & right = std::get<std::vector<Packet>>(o.value);

				for (std::size_t i = 0; i < left.size() && i < right.size(); i++) {
					if (auto result = left[i].compare(right[i])) {
						return result;
					}
				}

				return (left.size() < right.size()) * -1 + (left.size() > right.size());
			}

			return compare({std::get<int>(o.value)});
		}

		if (std::holds_alternative<std::vector<Packet>>(o.value)) {
			return Packet{std::get<int>(value)}.compare(o);
		}

		auto & left = std::get<int>(value);
		auto & right = std::get<int>(o.value);

		return (left < right) * -1 + (left > right);
	}

	bool operator==(const Packet & o) const {
		return value == o.value;
	}
};

Solution::Answer Day13::solve(std::string input) const {
	auto in = Utils::split(input, "\n\n");

	unsigned p1{};
	std::vector<Packet> p2;
	auto insert = [&](Packet && p) {
		auto ub = std::upper_bound(p2.begin(), p2.end(), p, [](const Packet & l, const Packet & r) {
			return l.compare(r) < 0;
		});
		return 1 + p2.insert(ub, p) - p2.begin();
	};

	for (std::size_t i = 0; i < in.size(); i++) {
		auto pairs = Utils::split(in[i], "\n");
		auto left = Packet{pairs[0]};
		auto right = Packet{pairs[1]};

		p1 += left.compare(right) < 0 ? i + 1 : 0;

		insert(std::move(left));
		insert(std::move(right));
	}

	return {std::to_string(p1),
			std::to_string(insert(Packet{"[[2]]"}) *
						   insert(Packet{"[[6]]"}))};
}

std::vector<Solution::Test> Day13::get_tests() const {
	return {
		{
			"[1,1,3,1,1]\n[1,1,5,1,1]\n\n[[1],[2,3,4]]\n[[1],4]\n\n[9]\n[[8,7,6]]\n\n[[4,4],4,4]\n[[4,4],4,4,4]\n\n[7,7,7,7]\n[7,7,7]\n\n[]\n[3]\n\n[[[]]]\n[[]]\n\n[1,[2,[3,[4,[5,6,7]]]],8,9]\n[1,[2,[3,[4,[5,6,0]]]],8,9]",
			{"13", "140"}
		}
	};
}
