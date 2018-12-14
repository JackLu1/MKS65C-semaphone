all : write.c
	gcc -o out write.c
	gcc -o ctrl control.c

run : all
	./out

setup : all
	./ctrl ${arg}

clean : 
	rm out
	rm ctrl
