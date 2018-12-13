all : setup.c write.c
	gcc setup.c write.c

run : all
	./a.out

setup: setup.c
	gcc -o set setup.c
	./set ${arg}

clean : a.out
	rm a.out
