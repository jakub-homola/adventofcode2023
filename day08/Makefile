
.PHONY: compile clean run

compile: task1.x task22.x

run: compile
	./task1.x < input.txt
	./task22.x < input.txt

clean:
	rm -f *.x

%.x: %.cpp Makefile
	g++ -g -O2 -fopenmp $< -o $@

