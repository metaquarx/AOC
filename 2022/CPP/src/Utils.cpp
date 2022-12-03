// SPDX-FileCopyrightText: 2022 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "Utils.hpp"

namespace Utils {

std::vector<std::string> split(const std::string & input, const std::string & delimiter) {
	std::vector<std::string> out;

	std::size_t start{};
	std::size_t end = input.find(delimiter);
	while (end != std::string::npos) {
		out.push_back(input.substr(start, end - start));
		start = end + delimiter.size();
		end = input.find(delimiter, start);
	}
	out.push_back(input.substr(start, end));

	if (out.size() && out.back().size() == 0) {
		out.pop_back();
	}

	return out;
}

}
