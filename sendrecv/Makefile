all: 503

503: 503.o
	mpic++ 503.o -o 503 -lstdc++ 

503.o: 503.cpp classmpi3.h
	mpic++ -c 503.cpp

run: output.txt

output.txt: 503
	./503

clean:
	rm -rf *.o

distclean: clean
	rm -rf 503

.PHONY: all run clean distclean