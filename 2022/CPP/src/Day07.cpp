// SPDX-FileCopyrightText: 2022 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "Day07.hpp"

#include "Utils.hpp"
#include <algorithm>
#include <numeric>
#include <stdexcept>

namespace {

struct File {
	std::string name;
	std::size_t size;
};

struct Directory {
	std::string name = "/";
	Directory * back = nullptr;
	std::vector<Directory> subdirs;
	std::vector<File> files;

	std::size_t total_size() const {
		return std::accumulate(subdirs.begin(), subdirs.end(), 0u,
					[](std::size_t l, Directory r) { return l + r.total_size();}) +
			   std::accumulate(files.begin(), files.end(), 0u,
			   		[](std::size_t l, File r) { return l + r.size; });
	}

	std::size_t sum_sub_100k() const {
		std::size_t total = total_size();
		return (total <= 100'000 ? total : 0) +
			   std::accumulate(subdirs.begin(), subdirs.end(), 0u,
			   		[](std::size_t l, Directory r) { return l + r.sum_sub_100k();});
	}

	std::size_t find_closest(std::size_t target) const {
		std::size_t total = total_size();
		for (auto & dir : subdirs) {
			auto recurse = dir.find_closest(target);
			if (recurse > target && recurse < total) {
				total = recurse;
			}
		}
		return total;
	}
};

}

Solution::Answer Day07::solve(std::string input) const {
	Directory top_level;
	Directory * current = &top_level;
	for (auto & line : Utils::split(input, "\n")) {
		auto parts = Utils::split(line, " ");
		if (parts[0] == "$") {
			if (parts[1] == "cd") {
				if (parts[2] == "/") { current = &top_level;
				} else if (parts[2] == "..") { current = current->back;
				} else {
					current = &*std::find_if(current->subdirs.begin(), current->subdirs.end(),
						[&](const Directory &d) { return d.name == parts[2]; });
				}
			} else if (parts[1] == "ls") {
			}
		} else {
			if (parts[0] == "dir") {
				current->subdirs.push_back({parts[1], current, {}, {}});
			} else {
				current->files.push_back({parts[1], std::stoul(parts[0])});
			}
		}
	}

	std::size_t free = 70'000'000 - top_level.total_size();
	return {std::to_string(top_level.sum_sub_100k()),
			std::to_string(top_level.find_closest(30'000'000 - free))};
}

std::vector<Solution::Test> Day07::get_tests() const {
	return {
		{
			"$ cd /\n$ ls\ndir a\n14848514 b.txt\n8504156 c.dat\ndir d\n$ cd a\n$ ls\ndir e\n29116 f\n2557 g\n62596 h.lst\n$ cd e\n$ ls\n584 i\n$ cd ..\n$ cd ..\n$ cd d\n$ ls\n4060174 j\n8033020 d.log\n5626152 d.ext\n7214296 k",
			{"95437", "24933642"}
		}
	};
}
