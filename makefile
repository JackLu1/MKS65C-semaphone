all : write.c
	gcc -o out write.c

run : all
	./out

setup: control.c
	gcc -o ctrl control.c
	./ctrl ${arg}

clean : 
	rm out
	rm ctrl
