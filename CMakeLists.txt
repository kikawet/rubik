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

################    RUBIK    ################
### This is the actual executable with the GUI
add_executable(${PROJECT_NAME} src/rubik.c)
target_link_libraries(${PROJECT_NAME} raylib cube draw)
add_dependencies(${PROJECT_NAME} libsolver)

################    SOLVER_PERF    ################
### Run a simulation without GUI
add_executable(solver_perf src/solver_perf.c)
target_link_libraries(solver_perf raylib cube solver)

################    SOLVER_TESTER    ################
### Brute force many solutions to check if solver works :P
add_executable(solver_tester src/solver_tester.c)
target_link_libraries(solver_tester raylib cube solver)

# ok so after implementing everything in parallel -fsanitize=thread just gives false positives confirmed using PTHREAD_MUTEX_ERRORCHECK
#set_target_properties(solver_tester PROPERTIES CMAKE_C_COMPILER clang)
#target_compile_options(solver_tester PRIVATE -fsanitize=undefined -fsanitize=thread)
#target_link_options(solver_tester PRIVATE -fsanitize=undefined -fsanitize=thread)


################    RAYLIB    ################
include_directories("raylib-5.5_linux_amd64/include")
add_library(raylib STATIC IMPORTED GLOBAL)
set_target_properties(raylib PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_LIST_DIR}/raylib-5.5_linux_amd64/lib/libraylib.a"
        INTERFACE_LINK_LIBRARIES m
)

################    SOURCES    ################
add_library(draw OBJECT src/draw.c)
add_library(cube OBJECT src/cube.c)

################    SOLVER    ################
### Generate a static solver for testing and a shared for hot reloading ###
add_library(solver OBJECT src/solver/$CACHE{SOLVER}.c)

add_library(libsolver SHARED src/solver/$CACHE{SOLVER}.c)
target_link_libraries(libsolver PRIVATE raylib cube)
set_target_properties(libsolver PROPERTIES OUTPUT_NAME "solver")

if ($CACHE{SOLVER} MATCHES ".*hkociemba.*")
    ### hkociemba only! will compile generated files and attach them into the solvers

    # You can uncomment next line to use generated headers directly but will take more time to compile or straight up f*ck your IDE
    # Note that if you enable this you need to remove "$<TARGET_OBJECTS:generated>" symbols or symbols will be duplicated wey
    #target_compile_definitions(solver PUBLIC NOT_A_PUSSY)
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

    # Inject myself into static and dynamic solver; I don't like doing it like this but is the best so far :/
    target_sources(solver INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/flipslicesorted_twist_depth3.o "$<TARGET_OBJECTS:generated>")
    add_dependencies(solver flipslicesorted_twist_depth3)

    target_sources(libsolver PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/flipslicesorted_twist_depth3.o "$<TARGET_OBJECTS:generated>")
    add_dependencies(libsolver flipslicesorted_twist_depth3)
endif ()