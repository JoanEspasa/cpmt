# Python + C++ + Z3 Example

This is a toy project demonstrating how to call C++ code that uses the Z3 SMT solver from Python using pybind11.

## Requirements

- CMake (3.10 or newer)
- Python (ofc)
- pybind11
- The Z3 SMT solver Python package

## Structure

- `solver.cpp`: C++ code that creates a Z3 solver with a trivial constraint
- `solver.py`: Python code that calls the C++ function
- `CMakeLists.txt`: CMake build configuration

## Installation
Please use a virtual environment

1. Install the Z3 SMT solver: `pip install z3-solver`
2. Install pybind11: `pip install pybind11`
3. Build the extension:
```
mkdir build
cd build
cmake ..
make
```

## Running the Example

After building, you can run the example:

```
python test.py
```

This will call the C++ function that uses Z3 to solve for x > 3 and print the result.

## How It Works

1. The C++ code (`solver.cpp`) defines a function that:
   - Creates a Z3 solver
   - Adds a constraint (x > 3)
   - Solves the constraint
   - Returns the solution to Python

2. pybind11 is used to expose the C++ function to Python

3. CMake handles the building and linking of the extension module

4. The Python script imports the built module and calls the function