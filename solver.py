import os
import sys
import z3

# Add the build directory to Python's module search path
build_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'build')
sys.path.insert(0, build_dir)

# Import the C++ module
import z3solver

def solve_with_z3():
    """
    Create a formula using Z3 Python API, pass it to C++ and solve
    """
    # Create Z3 variable
    x = z3.Int('x')
    
    # Create formula
    formula = x >= 3
    
    # Get variable names
    var_names = ['x']
    
    # Convert formula to string (using the SMT-LIB2 format)
    # Z3 Python API offers to_smt2_string() for SMT-LIB2 format
    # or sexpr() for S-expression (Z3's internal format)
    clause_str = formula.sexpr()
    
    # Call the C++ solver
    result = z3solver.solve_from_string(var_names, [clause_str])
    
    # Process the result
    if result.get("satisfiable", False):
        print(f"Solution found: x = {result['x']}")
    else:
        if "error" in result:
            print(f"Error: {result['error']}")
        else:
            print("No solution found")
    
    return result

def solve_complex_example():
    """
    Create a more complex formula with multiple clauses
    """
    # Create Z3 variables
    x = z3.Int('x')
    y = z3.Int('y')
    
    # Create formula with multiple constraints
    clause1 = x > 3
    clause2 = y > x
    clause3 = y < 10
    
    # Get variable names
    var_names = ['x', 'y']
    
    # Convert clauses to string format. We need to do it this way
    # as smtlib2 requires us the full file
    clause_strs = [
        clause1.sexpr(),
        clause2.sexpr(),
        clause3.sexpr()
    ]
    
    # Call the C++ solver
    result = z3solver.solve_from_string(var_names, clause_strs)
    
    # Process the result
    if result.get("satisfiable", False):
        print(f"Complex solution found: x = {result['x']}, y = {result['y']}")
    else:
        if "error" in result:
            print(f"Error in complex example: {result['error']}")
        else:
            print("No solution found for complex example")
    
    return result

def main():
    # Run the simple example
    print("Running simple example (x > 3):")
    solve_with_z3()
    
    # Run the complex example
    print("\nRunning complex example (x > 3, y > x, y < 10):")
    solve_complex_example()

if __name__ == "__main__":
    main()