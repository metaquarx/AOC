## 2022 C++ solutions

### Build

Find out your session token (eg by looking at the active cookies when visiting adventofcode.com)

`cmake -S . -B build -DAOC_SESSION_TOKEN=token`
`cmake --build build -j 4`

If your computer has more than 4 cores, I suggest changing the number above to something higher.

### Run

`./build/src/AOC day_number`
