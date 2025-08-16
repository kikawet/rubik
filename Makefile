
CFLAGS=-std=c99 -Wall -Wextra -Wpedantic -I./raylib-5.5_linux_amd64/include

# FIXME: right now the program links with libraylib.so because I want solver and program to share the same raylib instance.
# The goal is to statically link solver + raylib && fix rubik.c to not load anything

all: solver_tester

rubik: rubik.c model.o draw.o libsolver.so
	gcc $(CFLAGS) -L./raylib-5.5_linux_amd64/lib -o $@ rubik.c model.o draw.o -l:libraylib.so -lm -Wl,-rpath=./raylib-5.5_linux_amd64/lib

draw.o: draw.c draw.h
	gcc $(CFLAGS) -c $< -lm

model.o: model.c model.h
	gcc $(CFLAGS) -c $< -lm

libsolver.so: beginner_solver.c solver.h model.o
	gcc $(CFLAGS) -fPIC -shared -L./raylib-5.5_linux_amd64/lib -o $@ beginner_solver.c model.o -l:libraylib.so

solver.o: beginner_solver.c solver.h
	gcc $(CFLAGS) -o $@ -c $<

clean:
	rm rubik
	rm *.o
	rm *.so
