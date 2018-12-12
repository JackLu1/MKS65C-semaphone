all : setup.c write.c
	gcc setup.c write.c

run : all
	./a.out

setup: 
	echo "crv"

clean : a.out
	rm a.out
