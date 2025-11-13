# Devl Puzzle Solver


A solver for the [Torched Products "Mind Bending Octagonal Fractal Puzzle"](https://torchedproducts.com/products/mind-bending-fractal-puzzle?_pos=1&_sid=a1b3a04fb&_ss=r).

## Program
The program uses a brute force method solver find a solution. Specifically, it uses a version of the DLX algorithm by Donald Knuth. Unfortunately, the program is too slow to directly solve the 200pc puzzle. However, it's able to generate the correct 100pc solution which is then used by the second part of the solver [here](https://github.com/fawfle/devil-puzzle-translator).
