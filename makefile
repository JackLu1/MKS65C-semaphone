all : write.c
	gcc -o out write.c

run : all
	./out

setup: setup.c
	gcc -o set setup.c
	./set ${arg}

clean : 
	rm out
	rm set
