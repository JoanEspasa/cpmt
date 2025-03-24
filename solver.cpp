#include <z3++.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <iostream>
#include <vector>
#include <string>

namespace py = pybind11;

// Function that takes a list of SMT-LIB2 clauses and variable names
// Returns a dictionary with solution or error information
py::dict solve_from_string(const std::vector<std::string> &var_names,
                           const std::vector<std::string> &clause_strs)
{
    // Create a Z3 context with custom parameters
    z3::context ctx;
    z3::params p(ctx);
    p.set("model", true); // Enable model generation
    p.set("lemmas2console", true); // Enable model generation

    // Create a Z3 solver
    z3::solver solver(ctx);
    solver.set(p);

    py::dict result;
    result["satisfiable"] = true; // Default to true, will set to false if unsat

    z3::sort_vector sorts(ctx);
    sorts.push_back(ctx.int_sort());
    z3::func_decl_vector decls(ctx);

    // The vars
    std::vector<z3::expr> vars;
    try
    {
        // Create Z3 variables to use in our solver and store their declarations
        for (const auto &var_name : var_names)
        {
            z3::expr d = ctx.int_const(var_name.c_str());
            decls.push_back(d);
            vars.push_back(d);
        }

        // Parse each clause as a Z3 expression using the sexpr parser
        for (const std::string &clause_str : clause_strs) {
            z3::expr_vector clauses = ctx.parse_string(clause_str.c_str(),sorts, 0);
            std::cout << "from: " << clause_str << " parsed: " ;
            for (const auto  &clause : clauses) {
                std::cout << clause.to_string() << std::endl;
            }
            solver.add(clauses);
        }

        // Check if satisfiable
        if (solver.check() == z3::sat)
        {
            z3::model model = solver.get_model();

            // For each variable, get its value
            for (size_t i = 0; i < var_names.size(); i++)
            {
                z3::expr val = model.eval(vars[i], true);

                if (val.is_numeral())
                {
                    int int_val;
                    if (Z3_get_numeral_int(ctx, val, &int_val))
                    {
                        result[py::str(var_names[i])] = int_val;
                    }
                    else
                    {
                        // If it's a numeral that can't be converted to int, convert to string
                        result[py::str(var_names[i])] = py::str(Z3_ast_to_string(ctx, val));
                    }
                }
                else
                {
                    // Otherwise just convert to string
                    result[py::str(var_names[i])] = py::str(Z3_ast_to_string(ctx, val));
                }
            }
        }
        else
        {
            result["satisfiable"] = false;
        }
    }
    catch (const z3::exception &e)
    {
        // Handle Z3 errors
        std::cout << "Z3 exception: " << e.msg() << std::endl;
        result["error"] = py::str(e.msg());
        result["satisfiable"] = false;
    }

    return result;
}

// Module definition for pybind11
PYBIND11_MODULE(z3solver, m)
{
    m.doc() = "Python binding for Z3 example";
    m.def("solve_from_string", &solve_from_string,
          "Solve formula provided as string with variable names",
          py::arg("var_names"), py::arg("clause_strs"));
}