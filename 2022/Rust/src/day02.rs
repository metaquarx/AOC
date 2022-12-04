// SPDX-FileCopyrightText: 2022 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: GPL-3.0-only

extern crate num;
#[macro_use]
extern crate num_derive;
use strum::IntoEnumIterator;
use strum_macros::EnumIter;

use std::env;
use std::fs::File;
use std::io::BufReader;
use std::io::BufRead;

#[derive(PartialEq, Clone, Copy, FromPrimitive, EnumIter)]
enum State {
    Rock     = 0,
    Paper    = 1,
    Scissors = 2,
}

#[allow(non_upper_case_globals)]
impl State {
    const Lose: State = State::Rock;
    const Draw: State = State::Paper;
    const Win:  State = State::Scissors;

    fn from_i32(value: i32) -> State {
        match value {
            0 => State::Rock,
            1 => State::Paper,
            2 => State::Scissors,
            _ => panic!("Invalid input: {}", value)
        }
    }
}

fn status(lhs: State, rhs: State) -> State {
    match rhs as i32 - lhs as i32 {
        -2 | 1 => State::Win,
        0 => State::Draw,
        _ => State::Lose
    }
}

fn score(state: State, outcome: State) -> i32 {
    1i32 + (state as i32) + (outcome as i32) * 3
}

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() < 2 {
        println!("Not enough arguments: expected input filename as argument 1");
        std::process::exit(1);
    }
    let file = match File::open(&args[1]) {
        Err(why) => panic!("Couldn't open input file {}: {}", args[1], why),
        Ok(file) => file
    };
    let reader = BufReader::new(file);

    let mut p1: i32 = 0;
    let mut p2: i32 = 0;
    for line in reader.lines() {
        let other = State::from_i32((line.as_ref().expect("").chars().nth(0).unwrap() as i32) - ('A' as i32));
        let own   = State::from_i32((line.as_ref().expect("").chars().nth(2).unwrap() as i32) - ('X' as i32));

        p1 += score(own, status(other, own));

        for i in State::iter() {
            if status(other, i) == own {
                p2 += score(i, own);
            }
        }
    }

    println!("P1: {}", p1);
    println!("P2: {}", p2);
}
