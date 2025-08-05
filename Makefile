
CFLAGS=-std=c99 -Wall -Wextra -Wpedantic -I./raylib-5.5_linux_amd64/include

all: rubik

rubik: rubik.c model.o draw.o
	gcc $(CFLAGS) -L./raylib-5.5_linux_amd64/lib -o $@ $^ -l:libraylib.a -lm

draw.o: draw.c draw.h
	gcc $(CFLAGS) -c $< -lm

model.o: model.c model.h
	gcc $(CFLAGS) -c $< -lm

clean:
	rm rubik
	rm *.o
