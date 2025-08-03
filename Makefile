
all: rubik

rubik: rubik.c draw.c model.c
	gcc -std=c99 -Wall -Wextra -Wpedantic -I./raylib-5.5_linux_amd64/include -L./raylib-5.5_linux_amd64/lib -o $@ $< -l:libraylib.a -lm

clean:
	rm rubik
