// SPDX-FileCopyrightText: 2022 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "Day16.hpp"

#include "Utils.hpp"
#include <map>
#include <optional>
#include <algorithm>

namespace {

struct ValveID {
	char a, b;

	bool operator==(const ValveID & v) const {
		return v.a == a && v.b == b;
	}

	bool operator<(const ValveID & v) const {
		return a < v.a || (a == v.a && b < v.b);
	}

	struct Hash { std::size_t operator()(const ValveID & v) const {
		auto xh = std::hash<char>()(v.a);
		return std::hash<char>()(v.b) + 0x9e3779b9 + (xh << 6) + (xh >> 2);
	}};
};

struct Valve {
	int flowrate;
	std::vector<ValveID> connections;

	bool operator==(const Valve & v) const {
		return flowrate == v.flowrate;
	}
};

struct Volcanium {
	using VT = std::vector<ValveID>;

	std::map<ValveID, Valve> valves;
	std::map<ValveID, std::map<ValveID, int>> distances;
	std::vector<ValveID> nz;
	std::map<VT, std::map<ValveID, int>> cache;


	Volcanium(const std::vector<std::string> & inputs) {
		for (auto & valve : inputs) {
			auto parts = Utils::split(valve.c_str(), "; ");

			ValveID current{parts[0][6], parts[0][7]};
			Valve temp;
			temp.flowrate = std::stoi(parts[0].substr(23).c_str());

			for (auto connection : Utils::split(parts[1].substr(22 + (parts[1][6] == 's')), ", ")) {
				temp.connections.push_back({connection[0], connection[1]});
			}

			valves[current] = temp;
			if (temp.flowrate > 0) {
				nz.push_back(current);
			}
		}
	}
	const ValveID AA{'A', 'A'};

	std::map<ValveID, int> construct_paths(ValveID start) {
		std::map<ValveID, int> dists;
		std::vector<ValveID> queue; queue.push_back(start);

		while (queue.size()) {
			auto valve = queue.back(); queue.pop_back();
			auto d = dists[valve];
			for (auto & v : valves[valve].connections) {
				if (dists.count(v) == 0 || d + 1 < dists[v]) {
					dists[v] = d + 1;
					queue.push_back(v);
				}
			}
		}

		for (auto it = dists.begin(); it != dists.end(); ) {
			if (std::find(nz.begin(), nz.end(), it->first) != nz.end() && it->second != 0) {
				it->second++;
				it++;
			} else {
				it = dists.erase(it);
			}
		}

		return dists;
	}

	int path_value(const VT & visited, int total_time) {
		int value{};
		auto time = total_time;

		for (std::size_t i = 0; i < visited.size() - 1; i++) {
			time -= distances[visited[i]][visited[i + 1]];
			value += time * valves[visited[i + 1]].flowrate;
		}
		return value;
	}

	void store(VT visited, ValveID current, int value) {
		std::sort(visited.begin(), visited.end());
		cache[visited][current] = value;
	}

	std::optional<int> retrieve(VT visited, ValveID current) {
		std::sort(visited.begin(), visited.end());
		if (cache.count(visited) && cache[visited].count(current)) {
			return cache[visited][current];
		}
		return std::nullopt;
	}

	std::optional<int> check_cache(ValveID current, const VT & visited, int value, int total_time) {
		auto optimal = retrieve(visited, current);
		if (!optimal || optimal.value() < value) {
			store(visited, current, value);
			return std::nullopt;
		}
		return optimal;
	}

	int explore(ValveID current, const VT & visited, int value, int remaining_time, int total_time) {
		auto cache_result = check_cache(current, visited, value, total_time);
		if (cache_result) {
			return cache_result.value();
		}

		std::vector<ValveID> options;
		for (auto & [vid, valve] : distances[current]) {
			if (std::find(visited.begin(), visited.end(), vid) == visited.end() && remaining_time - distances[current][vid] >= 0) {
				options.push_back(vid);
			}
		}

		std::vector<int> subpath_values;
		subpath_values.push_back(value);
		for (auto & valve : options) {
			auto remaining = remaining_time - distances[current][valve];
			auto ve = visited;
			ve.push_back(valve);
			auto subpath = explore(valve, ve, path_value(ve, total_time), remaining, total_time);
			subpath_values.push_back(subpath);
		}
		return *std::max_element(subpath_values.begin(), subpath_values.end());
	}

	int most_pressure() {
		for (auto & vid : nz) {
			distances[vid] = construct_paths(vid);
		}
		distances[AA] = construct_paths(AA);

		return explore(AA, {AA}, 0, 30, 30);
	}

	std::optional<VT> best_pair_for(const VT & path, const std::map<VT, int> & optimal_path_values) {
		std::map<VT, int> options;
		for (auto & [k, v] : optimal_path_values) {
			if (!std::count_if(k.begin(), k.end(), [&](const auto & e) {
				return std::find(path.begin(), path.end(), e) != path.end();
			})) {
				options[k] = v;
			}
		}

		if (options.empty()) {
			return std::nullopt;
		}

		return std::max_element(options.begin(), options.end(), [](const auto & l, const auto & r) {
			return l.second < r.second;
		})->first;
	}

	int with_elephant() {
		cache.clear();
		explore(AA, {AA}, 0, 26, 26);

		std::map<VT, int> optimal_path_values;
		for (auto & [vec, um] : cache) {
			for (auto & [vid, value] : um) {
				auto key = vec;
				key.erase(key.begin());

				if ((optimal_path_values.count(key) == 0 || value > optimal_path_values[key]) && value != 0) {
					optimal_path_values[std::move(key)] = value;
				}
			}
		}

		int max{};
		for (auto & [path, val] : optimal_path_values) {
			if (auto best = best_pair_for(path, optimal_path_values)) {
				max = std::max(max, optimal_path_values[path] + optimal_path_values[*best]);
			}
		}
		return max;
	}
};

}

Solution::Answer Day16::solve(std::string input) const {
	Volcanium v{Utils::split(input, "\n")};

	return {std::to_string(v.most_pressure()),
			std::to_string(v.with_elephant())};
}

std::vector<Solution::Test> Day16::get_tests() const {
	return {
		{
			"Valve AA has flow rate=0; tunnels lead to valves DD, II, BB\nValve BB has flow rate=13; tunnels lead to valves CC, AA\nValve CC has flow rate=2; tunnels lead to valves DD, BB\nValve DD has flow rate=20; tunnels lead to valves CC, AA, EE\nValve EE has flow rate=3; tunnels lead to valves FF, DD\nValve FF has flow rate=0; tunnels lead to valves EE, GG\nValve GG has flow rate=0; tunnels lead to valves FF, HH\nValve HH has flow rate=22; tunnel leads to valve GG\nValve II has flow rate=0; tunnels lead to valves AA, JJ\nValve JJ has flow rate=21; tunnel leads to valve II",
			{"1651", "1707"}
		}
	};
}
