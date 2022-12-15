// SPDX-FileCopyrightText: 2022 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "Day15.hpp"

#include "Utils.hpp"
#include <cassert>
#include <limits>
#include <optional>
#include <unordered_map>
#include <unordered_set>

namespace {

using Utils::Coord;
using Sensors = std::unordered_map<Coord, Coord, Coord::Hash>;

int count_full(int row, const Sensors & sensors) {
	int minx = std::numeric_limits<int>::max();
	int maxx = std::numeric_limits<int>::min();

	for (auto & [sensor, beacon] : sensors) {
		auto distance = sensor.manhattan(beacon);
		auto dy = std::abs(sensor.y - row);
		if (dy < distance) {
			minx = std::min(minx, sensor.x - (distance - dy));
			maxx = std::max(maxx, sensor.x + (distance - dy));
		}
	}

	std::unordered_set<Coord, Coord::Hash> filled;
	for (auto & [sensor, beacon] : sensors) {
		if (beacon.y == row && beacon.x >= minx && beacon.x <= maxx) {
			if (filled.find(beacon) != filled.end())
				continue;

			filled.emplace(beacon);
		}
	}

	return maxx - minx - static_cast<int>(filled.size()) + 1;
}

Coord intersection(int ka, int a, int kb, int b, int px, int py) {
	auto x = (b - a) / (ka - kb);
	auto y = ka * x + a;
	return {x + px, y + py};
}

std::optional<unsigned long> check(int max_pos, const Sensors & sensors, Coord c) {
	if (c.x < 0 || c.x > max_pos || c.y < 0 || c.y > max_pos) {
		return std::nullopt;
	}

	for (auto & [sensor, beacon] : sensors) {
		if (sensor.manhattan(c) <= sensor.manhattan(beacon)) {
			return std::nullopt;
		}
	}

	return static_cast<unsigned long>(c.x) * 4'000'000ul + static_cast<unsigned>(c.y);
}

std::optional<unsigned long> operator||(
	const std::optional<unsigned long> left,
	const std::optional<unsigned long> right) {
	return left ? left : (right ? right : std::nullopt);
}

unsigned long find_empty(int max_pos, const Sensors & sensors) {
	for (unsigned i = 0; i < sensors.size(); i++) {
		for (unsigned j = i + 1; j < sensors.size(); j++) {
			auto & [a, ba] = *std::next(sensors.begin(), i);
			auto & [b, bb] = *std::next(sensors.begin(), j);

			auto da = a.manhattan(ba);
			auto db = b.manhattan(bb);

			if (a.manhattan(b) - 1 > da + db)
				continue;

			if (auto result =
				check(max_pos, sensors,
					intersection( 1, -(a.x - da) + a.y, -1,  (b.x - db) + b.y, -1,  0)) ||
				check(max_pos, sensors,
					intersection( 1, -(a.x - da) + a.y, -1,  (b.x + db) + b.y,  0,  1)) ||
				check(max_pos, sensors,
					intersection( 1, -(a.x + da) + a.y, -1,  (b.x - db) + b.y,  0, -1)) ||
				check(max_pos, sensors,
					intersection( 1, -(a.x + da) + a.y, -1,  (b.x + db) + b.y,  1,  0)) ||
				check(max_pos, sensors,
					intersection(-1,  (a.x + da) + a.y,  1, -(b.x + db) + b.y,  1,  0)) ||
				check(max_pos, sensors,
					intersection(-1,  (a.x + da) + a.y,  1, -(b.x - db) + b.y,  0,  1)) ||
				check(max_pos, sensors,
					intersection(-1,  (a.x - da) + a.y,  1, -(b.x + db) + b.y,  0, -1)) ||
				check(max_pos, sensors,
					intersection(-1,  (a.x - da) + a.y,  1, -(b.x - db) + b.y, -1,  0))) {
				return *result;
			}
		}
	}

	return 0;
}

}

Solution::Answer Day15::solve(std::string input) const {
	Sensors sensors;
	for (auto & line : Utils::split(input, "\n")) {
		Coord s, b;
		auto c = std::sscanf(
			line.c_str(), "Sensor at x=%i, y=%i: closest beacon is at x=%i, y=%i",
			&s.x, &s.y, &b.x, &b.y);
		assert(c == 4);
		sensors[s] = b;
	}

	bool example = input.find("Sensor at x=2, y=18") == 0;

	return {std::to_string(count_full(example ? 10 : 2'000'000, sensors)),
			std::to_string(find_empty(example ? 20 : 4'000'000, sensors))};
}

std::vector<Solution::Test> Day15::get_tests() const {
	return {
		{
			"Sensor at x=2, y=18: closest beacon is at x=-2, y=15\nSensor at x=9, y=16: closest beacon is at x=10, y=16\nSensor at x=13, y=2: closest beacon is at x=15, y=3\nSensor at x=12, y=14: closest beacon is at x=10, y=16\nSensor at x=10, y=20: closest beacon is at x=10, y=16\nSensor at x=14, y=17: closest beacon is at x=10, y=16\nSensor at x=8, y=7: closest beacon is at x=2, y=10\nSensor at x=2, y=0: closest beacon is at x=2, y=10\nSensor at x=0, y=11: closest beacon is at x=2, y=10\nSensor at x=20, y=14: closest beacon is at x=25, y=17\nSensor at x=17, y=20: closest beacon is at x=21, y=22\nSensor at x=16, y=7: closest beacon is at x=15, y=3\nSensor at x=14, y=3: closest beacon is at x=15, y=3\nSensor at x=20, y=1: closest beacon is at x=15, y=3",
			{"26", "56000011"}
		}
	};
}
