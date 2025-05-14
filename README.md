# Pseudocode
Cambridge (A Level) pseudocode to C++(20) transpiler.

# Usage
You are free to use anything provided, for non-commercial use. 

You may **not** sell:
- Source code
- Transpiled code
- This transpilation as a service
- Anything part of this repository, or produced by anything in this repository

# Note
  This product is provided AS IS, WITHOUT ANY WARRANTY for ANY PURPOSE.
  Use it at your own discretion.

# Requirements
- A standard C++ compiler with (complete) C++20 support is required
- The commands provided work ONLY for g++ (GNU C++ compiler). You will need different commands for other compilers

# Contributing
  You are wellcome to open a pull request, or open an issue, or ask for help in discussions.

# Benefits
- You can run pseudocode locally, for free
- Your pseudcode runs as fast as a C++ program, especially due to compiler optimisations

# Limitations:
- Using (x, y : REAL) etcetra in functions. Ever variable has to be marked explicitly. (BYREF and BYVAL works properly)
- Use of pointers is not supported yet
- The code produced is *not* meant to be human readable. It is meant for execution
- Arrays are declared without any colons. Just specify their dimensions
- Using DO after while, or using MOD as a function, and other O level formats will currently NOT work
- The error messages may be a bit cryptic, if you do not understand c++. Open the most recent log file in the logs folder, and you'll see slightly more friendly messages
- WRONG CODE CAN RUN. IF IT RUNS, DOESN'T MEAN IT IS FREE OF SYNTAX ERRORS!
-   Example: You can end a function with ENDTYPE, ENDWHILE, etcetra
