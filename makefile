all : code.c
	gcc code.c

run : all
	./a.out

clean : a.out
	rm a.out
