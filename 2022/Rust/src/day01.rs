// SPDX-FileCopyrightText: 2022 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: GPL-3.0-only

use std::env;
use std::fs::File;
use std::error::Error;
use std::io::BufReader;
use std::io::BufRead;

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() < 2 {
        println!("Not enough arguments: expected input filename as argument 1");
        std::process::exit(1);
    }

    let file = match File::open(&args[1]) {
        Err(why) => panic!("Couldn't open input file {}: {}", args[1], <dyn Error>::to_string(&why)),
        Ok(file) => file
    };

    let reader = BufReader::new(file);
    let mut elves = Vec::with_capacity(1000);
    elves.push(0u32);
    for line in reader.lines() {
        if line.as_ref().unwrap().is_empty() {
            elves.push(0u32);
        } else {
            *elves.last_mut().unwrap() += line.unwrap().parse::<u32>().unwrap();
        }
    }
    elves.pop();

    elves.sort();
    println!("P1: {}", elves.last().unwrap());
    println!("P2: {}", &elves[elves.len() - 3 .. elves.len()].iter().fold(0, |a, b| a + b));
}
