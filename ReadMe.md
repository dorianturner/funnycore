# Computer Architecture CW FunnyCore Compiler

The spec expects us to get our assembly program right without giving us any
way to test our code.

Screw that.

# Usage instructions

First compile the program with `g++ main.cpp -o interpreter`

Create your test program and save it in a file e.g. prog.txt (There is an example there already)

Run with `./interpreter <file> <optional register initial values>`

To run the example file would be `./test prog.txt`

In your optional register values they are just separated by commas e.g. `10,2` with no spaces

Enjoy.

# Testing your solution

Compile the testing file division_tests with `g++ division_tests.cpp -o tests`

Run tests similarly to above with `./tests <file>`

Should see an output similar to this if you did it right

```
PASS: 12,8 -> Quotient: 1, Remainder: 4
Tested 40 valid cases.
Average cycles: 1041.25
```

