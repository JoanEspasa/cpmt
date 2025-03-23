cmake_minimum_required(VERSION 3.10) # Set the minimum required version of CMake
project(cpp_z3_solver)               # Set the project name
set(CMAKE_CXX_STANDARD 14)           # Set the C++ standard to C++14

# Find Python and components. Note that Interpreter is required for pybind11
# and the Development component is also required in case we end up using pybind11 lower level API
find_package(Python 3 COMPONENTS Interpreter Development REQUIRED)

# -----------------------------------------------------------------------------------------
# Find pybind11
# -----------------------------------------------------------------------------------------
execute_process(
    COMMAND "${Python_EXECUTABLE}" -c "import pybind11; print(pybind11.get_cmake_dir())"
    OUTPUT_VARIABLE PYBIND11_CMAKE_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
    RESULT_VARIABLE PYBIND11_RESULT
)

if(NOT "${PYBIND11_RESULT}" STREQUAL "0")
    message(FATAL_ERROR "pybind11 not found. Please install with 'pip install pybind11'")
endif()

# When looking for packages, also check in this directory where pybind11's CMake files are located
list(APPEND CMAKE_PREFIX_PATH "${PYBIND11_CMAKE_DIR}")

# Now look for pybind11 and "import" it. CONFIG is required to find the custom
# pybind11Config.cmake file that comes with pybind11
# and REQUIRED is used to make sure the package is found, otherwise CMake will throw an error
find_package(pybind11 CONFIG REQUIRED)

# -----------------------------------------------------------------------------------------
# Find Z3 from the Python z3-solver package
# We use the Python z3-solver package to find the Z3 include and library directories
# We want to do so to make sure the Z3 Python API used and the one linked to are exactly the same
# -----------------------------------------------------------------------------------------
execute_process(
    COMMAND "${Python_EXECUTABLE}" -c "
import os
import z3
# Get the Z3 Python package directory
z3_dir = os.path.dirname(z3.__file__)
# Print the Z3 include directory
print(os.path.join(z3_dir, 'include'), end='')
"
    OUTPUT_VARIABLE Z3_INCLUDE_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
    RESULT_VARIABLE Z3_PYTHON_RESULT
)

execute_process(
    COMMAND "${Python_EXECUTABLE}" -c "
import os
import z3
# Get the Z3 Python package directory
z3_dir = os.path.dirname(z3.__file__)
# Print the Z3 lib directory
print(os.path.join(z3_dir, 'lib'), end='')
"
    OUTPUT_VARIABLE Z3_LIB_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
)

if(NOT "${Z3_PYTHON_RESULT}" STREQUAL "0")
    message(FATAL_ERROR "z3-solver Python package not found. Please install with 'pip install z3-solver'")
endif()

message(STATUS "Z3 include directory: ${Z3_INCLUDE_DIR}")
message(STATUS "Z3 library directory: ${Z3_LIB_DIR}")

# Create the Python module from the solver.cpp source file
# note that this is a pybind11 specific function, and not a standard CMake function
# The first argument is the name of the module, and the second is the source file
# the source file will be compiled and linked to create the shared library
pybind11_add_module(z3solver solver.cpp)        

# Find the Z3 library in the Python package and set the library name based on the OS
# Note we do not support Windows in the project.
if(APPLE)
    set(Z3_LIBRARY_NAME "${Z3_LIB_DIR}/libz3.dylib")
else()
    set(Z3_LIBRARY_NAME "${Z3_LIB_DIR}/libz3.so")
endif()

# Link with Z3:
# when you compile the z3solver module, look in this directory for any header files it needs.
# The PRIVATE keyword means that the include directory is only used when compiling the z3solver module
target_include_directories(z3solver PRIVATE ${Z3_INCLUDE_DIR})
# when creating the final module, include the Z3 library code so the z3solver module can call Z3's functions
target_link_libraries(z3solver PRIVATE ${Z3_LIBRARY_NAME})

# Set output directory
set_target_properties(z3solver PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build"
)