import os
import sys
import z3

# Add the build directory to Python's module search path
build_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'build')
sys.path.insert(0, build_dir)
z3_dir = os.path.dirname(z3.__file__) # the directory where the Z3 Python module is located
sys.path.insert(0, os.path.join(z3_dir, 'lib'))

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