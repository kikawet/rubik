cmake_minimum_required(VERSION 4.1)
#set(CMAKE_C_COMPILER tcc) # If you enable this clion dies
set(CMAKE_C_STANDARD 99)
add_compile_options(-Wall -Wextra -Wpedantic)
# The other way to set cache variable doesn't work :<
set(SOLVER "hkociemba_optimal_solver_parallel" CACHE FILEPATH "helps")

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

################    STATIC SOLVER    ################
add_library(solver OBJECT src/solver/$CACHE{SOLVER}.c)
#target_compile_definitions(solver PUBLIC NOT_A_PUSSY)
target_sources(solver INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/flipslicesorted_twist_depth3.o "$<TARGET_OBJECTS:generated>")
add_dependencies(solver flipslicesorted_twist_depth3)

################    DYNAMIC SOLVER    ################
project(LIBSOLVER DESCRIPTION "Dynamic solver library for hotreload")
add_library(${PROJECT_NAME} SHARED src/solver/$CACHE{SOLVER}.c)
target_sources(${PROJECT_NAME} PRIVATE flipslicesorted_twist_depth3.o "$<TARGET_OBJECTS:generated>")
target_link_libraries(${PROJECT_NAME} raylib model)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "solver")
add_dependencies(${PROJECT_NAME} flipslicesorted_twist_depth3)

################    RUBIK    ################
project(rubik DESCRIPTION "GUI with 3D rubik cube simulation")
add_executable(${PROJECT_NAME} src/rubik.c)
target_link_libraries(${PROJECT_NAME} raylib model draw)
add_dependencies(${PROJECT_NAME} LIBSOLVER)

################    SOLVER_PERF    ################
project(solver_perf DESCRIPTION "Run a simulation without GUI")
add_executable(${PROJECT_NAME} src/solver_perf.c)
target_link_libraries(${PROJECT_NAME} raylib model solver)
