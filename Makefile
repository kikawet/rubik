
all: rubik

rubik: rubik.c
	gcc -std=c99 -Wall -Wpedantic -I./raylib-5.5_linux_amd64/include -L./raylib-5.5_linux_amd64/lib -o $@ $^ -l:libraylib.a -lm

clean:
	rm rubik
