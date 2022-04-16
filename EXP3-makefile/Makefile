text : text.o head.o
	gcc -o text text.o head.o
text.o : text.c
	gcc -c text.c
head.o : head.c head.h
	gcc -c head.c
clean:     
	rm -rf *.o