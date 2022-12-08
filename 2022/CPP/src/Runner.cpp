// SPDX-FileCopyrightText: 2022 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "Session.hpp"
#include "Day01.hpp"
#include "Day02.hpp"
#include "Day03.hpp"
#include "Day04.hpp"
#include "Day05.hpp"
#include "Day06.hpp"
#include "Day07.hpp"

#include <iostream>
#include <stdexcept>
#include <filesystem>
#include <fstream>
#include <cpr/cpr.h>

constexpr unsigned final_solution = 7;

int main(int argc, char * argv[]) {
	using namespace std::chrono_literals;
	namespace fs = std::filesystem;

	if (argc < 2) {
		std::cout << "Not enough parameters. Expected day number as parameter 1" << std::endl;
		return 1;
	}

	unsigned day_n;
	try {
		day_n = static_cast<unsigned>(std::stoul(argv[1]));
	} catch (std::exception &) {
		std::cout << "Invalid day number \"" << argv[1] << "\", expected number 1-" << final_solution << std::endl;
		return 1;
	}

	if (day_n < 1 || day_n > final_solution) {
		std::cout << "Invalid day number \"" << argv[1] << "\", expected number 1-" << final_solution << std::endl;
		return 1;
	}

	// get input
	std::string input;
	{
		std::string filename = "inputs/day" + std::to_string(day_n) + ".txt";

		if (fs::exists(filename)) { // check for cache
			std::ifstream file("inputs/day" + std::to_string(day_n) + ".txt");
			if (!file) {
				std::cout << "Unable to open file " << filename << std::endl;
				return 1;
			}
			input = (std::stringstream() << file.rdbuf()).str();
			if (!file) {
				std::cout << "Unable to read from file " << filename << std::endl;
				return 1;
			}
		} else if ([&](){
			for (auto & entry : fs::directory_iterator("inputs/")) {
				if (fs::last_write_time(entry) + 15min > fs::file_time_type::clock::now()) {
					return true;
				}
			}
			return false;
		}()) {
			std::cout << "Input not cached. Unable to request: throttled to once per 15 mins" << std::endl;
			return 1;
		} else {
			std::cout << "Input not cached. Requesting... " << std::flush;
			fs::create_directory("inputs/");

			auto r = cpr::Get(
				cpr::Url{"https://adventofcode.com/2022/day/" + std::to_string(day_n) + "/input"},
				cpr::Cookies{{"session", get_session()}},
				cpr::Header{{"User-Agent", "https://github.com/metaquarx/AOC by metaquarx at protonmail dot com"}}
			);

			std::cout << "[" << r.elapsed << "] " << r.status_code << ": "
					  << (r.status_code < 400 ? "Success" : "Error:") << std::endl;
			if (r.status_code >= 400) {
				if (r.status_code == 0) {
					std::cout << r.error.message << std::endl;
				} else {
					std::cout << "Error making request" << std::endl;
				}
				return 1;
			}

			input = r.text;

			// cache
			std::ofstream file(filename);
			file << input;
		}
	}

	// run solution
	std::unique_ptr<Solution> sol;
	switch (day_n) {
		case 1: sol = std::make_unique<Day01>(); break;
		case 2: sol = std::make_unique<Day02>(); break;
		case 3: sol = std::make_unique<Day03>(); break;
		case 4: sol = std::make_unique<Day04>(); break;
		case 5: sol = std::make_unique<Day05>(); break;
		case 6: sol = std::make_unique<Day06>(); break;
		case 7: sol = std::make_unique<Day07>(); break;
		default: throw std::runtime_error("unreachable");
	}

	auto tests = sol->get_tests();
	for (std::size_t i = 0; i < tests.size(); i++) {
		auto ans = sol->solve(tests[i].input);

		auto a = ans.p1;
		auto e = tests[i].expected_output.p1;
		std::cout << "Test " << i + 1 << ".1: "
				  << (a == e ? "Pass" : "Fail (expected " + e + ", got " + a + ")")
				  << std::endl;

		auto a2 = ans.p2;
		auto e2 = tests[i].expected_output.p2;
		std::cout << "Test " << i + 1 << ".2: "
				  << (a2 == e2 ? "Pass" : "Fail (expected " + e2 + ", got " + a2 + ")")
				  << std::endl;

		if ((a != e && e != "") || (a2 != e2 && e2 != "")) {
			return 1;
		}
	}

	auto out = sol->solve(input);
	std::cout << "Part 1: " << out.p1 << std::endl;
	std::cout << "Part 2: " << out.p2 << std::endl;
}
