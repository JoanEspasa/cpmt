import os
import sys

# Add the build directory to Python's module search path
build_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'build')
sys.path.insert(0, build_dir)

import z3solver

def main():
    # Call the C++ function that uses Z3
    result = z3solver.solve_greater_than_three()
    
    if result != -1:
        print(f"Solution found: x = {result}")
    else:
        print("No solution found")

if __name__ == "__main__":
    main()