CC=tcc
CFLAGS=-std=c99 -g -ggdb -Wall -Wextra -Wpedantic -I./raylib-5.5_linux_amd64/include

SOLVER=hkociemba_optimal_solver.c flipslicesorted_twist_depth3.o

# FIXME: right now the program links with libraylib.so because I want solver and program to share the same raylib instance.
# The goal is to statically link solver + raylib && fix rubik.c to not load anything

all: rubik solver_perf

# GUI with 3D rubik cube simulation
rubik: rubik.c model.o draw.o libsolver.so
	gcc $(CFLAGS) -L./raylib-5.5_linux_amd64/lib -o $@ rubik.c model.o draw.o -l:libraylib.so -lm -Wl,-rpath=./raylib-5.5_linux_amd64/lib


# Run a simulation without GUI
solver_perf: solver_perf.c $(SOLVER) model.o
	$(CC) $(CFLAGS) -L./raylib-5.5_linux_amd64/lib -o $@ $^ -l:libraylib.a -lm

draw.o: draw.c draw.h
	gcc $(CFLAGS) -c $< -lm

model.o: model.c model.h
	gcc $(CFLAGS) -c $< -lm

libsolver.so: $(SOLVER) solver.h model.o
	gcc $(CFLAGS) -fPIC -shared -L./raylib-5.5_linux_amd64/lib -o $@ $(SOLVER) model.o -l:libraylib.so

solver.o: $(SOLVER) solver.h
	$(CC) $(CFLAGS) -o $@ -c $<


flipslicesorted_twist_depth3.o: generated/flipslicesorted_twist_depth3.bin
	ld -r -b binary -o $@ $^
	objcopy --redefine-sym _binary_generated_flipslicesorted_twist_depth3_bin_start=flipslicesorted_twist_depth3_start \
            --redefine-sym _binary_generated_flipslicesorted_twist_depth3_bin_end=flipslicesorted_twist_depth3_end \
            --redefine-sym _binary_generated_flipslicesorted_twist_depth3_bin_size=flipslicesorted_twist_depth3_size \
            $@

clean:
	rm -f rubik
	rm -f solver_perf
	rm -f *.o
	rm -f *.so
