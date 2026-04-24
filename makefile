.POSIX:
CC = cc
INCLUDE_FLAGS = -Iinclude
CFLAGS = -O3 -Werror -Wall -Wextra $(INCLUDE_FLAGS)

matrix-multiplication: src/main.o src/coppersmith-winograd.o src/matrix-common.o src/matrix.o src/strassen.o
	$(CC) $(CFLAGS) -o matrix-multiplication src/main.o src/coppersmith-winograd.o src/matrix-common.o src/matrix.o src/strassen.o
src/main.o: src/main.c
src/coppersmith-winograd.o: src/coppersmith-winograd.c
src/matrix-common.o: src/matrix-common.c
src/matrix.o: src/matrix.c
src/strassen.o: src/strassen.c

clean:
	rm -f matrix-multiplication src/*.o
