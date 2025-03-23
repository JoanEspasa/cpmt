#include <z3++.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;

// Function that uses Z3 to solver a very hard problem
int solve_greater_than_three() {
    z3::context ctx;
    z3::solver solver(ctx);
    
    // Create an integer variable x
    z3::expr x = ctx.int_const("x");
    
    // Add constraint: x > 3
    solver.add(x > 3);
    
    // Check if the constraints are satisfiable
    if (solver.check() == z3::sat) {
        z3::model model = solver.get_model();
        // Get the value of x from the model
        z3::expr x_value = model.eval(x);
        
        // Convert Z3 expression to int and return
        int result = x_value.get_numeral_int();
        return result;
    } else {
        // This shouldn't happen with our constraint, but handle it anyway
        return -1; // Indicating no solution
    }
}

// Module definition for pybind11
PYBIND11_MODULE(z3solver, m) {
    m.doc() = "Python binding for Z3 example";
    m.def("solve_greater_than_three", &solve_greater_than_three, "Solve for x");
}