cmake_minimum_required(VERSION 4.1)
project(rubik DESCRIPTION "GUI with 3D rubik cube simulation" LANGUAGES C)

#set(CMAKE_C_COMPILER tcc) # If you enable this clion dies
set(CMAKE_C_STANDARD 99)
add_compile_options(-O2 -Wall -Wextra -Wpedantic)
# The other way to set cache variable doesn't work :<
set(SOLVER "solver_hkociemba_optimal" CACHE FILEPATH "There are multiple solvers implemented src/solver, the default may take up to 7h")
add_compile_definitions("CMAKE_SOURCE_DIR=\"${CMAKE_SOURCE_DIR}/\"")

message("CMAKE_C_FLAGS are '${CMAKE_C_FLAGS}'")
message("SOLVER is $CACHE{SOLVER}")

################    RAYLIB    ################
include_directories("raylib-5.5_linux_amd64/include")
add_library(raylib STATIC IMPORTED GLOBAL)
set_target_properties(raylib PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_LIST_DIR}/raylib-5.5_linux_amd64/lib/libraylib.a"
        INTERFACE_LINK_LIBRARIES m
)

################    GENERATED    ################
add_custom_command(
        OUTPUT flipslicesorted_twist_depth3.o
        COMMAND ld -r -b binary -o ${CMAKE_CURRENT_BINARY_DIR}/flipslicesorted_twist_depth3.o generated/flipslicesorted_twist_depth3.bin
        COMMAND objcopy --redefine-sym _binary_generated_flipslicesorted_twist_depth3_bin_start=flipslicesorted_twist_depth3_start
        --redefine-sym _binary_generated_flipslicesorted_twist_depth3_bin_end=flipslicesorted_twist_depth3_end
        --redefine-sym _binary_generated_flipslicesorted_twist_depth3_bin_size=flipslicesorted_twist_depth3_size
        ${CMAKE_CURRENT_BINARY_DIR}/flipslicesorted_twist_depth3.o
        MAIN_DEPENDENCY generated/flipslicesorted_twist_depth3.bin
        # We need to set the CWD because otherwise the symbols in the .o will have the full path
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        CODEGEN
)
# This is a trick to prevent cmake from building .o multiple times we will need to call add_dependencies later
add_custom_target(flipslicesorted_twist_depth3 DEPENDS flipslicesorted_twist_depth3.o)
file(GLOB generated_c CONFIGURE_DEPENDS "generated/*.c")
add_library(generated OBJECT ${generated_c})

################    SOURCES    ################
add_library(draw OBJECT src/draw.c)
add_library(model OBJECT src/model.c)

################    SOLVER    ################
### Generate a static solver for testing and a shared for hot reloading ###
add_library(solver OBJECT src/solver/$CACHE{SOLVER}.c)
# This will use generated headers directly but will take more time to compile or straight up f*ck your IDE
# Note that if you enable this you need to remove "$<TARGET_OBJECTS:generated>" symbols or symbols will be duplicated wey
#target_compile_definitions(solver PUBLIC NOT_A_PUSSY)
target_sources(solver INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/flipslicesorted_twist_depth3.o "$<TARGET_OBJECTS:generated>")
add_dependencies(solver flipslicesorted_twist_depth3)

add_library(libsolver SHARED src/solver/$CACHE{SOLVER}.c)
target_sources(libsolver PRIVATE flipslicesorted_twist_depth3.o "$<TARGET_OBJECTS:generated>")
target_link_libraries(libsolver PRIVATE raylib model)
set_target_properties(libsolver PROPERTIES OUTPUT_NAME "solver")
add_dependencies(libsolver flipslicesorted_twist_depth3)

################    RUBIK    ################
### This is the actual executable with the GUI
add_executable(${PROJECT_NAME} src/rubik.c)
target_link_libraries(${PROJECT_NAME} raylib model draw)
add_dependencies(${PROJECT_NAME} libsolver)

################    SOLVER_PERF    ################
### Run a simulation without GUI
add_executable(solver_perf src/solver_perf.c)
target_link_libraries(solver_perf raylib model solver)

################    SOLVER_TESTER    ################
### Brute force many solutions to check if solver works :P
add_executable(solver_tester src/solver_tester.c)
target_link_libraries(solver_tester raylib model solver)
set_target_properties(solver_tester PROPERTIES CMAKE_C_COMPILER clang) # Pretty sure this doesn't work but can't test it :v
target_compile_options(solver_tester PRIVATE -fsanitize=address -fsanitize=undefined)
target_link_options(solver_tester PRIVATE -fsanitize=address -fsanitize=undefined)
