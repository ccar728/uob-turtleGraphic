CFLAGS = `sdl2-config --cflags` -O2 -Wall -Wextra -pedantic -Wfloat-equal -ansi
INCS = neillsdl2.h
VALGRIND == --error-exitcode=1 --quiet --leak-check=full
SOURCES = neillsdl2.c
LIBS = `sdl2-config --libs` -lm
CC = gcc

TEST1 = test0.txt
TEST2 = test1.txt
TEST3 = test2.txt
TEST4 = test3.txt
TEST5 = test4.txt
TEST6 = test5.txt
EXT = ext1.txt

all: parse_test parse interp_test interp extension

parse_test: parse_test.c parse.h
	$(CC) parse_test.c -o parse_test $(CFLAGS)

parse: parse.c parse.h
	$(CC) parse.c -o parse $(CFLAGS)

interp_test: interp_test.c interp_test.h
	$(CC) interp_test.c -o interp_test $(CFLAGS)

interp: interp.c interp.h $(SOURCES) $(INCS)
	$(CC) interp.c $(SOURCES) -o interp $(CFLAGS) $(LIBS)

extension: extension.c extension.h $(SOURCES) $(INCS)
	$(CC) extension.c $(SOURCES) -o extension $(CFLAGS) $(LIBS)

run: all
		./parse_test
		./parse $(TEST1)
		./parse $(TEST2)
		./parse $(TEST3)
		./parse $(TEST4)
		./parse $(TEST5)
		./parse $(TEST6)
		
		./interp_test
		./interp $(TEST1)
		./interp $(TEST2)
		./interp $(TEST3)
		./interp $(TEST4)
		./interp $(TEST5)
		./interp $(TEST6)
		
		./extension $(TEST1)
		./extension $(TEST3)
		./extension $(TEST5)
		./extension $(TEST6)
		./extension ext1.txt

# gcc interp.c neillsdl2.c -o tst `sdl2-config --cflags` -O2 -Wall -Wextra -pedantic -Wfloat-equal -ansi `sdl2-config --libs` -lm