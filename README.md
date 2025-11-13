# Devl Puzzle Solver (200pc)


A solver for the [Torched Products "Mind Bending Octagonal Fractal Puzzle"](https://torchedproducts.com/products/mind-bending-fractal-puzzle?_pos=1&_sid=a1b3a04fb&_ss=r).

## Program
The program uses a brute force method solver find a solution. Specifically, it uses a version of the DLX algorithm by Donald Knuth. Unfortunately, the program is too slow to directly solve the 200pc puzzle. However, it's able to generate the correct 100pc solution which is then used by the second part of the solver [here](https://github.com/fawfle/devil-puzzle-translator). This program is the 200pc solver version.

## Planned Features (that will never happen)
Because the program uses DLX, it converts the puzzle to an exact cover matrix. Generating the matrix takes a significant portion of time which could be avoided by storing the solution. My solver is also incredibly memory (and likely by extension time) inneficient when generating the matrix.
